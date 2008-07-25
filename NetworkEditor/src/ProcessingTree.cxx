/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "ProcessingTree.hxx" 
#include <QtGui/QMouseEvent>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <iostream>

#include <CLAM/ProcessingFactory.hxx> 


namespace NetworkGUI
{

ProcessingTree::ProcessingTree( QWidget * parent)
	: QWidget(parent)
	,_treeWidget(0)
{
	QVBoxLayout * globalWidgetLayout = new QVBoxLayout;
	QHBoxLayout * searchBarLayout = new QHBoxLayout;

	_searchEdit = new QLineEdit(this);
	_clearButton = new QPushButton(QIcon(":/icons/images/clear_left.png"),"",this);
	_treeWidget = new QTreeWidget(this);
	_treeWidget->setColumnCount(1);
	_treeWidget->setHeaderLabels(QStringList() << tr("Processings toolbox"));
	_treeWidget->setRootIsDecorated( true );
	_treeWidget->header()->hide();

	searchBarLayout->addWidget(new QLabel(tr("Filter:")));
	searchBarLayout->addWidget(_searchEdit);
	searchBarLayout->addWidget(_clearButton);
	globalWidgetLayout->addWidget(_treeWidget);
//	globalWidgetLayout->addWidget(label);
	globalWidgetLayout->addLayout(searchBarLayout);
	this->setLayout(globalWidgetLayout);

	connect(_clearButton,SIGNAL(clicked(bool)),_searchEdit,SLOT(clear()));
	connect(_searchEdit,SIGNAL(textChanged(const QString)),this,SLOT(filterProcessings(const QString)));

	CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
	CLAM::ProcessingFactory::Values categories = factory.GetSetOfValues("category");
	CLAM::ProcessingFactory::Values::const_iterator itCategory;

	for(itCategory = categories.begin(); itCategory != categories.end(); itCategory++)
	{
		CLAM::ProcessingFactory::Keys keys = factory.GetKeys("category", *itCategory);
		//std::cout << "Category: " << *itCategory << std::endl;
		if( keys.size() == 0 ) continue;
		CLAM::ProcessingFactory::Keys::const_iterator itKey;
		std::string category = *itCategory;
		QTreeWidgetItem * categoryTree = new QTreeWidgetItem( _treeWidget, QStringList() << category.c_str());
		for(itKey = keys.begin(); itKey != keys.end(); itKey++)
		{
			std::string key = *itKey;

			std::string description = key;
			CLAM::ProcessingFactory::Values descriptionList=factory.GetValuesFromAttribute(key, "description");
			if (! descriptionList.empty()) description=descriptionList.front();

			QTreeWidgetItem * item = new QTreeWidgetItem( categoryTree, QStringList() << description.c_str());
			
			CLAM::ProcessingFactory::Values iconList=factory.GetValuesFromAttribute(key,"icon");
			std::string iconPath = "processing.png";
			if (! iconList.empty()) iconPath = iconList.front();
			item->setIcon(0, QIcon(QString(":/icons/images/%1").arg(iconPath.c_str())));

			item->setText(1, key.c_str());

			QString tooltipText=QString(tr("name: %1").arg(key.c_str()));
			CLAM::ProcessingFactory::Pairs pairsFromKey=factory.GetPairsFromKey(key);
			CLAM::ProcessingFactory::Pairs::const_iterator itPairs;
			for(itPairs = pairsFromKey.begin();itPairs!=pairsFromKey.end();itPairs++)
			{
				std::string attribute = itPairs->attribute;
				std::string value = itPairs->value;
				if(attribute!="icon" && value!=key && value!="")
				{
					tooltipText+=QString( tr("\n%1: %2")
						.arg(attribute.c_str())
						.arg(value.c_str()) );
				}
			}
			item->setToolTip(0,tooltipText);
		}
	}
	connect( _treeWidget, SIGNAL( itemPressed(QTreeWidgetItem *,int) ),
		 this, SLOT( PressProcessing(QTreeWidgetItem *,int) ));
}

ProcessingTree::~ProcessingTree()
{
}

void ProcessingTree::PressProcessing(QTreeWidgetItem * item, int column)
{	
	if (!item) return;
	if (!item->parent()) return;
	// always use column 1
	QString className = item->text(1);
	QDrag *drag = new QDrag( _treeWidget);
	QMimeData * data = new QMimeData;
	data->setText(className);
	drag->setMimeData(data);
	drag->setPixmap(QPixmap(":/icons/images/processing.png"));
	Qt::DropAction dropAction = drag->start();
}


} // namespace NetworkGUI

