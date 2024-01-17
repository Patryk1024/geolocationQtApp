#pragma once
#include <QString>

class CredentialsInstance
{
private:
	// This is simple class AND NOT SECURE, just for this solution
	class Credentials
	{
	public:
		bool setAPIKey(QString key);
		const QString getAPIKey() const;
	private:
		QString apiKey = "";
		const QString debugAPIKey = "";	//not secure, just for testing this solution
	};

public:
	static CredentialsInstance& unique_instance();

	bool setAPIKey(QString key);
	const QString getAPIKey() const;

private:
	CredentialsInstance();
	std::unique_ptr<Credentials> credentials;
};
