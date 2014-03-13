

#include "CanbusData.hpp"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <bb/data/JsonDataAccess>
#include <QDir>
#include <QFile>

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

void CanbusData::setUri(const QString& uri){
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
	if (response.trimmed().isEmpty()) {
		qDebug() << "no data";
		return;
	}
	qDebug() << response;
	JsonDataAccess jda;
	QVariant data;
	QVariantList dataList;
	if (successFromServer) {
		data = jda.loadFromBuffer(response);
		switch (canbusRequest) {
		case CANBUS_REQUEST_NEXT_DATA:
			emit responseData(data);
			dataList = data.toList();
			qDebug() << "got canbus data: #" << dataList.size();
			for (int d = 0; d < dataList.size(); ++d) {
				QVariantMap map;
				map = dataList.at(d).toMap();
				qDebug() << map;
			}
			break;
		case CANBUS_REQUEST_DELETE_DATA:
			//
			break;
		default:
			qDebug() << "wrong request code";
			break;
		}
	}

}

void CanbusData::deleteTransferedData(const QStringList& listOfOids) {
	QString uri;
	uri = mBaseUrl;
	uri += "canbus/";
	uri += "values";
	uri += "/";
	for (int i = 0; i < listOfOids.size(); ++i) {
		uri += listOfOids.at(i);
		QNetworkRequest request(uri);
		request.setRawHeader(HEADER_CANBUS_REQUEST,
				QByteArray::number(CANBUS_REQUEST_DELETE_DATA));
		QNetworkReply* reply = mNetworkAccessManager->get(request);
		bool ok;
		ok = connect(reply, SIGNAL(finished()), this, SLOT(onDataReply()));
		if (!ok) {
			qWarning() << "Connect FAILS: deleteTransferedData - onDataReply";
		}
	}
}

CanbusData::~CanbusData() {
}

