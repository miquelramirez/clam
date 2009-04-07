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
#include "SourceEditor.hxx"
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
    : SchemaBrowser( parent, fl )
    , sourceIcon(":/icons/images/gear.png")
    , scopeIcon(":/icons/images/xkill.png")
    , attributeIcon(":/icons/images/label.png")
{
	setObjectName( "aggregationEditor" );

	attributeList->setHeaderLabels( QStringList()
		<< tr( "Source" )
		<< tr( "Target" )
		);
	connect(attributeList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
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
	sourceItem = new QTreeWidgetItem(attributeList);
	sourceItem->setText( 0, source.c_str() );
	sourceItem->setIcon( 0, sourceIcon );
	attributeList->expandItem(sourceItem);
	
	setListedSchema(schema, sourceItem);	
	
}

void AggregationEditor::setSchema()
{	
	//attributeProperties->hide();
	attributeList->clear();
	parseSources();

	CLAM_Annotator::Schema sourceSchema;
	for(int i = 0; i <mParser.sources.size(); i++)
	{
		Source & source = mParser.sources[i];
		CLAM::XMLStorage::Restore(sourceSchema, source.path+"/"+source.schemaFile);
		addSource(source.source, sourceSchema);
	}
	attributeList->show();
	attributeList->resizeColumnToContents(0);
	attributeList->resizeColumnToContents(1);
	attributeList->show();	
}

void AggregationEditor::addAttribute(const std::string & scope, const std::string & name, QTreeWidgetItem * parent)
{
	QTreeWidgetItem * scopeItem = 0;
	parseMap();
	Qt::CheckState  state=Qt::Unchecked;
	if(!(scopeItem=hasScope(scope, parent)))
	{
		scopeItem = new QTreeWidgetItem( parent);
		scopeItem->setText( 0, scope.c_str() );
		scopeItem->setIcon( 0, scopeIcon );
		attributeList->expandItem(scopeItem);		
	}
	
	QTreeWidgetItem * item = new QTreeWidgetItem( scopeItem);
	item->setText( 0, name.c_str() );
	item->setIcon( 0, attributeIcon );
	
	for(int i=0; i<mParser.maps.size(); i++)
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
	attributeProperties->hide();
	//mSchema = &schema;
	SchemaAttributes & attributes = schema.GetAttributes();
	for (SchemaAttributes::iterator it = attributes.begin();
			it!=attributes.end();
			it++)
	{
		addAttribute(it->GetScope(), it->GetName(), parentItem);
	}

	attributeList->show();
	attributeList->resizeColumnToContents(0);
	attributeList->resizeColumnToContents(1);
	attributeList->show();
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
void AggregationEditor::parseSources()
{
	std::string::size_type posStart=0;
	std::string::size_type posEnd, posSourcesEnd;

	int arraySize = -1;

	while(posStart!=std::string::npos) 
	{
		posStart=mConfig.find("FileMetadataSource", posStart+1,18);  //Keyword "FileMetadataSource" 
		arraySize++;
	}
	if(!arraySize)
	{
		std::cout<< "There is no source. Please check whether the Aggregator is correctly configured." <<std::endl;//todo: replace this with unique Failure assertion
		return;
	}

	mParser.sources.resize(arraySize);

	posStart = mConfig.find("sources", 0);
	posEnd = mConfig.find("),", posStart+1);
	posSourcesEnd = mConfig.find("]", posStart+1);

	for(int i=0; i<arraySize && posEnd<posSourcesEnd; i++)
	{
		Source & source = mParser.sources[i];
		source.source = parseQuotationMark(posStart, posEnd, "(");
		source.path = parseQuotationMark(posStart, posEnd, "path");
		source.schemaFile = parseQuotationMark(posStart, posEnd, "schemaFile");
		source.suffix = parseQuotationMark(posStart, posEnd, "poolSuffix");
		source.extractor = parseQuotationMark(posStart, posEnd, "extractor");

		posStart = posEnd;
		posEnd = mConfig.find("),", posStart+1);		
	}
}

void AggregationEditor::parseMap() 
{
	std::string::size_type posStart=0;
	std::string::size_type posA, posB, posEnd, posAttributesEnd;

	int arraySize = -1;

	posStart = mConfig.find("map",0);         //keyword "map" 
	posA = posStart;	
	posEnd = mConfig.find("]", posStart+1);
	posAttributesEnd = posEnd; 

	while(posA<posEnd)  
	{
		posA = mConfig.find("),", posA+1);  //key word"),"
		arraySize++;
	}

	if(!arraySize)	return;
	mParser.maps.resize(arraySize);

	posEnd = mConfig.find("),", posStart+1);
	
	for(int i=0; i<arraySize && posEnd<posAttributesEnd; i++)
	{
		AttributeMap & map = mParser.maps[i];
		posA = mConfig.rfind("::", posEnd);
		posB = mConfig.rfind("\"", posEnd);
		map.sourceAttribute = mConfig.substr(posA+2, posB-posA-2);
		posB = mConfig.rfind("\"", posA);
		map.sourceScope = mConfig.substr(posB+1, posA-posB-1);
		posA = mConfig.rfind("\"", posB-1);
		posB = mConfig.rfind("\"", posA-1);
		map.sourceId = mConfig.substr(posB+1, posA-posB-1);
		
		posA = mConfig.rfind("::", posB-1);
		posB = mConfig.rfind("\"", posB-1);
		map.targetAttribute = mConfig.substr(posA+2, posB-posA-2);
		posB = mConfig.rfind("\"", posA);
		map.targetScope = mConfig.substr(posB+1, posA-posB-1);

		posEnd = mConfig.find("),", posEnd+1);		
	}	
}



std::string AggregationEditor::parseQuotationMark(std::string::size_type beginPos, std::string::size_type limitedPos, std::string keyWord )
{
	std::string::size_type pos2;
	pos2 = mConfig.find(keyWord, beginPos);
	if(pos2==std::string::npos||pos2>limitedPos)
	{
		std::cout<< "can not find" << keyWord << std::endl;
		return "";		
	}
	else
	{
		beginPos = mConfig.find("\"", pos2+1, 1);
		pos2= mConfig.find("\"",beginPos+1);
		return mConfig.substr(beginPos+1,pos2-beginPos-1); 
	}
	
}

void AggregationEditor::updateCurrentAttribute()
{
	return;
}

void AggregationEditor::languageChange()
{
	return;
}

void AggregationEditor::editSource(QTreeWidgetItem * current)
{
	//pop out a new QDialog with "Source   Extractor   Suffix   SchemaFile  ConfigFile" list
	SourceEditor sourceEditor(this, &mParser);
	if(sourceEditor.exec()==QDialog::Rejected) //maybe not a conventional way to consider 'X' as accepted 
	{
		sourceEditor.applyUpdates();
		return;
	}
}

void AggregationEditor::renameTarget(QTreeWidgetItem * current)
{
	current->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsEditable);
	attributeList->editItem(current, 1);
}

void AggregationEditor::editConfiguration()
{
	QTreeWidgetItem * current = attributeList->currentItem();
	if(!current) return;
	QTreeWidgetItem * parent=current->parent();

	if(!parent)    //Source
		editSource(current);
	else
		renameTarget(current);

}

void AggregationEditor::setConfiguration()
{
	QTreeWidgetItem * attributeItem;
	QTreeWidgetItem * scopeItem;
	QTreeWidgetItem * sourceItem;
	std::string::size_type posStart=0;
	std::string::size_type posA, posB, posEnd, posAttributesEnd;

	int arraySize = -1;

	posStart = mConfig.find("map",0);         
	posA = mConfig.find("[", posStart+1);  
	posB = mConfig.find("]", posStart+1);
	mConfig.erase(posA+1, posB-posA-1); //erase the map content
	std::string newContent="\n";
	std::string str1="\t(\"";
	std::string str2="::";
	std::string str3="\", \"";
	std::string str4="::";
	std::string str5="\"),\n";

	for(int i=0; i<attributeList->topLevelItemCount(); i++)  //ugly.....is there any clearer way?
	{
		sourceItem=attributeList->topLevelItem(i);
		for(int j=0; j<sourceItem->childCount();j++)
		{
			scopeItem = sourceItem->child(j);
			for(int k=0; k<scopeItem->childCount();k++)
			{
				attributeItem = scopeItem->child(k);
					if(attributeItem->checkState(0)==Qt::Checked)
				{
					//the default names will be automatically writen after "accepted"
					if(attributeItem->text(1)=="") attributeItem->setText(1, attributeItem->text(0));
					if(scopeItem->text(1)=="") scopeItem->setText(1, scopeItem->text(0));
					newContent+=str1+scopeItem->text(1).toStdString()+str2+
						attributeItem->text(1).toStdString()+str3+
						sourceItem->text(0).toStdString()+str3+
						scopeItem->text(0).toStdString()+str4+
						attributeItem->text(0).toStdString()+str5;
				}
			}
			
		}
		
	}
	mConfig.insert(posA+1, newContent);
	std::cout<< "the newly edited Configuration is ..............\n" << mConfig << std::endl;
	
}



