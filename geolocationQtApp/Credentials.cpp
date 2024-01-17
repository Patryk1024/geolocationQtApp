#include "Credentials.h"

#include <QDebug>
#include <QMessageBox>

bool CredentialsInstance::Credentials::setAPIKey(QString key)
{
    apiKey = key;
    if (apiKey.isEmpty())
    {
        qDebug() << "No API key given";
        return false;
    }
    else if (apiKey == "debug")
    {
        //NOT SECURED
        apiKey = debugAPIKey;
        qDebug() << "debug API key!! Security Warning!!";
    }
    qDebug() << "API key set correct";
    return true;
}

const QString CredentialsInstance::Credentials::getAPIKey() const
{
    return apiKey;
}

CredentialsInstance::CredentialsInstance()
{
    try
    {
        credentials = std::make_unique<Credentials>();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(nullptr, "Initialization Error", e.what());
    }
}

CredentialsInstance& CredentialsInstance::unique_instance()
{
    static CredentialsInstance credentialsInstance;
    return credentialsInstance;
}

bool CredentialsInstance::setAPIKey(QString key)
{
    /*if (!credentials)
    {
        unique_instance();
    }*/
    return credentials->setAPIKey(key);
}

const QString CredentialsInstance::getAPIKey() const
{
    /*if (!credentials)
    {
        unique_instance();
    }*/
    return credentials->getAPIKey();
}
