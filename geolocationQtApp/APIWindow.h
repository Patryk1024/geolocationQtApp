#pragma once
#include "ui_APIWindow.h"

#include <QWidget>
#include <QtWidgets/QMainWindow>
#include <QCloseEvent>

class APIWindow : public QWidget
{
	Q_OBJECT

public:
	APIWindow(QWidget *parent = nullptr);
	~APIWindow();

	void setMainApp(QMainWindow* app);

signals:
	void windowClosed();
	void acceptButtonClicked(const QString& ipURL, const QString& fullURL);
	void checkDuplicateIPRequested(const QString& ip);

private slots:
	void handleLineEditChanged(const QString& text);
	void onAcceptAPIButtonClicked();

private:
	Ui::APIWindowClass ui;
	QMainWindow* mainApp;

	void closeEvent(QCloseEvent* event) override;
};
