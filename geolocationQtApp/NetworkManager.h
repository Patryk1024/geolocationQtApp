#pragma once

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject* parent = nullptr);
    void downloadJsonData(const QUrl& url);

signals:
    void jsonDownloaded(const QByteArray& data);
    void downloadFailed(const QString& errorString);

private slots:
    void downloadFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* networkManager;
};
