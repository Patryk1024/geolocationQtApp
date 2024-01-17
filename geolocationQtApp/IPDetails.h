#pragma once

#include <iostream>
//#include <string>
#include <vector>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

// Class representing the IP details
class IPDetails
{
public:
    // Class representing the location data
    class Location
    {
    public:
        //int geonameId;
        QString geonameId;
        QString capital;
        std::vector<QString> languages; //not used for now
        QString countryFlag;
        QString countryFlagEmoji;
        QString countryFlagEmojiUnicode;
        QString callingCode;
        bool isEu;

        Location() = default;
        // Constructor with all parameters
        Location(const QString& geonameId, const QString& capital, const std::vector<QString>& languages,
            const QString& countryFlag, const QString& countryFlagEmoji,
            const QString& countryFlagEmojiUnicode, const QString& callingCode,
            bool isEu)
            : geonameId(geonameId), capital(capital), languages(languages),
            countryFlag(countryFlag), countryFlagEmoji(countryFlagEmoji),
            countryFlagEmojiUnicode(countryFlagEmojiUnicode), callingCode(callingCode),
            isEu(isEu) {}
        /*Location(int geonameId, const QString& capital, const std::vector<QString>& languages,
            const QString& countryFlag, const QString& countryFlagEmoji,
            const QString& countryFlagEmojiUnicode, const QString& callingCode,
            bool isEu)
            : geonameId(geonameId), capital(capital), languages(languages),
            countryFlag(countryFlag), countryFlagEmoji(countryFlagEmoji),
            countryFlagEmojiUnicode(countryFlagEmojiUnicode), callingCode(callingCode),
            isEu(isEu) {}*/

        // Setter to initialize values from a QJsonObject
        void setLocationFromJson(const QJsonObject& locationObj);
        void printLocationDetails() const;
    };

public:
    QString ip;
    QString url;
    QString type;
    QString continentCode;
    QString continentName;
    QString countryCode;
    QString countryName;
    QString regionCode;
    QString regionName;
    QString city;
    QString zip;
    QString latitude;
    QString longitude;
    /*double latitude;
    double longitude;*/
    Location location;

    IPDetails() = default;
    // Constructor with all parameters
    IPDetails(const QString& ip, const QString& url, const QString& type, const QString& continentCode,
        const QString& continentName, const QString& countryCode,
        const QString& countryName, const QString& regionCode,
        const QString& regionName, const QString& city, const QString& zip,
        const QString& latitude, const QString& longitude, const Location& location)
        : ip(ip), url(url), type(type), continentCode(continentCode), continentName(continentName),
        countryCode(countryCode), countryName(countryName), regionCode(regionCode),
        regionName(regionName), city(city), zip(zip), latitude(latitude), longitude(longitude),
        location(location) {}
    // Constructor for Database load function
    IPDetails(const QString& ip, const QString& continentName, const QString& countryName,
        const QString& regionName, const QString& city, const QString& zip,
        const QString& latitude, const QString& longitude)
        : ip(ip), url(QString()), continentName(continentName), countryName(countryName), regionName(regionName),
        city(city), zip(zip), latitude(latitude), longitude(longitude) 
    {
        url = QString();
    }

    // Setter to initialize values from a QJsonObject
    void setIPDetailsFromJson(const QJsonObject& ipObj);
    void printIPDetails() const;
};

