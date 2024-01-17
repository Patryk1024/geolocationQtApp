#include "IPDetails.h"

void IPDetails::Location::setLocationFromJson(const QJsonObject& locationObj)
{
	/*geonameId = locationObj["geoname_id"].toInt();*/
	geonameId = locationObj["geoname_id"].toString();
	capital = locationObj["capital"].toString();

	languages.clear();
	QJsonArray languagesArray = locationObj["languages"].toArray();
	for (const auto& lang : languagesArray)
	{
		languages.push_back(lang.toObject()["name"].toString());
	}

	countryFlag = locationObj["country_flag"].toString();
	countryFlagEmoji = locationObj["country_flag_emoji"].toString();
	countryFlagEmojiUnicode = locationObj["country_flag_emoji_unicode"].toString();
	callingCode = locationObj["calling_code"].toString();
	isEu = locationObj["is_eu"].toBool();
}

void IPDetails::Location::printLocationDetails() const
{
	qDebug() << "Geoname ID: " << geonameId;
	qDebug() << "Capital: " << capital;
	qDebug() << "Languages: ";
	for (const auto& lang : languages)
	{
		qDebug() << lang << " ";
	}
	qDebug();
	qDebug() << "Country Flag: " << countryFlag;
	qDebug() << "Country Flag Emoji: " << countryFlagEmoji;
	qDebug() << "Country Flag Emoji Unicode: " << countryFlagEmojiUnicode;
	qDebug() << "Calling Code: " << callingCode;
	qDebug() << "Is EU: " << (isEu ? "true" : "false");
}

void IPDetails::setIPDetailsFromJson(const QJsonObject& ipObj)
{
	ip = ipObj["ip"].toString();
	url = QString();
	type = ipObj["type"].toString();
	continentCode = ipObj["continent_code"].toString();
	continentName = ipObj["continent_name"].toString();
	countryCode = ipObj["country_code"].toString();
	countryName = ipObj["country_name"].toString();
	regionCode = ipObj["region_code"].toString();
	regionName = ipObj["region_name"].toString();
	city = ipObj["city"].toString();
	zip = ipObj["zip"].toString();
	latitude = ipObj["latitude"].toString();
	longitude = ipObj["longitude"].toString();
	/*latitude = ipObj["latitude"].toDouble();
	longitude = ipObj["longitude"].toDouble();*/

	QJsonObject locationObj = ipObj["location"].toObject();
	location.setLocationFromJson(locationObj);
}

void IPDetails::printIPDetails() const
{
	qDebug() << "IP: " << ip;
	qDebug() << "URL: " << url;
	qDebug() << "Type: " << type;
	qDebug() << "Continent Code: " << continentCode;
	qDebug() << "Continent Name: " << continentName;
	qDebug() << "Country Code: " << countryCode;
	qDebug() << "Country Name: " << countryName;
	qDebug() << "Region Code: " << regionCode;
	qDebug() << "Region Name: " << regionName;
	qDebug() << "City: " << city;
	qDebug() << "ZIP: " << zip;
	qDebug() << "Latitude: " << latitude;
	qDebug() << "Longitude: " << longitude;
	qDebug() << "Location Details:";
	location.printLocationDetails();
}
