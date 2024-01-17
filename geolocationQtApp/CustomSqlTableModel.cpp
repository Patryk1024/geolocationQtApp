#include "CustomSqlTableModel.h"
#include "RegexValidation.h"

#include <algorithm>
#include <QMessageBox>

bool CustomSqlTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	bool invalid = false;
	bool noDuplicateIP = true;
	switch (index.column())
	{
	case 0:
		if (!RegexValidation::isValidIPv4(value.toString()) && !RegexValidation::isValidIPv6(value.toString()))
		{
			// Handle invalid IP format here
			//return false; // Reject the edit
			invalid = true;
		}
		else
		{
			if (value.toString() != this->data(index).toString())
			{
				for (int row = 0; row < this->rowCount(); ++row)
				{
					QModelIndex currentIndex = this->index(row, 0); // IP is in the first column
					if (currentIndex.isValid())
					{
						QString existingIP = this->data(currentIndex).toString();
						if (value.toString() == existingIP)
						{
							noDuplicateIP = false;
							invalid = true;
							break;
						}
					}
				}
			}
		}
		break;

	case 2:
		if (!RegexValidation::isValidType(value.toString()))
		{
			invalid = true;
		}
		break;

	case 3:
	case 5:
	case 7:
		if (!RegexValidation::isValidGeoCode(value.toString()))
		{
			invalid = true;
		}
		break;

	case 4:
	case 6:
	case 8:
	case 9:
	case 14:
		if (!RegexValidation::isValidCityName(value.toString()))
		{
			invalid = true;
		}
		break;

	case 10: // ZIP
		if (!RegexValidation::isValidZIP(value.toString()))
		{
			invalid = true;
		}
		break;

	case 11: // Latitude
		if (!RegexValidation::isValidLatitude(value.toDouble()))
		{
			invalid = true;
		}
		break;

	case 12: // Longitude
		if (!RegexValidation::isValidLongitude(value.toDouble()))
		{
			invalid = true;
		}
		break;

	case 13: // Longitude
		if (!RegexValidation::isValidGeonameID(value.toInt()))
		{
			invalid = true;
		}
		break;

	default:
		//qDebug() << "ERROR, unknown data set!";
		break;
	}

	if (invalid)
	{
		if (noDuplicateIP)
		{
			qDebug() << "Warning, check data because it looks invalid\nColumn " + QString::number(index.column()) + " value: " + value.toString();
		}
		else
		{
			qDebug() << "Warning, duplicate ip rejected";
			QMessageBox::information(nullptr, "Data edit", "Rejected duplicated IP");
		}
		return false;
	}

	return QSqlTableModel::setData(index, value, role);
}
