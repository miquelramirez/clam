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

#include "AggregationEditor.hxx"
#include "Assert.hxx"

#include <CLAM/XMLStorage.hxx>
#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QTextBrowser>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QFrame>
#include <iostream>

AggregationEditor::AggregationEditor(QWidget *parent)
    : QTreeWidget( parent)
	, _project(0)
    , sourceIcon(":/icons/images/gear.png")
    , scopeIcon(":/icons/images/xkill.png")
    , attributeIcon(":/icons/images/label.png")
	, _reloading(false)
{
	setHeaderLabels( QStringList()
		<< tr( "Source" )
		<< tr( "Target" )
		);
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(editConfiguration()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
AggregationEditor::~AggregationEditor()
{
}

void AggregationEditor::bindProject(CLAM_Annotator::Project & project)
{
	_project = & project;
	reloadProject();
}

void AggregationEditor::reloadProject()
{
	_reloading = true;
	clear();
	for (unsigned i=0; i<_project->GetSources().size(); i++)
	{
		CLAM_Annotator::Extractor & extractor = _project->GetSources()[i];
		addSource(extractor.GetName(), extractor.schema());
	}
	show();
	resizeColumnToContents(0);
	resizeColumnToContents(1);
	show();	
	_reloading = false;
}

void AggregationEditor::addSource(const std::string & source, const CLAM_Annotator::Schema & schema)
{

	QTreeWidgetItem * sourceItem = 0;
	sourceItem = new QTreeWidgetItem(this);
	sourceItem->setText( 0, source.c_str() );
	sourceItem->setIcon( 0, sourceIcon );
	expandItem(sourceItem);
	
	typedef std::list<CLAM_Annotator::SchemaAttribute> SchemaAttributes;
	SchemaAttributes & attributes = schema.GetAttributes();
	for (SchemaAttributes::iterator it = attributes.begin();
			it!=attributes.end();
			it++)
	{
		addAttribute(it->GetScope(), it->GetName(), sourceItem);
	}
}

void AggregationEditor::addAttribute(const std::string & scope, const std::string & name, QTreeWidgetItem * parent)
{
	QFont changedFont=font();
	changedFont.setBold(true);
	Qt::CheckState  state=Qt::Unchecked;
	QTreeWidgetItem * scopeItem = hasScope(scope, parent);
	if(not scopeItem)
	{
		scopeItem = new QTreeWidgetItem( parent);
		scopeItem->setText( 0, scope.c_str() );
		scopeItem->setIcon( 0, scopeIcon );
		parent->addChild(scopeItem);
		expandItem(scopeItem);		
	}
	QTreeWidgetItem * item = new QTreeWidgetItem;
	item->setText( 0, name.c_str() );
	item->setIcon( 0, attributeIcon );
	std::string source = parent->text(0).toStdString();
	for(unsigned i=0; i<_project->GetMaps().size(); i++)
	{
		const CLAM_Annotator::AttributeMap & map = _project->GetMaps()[i];
		if (map.GetSource() != source) continue;
		if (map.GetSourceAttribute() != name) continue;
		state=Qt::Checked;
		item->setText(1, map.GetTargetAttribute().c_str());
		scopeItem->setText(1, map.GetTargetScope().c_str());
		bool renamed = map.GetTargetAttribute() != map.GetSourceAttribute();
		if (renamed) item->setFont(1,changedFont);
		bool scopeRenamed = map.GetTargetScope() != map.GetSourceScope();
		if (scopeRenamed) scopeItem->setFont(1,changedFont);
		break;
	}
	item->setCheckState( 0, state);
	scopeItem->addChild(item);
}


QTreeWidgetItem *  AggregationEditor::hasScope(const std::string & scope,  QTreeWidgetItem * parent)
{
	for (int i=0; i<parent->childCount(); i++)
	{
		QTreeWidgetItem * scopeItem = parent->child(i);
		if (scopeItem->text(0)==scope.c_str()) return scopeItem;
	}
	return 0;
}


void AggregationEditor::loadConfig(std::string config)
{
	parseSources(config);
	parseMap(config);
}
 // TODO: dealing with exceptions
 // TODO: allowing "#" in the script
void AggregationEditor::parseSources(const std::string & config)
{
	unsigned arraySize = 0;
	for (size_t pos = 0; true; pos++)
	{
		pos=config.find("FileMetadataSource", pos);
		if (pos == std::string::npos) break;
		arraySize++;
	}
	if(!arraySize)
	{
		std::cout<< "There is no source. Please check whether the Aggregator is correctly configured." <<std::endl;//todo: replace this with unique Failure assertion
		return;
	}

	size_t posStart = config.find("sources", 0);
	size_t posEnd = config.find("),", posStart+1);
	size_t posSourcesEnd = config.find("]", posStart+1);

	_project->GetSources().resize(arraySize);
	for(unsigned i=0; i<arraySize && posEnd<posSourcesEnd; i++)
	{
		CLAM_Annotator::Extractor & source = _project->GetSources()[i];
		source.SetName(parseQuotationMark(config, posStart, posEnd, "("));
		parseQuotationMark(config, posStart, posEnd, "path");
		source.SetSchema(parseQuotationMark(config, posStart, posEnd, "schemaFile"));
		source.SetPoolSuffix(parseQuotationMark(config, posStart, posEnd, "poolSuffix"));
		source.SetExtractor(parseQuotationMark(config, posStart, posEnd, "extractor"));
		posStart = posEnd;
		posEnd = config.find("),", posStart+1);		
	}
}

void AggregationEditor::parseMap(const std::string & config) 
{
	std::string::size_type posStart=0;
	std::string::size_type posA, posB, posEnd, posAttributesEnd;

	posStart = config.find("map",0);         //keyword "map" 
	posA = posStart;	
	posEnd = config.find("]", posStart+1);
	posAttributesEnd = posEnd; 

	unsigned arraySize = 0;
	while(true)  
	{
		posA = config.find("),", posA+1);  //key word"),"
		if (posA>=posEnd) break;
		arraySize++;
	}

	if(!arraySize)	return;
	_project->GetMaps().resize(arraySize);
	posEnd = config.find("),", posStart+1);
	for(unsigned i=0; i<arraySize && posEnd<posAttributesEnd; i++)
	{
		CLAM_Annotator::AttributeMap & map = _project->GetMaps()[i];
		posA = config.rfind("::", posEnd);
		posB = config.rfind("\"", posEnd);
		map.SetSourceAttribute(config.substr(posA+2, posB-posA-2));
		posB = config.rfind("\"", posA);
		map.SetSourceScope(config.substr(posB+1, posA-posB-1));
		posA = config.rfind("\"", posB-1);
		posB = config.rfind("\"", posA-1);
		map.SetSource(config.substr(posB+1, posA-posB-1));
		
		posA = config.rfind("::", posB-1);
		posB = config.rfind("\"", posB-1);
		map.SetTargetAttribute(config.substr(posA+2, posB-posA-2));
		posB = config.rfind("\"", posA);
		map.SetTargetScope (config.substr(posB+1, posA-posB-1));

		posEnd = config.find("),", posEnd+1);		
	}	
}



std::string AggregationEditor::parseQuotationMark(const std::string & config, size_t beginPos, size_t limitedPos, std::string keyWord )
{
	std::string::size_type pos2;
	pos2 = config.find(keyWord, beginPos);
	if(pos2==std::string::npos||pos2>limitedPos)
	{
		std::cout<< "can not find" << keyWord << std::endl;
		return "";		
	}
	else
	{
		beginPos = config.find("\"", pos2+1, 1);
		pos2= config.find("\"",beginPos+1);
		return config.substr(beginPos+1,pos2-beginPos-1); 
	}
}

std::string AggregationEditor::outputConfig()
{
	std::string config = "\nsources = [\n";
	for (unsigned i=0; i<_project->GetSources().size(); i++)
	{
		CLAM_Annotator::Extractor & source = _project->GetSources()[i];
		config+=
			"\t(\""+source.GetName()+"\", FileMetadataSource(\n"
			"\t\tpath=\""+"."+"\",\n"
			"\t\tschemaFile=\""+source.GetSchema()+"\",\n"
			"\t\tpoolSuffix=\""+source.GetPoolSuffix()+"\",\n"
			"\t\textractor=\""+source.GetExtractor()+"\")),\n"
		;
	}
	config	+= "\t]\n";
	config += "map = [\n";
	for (unsigned i=0; i<_project->GetMaps().size(); i++)
	{
		CLAM_Annotator::AttributeMap & map = _project->GetMaps()[i];
		config += 
			"\t(\""+map.GetTargetScope()+"::"+map.GetTargetAttribute()+"\" , "
				"\""+map.GetSource()+"\", "
				"\""+map.GetSourceScope()+"::"+map.GetSourceAttribute()+"\"),\n"
			;
	}
	config	+= "\t]\n";
	std::cout<< "the newly edited Configuration is ..............\n" << config << std::endl;
	return config;	
}

void AggregationEditor::renameTarget(QTreeWidgetItem * current)
{
	current->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsEditable);
	editItem(current, 1);
}

void AggregationEditor::editConfiguration()
{
	QTreeWidgetItem * current = currentItem();
	if(!current) return;
	QTreeWidgetItem * parent=current->parent();

	if(!parent) return;   //Source
	// Scope or attribute
	renameTarget(current);

}

void AggregationEditor::takeMaps()
{
	if (_reloading) return;
	_project->GetMaps().clear();
	for(int i=0; i<topLevelItemCount(); i++)  //ugly.....is there any clearer way?
	{
		QTreeWidgetItem * sourceItem = topLevelItem(i);
		for(int j=0; j<sourceItem->childCount();j++)
		{
			QTreeWidgetItem * scopeItem = sourceItem->child(j);
			for(int k=0; k<scopeItem->childCount();k++)
			{
				QTreeWidgetItem * attributeItem = scopeItem->child(k);
				if(attributeItem->checkState(0)!=Qt::Checked) continue;
				CLAM_Annotator::AttributeMap map;
				map.SetTargetScope(scopeItem->text(1).toStdString());
				map.SetTargetAttribute(attributeItem->text(1).toStdString());
				map.SetSource(sourceItem->text(0).toStdString());
				map.SetSourceScope(scopeItem->text(0).toStdString());
				map.SetSourceAttribute(attributeItem->text(0).toStdString());
				if (map.GetTargetScope()=="") map.SetTargetScope(map.GetSourceScope());
				if (map.GetTargetAttribute()=="") map.SetTargetAttribute(map.GetSourceAttribute());
				_project->GetMaps().push_back(map);
			}
		}
	}
	_project->SetConfiguration(outputConfig());
}




