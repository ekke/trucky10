

#ifndef CANBUSDATA_HPP_
#define CANBUSDATA_HPP_

#include <qobject.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <qvariant.h>


class CanbusData: public QObject {
	Q_OBJECT

public:

	CanbusData(QObject *parent = 0);

	Q_INVOKABLE
	void askForNextData();

	Q_INVOKABLE
	void setUri(const QString& uri);

signals:

	void responseData(const QVariantList& canbusData);

private Q_SLOTS:

	void onDataReply();

private:

	QNetworkAccessManager* mNetworkAccessManager;
	bool mInitialized;
	QString mBaseUrl;

	void deleteTransferedData(const QStringList& listOfOids);
	void processCanbusData(QVariant& data);

	virtual ~CanbusData();
};

#endif /* CANBUSDATA_HPP_ */
