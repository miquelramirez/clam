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

#ifndef AggregationEditor_hxx
#define AggregationEditor_hxx

#include "Schema.hxx"
#include "SchemaBrowser.hxx"
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

class AggregationEditor : public SchemaBrowser
{
    Q_OBJECT

public:
	AggregationEditor( SchemaBrowser* parent = 0, Qt::WFlags fl = 0 );
	~AggregationEditor();
	void setSchema();
	void addAttribute(const std::string & scope, const std::string & name, QTreeWidgetItem* parent);
	void setListedSchema(CLAM_Annotator::Schema & schema, QTreeWidgetItem* parent);
	void addSource(const std::string & source, CLAM_Annotator::Schema & schema);
	void loadConfig(std::string config)
	{
		mConfig=config;
	}
private:
	/*QSplitter* splitter1;
	//QTreeWidget* attributeList;
	QSplitter* splitter2;
	QFrame* attributeProperties;
	QSpinBox* minSpin;
	QSpinBox* maxSpin;
	QLineEdit* childEdit;
	QLabel* minLabel;
	QLabel* maxLabel;
	QLabel* childLabel;
	QTextBrowser* attributeDocumentation;*/

struct configurationParser
{
  std::string * source, *extractor, *suffix, *schemaFile, *configFile, *path;
  std::string *targetScope, *targetAttribute, *sourceId, *sourceScope, *sourceAttribute;
};

	

protected:
    QHBoxLayout* aggregationEditorLayout;
    //QGridLayout* attributePropertiesLayout;

protected slots:
	void languageChange();
	void updateCurrentAttribute();
	void onDoubleClick();

private:
	int parseSources();
	int parseMap();
	void editSource();
	void renameTarget();
	QTreeWidgetItem *  hasScope(const std::string & scope,  QTreeWidgetItem * parent);
	std::string parseQuotationMark(std::string::size_type beginPos, std::string::size_type limitedPos, std::string keyWord );

private:
	QPixmap sourceIcon;
	QPixmap scopeIcon;
	QPixmap attributeIcon;

		
	configurationParser mParser;
	std::string mConfig;
	//CLAM_Annotator::Schema * mSchema;
	//CLAM_Annotator::Schema * mSourceSchema;
	
};

#endif

