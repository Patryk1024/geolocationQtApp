#include "APIWindow.h"
#include "RegexValidation.h"
#include <QMessageBox>
#include "geolocationQtApp.h"

APIWindow::APIWindow(QWidget *parent)
	: QWidget(parent),
	mainApp(nullptr)
{
	try
	{
		ui.setupUi(this);

		// Set the window flag to keep the widget always on top
		setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		// Set window flags to prevent resizing and only allow the close button
		setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint);
		setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
		setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);
		setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

		// Initially disable the acceptAPIButton
		ui.acceptAPIButton->setEnabled(false);

		connect(ui.lineEdit, &QLineEdit::textChanged, this, &APIWindow::handleLineEditChanged);
		connect(ui.acceptAPIButton, &QPushButton::clicked, this, &APIWindow::onAcceptAPIButtonClicked);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(this, "Initialization Error", e.what());
	}
}

APIWindow::~APIWindow()
{}

void APIWindow::setMainApp(QMainWindow* app)
{
	mainApp = app;
}

void APIWindow::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);
	emit windowClosed(); // Emit signal when the window is closed
}

void APIWindow::onAcceptAPIButtonClicked()
{
	QString text = ui.lineEdit->text();
	QString trimmedText = text.trimmed();
	QString trimmedNoPrefixText = trimmedText;
	trimmedText = RegexValidation::isValidUrl(trimmedText) ? trimmedText : QString();

	// Remove URL prefix if present
	if (trimmedNoPrefixText.startsWith("https://"))
	{
		trimmedNoPrefixText.remove(0, QString("https://").length());
	}
	else if (trimmedNoPrefixText.startsWith("http://"))
	{
		trimmedNoPrefixText.remove(0, QString("http://").length());
	}
	ui.lineEdit->clear();
	emit acceptButtonClicked(trimmedNoPrefixText, trimmedText);
	close();
}

void APIWindow::handleLineEditChanged(const QString& text)
{
	// Check if the lineEdit text is empty or not a valid URL or IP
	QString trimmedText = text.trimmed(); // Trim spaces from beginning and end

	bool isValidIP = RegexValidation::isValidIPv4(trimmedText) || RegexValidation::isValidIPv6(trimmedText);
	bool isValid = !trimmedText.isEmpty() && (RegexValidation::isValidUrl(trimmedText) || isValidIP);
	bool noDuplicateIP = true;
	if (isValid)
	{
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
		noDuplicateIP = !(specificApp->checkDuplicateIPInTable(trimmedText));
		if (!noDuplicateIP)
		{
			ui.lineEdit->setStyleSheet("border: 1px solid yellow");
			ui.StatusDescribeLabel->setText("Status: duplicate ip");
			isValid = isValid && noDuplicateIP;
		}
		else
		{
			ui.lineEdit->setStyleSheet("border: 1px solid green");
			ui.StatusDescribeLabel->setText("Status: valid");
		}
	}
	else
	{
		ui.lineEdit->setStyleSheet("border: 1px solid red"); // Set a red border for incorrect data
		ui.StatusDescribeLabel->setText("Status: invalid");
	}

	// Enable/disable the acceptAPIButton based on validity
	ui.acceptAPIButton->setEnabled(isValid);
}
