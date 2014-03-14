#include "CanbusData.hpp"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <bb/data/JsonDataAccess>
#include <QDir>
#include <QFile>
#include <QDateTime>

#include <qdebug.h>

static const QByteArray HEADER_CANBUS_REQUEST = "x-bb10-canbus-request";
static const int CANBUS_REQUEST_NEXT_DATA = 1;
static const int CANBUS_REQUEST_DELETE_DATA = 2;

using namespace bb::data;
//using namespace bb::cascades;

CanbusData::CanbusData(QObject *parent) :
		QObject(parent), mNetworkAccessManager(new QNetworkAccessManager(this)) {

	mBaseUrl = "";
	mInitialized = true;
}

void CanbusData::setUri(const QString& uri) {
	mBaseUrl = uri;
}

void CanbusData::askForNextData() {
	QString uri;
	uri = mBaseUrl;
	uri += "canbus/";
	uri += "values";
	qDebug() << uri;
	QNetworkRequest request(uri);
	request.setRawHeader(HEADER_CANBUS_REQUEST,
			QByteArray::number(CANBUS_REQUEST_NEXT_DATA));
	QNetworkReply* reply = mNetworkAccessManager->get(request);
	bool ok;
	ok = connect(reply, SIGNAL(finished()), this, SLOT(onDataReply()));
	if (!ok) {
		qWarning() << "Connect FAILS: askForNextData - onDataReply";
	}
}

void CanbusData::onDataReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	int canbusRequest =
			reply->request().rawHeader(HEADER_CANBUS_REQUEST).toInt();
	QString response;
	QString replyCode;
	bool successFromServer = false;
	if (reply) {
		replyCode =
				reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer = reply->readAll();
				response = QString::fromUtf8(buffer);
				successFromServer = true;
			}

		} else {
			qDebug() << reply->errorString() << " Reply Code: " << replyCode;
		}
		reply->deleteLater();
	}
	QVariantList emptyList;
	if (response.trimmed().isEmpty()) {
		qDebug() << "no data";
		emit responseData(emptyList);
		return;
	}
	qDebug() << response;
	JsonDataAccess jda;
	QVariant data;
	if (successFromServer) {
		data = jda.loadFromBuffer(response);
		switch (canbusRequest) {
		case CANBUS_REQUEST_NEXT_DATA:
			processCanbusData(data);
			break;
		case CANBUS_REQUEST_DELETE_DATA:
			//
			qDebug() << "deleted ONE OID";
			break;
		default:
			qDebug() << "wrong request code";
			break;
		}
	}
}

void CanbusData::processCanbusData(QVariant& data) {
	QStringList deleteOidList;
	QVariantList dataList;
	QVariantList canbusDataList;
	dataList = data.toList();
	qDebug() << "got canbus data: #" << dataList.size();
	for (int d = 0; d < dataList.size(); ++d) {
		QVariantMap map;
		map = dataList.at(d).toMap();
		QString oid;
		oid = map.value("_id").toMap().value("$oid").toString();
		deleteOidList << oid;
		int sensor = map.value("sensor").toInt();
		int value = map.value("value").toInt();
		QVariantMap timeMap;
		timeMap = map.value("time").toMap();
		QString timestamp = timeMap.value("$date").toString();
		QString timeString;
		QStringList timeSplit;
		timeSplit = timestamp.split("T");
		if (timeSplit.size() == 2) {
			timeString = timeSplit.at(1).left(8);
		} else {
			timeString = "00:00:00";
		}
		QString day;
		day = timestamp.left(10);
		qDebug() << "OID: " << oid << "sensor: " << sensor << " value: "
				<< value << " timestamp: " << timestamp;
		qDebug() << "day: " << day << " time: " << timeString;
		QString sensorName;
		QString imageName;
		switch (sensor) {
			case 16:
				sensorName = "Clutch";
				imageName = "clutch";
				break;
			case 5:
				sensorName = "ETC Temperature";
				imageName = "temperature";
				break;
			case 12:
				sensorName = "RPM";
				imageName = "rpm";
				break;
			case 13:
				sensorName = "Speed km/h";
				imageName = "speed";
				break;
			case 20:
				sensorName = "Lights";
				imageName = "lights";
				break;
			default:
				sensorName = "unsupported signal ";
				sensorName += QString::number(sensor);
				imageName = "icon";
				break;
		}

		QVariantMap canbusMap;
		canbusMap.insert("timestamp", timestamp);
		canbusMap.insert("oid",oid);
		canbusMap.insert("sensor", sensor);
		canbusMap.insert("sensorValue", value);
		canbusMap.insert("sensorName",sensorName);
		canbusMap.insert("imageName",imageName);
		canbusMap.insert("day", day);
		canbusMap.insert("timeString", timeString);
		canbusDataList.append(canbusMap);
	}
	qDebug() << "datalist #:" << canbusDataList.size();
	emit responseData(canbusDataList);
	deleteTransferedData(deleteOidList);
}

void CanbusData::deleteTransferedData(const QStringList& listOfOids) {
	QString uri;
	uri = mBaseUrl;
	uri += "canbus/";
	uri += "values";
	uri += "/";
	//
	for (int i = 0; i < listOfOids.size(); ++i) {
		QString deleteUri;
		deleteUri = uri;
		deleteUri += listOfOids.at(i);
		qDebug() << deleteUri;
		QNetworkRequest request(deleteUri);
		request.setRawHeader(HEADER_CANBUS_REQUEST,
				QByteArray::number(CANBUS_REQUEST_DELETE_DATA));
		QNetworkReply* reply = mNetworkAccessManager->deleteResource(request);
		bool ok;
		ok = connect(reply, SIGNAL(finished()), this, SLOT(onDataReply()));
		if (!ok) {
			qWarning() << "Connect FAILS: deleteTransferedData - onDataReply";
		}
	}
}

CanbusData::~CanbusData() {
}

