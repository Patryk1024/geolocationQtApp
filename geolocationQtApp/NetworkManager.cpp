#include "NetworkManager.h"
#include <QMessageBox>

NetworkManager::NetworkManager(QObject* parent) : QObject(parent)
{
	try
	{
		networkManager = new QNetworkAccessManager(this);
		connect(networkManager, &QNetworkAccessManager::finished, this, &NetworkManager::downloadFinished);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(nullptr, "Initialization Error", e.what());
	}
}

void NetworkManager::downloadJsonData(const QUrl& url)
{
	QNetworkRequest request(url);
	networkManager->get(request);
}

void NetworkManager::downloadFinished(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray data = reply->readAll();
		emit jsonDownloaded(data);
	}
	else
	{
		emit downloadFailed(reply->errorString());
	}

	reply->deleteLater();
}
