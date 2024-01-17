#include "AddDataWindow.h"
#include "RegexValidation.h"

#include <QMessageBox>
#include "geolocationQtApp.h"

AddDataWindow::AddDataWindow(QWidget* parent)
	: QWidget(parent),
	mainApp(nullptr)
{
	try
	{
		ui.setupUi(this);
		formCorrect = false;

		// Set the window flag to keep the widget always on top
		setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		// Set window flags to prevent resizing and only allow the close button
		setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint);
		setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
		setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);
		setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

		// Initially disable the acceptAPIButton
		ui.OKpushButton->setEnabled(false);

		QList<QLineEdit*> lineEdits = {
			ui.IPlineEdit,
			ui.URLlineEdit,
			ui.ContinentCodelineEdit,
			ui.ContinentNamelineEdit,
			ui.CountryCodelineEdit,
			ui.CountryNamelineEdit,
			ui.RegionCodelineEdit,
			ui.RegionNamelineEdit,
			ui.CitylineEdit,
			ui.ZIPCodelineEdit,
			ui.LatitudelineEdit,
			ui.LongitudelineEdit,
			ui.GeonameIdlineEdit,
			ui.CapitallineEdit,
			ui.CountryFlaglineEdit,
			ui.CountryFlagEmojilineEdit,
			ui.CountryFlagEmojiUnicodeIdlineEdit,
			ui.CallingCodelineEdit
		};

		// Connect each QLineEdit to the validateData slot
		for (QLineEdit* lineEdit : lineEdits)
		{
			connect(lineEdit, &QLineEdit::textChanged, this, &AddDataWindow::validateData);
		}

		connect(ui.OKpushButton, &QPushButton::clicked, this, &AddDataWindow::onOKpushButtonClicked);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(this, "Initialization Error", e.what());
	}
}

AddDataWindow::~AddDataWindow()
{}

void AddDataWindow::setMainApp(QMainWindow* app)
{
	mainApp = app;
}

void AddDataWindow::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);
	emit windowClosed(); // Emit signal when the window is closed
}

void AddDataWindow::validateData()
{
	QObject* sender = QObject::sender();
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender);
	if (!lineEdit)
	{
		qDebug() << "Warning! Unknown data is validating";
		return;
	}
	auto lineEditText = lineEdit->text();
	bool valid = true;
	bool noDuplicateIP = true;

	if (sender == ui.IPlineEdit)
	{
		auto v4 = RegexValidation::isValidIPv4(lineEditText);
		auto v6 = RegexValidation::isValidIPv6(lineEditText);
		valid = v4 || v6;
		//propose ip type
		if (valid)
		{
			if (v4)
			{
				ui.TypecomboBox->setCurrentIndex(1);
			}
			else
			{
				ui.TypecomboBox->setCurrentIndex(2);
			}
			if (!mainApp)
			{
				qDebug() << "Error, no pointer to main app window";
				QMessageBox::critical(this, "Add data with API", "No pointer to main app window, error in code");
			}
			geolocationQtApp* specificApp = dynamic_cast<geolocationQtApp*>(mainApp);

			if (!specificApp)
			{
				qDebug() << "Error, mainApp is not of type geolocationQtApp";
				QMessageBox::critical(this, "Add data with API", "mainApp is not of type geolocationQtApp");
				return;
			}
			noDuplicateIP = !(specificApp->checkDuplicateIPInTable(lineEditText));
			if (!noDuplicateIP)
			{
				valid = valid && noDuplicateIP;
			}
		}
	}
	else if (sender == ui.URLlineEdit)
	{
		valid = RegexValidation::isValidUrl(lineEditText);
	}
	else if (sender == ui.ContinentCodelineEdit)
	{
		valid = RegexValidation::isValidGeoCode(lineEditText);
	}
	else if (sender == ui.ContinentNamelineEdit)
	{
		valid = RegexValidation::isValidCityName(lineEditText);
	}
	else if (sender == ui.CountryCodelineEdit)
	{
		valid = RegexValidation::isValidGeoCode(lineEditText);
	}
	else if (sender == ui.CountryNamelineEdit)
	{
		valid = RegexValidation::isValidCityName(lineEditText);
	}
	else if (sender == ui.RegionCodelineEdit)
	{
		valid = RegexValidation::isValidGeoCode(lineEditText);
	}
	else if (sender == ui.RegionNamelineEdit)
	{
		valid = RegexValidation::isValidCityName(lineEditText);
	}
	else if (sender == ui.CitylineEdit)
	{
		valid = RegexValidation::isValidCityName(lineEditText);
	}
	else if (sender == ui.ZIPCodelineEdit)
	{
		valid = RegexValidation::isValidZIP(lineEditText);
	}
	else if (sender == ui.LatitudelineEdit)
	{
		valid = RegexValidation::isValidLatitude(lineEditText.toDouble());
	}
	else if (sender == ui.LongitudelineEdit)
	{
		valid = RegexValidation::isValidLongitude(lineEditText.toDouble());
	}
	else if (sender == ui.GeonameIdlineEdit)
	{
		valid = RegexValidation::isValidGeonameID(lineEditText.toInt());
	}
	else if (sender == ui.CapitallineEdit)
	{
		valid = RegexValidation::isValidCityName(lineEditText);
	}
	//else if (sender == ui.TypecomboBox)
	//{
	//    // not needed
	//}
	else if (sender == ui.CountryFlaglineEdit)
	{
		valid = RegexValidation::isValidUrl(lineEditText);
	}
	else if (sender == ui.CountryFlagEmojilineEdit)
	{
		valid = RegexValidation::isValidUrl(lineEditText);
	}
	else if (sender == ui.CountryFlagEmojiUnicodeIdlineEdit)
	{
		valid = RegexValidation::isValidUrl(lineEditText);
	}
	else if (sender == ui.CallingCodelineEdit)
	{
		valid = RegexValidation::isValidUrl(lineEditText);
	}

	if (valid)
	{
		lineEdit->setStyleSheet("border: 1px solid green");
		ui.StatusDescribeLabel->setText("Status: valid");
	}
	else
	{
		if (!noDuplicateIP)
		{
			lineEdit->setStyleSheet("border: 1px solid yellow");
			ui.StatusDescribeLabel->setText("Status: duplicate ip");
		}
		else
		{
			lineEdit->setStyleSheet("border: 1px solid red"); // Set a red border for incorrect data
			ui.StatusDescribeLabel->setText("Status: invalid");
		}
		if (lineEdit->text().isEmpty())
		{
			lineEdit->setStyleSheet("");
		}
	}
	formCorrect = (valid && (!ui.IPlabel->text().isEmpty() || !ui.URLlineEdit->text().isEmpty()));
	ui.OKpushButton->setEnabled(formCorrect);
}

void AddDataWindow::onOKpushButtonClicked()
{
	IPDetails::Location location(ui.GeonameIdlineEdit->text(),
		ui.CapitallineEdit->text(),
		std::vector<QString>{},
		ui.CountryFlaglineEdit->text(),
		ui.CountryFlagEmojilineEdit->text(),
		ui.CountryFlagEmojiUnicodeIdlineEdit->text(),
		ui.CallingCodelineEdit->text(),
		ui.IsEUcheckBox->isChecked());

	IPDetails ipDetails(ui.IPlineEdit->text(),
		ui.URLlineEdit->text(),
		ui.TypecomboBox->currentText(),
		ui.ContinentCodelineEdit->text(),
		ui.ContinentNamelineEdit->text(),
		ui.CountryCodelineEdit->text(),
		ui.CountryNamelineEdit->text(),
		ui.RegionCodelineEdit->text(),
		ui.RegionNamelineEdit->text(),
		ui.CitylineEdit->text(),
		ui.ZIPCodelineEdit->text(),
		ui.LatitudelineEdit->text(),
		ui.LongitudelineEdit->text(),
		location);

	QList<QLineEdit*> lineEdits = {
		ui.IPlineEdit,
		ui.URLlineEdit,
		ui.ContinentCodelineEdit,
		ui.ContinentNamelineEdit,
		ui.CountryCodelineEdit,
		ui.CountryNamelineEdit,
		ui.RegionCodelineEdit,
		ui.RegionNamelineEdit,
		ui.CitylineEdit,
		ui.ZIPCodelineEdit,
		ui.LatitudelineEdit,
		ui.LongitudelineEdit,
		ui.GeonameIdlineEdit,
		ui.CapitallineEdit,
		ui.CountryFlaglineEdit,
		ui.CountryFlagEmojilineEdit,
		ui.CountryFlagEmojiUnicodeIdlineEdit,
		ui.CallingCodelineEdit
	};

	// Connect each QLineEdit to the validateData slot
	for (QLineEdit* lineEdit : lineEdits)
	{
		lineEdit->clear();
	}
	ui.IsEUcheckBox->setChecked(false);
	ui.TypecomboBox->setCurrentIndex(0);
	emit OKpushButtonClicked(ipDetails);
	close();
}
