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

AggregationEditor::AggregationEditor(QWidget *parent, Qt::WFlags fl )
    : QTreeWidget( parent)
    , sourceIcon(":/icons/images/gear.png")
    , scopeIcon(":/icons/images/xkill.png")
    , attributeIcon(":/icons/images/label.png")
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

typedef std::list<CLAM_Annotator::SchemaAttribute> SchemaAttributes;
void AggregationEditor::addSource(const std::string & source, CLAM_Annotator::Schema & schema)
{

	QTreeWidgetItem * sourceItem = 0;
	sourceItem = new QTreeWidgetItem(this);
	sourceItem->setText( 0, source.c_str() );
	sourceItem->setIcon( 0, sourceIcon );
	expandItem(sourceItem);
	
	setListedSchema(schema, sourceItem);	
	
}

void AggregationEditor::loadProject(const CLAM_Annotator::Project & project)
{
	for (unsigned i=0; i<project.GetSources().size(); i++)
	{
		const CLAM_Annotator::Extractor & extractor = project.GetSources()[i];
		Source source;
		source.source = extractor.GetName();
		source.path = ".";
		source.schemaFile = extractor.GetSchema();
		source.suffix = extractor.GetPoolSuffix();
		source.extractor = extractor.GetExtractor();
	}
}

void AggregationEditor::setSchema()
{	
	clear();

	for(unsigned i = 0; i <mParser.sources.size(); i++)
	{
		CLAM_Annotator::Schema sourceSchema;
		Source & source = mParser.sources[i];
		CLAM::XMLStorage::Restore(sourceSchema, source.path+"/"+source.schemaFile);
		addSource(source.source, sourceSchema);
	}
	show();
	resizeColumnToContents(0);
	resizeColumnToContents(1);
	show();	
}

void AggregationEditor::addAttribute(const std::string & scope, const std::string & name, QTreeWidgetItem * parent)
{
	QTreeWidgetItem * scopeItem = 0;
	Qt::CheckState  state=Qt::Unchecked;
	if(!(scopeItem=hasScope(scope, parent)))
	{
		scopeItem = new QTreeWidgetItem( parent);
		scopeItem->setText( 0, scope.c_str() );
		scopeItem->setIcon( 0, scopeIcon );
		expandItem(scopeItem);		
	}
	
	QTreeWidgetItem * item = new QTreeWidgetItem( scopeItem);
	item->setText( 0, name.c_str() );
	item->setIcon( 0, attributeIcon );
	
	for(unsigned i=0; i<mParser.maps.size(); i++)
	{
		if (mParser.maps[i].sourceAttribute != name) continue;
		if (mParser.maps[i].sourceId.c_str()!=parent->text(0)) continue;
		state=Qt::Checked;
		item->setText(1, mParser.maps[i].targetAttribute.c_str());
		scopeItem->setText(1, mParser.maps[i].targetScope.c_str());
	}

	item->setCheckState( 0, state);
}


void AggregationEditor::setListedSchema(CLAM_Annotator::Schema & schema, QTreeWidgetItem * parentItem)
{
	//mSchema = &schema;
	SchemaAttributes & attributes = schema.GetAttributes();
	for (SchemaAttributes::iterator it = attributes.begin();
			it!=attributes.end();
			it++)
	{
		addAttribute(it->GetScope(), it->GetName(), parentItem);
	}

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


 // TODO: dealing with exceptions
 // TODO: allowing "#" in the script
void AggregationEditor::parseSources(const std::string & config)
{
	std::string::size_type posStart=0;
	std::string::size_type posEnd, posSourcesEnd;

	unsigned arraySize = 0;

	while (true) 
	{
		posStart=config.find("FileMetadataSource", posStart+1,18);  //Keyword "FileMetadataSource" 
		if (posStart == std::string::npos) break;
		arraySize++;
	}
	if(!arraySize)
	{
		std::cout<< "There is no source. Please check whether the Aggregator is correctly configured." <<std::endl;//todo: replace this with unique Failure assertion
		return;
	}

	posStart = config.find("sources", 0);
	posEnd = config.find("),", posStart+1);
	posSourcesEnd = config.find("]", posStart+1);

	mParser.sources.resize(arraySize);
	for(unsigned i=0; i<arraySize && posEnd<posSourcesEnd; i++)
	{
		Source & source = mParser.sources[i];
		source.source = parseQuotationMark(config, posStart, posEnd, "(");
		source.path = parseQuotationMark(config, posStart, posEnd, "path");
		source.schemaFile = parseQuotationMark(config, posStart, posEnd, "schemaFile");
		source.suffix = parseQuotationMark(config, posStart, posEnd, "poolSuffix");
		source.extractor = parseQuotationMark(config, posStart, posEnd, "extractor");

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
	mParser.maps.resize(arraySize);

	posEnd = config.find("),", posStart+1);
	
	for(unsigned i=0; i<arraySize && posEnd<posAttributesEnd; i++)
	{
		AttributeMap & map = mParser.maps[i];
		posA = config.rfind("::", posEnd);
		posB = config.rfind("\"", posEnd);
		map.sourceAttribute = config.substr(posA+2, posB-posA-2);
		posB = config.rfind("\"", posA);
		map.sourceScope = config.substr(posB+1, posA-posB-1);
		posA = config.rfind("\"", posB-1);
		posB = config.rfind("\"", posA-1);
		map.sourceId = config.substr(posB+1, posA-posB-1);
		
		posA = config.rfind("::", posB-1);
		posB = config.rfind("\"", posB-1);
		map.targetAttribute = config.substr(posA+2, posB-posA-2);
		posB = config.rfind("\"", posA);
		map.targetScope = config.substr(posB+1, posA-posB-1);

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

void AggregationEditor::setConfiguration()
{
	mParser.maps.clear();
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
				//the default names will be automatically writen after "accepted"
				if(attributeItem->text(1)=="") attributeItem->setText(1, attributeItem->text(0));
				if(scopeItem->text(1)=="") scopeItem->setText(1, scopeItem->text(0));
				AttributeMap map;
				map.targetScope = scopeItem->text(1).toStdString();
				map.targetAttribute = attributeItem->text(1).toStdString();
				map.sourceId = sourceItem->text(0).toStdString();
				map.sourceScope = scopeItem->text(0).toStdString();
				map.sourceAttribute = attributeItem->text(0).toStdString();
				mParser.maps.push_back(map);
			}
		}
	}
}



