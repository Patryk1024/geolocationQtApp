#include "geolocationQtApp.h"
#include "IPDetails.h"

#include <QJsonDocument>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlError>
#include <QBoxLayout>
#include "Credentials.h"

geolocationQtApp::geolocationQtApp(QWidget* parent)
	: QMainWindow(parent),
	networkManager(std::make_shared<NetworkManager>(this))
{
	try
	{
		ui.setupUi(this);
		lastUrl = QString();
		/*auto& db = DatabaseApp::unique_instance().database;*/
		if (DatabaseApp::unique_instance().initDatabase())
		{
			ui.actionReinitialize->setEnabled(false);
			ui.actionRemove_database->setEnabled(true);
		}
		else
		{
			QMessageBox::warning(this, "Database initialization", "Database initialization falied, try again");
			ui.actionReinitialize->setEnabled(true);
			ui.actionRemove_database->setEnabled(false);
		}

		setCentralWidget(ui.centralWidget);
		QVBoxLayout* layout = new QVBoxLayout(ui.centralWidget);
		layout->addWidget(ui.tableView);

		connect(ui.actionAPI, &QAction::triggered, this, &geolocationQtApp::onactionAPITriggered);
		connect(ui.actionAdd_IP_Geolocation, &QAction::triggered, this, &geolocationQtApp::onactionAdd_IP_Geolocationriggered);
		connect(ui.actionRemove_IP_Geolocation, &QAction::triggered, this, &geolocationQtApp::onactionRemove_IP_GeolocationTriggered);
		connect(ui.actionRemove_database, &QAction::triggered, this, &geolocationQtApp::onactionRemove_databaseTriggered);
		connect(ui.actionReinitialize, &QAction::triggered, this, &geolocationQtApp::onactionReinitializeTriggered);
		//connect(networkManager, &NetworkManager::jsonDownloaded, this, &geolocationQtApp::handleJsonDownload);
		connect(networkManager.get(), &NetworkManager::jsonDownloaded, [this](const QByteArray& data) { handleJsonDownload(data); });
		//connect(networkManager, &NetworkManager::downloadFailed, this, &geolocationQtApp::handleDownloadFailure);
		connect(networkManager.get(), &NetworkManager::downloadFailed, [this](const QString& errorString) { handleDownloadFailure(errorString); });
		connect(&apiWindow, &APIWindow::windowClosed, this, &geolocationQtApp::onAPIWindowClosed);
		connect(&addDataWindow, &AddDataWindow::windowClosed, this, &geolocationQtApp::onAPIWindowClosed);
		connect(&apiWindow, &APIWindow::acceptButtonClicked, this, &geolocationQtApp::onAcceptAPIButtonClicked);
		connect(&addDataWindow, &AddDataWindow::OKpushButtonClicked, this, &geolocationQtApp::onOKpushButtonClicked);

		//TableInitialization(db);
		TableInitialization();

		apiWindow.setMainApp(this);
		addDataWindow.setMainApp(this);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(this, "Initialization Error", e.what());
		// Optionally, rethrow the exception if you want to propagate it further
		// throw;
	}
}

//template <typename DatabaseType>
//void geolocationQtApp::TableInitialization(std::unique_ptr<DatabaseType>& db)
//void geolocationQtApp::TableInitialization(std::unique_ptr<Database>& db)
void geolocationQtApp::TableInitialization()
{
	//CustomSqlTableModel* model = new CustomSqlTableModel();
	model = std::make_shared<CustomSqlTableModel>();

	model->setTable(DatabaseApp::unique_instance().getTableName());
	model->select(); // Populate the model with data from the table

	// Rename columns
	model->setHeaderData(0, Qt::Horizontal, "IP");
	model->setHeaderData(1, Qt::Horizontal, "URL");
	model->setHeaderData(2, Qt::Horizontal, "Type");
	model->setHeaderData(3, Qt::Horizontal, "Continent code");
	model->setHeaderData(4, Qt::Horizontal, "Continent name");
	model->setHeaderData(5, Qt::Horizontal, "Country code");
	model->setHeaderData(6, Qt::Horizontal, "Country name");
	model->setHeaderData(7, Qt::Horizontal, "Region code");
	model->setHeaderData(8, Qt::Horizontal, "Region name");
	model->setHeaderData(9, Qt::Horizontal, "City");
	model->setHeaderData(10, Qt::Horizontal, "ZIP code");
	model->setHeaderData(11, Qt::Horizontal, "Latitude");
	model->setHeaderData(12, Qt::Horizontal, "Longitude");
	model->setHeaderData(13, Qt::Horizontal, "Geoname id");
	model->setHeaderData(14, Qt::Horizontal, "Capital");
	model->setHeaderData(15, Qt::Horizontal, "Country flag");
	model->setHeaderData(16, Qt::Horizontal, "Country flag emoji");
	model->setHeaderData(17, Qt::Horizontal, "Country flag emoji unicode id");
	model->setHeaderData(18, Qt::Horizontal, "Calling code");
	model->setHeaderData(19, Qt::Horizontal, "Is EU");

	ui.tableView->setModel(model.get());
}

geolocationQtApp::~geolocationQtApp()
{
	auto db = QSqlDatabase::QSqlDatabase();
	if (db.isOpen())
	{
		db.close();
	}
}

void geolocationQtApp::closeEvent(QCloseEvent* event)
{
	// Close the APIWindow instance when the main window is closed
	apiWindow.close();
	addDataWindow.close();
	event->accept(); // Accept the close event for the main window
}

bool geolocationQtApp::checkDuplicateIPInTable(const QString& ip)
{
	//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(ui.tableView->model());
	if (model)
	{
		for (int row = 0; row < model->rowCount(); ++row)
		{
			QModelIndex index = model->index(row, 0); // IP is in the first column
			if (index.isValid())
			{
				QString existingIP = model->data(index).toString();
				if (ip == existingIP)
				{
					// Duplicate IP found
					return true;
				}
			}
		}
	}
	// No duplicate IP found
	return false;
}

void geolocationQtApp::onactionRemove_IP_GeolocationTriggered()
{
	//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(sender()->parent()->findChild<QTableView*>()->model());
	//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(ui.tableView->model());
	//QItemSelectionModel* selection = sender()->parent()->findChild<QTableView*>()->selectionModel();
	QItemSelectionModel* selection = ui.tableView->selectionModel();

	if (model && selection)
	{
		QModelIndexList selectedRows = selection->selectedRows();

		if (selectedRows.isEmpty())
		{
			QMessageBox::information(this, "Removing IP details", "Please select row.");
			return;
		}

		QStringList ipsToDelete;
		for (const QModelIndex& index : selectedRows)
		{
			// Get the IP value from the selected row
			QString ip = model->data(model->index(index.row(), 0)).toString();
			ipsToDelete.append(ip);
		}

		// Call the function to remove rows from the database
		if (DatabaseApp::unique_instance().removeRowsFromDatabase(ipsToDelete))
		{
			// Remove the rows from the model
			for (const QModelIndex& index : selectedRows)
			{
				model->removeRow(index.row());
			}

			// Update the view directly
			model->select();
			ui.tableView->setModel(model.get());

			/*if (model->submitAll())
			{
				qDebug() << "Rows deleted successfully.";
				model->select();
				ui.tableView->setModel(model);
			}
			else
			{
				QString errorMsg = "Error deleting rows: " + model->lastError().text();
				qDebug() << errorMsg;
				QMessageBox::critical(this, "Removing IP details", errorMsg);
				model->revertAll();
			}*/
		}
		else
		{
			QMessageBox::critical(this, "Removing IP details", "Error deleting rows from the database.");
		}
	}
}

void geolocationQtApp::onactionAPITriggered()
{
	apiWindow.show();
	this->setEnabled(false); // Disable the main window
	//QUrl url("http://api.ipstack.com/134.201.250.155?access_key=348c5ef56bf34ebc91182562aeefe077");
	//networkManager->downloadJsonData(url);
}

void geolocationQtApp::onactionAdd_IP_Geolocationriggered()
{
	addDataWindow.show();
	this->setEnabled(false); // Disable the main window
}

void geolocationQtApp::onactionRemove_databaseTriggered()
{
	//auto& db = DatabaseApp::unique_instance().database;
	if (DatabaseApp::unique_instance().removeDatabase())
	{
		ui.actionReinitialize->setEnabled(true);
		ui.actionRemove_database->setEnabled(false);
		//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(sender()->parent()->findChild<QTableView*>()->model());
		//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(ui.tableView->model());
		if (model)
		{
			model->select();
			ui.tableView->setModel(model.get());
		}
		QMessageBox::information(this, "Removing database", "Database removed");
	}
	else
	{
		ui.actionReinitialize->setEnabled(false);
		ui.actionRemove_database->setEnabled(true);
		QMessageBox::critical(this, "Removing database", "Database cannot be removed");
	}
}

void geolocationQtApp::onactionReinitializeTriggered()
{
	//auto& db = DatabaseApp::unique_instance().database;
	if (DatabaseApp::unique_instance().initDatabase())
	{
		TableInitialization();
		QMessageBox::information(this, "Database reinitialization", "Database reinitialization completed");
		ui.actionReinitialize->setEnabled(false);
		ui.actionRemove_database->setEnabled(true);
	}
	else
	{
		QMessageBox::warning(this, "Database reinitialization", "Database reinitialization falied, try again");
		ui.actionReinitialize->setEnabled(true);
		ui.actionRemove_database->setEnabled(false);
	}
}

void geolocationQtApp::handleJsonDownload(const QByteArray& data)
{
	// Process the downloaded JSON data here
	auto jsonDataStr = QString::fromUtf8(data);

	auto jsonDoc = QJsonDocument::fromJson(jsonDataStr.toUtf8());
	if (!jsonDoc.isObject()) {
		qDebug() << "Invalid JSON data!";
		QMessageBox::warning(this, "Add data with API", "Ivalid JSON data!");
	}

	auto jsonData = jsonDoc.object();
	//auto& db = DatabaseApp::unique_instance().database;
	if (DatabaseApp::unique_instance().addDataToDatabase(jsonData, lastUrl))
	{
		//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(sender()->parent()->findChild<QTableView*>()->model());
		//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(ui.tableView->model());
		if (model)
		{
			model->select();
			ui.tableView->setModel(model.get());
		}
	}
	else
	{
		QMessageBox::warning(this, "Add data with API", "There was problem with adding data to database\n" + DatabaseApp::unique_instance().getLastErrorFromQuery());
	}
}

void geolocationQtApp::handleDownloadFailure(const QString& errorString)
{
	// Handle error when downloading
	qDebug() << "Download failed:" << errorString;
	QMessageBox::warning(this, "Add data with API", "There was problem with adding data to database:\n" + errorString);
}

void geolocationQtApp::onAPIWindowClosed()
{
	this->setEnabled(true); // Re-enable the main window
}

void geolocationQtApp::onAddDataWindowClosed()
{
	this->setEnabled(true); // Re-enable the main window
}

void geolocationQtApp::onAcceptAPIButtonClicked(const QString& ipURL, const QString& fullURL)
{
	QUrl url("http://api.ipstack.com/" + ipURL + "?access_key=" + CredentialsInstance::unique_instance().getAPIKey());
	lastUrl = fullURL;
	networkManager->downloadJsonData(url);
	apiWindow.close();
	this->setEnabled(true);
}

void geolocationQtApp::onOKpushButtonClicked(const IPDetails& ipDetails)
{
	//auto& db = DatabaseApp::unique_instance().database;
	if (DatabaseApp::unique_instance().addDataToDatabase(ipDetails))
	{
		//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(sender()->parent()->findChild<QTableView*>()->model());
		//CustomSqlTableModel* model = qobject_cast<CustomSqlTableModel*>(ui.tableView->model());
		if (model)
		{
			model->select();
			ui.tableView->setModel(model.get());
		}
	}
	else
	{
		QMessageBox::critical(&addDataWindow, "Add IP/URL data", "There was problem with adding data to database\n" + DatabaseApp::unique_instance().getLastErrorFromQuery());
	}
	addDataWindow.close();
	this->setEnabled(true);
}
