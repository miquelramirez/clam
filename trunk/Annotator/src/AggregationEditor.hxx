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
#include <QtGui/QTreeWidget>

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

class AggregationEditor : public QTreeWidget
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
		parseSources(config);
		parseMap(config);
	}
	std::string outputConfig()
	{
		std::string config = "\n\n\nsources = [\n";
		for (unsigned i=0; i<mParser.sources.size(); i++)
		{
			Source & source = mParser.sources[i];
			config+=
				"\t(\""+source.source+"\", FileMetadataSource(\n"
				"\t\tpath=\""+source.path+"\",\n"
				"\t\tschemaFile=\""+source.schemaFile+"\",\n"
				"\t\tpoolSuffix=\""+source.suffix+"\",\n"
				"\t\textractor=\""+source.extractor+"\")),\n"
			;
		}
		config	+= "\t]\n";
		config += "map = [\n";
		for (unsigned i=0; i<mParser.maps.size(); i++)
		{
			AttributeMap & map = mParser.maps[i];
			config += 
				"\t(\""+map.targetScope+"::"+map.targetAttribute+"\" , "
					"\""+map.sourceId+"\", "
					"\""+map.sourceScope+"::"+map.sourceAttribute+"\"),\n"
				;
		}
		config	+= "\t]\n";
		std::cout<< "the newly edited Configuration is ..............\n" << config << std::endl;
		return config;	
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
	void editConfiguration();
	

private:
	void parseSources(const std::string & config);
	void parseMap(const std::string & config);
	void renameTarget(QTreeWidgetItem * current);
	QTreeWidgetItem *  hasScope(const std::string & scope,  QTreeWidgetItem * parent);
	std::string parseQuotationMark(const std::string & config, size_t beginPos, size_t limitedPos, std::string keyWord );

private:
	QPixmap sourceIcon;
	QPixmap scopeIcon;
	QPixmap attributeIcon;
		
	ConfigurationParser mParser;
	std::string mConfig;
};

#endif

