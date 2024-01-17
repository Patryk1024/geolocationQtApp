#pragma once

#include "ui_AddDataWindow.h"
#include "IPDetails.h"

#include <QWidget>
#include <QtWidgets/QMainWindow>
#include <QCloseEvent>

class AddDataWindow : public QWidget
{
	Q_OBJECT

public:
	AddDataWindow(QWidget *parent = nullptr);
	~AddDataWindow();

	void setMainApp(QMainWindow* app);

signals:
	void windowClosed();
	void OKpushButtonClicked(const IPDetails& ipDetails);

private slots:
	void validateData();
	void onOKpushButtonClicked();

private:
	Ui::AddDataWindowClass ui;
	bool formCorrect;
	QMainWindow* mainApp;

	void closeEvent(QCloseEvent* event) override;
};
