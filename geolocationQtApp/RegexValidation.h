#pragma once

#include <QString>

class RegexValidation
{
public:
	static bool isValidIPv4(const QString& ip);
	static bool isValidIPv6(const QString& ip);
	static bool isValidType(const QString& type);
	static bool isValidGeoCode(const QString& code);
	static bool isValidCityName(const QString& city);	// I will use it to check all names
	static bool isValidZIP(const QString& zipCode);
	static bool isValidLatitude(const double latitude);
	static bool isValidLongitude(const double longitude);
	static bool isValidGeonameID(const int geonameID);
	static bool isValidUrl(const QString& urlString);
	static bool isValidUnicode(const QString& unicodeString);
};
