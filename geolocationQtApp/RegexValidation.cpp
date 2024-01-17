#include "RegexValidation.h"

#include <regex>
#include <QRegularExpression>

bool RegexValidation::isValidIPv4(const QString& ip)
{
	std::regex ipRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	auto result = std::regex_match(ip.toStdString(), ipRegex);
	return result;
}

bool RegexValidation::isValidIPv6(const QString& ip)
{
	QRegularExpression ipv6ExpandedRegex("\\A[[:xdigit:]]{1,4}(:[[:xdigit:]]{1,4}){7}\\z");
	QRegularExpression ipv6ShorthandRegexNot("\\A(.*?[[:xdigit:]](:|\\z)){8}");
	QRegularExpression ipv6ShorthandRegex(QStringLiteral(R"(\A([[:xdigit:]]{1,4}(:[[:xdigit:]]{1,4}){0,6})?::([[:xdigit:]]{1,4}(:[[:xdigit:]]{1,4}){0,6})?\z)"));
	/*QRegularExpression ipv6DottedExpandedRegex("\\A[[:xdigit:]]{1,4}(:[[:xdigit:]]{1,4}){5}:(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])(\\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])){3}\\z");
	QRegularExpression ipv6DottedShorthandRegexNot("\\A(.*?[[:xdigit:]]:){6}");*/
	QRegularExpression regexIpv6WithIpv4Suffix(QStringLiteral(R"(\A::([f]{4}:)(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])(\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])){3}\z)"));

	// IPv6 expanded
	if (ipv6ExpandedRegex.match(ip).hasMatch())
	{
		return true;
	}

	// IPv6 shorthand
	if ((!ipv6ShorthandRegexNot.match(ip).hasMatch()) && ipv6ShorthandRegex.match(ip).hasMatch())
	{
		return true;
	}

	// IPv6 dotted-quad notation, expanded
	/*if (ipv6DottedExpandedRegex.match(ip).hasMatch())
	{
		return true;
	}*/

	// IPv6 dotted-quad notation, shorthand
	if (regexIpv6WithIpv4Suffix.match(ip).hasMatch())
	{
		return true;
	}

	return false;
}

bool RegexValidation::isValidType(const QString& type)
{
	// Validate if type is either 'ipv4' or 'ipv6'
	return (type.toLower() == "ipv4" || type.toLower() == "ipv6");
}

bool RegexValidation::isValidGeoCode(const QString& code)
{
	// Validate if code is only two letters
	std::regex geoCodeRegex("^[A-Za-z]{2}$");
	return std::regex_match(code.toStdString(), geoCodeRegex);
}

bool RegexValidation::isValidCityName(const QString& city)
{
	// Not the best regex for cities but still good
	// ^[\\p{L} -'í]+$
	// this regexes not work for std::regex
	// std::regex cityNameRegex("^[a-zA-Z\\x{0080}-\\x{024F}\\s\\/\-\\`\\.\\\"']+");
	// because of this I will only add polish letters
	//std::regex cityNameRegex("^[a-zA-Zπ•Ê∆Í ≥£Ò—Û”úåüèøØ\\s\\/\-\\`\\.\\\"']+");
	//std::wstring wCity = city.toStdWString();
	//std::string sCity = std::string(wCity.begin(), wCity.end());
	//auto result = std::regex_match(sCity, cityNameRegex);
	//if (!result)
	//{
	//	// Convert QString to std::wstring
	//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//	std::wstring wCity = converter.from_bytes(city.toStdString());
	//	std::string sCity = std::string(wCity.begin(), wCity.end());
	//	result = std::regex_match(sCity, cityNameRegex);
	//}
	//return result;

	// Okay, I've found out that I tried to use QRegExp not QRegularExpression, that's why I've had problem on the beggining

	// Define the regex pattern for city name validation
	QRegularExpression cityNameRegex("^[a-zA-Z\\x{0080}-\\x{024F}\\s\\/\-\\`\\.\\\"']+");

	// Set Unicode option for the regex pattern
	cityNameRegex.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);

	// Perform the regex match on the QString directly
	QRegularExpressionMatch match = cityNameRegex.match(city);
	auto result = match.hasMatch();
	return result;
}

bool RegexValidation::isValidZIP(const QString& zipCode)
{
	std::regex zipCodeRegex("^\\d{5}(?:[-\\s]\\d{4})?$");
	std::regex zipCodePLRegex("^\\d{2}-\\d{3}$");
	return std::regex_match(zipCode.toStdString(), zipCodeRegex) || std::regex_match(zipCode.toStdString(), zipCodePLRegex);
}

bool RegexValidation::isValidLatitude(const double latitude)
{
	// Latitude ranges from -90 to +90 degrees
	return (latitude >= -90.0 && latitude <= 90.0);
}

bool RegexValidation::isValidLongitude(const double longitude)
{
	// Longitude ranges from -180 to +180 degrees
	return (longitude >= -180.0 && longitude <= 180.0);
}

bool RegexValidation::isValidGeonameID(const int geonameID)
{
	return (geonameID > 0);
}

bool RegexValidation::isValidUrl(const QString& urlString)
{
	// Regular expression pattern for URL validation
	//QRegularExpression urlRegex("^((https?|ftp|file):\\/\\/)?([\\w-]+\\.)+[\\w-]+(\\/\\S*)?$", QRegularExpression::CaseInsensitiveOption);
	//QRegularExpression urlRegex("^(https?|ftp|file)?:\\/\\/(www\\.)?([\\w-]+\\.)+[\\w-]+(\\/\\S*)?(?!\\.[0-9]+$)$", QRegularExpression::CaseInsensitiveOption);
	QString regexString = R"(^(http(s)?:\/\/)?(www\.)?([a-zA-Z0-9])+([\-\.]{1}[a-zA-Z0-9]+)*\.[a-zA-Z]{2,5}(:[0-9]{1,5})?(\/[^\s]*)?$)";
	QRegularExpression urlRegex(regexString);

	bool isUrl = urlRegex.match(urlString).hasMatch();
	bool result = isUrl;// && !isValidIPv4(urlString) && !isValidIPv6(urlString);
    return result;
}

bool RegexValidation::isValidUnicode(const QString& unicodeString)
{
	QRegularExpression pattern("^(U\\+[0-9a-fA-F]{4} U\\+[0-9a-fA-F]{4}|\\\\u[0-9a-fA-F]{4}\\\\u[0-9a-fA-F]{4})$");
	QRegularExpressionMatch match = pattern.match(unicodeString);

	auto result = match.hasMatch();
	return result;
}
