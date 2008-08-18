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

AggregationEditor::AggregationEditor( SchemaBrowser* parent, Qt::WFlags fl )
    : SchemaBrowser( parent, fl )
    , sourceIcon(":/icons/images/gear.png")
    , scopeIcon(":/icons/images/xkill.png")
    , attributeIcon(":/icons/images/label.png")
{
	setObjectName( "aggregationEditor" );
	//aggregationEditorLayout = new QHBoxLayout( this );
	//aggregationEditorLayout->setMargin(11);
	//aggregationEditorLayout->setSpacing(6);

	//splitter1 = new QSplitter( this );
	//splitter1->setOrientation( Qt::Horizontal );
	//attributeList = new QTreeWidget( splitter1 );
	attributeList->setHeaderLabels( QStringList()
		<< tr( "Source" )
		<< tr( "Target" )
		);
	//attributeList->setRootIsDecorated( true );
	//attributeList->setSortingEnabled( false );
	//attributeList->setAlternatingRowColors(true);
	//attributeList->resizeColumnToContents(0);
	//attributeList->resizeColumnToContents(1);

	//splitter2 = new QSplitter( splitter1 );
	//splitter2->setOrientation( Qt::Vertical );

	//attributeProperties = new QFrame( splitter2 );
	//attributeProperties->setFrameShape( QFrame::StyledPanel );
	//attributeProperties->setFrameShadow( QFrame::Raised );
	//attributePropertiesLayout = new QGridLayout( attributeProperties ); 
	//attributePropertiesLayout->setMargin(11);
	//attributePropertiesLayout->setSpacing(6);

	//minLabel = new QLabel( attributeProperties );
	//attributePropertiesLayout->addWidget( minLabel, 0, 0 );

	//minSpin = new QSpinBox( attributeProperties );
	//attributePropertiesLayout->addWidget( minSpin, 0, 1 );

	//maxLabel = new QLabel( attributeProperties );
	//attributePropertiesLayout->addWidget( maxLabel, 1, 0 );

	//maxSpin = new QSpinBox( attributeProperties );
	//attributePropertiesLayout->addWidget( maxSpin, 1, 1 );

	//childLabel = new QLabel( attributeProperties );
	//attributePropertiesLayout->addWidget( childLabel, 2, 0 );

	//childEdit = new QLineEdit( attributeProperties );
	//attributePropertiesLayout->addWidget( childEdit, 2, 1 );

	//attributeDocumentation = new QTextBrowser( splitter2 );  //jun: needed?
	//attributeDocumentation->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum);

	//aggregationEditorLayout->addWidget( splitter1 );
	//languageChange();

	//resize( QSize(740, 346).expandedTo(minimumSizeHint()) );
	//connect(attributeList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
         //   this, SLOT(updateCurrentAttribute()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
AggregationEditor::~AggregationEditor()
{
	delete mParser.source;	
	delete mParser.extractor;
	delete mParser.schemaFile;	
	delete mParser.suffix;
	delete mParser.configFile;
	delete mParser.path;
	//is "delete mParser" just enough?
}


void AggregationEditor::addSource(const std::string & source, CLAM_Annotator::Schema & schema)
{
	QList<QTreeWidgetItem *> sourceItems = attributeList->findItems(source.c_str(),Qt::MatchExactly);
	QTreeWidgetItem * sourceItem = 0;
	if (sourceItems.size()==0)
	{
		sourceItem = new QTreeWidgetItem(attributeList);
		sourceItem->setText( 0, source.c_str() );
		sourceItem->setIcon( 0, sourceIcon );
		attributeList->expandItem(sourceItem);
	}
	else
		sourceItem = sourceItems[0];

	SchemaBrowser::setListedSchema(schema, sourceItem);
	
}

void AggregationEditor::setSchema()
{	
	//attributeProperties->hide();
	attributeList->clear();
	int arraysize=parseConfiguration();

	//mSourceSchema=new CLAM_Annotator::Schema[arraysize];  //may just be a local variable
	CLAM_Annotator::Schema sourceSchema;
	for(int it = 0; it <arraysize; it++)
	{
		CLAM::XMLStorage::Restore(sourceSchema, mParser.path[it]+"/"+mParser.schemaFile[it]);
		addSource(mParser.source[it], sourceSchema);
		//SchemaBrowser::setSchema(sourceSchema); //jun added for debug
	}
	attributeList->show();
	attributeList->resizeColumnToContents(0);
	attributeList->resizeColumnToContents(1);
	attributeList->show();	
}


int AggregationEditor::parseConfiguration()
{
	std::string::size_type posStart=0;
	std::string::size_type posEnd, posSourcesEnd;

	int arraysize=-1;
	while(posStart!=std::string::npos)  //to get the count of sources 
	{
		posStart=mConfig.find("FileMetadataSource", posStart+1,18);
		arraysize++;
	}
	if(!arraysize)
	{
		std::cout<< "There is no source. Please check whether the Aggregator is correctly configured." <<std::endl;
		return arraysize;
	}


	mParser.source=new std::string[arraysize];	
	mParser.extractor=new std::string[arraysize];
	mParser.schemaFile=new std::string[arraysize];	
	mParser.suffix=new std::string[arraysize];
	mParser.configFile=new std::string[arraysize];
	mParser.path=new std::string[arraysize];

	posStart = mConfig.find("sources", 0);
	posEnd = mConfig.find("),", posStart+1);
	posSourcesEnd = mConfig.find("]", posStart+1);

	for(int i=0; i<arraysize && posEnd<posSourcesEnd; i++)
	{
		mParser.source[i] = parseQuotationMark(posStart, posEnd, "(");
		mParser.path[i] = parseQuotationMark(posStart, posEnd, "path");
		mParser.schemaFile[i] = parseQuotationMark(posStart, posEnd, "schemaFile");
		mParser.suffix[i] = parseQuotationMark(posStart, posEnd, "poolSuffix");
		mParser.extractor[i] = parseQuotationMark(posStart, posEnd, "extractor");

		posStart = posEnd;
		posEnd = mConfig.find("),", posStart+1);		
	}	
	return arraysize;
	
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