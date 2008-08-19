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

#ifndef SchemaBrowser_hxx
#define SchemaBrowser_hxx

#include <QtCore/QVariant>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>

#include <string>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSplitter;
class QTreeWidget;
class QFrame;
class QSpinBox;
class QLabel;
class QTextBrowser;
class QLineEdit;
class QTreeWidgetItem;
namespace CLAM_Annotator { class Schema; }

class SchemaBrowser : public QWidget
{
    Q_OBJECT

public:
	SchemaBrowser( QWidget* parent = 0, Qt::WFlags fl = 0 );
	~SchemaBrowser();
	void setSchema(CLAM_Annotator::Schema & schema);
	void setListedSchema(CLAM_Annotator::Schema & schema, QTreeWidget* parent);

	void addAttribute(const std::string & scope, const std::string & name, const std::string & type, QTreeWidget* parent);


private:
    QSplitter* splitter1;
    //QTreeWidget* attributeList; 
    QSplitter* splitter2;
    QSpinBox* minSpin;
    QSpinBox* maxSpin;
    QLineEdit* childEdit;
    QLabel* minLabel;
    QLabel* maxLabel;
    QLabel* childLabel;
    QTextBrowser* attributeDocumentation;

protected:
    QHBoxLayout* schemaBrowserLayout;
	QTreeWidget* attributeList; 
    QGridLayout* attributePropertiesLayout;
	QFrame* attributeProperties;

protected slots:
    virtual void languageChange();
    virtual void updateCurrentAttribute();

private:
	QTreeWidgetItem * findAttributeItem(const QString & scope, const QString & name);
	QTreeWidgetItem * findScopeItem(const QString & scope);


private:
    QPixmap scopeIcon;
    QPixmap attributeIcon;

    CLAM_Annotator::Schema * mSchema;
};

#endif // SchemaBrowser_hxx

