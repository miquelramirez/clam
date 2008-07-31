/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef ConfigurationEditor_hxx
#define ConfiugrationEditor_hxx

#include "ui_ConfigurationEditor.hxx"
#include <QtCore/QFile>


class ConfigurationEditor : public QDialog
{
		Q_OBJECT
	public:
		ConfigurationEditor(QWidget * parent =0)
			: QDialog(parent)
		{
			_ui.setupUi(this);
		}
		virtual ~ConfigurationEditor();

		void setConfiguration(QString filePath);

	private slots:
		void on_okButton_clicked();
		
	private:
		Ui::ConfigurationEditor _ui;
		QFile * mfile;
};

#endif//ConfigurationEditor_hxx

