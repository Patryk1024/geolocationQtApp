#pragma once
#include <qsqltablemodel.h>
class CustomSqlTableModel : public QSqlTableModel
{
	using QSqlTableModel::QSqlTableModel;
	Q_OBJECT
public:
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

private:

};
