/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef QFileLineEdit_hxx
#define QFileLineEdit_hxx

#include <QtGui/QLineEdit>
#include <QtGui/QFileDialog>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>

class QFileLineEdit : public QWidget
{
	Q_OBJECT
	QLineEdit * _lineEdit;
	QPushButton * _launcher;
	QString _dialogCaption;
	QString _filters;
	bool _isReading;
	bool _isDir;
public:
	QFileLineEdit(QWidget * parent=0)
	: QWidget(parent)
	, _isReading(true)
	, _isDir(false)
	{
		QHBoxLayout * layout = new QHBoxLayout(this);
		_lineEdit = new QLineEdit(this);
		_lineEdit->setMinimumWidth(300);
		_launcher = new QPushButton(QIcon(":/icons/images/fileopen.png"), "...", this);
		_launcher->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
		connect(_launcher, SIGNAL(clicked()), this, SLOT(openFileDialog()));
		layout->addWidget(_lineEdit);
		layout->addWidget(_launcher);
	}
	~QFileLineEdit();
	void setLocation(const QString & location)
	{
		_lineEdit->setText(location);
	}
	QString location() const
	{
		return _lineEdit->text();
	}
	void setDialogCaption(const QString & caption)
	{
		_dialogCaption = caption;
	}
	void setFilters(const QString & filters)
	{
		_filters = filters;
	}
	void setWriteMode(bool beWriteMode = true)
	{
		_isReading = ! beWriteMode;
	}
	void setDirMode(bool beDirMode = true)
	{
		_isDir = beDirMode;
	}
public slots:
	void openFileDialog()
	{
		QString file;
		if (_isDir)
			file = QFileDialog::getExistingDirectory(this,
				_dialogCaption, _lineEdit->text());
		else if (_isReading)
			file = QFileDialog::getOpenFileName(this,
				_dialogCaption, _lineEdit->text(), _filters);
		else
			file = QFileDialog::getSaveFileName(this,
				_dialogCaption, _lineEdit->text(), _filters);

		if (file.isEmpty()) return;
		_lineEdit->setText(file);
	}
};




#endif // QFileLineEdit_hxx

