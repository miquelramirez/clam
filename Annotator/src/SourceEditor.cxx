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

#include "SourceEditor.hxx"
//#include <QtCore/QTextStream>

SourceEditor::~SourceEditor()
{
}

void SourceEditor::applyUpdates()
{
	if(_itemChangedFlag)
	{
		std::string config="sources = [\n"; 
		int rowCount=_ui.parameterTable->rowCount();
		std::string str1="\t(\"";
		std::string str2="\", FileMetadataSource(path=\"";
		std::string str3="\",\n\t\tschemaFile=\"";
		std::string str4="\",\n\t\tpoolSuffix=\"";
		std::string str5="\",\n\t\textractor=\"";
		std::string str6="\")),\n";
		std::string str7="]\n\nmap = [\n]\n";

		for(int i=0; i<rowCount; i++)
		{
			config=config+str1+_ui.parameterTable->item(i,0)->text().toStdString()+
				str2+ _ui.parameterTable->item(i,5)->text().toStdString()+str3+
				_ui.parameterTable->item(i,2)->text().toStdString()+str4+
				_ui.parameterTable->item(i,3)->text().toStdString()+str5+
				_ui.parameterTable->item(i,1)->text().toStdString()+str6;
		}		
	config+=str7;  //Given the sources are edited, clear the map.	
	std::cout<< mAggregationEditor->outputConfig() << std::endl;
	mAggregationEditor->loadConfig(config);
	std::cout<< "after reloading is ..............\n" << std::endl;
	std::cout<< mAggregationEditor->outputConfig() << std::endl;
	mAggregationEditor->setSchema();
	}
		
}

void SourceEditor::loadParameter(AggregationEditor::configurationParser* parser)
{
	for (int i=0; i<parser->sourceCnt; i++)
	{
		QTableWidgetItem *sourceItem = new QTableWidgetItem(parser->source[i].c_str(),0);
		QTableWidgetItem *extractorItem = new QTableWidgetItem(parser->extractor[i].c_str(),0);
		QTableWidgetItem *schemaFileItem = new QTableWidgetItem(parser->schemaFile[i].c_str(),0);
		QTableWidgetItem *suffixItem = new QTableWidgetItem(parser->suffix[i].c_str(),0);
		QTableWidgetItem *configFileItem = new QTableWidgetItem(parser->configFile[i].c_str(),0);
		QTableWidgetItem *pathItem = new QTableWidgetItem(parser->path[i].c_str(),0);
		_ui.parameterTable->insertRow(i); //(i+1)?
		_ui.parameterTable->setItem(i, 0, sourceItem);
		_ui.parameterTable->setItem(i, 1, extractorItem);
		_ui.parameterTable->setItem(i, 2, schemaFileItem);
		_ui.parameterTable->setItem(i, 3, suffixItem);
		_ui.parameterTable->setItem(i, 4, configFileItem);
		_ui.parameterTable->setItem(i, 5, pathItem);				
	}

}

void SourceEditor::setItemChangedFlag()
{
	_itemChangedFlag=1;
}

void SourceEditor::on_addButton_clicked()
{
	_ui.parameterTable->insertRow(_ui.parameterTable->rowCount());
}

void SourceEditor::on_deleteButton_clicked()
{
	_ui.parameterTable->removeRow(_ui.parameterTable->currentRow());
}


