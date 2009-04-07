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
#include "Project.hxx"
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
	AggregationEditor( QWidget * parent = 0, Qt::WFlags fl = 0 );
	~AggregationEditor();
	void setSchema();
	void setConfiguration();
	void addAttribute(const std::string & scope, const std::string & name, QTreeWidgetItem* parent);
	void setListedSchema(CLAM_Annotator::Schema & schema, QTreeWidgetItem* parent);
	void addSource(const std::string & source, CLAM_Annotator::Schema & schema);
	void loadConfig(std::string config)
	{
		mConfig=config;
	}
	std::string outputConfig()
	{
		return mConfig;	
	}

	struct Source
	{
		std::string source;
		std::string extractor;
		std::string suffix;
		std::string schemaFile;
		std::string configFile;
		std::string path;
	};

	struct AttributeMap
	{
		std::string targetScope;
		std::string targetAttribute;
		std::string sourceId;
		std::string sourceScope;
		std::string sourceAttribute;
	};

	struct ConfigurationParser
	{
		std::vector<Source> sources;
		std::vector<AttributeMap> maps;
	};

	

protected:
    QHBoxLayout* aggregationEditorLayout;
    //QGridLayout* attributePropertiesLayout;

protected slots:
	void languageChange();
	void updateCurrentAttribute();
	void editConfiguration();
	

private:
	int parseSources();
	int parseMap();
	void editSource(QTreeWidgetItem * current);
	void renameTarget(QTreeWidgetItem * current);
	QTreeWidgetItem *  hasScope(const std::string & scope,  QTreeWidgetItem * parent);
	std::string parseQuotationMark(std::string::size_type beginPos, std::string::size_type limitedPos, std::string keyWord );

private:
	QPixmap sourceIcon;
	QPixmap scopeIcon;
	QPixmap attributeIcon;
		
	ConfigurationParser mParser;
	std::string mConfig;

};

#endif

