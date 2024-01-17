#include "Database.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

// Open a connection to the SQLite database and create table
bool DatabaseApp::Database::initDatabase()
{
	const QString path = QString::fromStdString(dbPath.string());
	auto db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(path);
	if (!db.open())
	{
		qDebug() << "Error: Failed to open database:" << db.lastError().text();
		return false;
	}
	qDebug() << "Connected to the database.";

	QSqlQuery query;
	query.exec("CREATE TABLE IF NOT EXISTS ip_data ("
		"ip TEXT NULL, "
		"url TEXT NULL,"
		"type TEXT, "
		"continent_code TEXT, "
		"continent_name TEXT, "
		"country_code TEXT, "
		"country_name TEXT, "
		"region_code TEXT, "
		"region_name TEXT, "
		"city TEXT, "
		"zip TEXT, "
		"latitude REAL, "
		"longitude REAL, "
		"geoname_id INTEGER, "
		"capital TEXT, "
		"country_flag TEXT, "
		"country_flag_emoji TEXT, "
		"country_flag_emoji_unicode TEXT, "
		"calling_code TEXT, "
		"is_eu INTEGER CHECK (is_eu IN (0, 1)))"); // Constraint added for boolean values
	if (query.lastError().isValid())
	{
		qDebug() << "Error creating table:" << query.lastError().text();
		db.close();
		return false;
	}

	//db.close();
	return true;
}

bool DatabaseApp::Database::addDataToDatabase(const IPDetails& ipDetails)
{
	auto db = QSqlDatabase::database();
	if (!db.open())
	{
		qDebug() << "Error: Failed to open database:" << db.lastError().text();
		return false;
	}
	qDebug() << "Connected to the database.";

	QSqlQuery query;
	query.prepare("INSERT INTO ip_data "
		"(ip, url, type, continent_code, continent_name, country_code, country_name, "
		"region_code, region_name, city, zip, latitude, longitude, geoname_id, "
		"capital, country_flag, country_flag_emoji, country_flag_emoji_unicode, "
		"calling_code, is_eu) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	query.addBindValue(ipDetails.ip);
	query.addBindValue(ipDetails.url);
	query.addBindValue(ipDetails.type);
	query.addBindValue(ipDetails.continentCode);
	query.addBindValue(ipDetails.continentName);
	query.addBindValue(ipDetails.countryCode);
	query.addBindValue(ipDetails.countryName);
	query.addBindValue(ipDetails.regionCode);
	query.addBindValue(ipDetails.regionName);
	query.addBindValue(ipDetails.city);
	query.addBindValue(ipDetails.zip);
	query.addBindValue(ipDetails.latitude);
	query.addBindValue(ipDetails.longitude);

	auto location = ipDetails.location;
	query.addBindValue(location.geonameId);
	query.addBindValue(location.capital);
	query.addBindValue(location.countryFlag);
	query.addBindValue(location.countryFlagEmoji);
	query.addBindValue(location.countryFlagEmojiUnicode);
	query.addBindValue(location.callingCode);
	query.addBindValue(location.isEu ? 1 : 0); // Store boolean value as integer (0 or 1)

	if (!query.exec())
	{
		qDebug() << "Error inserting data:" << query.lastError().text();
		lastErrorFromQuery = query.lastError().text();
		return false;
	}
	else
	{
		qDebug() << "Data inserted successfully!";
	}

	//db.close();
	return true;
}

// Insert data into the table
bool DatabaseApp::Database::addDataToDatabase(const QJsonObject& jsonData, const QString& url)
{
	auto db = QSqlDatabase::database();
	if (!db.open())
	{
		qDebug() << "Error: Failed to open database:" << db.lastError().text();
		return false;
	}
	qDebug() << "Connected to the database.";

	QSqlQuery query;
	query.prepare("INSERT INTO ip_data "
		"(ip, url, type, continent_code, continent_name, country_code, country_name, "
		"region_code, region_name, city, zip, latitude, longitude, geoname_id, "
		"capital, country_flag, country_flag_emoji, country_flag_emoji_unicode, "
		"calling_code, is_eu) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	query.addBindValue(jsonData["ip"].toString());
	query.addBindValue(url);
	query.addBindValue(jsonData["type"].toString());
	query.addBindValue(jsonData["continent_code"].toString());
	query.addBindValue(jsonData["continent_name"].toString());
	query.addBindValue(jsonData["country_code"].toString());
	query.addBindValue(jsonData["country_name"].toString());
	query.addBindValue(jsonData["region_code"].toString());
	query.addBindValue(jsonData["region_name"].toString());
	query.addBindValue(jsonData["city"].toString());
	query.addBindValue(jsonData["zip"].toString());
	query.addBindValue(jsonData["latitude"].toDouble());
	query.addBindValue(jsonData["longitude"].toDouble());

	QJsonObject location = jsonData["location"].toObject();
	query.addBindValue(location["geoname_id"].toInt());
	query.addBindValue(location["capital"].toString());
	query.addBindValue(location["country_flag"].toString());
	query.addBindValue(location["country_flag_emoji"].toString());
	query.addBindValue(location["country_flag_emoji_unicode"].toString());
	query.addBindValue(location["calling_code"].toString());
	query.addBindValue(location["is_eu"].toBool() ? 1 : 0); // Store boolean value as integer (0 or 1)

	if (!query.exec())
	{
		qDebug() << "Error inserting data:" << query.lastError().text();
		lastErrorFromQuery = query.lastError().text();
		return false;
	}
	else
	{
		qDebug() << "Data inserted successfully!";
	}

	//db.close();
	return true;
}

//not used for now
// Get data from database and return vector of them as IPDetails in parameter
bool DatabaseApp::Database::loadDataFromDatabase(std::vector<IPDetails>& ipDetails)
{
	auto db = QSqlDatabase::database();
	if (!db.open())
	{
		qDebug() << "Error: Failed to open database:" << db.lastError().text();
		return false;
	}
	qDebug() << "Connected to the database.";

	// Retrieving data from the database
	QSqlQuery query;
	query.exec("SELECT ip, continent_name, country_name, region_name, city, zip, latitude, longitude FROM ip_data");
	//query.exec("SELECT * FROM ip_data");

	// Displaying the list of items
	ipDetails.clear();
	while (query.next())
	{
		/*ipDetails.push_back(IPDetails{ query.value(0).toString().toStdString(), query.value(1).toString().toStdString(), query.value(2).toString().toStdString(), query.value(3).toString().toStdString(), query.value(4).toString().toStdString(), query.value(5).toString().toStdString(), query.value(6).toDouble(), query.value(7).toDouble()});*/
		//ipDetails.push_back(IPDetails{ query.value(0).toString(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString(), query.value(5).toString(), query.value(6).toDouble(), query.value(7).toDouble() });
		ipDetails.push_back(IPDetails{ query.value(0).toString(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString(), query.value(5).toString(), query.value(6).toString(), query.value(7).toString() });

		//qDebug() << "IP:" << ip;
		//ipDetails.back().printIPDetails();
	}

	// Close the database connection
	//db.close();

	return true;
}

bool DatabaseApp::Database::removeRowsFromDatabase(const QStringList& ips)
{
	auto db = QSqlDatabase::database();
	if (!db.open())
	{
		qDebug() << "Error opening database:" << db.lastError().text();
		return false;
	}

	QSqlQuery query(db);
	db.transaction(); // Start a transaction

	for (const QString& ip : ips)
	{
		// Delete the corresponding row from the database
		query.prepare("DELETE FROM ip_data WHERE ip = ?");
		query.addBindValue(ip);

		if (!query.exec())
		{
			db.rollback(); // Rollback the transaction on error
			qDebug() << "Error deleting row from the database:" << query.lastError().text();
			return false;
		}
	}

	if (!db.commit())
	{
		qDebug() << "Error committing transaction:" << db.lastError().text();
		db.rollback(); // Rollback the transaction on commit error
		return false;
	}

	return true;
}

bool DatabaseApp::Database::removeDatabase()
{
	auto db = QSqlDatabase::database();
	if (db.open())
	{
		db.close();
		qDebug() << "Connection to database closed";
	}
	if (!std::filesystem::exists(dbPath))
	{
		qDebug() << "Database not exists";
		return false;
	}
	if (!std::filesystem::remove(dbPath))
	{
		qDebug() << "Database could not be removed";
		return false;
	}
	qDebug() << "Database removed";
	return true;
}

const std::filesystem::path DatabaseApp::Database::getDBPath() const
{
	return dbPath;
}

const QString DatabaseApp::Database::getTableName() const
{
	return tableName;
}

const QString DatabaseApp::Database::getLastErrorFromQuery() const
{
	return lastErrorFromQuery;
}

bool DatabaseApp::Database::isDatabaseOpen()
{
	auto db = QSqlDatabase::database();
	return db.isOpen();
}

DatabaseApp& DatabaseApp::unique_instance()
{
	static DatabaseApp databaseApp;
	return databaseApp;
}

bool DatabaseApp::initDatabase()
{
	return database->initDatabase();
}

bool DatabaseApp::addDataToDatabase(const IPDetails& ipDetails)
{
	return database->addDataToDatabase(ipDetails);
}

bool DatabaseApp::addDataToDatabase(const QJsonObject& jsonData, const QString& url)
{
	return database->addDataToDatabase(jsonData, url);
}

bool DatabaseApp::loadDataFromDatabase(std::vector<IPDetails>& ipDetails)
{
	return database->loadDataFromDatabase(ipDetails);
}

bool DatabaseApp::removeRowsFromDatabase(const QStringList& ips)
{
	return database->removeRowsFromDatabase(ips);
}

bool DatabaseApp::removeDatabase()
{
	return database->removeDatabase();
}

const std::filesystem::path DatabaseApp::getDBPath() const
{
	return database->getDBPath();
}

const QString DatabaseApp::getTableName() const
{
	return database->getTableName();
}

const QString DatabaseApp::getLastErrorFromQuery() const
{
	return database->getLastErrorFromQuery();
}

bool DatabaseApp::isDatabaseOpen()
{
	return database->isDatabaseOpen();
}

DatabaseApp::DatabaseApp()
{
	database = std::make_unique<Database>();
}
