#pragma once

#include "IPDetails.h"

#include <filesystem>
#include <memory>
#include <QSqlDatabase>
#include <QJsonObject>
#include <vector>

class DatabaseApp
{
private:
	class Database
	{
	public:
		Database() = default;

		// data
		bool initDatabase();
		bool addDataToDatabase(const IPDetails& ipDetails);
		bool addDataToDatabase(const QJsonObject& jsonData, const QString& url = QString());	//not used for now
		bool loadDataFromDatabase(std::vector<IPDetails>& ipDetails);	//not used for now
		bool removeRowsFromDatabase(const QStringList& ips);

		// database
		bool removeDatabase();

		// getters
		const std::filesystem::path getDBPath() const;
		const QString getTableName() const;
		const QString getLastErrorFromQuery() const;

		bool isDatabaseOpen();
	private:
		const std::filesystem::path dbPath{ "appDB.db" };
		const QString tableName = "ip_data";
		const QString API = "348c5ef56bf34ebc91182562aeefe077";
		QString lastErrorFromQuery{};
	};
public:
	static DatabaseApp& unique_instance();

	//getters to inner class
	bool initDatabase();
	bool addDataToDatabase(const IPDetails& ipDetails);
	bool addDataToDatabase(const QJsonObject& jsonData, const QString& url = QString());	//not used for now
	bool loadDataFromDatabase(std::vector<IPDetails>& ipDetails);	//not used for now
	bool removeRowsFromDatabase(const QStringList& ips);

	// database
	bool removeDatabase();

	// getters
	const std::filesystem::path getDBPath() const;
	const QString getTableName() const;
	const QString getLastErrorFromQuery() const;

	bool isDatabaseOpen();

private:
	DatabaseApp();
	std::unique_ptr<Database> database;
};
