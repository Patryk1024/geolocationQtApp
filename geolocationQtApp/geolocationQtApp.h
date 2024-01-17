#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include <memory>
#include <vector>

#include "ui_geolocationQtApp.h"
#include "Database.h"
#include "networkmanager.h"
#include "IPDetails.h"
#include "APIWindow.h"
#include "AddDataWindow.h"
#include "CustomSqlTableModel.h"

class geolocationQtApp : public QMainWindow
{
	Q_OBJECT

public:
	geolocationQtApp(QWidget* parent = nullptr);
	~geolocationQtApp();

	bool checkDuplicateIPInTable(const QString& ip);
	void TableInitialization();
	//template <typename DatabaseType>
	//void TableInitialization(std::unique_ptr<DatabaseType>& db);
	//void TableInitialization(std::unique_ptr<Database>& db);

private:
	void closeEvent(QCloseEvent* event) override;

signals:
	void resized();

private slots:
	void onactionAdd_IP_Geolocationriggered();
	void onactionRemove_IP_GeolocationTriggered();
	void onactionAPITriggered();
	void onactionRemove_databaseTriggered();
	void onactionReinitializeTriggered();
	void onAPIWindowClosed();
	void onAddDataWindowClosed();
	void onAcceptAPIButtonClicked(const QString& ipURL, const QString& fullURL);
	void onOKpushButtonClicked(const IPDetails& ipDetails);

	void handleJsonDownload(const QByteArray& data);
	void handleDownloadFailure(const QString& errorString);

private:
	Ui::geolocationQtAppClass ui;
	APIWindow apiWindow;
	AddDataWindow addDataWindow;
	QString lastUrl;
	//NetworkManager* networkManager = nullptr;
	std::shared_ptr<NetworkManager> networkManager;
	std::shared_ptr<CustomSqlTableModel> model;
};
