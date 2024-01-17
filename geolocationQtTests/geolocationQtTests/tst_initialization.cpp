
#include <gtest/gtest.h>
#include "../../geolocationQtApp/Database.cpp"

class DatabaseTest : public ::testing::Test {
protected:
	// Set up code to be called before each test
	void SetUp() override
	{
		// Perform any necessary setup here
		// This will be executed before each test case
		/*auto& db = DatabaseApp::unique_instance().database;
		db->removeDatabase();*/
		DatabaseApp::unique_instance().removeDatabase();
	}

	// Optional: Tear down code to be called after each test
	void TearDown() override
	{
		/*auto& db = DatabaseApp::unique_instance().database;
		db->removeDatabase();*/
	}
};

TEST_F(DatabaseTest, Initialization)
{
	/*auto& db = DatabaseApp::unique_instance().database;
	db->initDatabase();*/
	DatabaseApp::unique_instance().initDatabase();
	std::string test = DatabaseApp::unique_instance().getDBPath().string();
	auto isExist = std::filesystem::exists(DatabaseApp::unique_instance().getDBPath());
	auto isOpen = DatabaseApp::unique_instance().isDatabaseOpen();
	//EXPECT_EQ(1, 1);
	EXPECT_TRUE(isExist && isOpen);
}

TEST_F(DatabaseTest, AddDataToDatabase)
{
	/*auto& db = DatabaseApp::unique_instance().database;
	db->initDatabase();*/
	DatabaseApp::unique_instance().initDatabase();

	QString jsonDataStr = "{\"ip\": \"134.201.250.155\", \"type\": \"ipv4\", \"continent_code\": \"NA\", \"continent_name\": \"North America\", \"country_code\": \"US\", \"country_name\": \"United States\", \"region_code\": \"CA\", \"region_name\": \"California\", \"city\": \"Los Angeles\", \"zip\": \"90012\", \"latitude\": 34.0655517578125, \"longitude\": -118.24053955078125, \"location\": {\"geoname_id\": 5368361, \"capital\": \"Washington D.C.\", \"languages\": [{\"code\": \"en\", \"name\": \"English\", \"native\": \"English\"}], \"country_flag\": \"https://assets.ipstack.com/flags/us.svg\", \"country_flag_emoji\": \"\\ud83c\\uddfa\\ud83c\\uddf8\", \"country_flag_emoji_unicode\": \"U+1F1FA U+1F1F8\", \"calling_code\": \"1\", \"is_eu\": false}}";

	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonDataStr.toUtf8());
	EXPECT_TRUE(jsonDoc.isObject());

	QJsonObject jsonData = jsonDoc.object();

	EXPECT_TRUE(DatabaseApp::unique_instance().addDataToDatabase(jsonData));
}
