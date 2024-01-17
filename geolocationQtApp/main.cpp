#include "geolocationQtApp.h"
#include "Credentials.h"

#include <QtWidgets/QApplication>
#include <QString>
#include <QMessageBox>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	if (argc > 1)
	{
		QString apiKey(argv[1]);
		if (!CredentialsInstance::unique_instance().setAPIKey(apiKey))
		{
			// Handle error and close the app
			QMessageBox::critical(nullptr, "App initialization", "Failed to set API key, app will shutdown. Check key.");
			return 1;
		}
	}
	else
	{
		// Handle error and close the app
		QMessageBox::critical(nullptr, "App initialization", "No api key passed to app as argument, app will shutdown. Check parameters.");
		return 1;
	}
	geolocationQtApp w;
	w.show();
	return a.exec();
}
