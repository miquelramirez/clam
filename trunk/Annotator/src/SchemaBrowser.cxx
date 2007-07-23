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

#include "SchemaBrowser.hxx"

#include "Schema.hxx"

#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QTextBrowser>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QFrame>

SchemaBrowser::SchemaBrowser( QWidget* parent, Qt::WFlags fl )
    : QWidget( parent, fl )
	, scopeIcon(":/icons/images/xkill.png")
	, attributeIcon(":/icons/images/label.png")
    , mSchema(0)
{
	setObjectName( "schemaBrowser" );
    schemaBrowserLayout = new QHBoxLayout( this ); 
    schemaBrowserLayout->setMargin(11);
	schemaBrowserLayout->setSpacing(6);

    splitter1 = new QSplitter( this );
    splitter1->setOrientation( Qt::Horizontal );

    attributeList = new QTreeWidget( splitter1 );
    attributeList->setHeaderLabels( QStringList()
			<< tr( "Attribute" )
			<< tr( "Type" )
			);
    attributeList->setRootIsDecorated( true );
    attributeList->setSortingEnabled( false );
	attributeList->setAlternatingRowColors(true);
	attributeList->resizeColumnToContents(0);
	attributeList->resizeColumnToContents(1);

    splitter2 = new QSplitter( splitter1 );
    splitter2->setOrientation( Qt::Vertical );

    attributeProperties = new QFrame( splitter2 );
    attributeProperties->setFrameShape( QFrame::StyledPanel );
    attributeProperties->setFrameShadow( QFrame::Raised );
    attributePropertiesLayout = new QGridLayout( attributeProperties ); 
	attributePropertiesLayout->setMargin(11);
	attributePropertiesLayout->setSpacing(6);

    minLabel = new QLabel( attributeProperties );
    attributePropertiesLayout->addWidget( minLabel, 0, 0 );

    minSpin = new QSpinBox( attributeProperties );
    attributePropertiesLayout->addWidget( minSpin, 0, 1 );

    maxLabel = new QLabel( attributeProperties );
    attributePropertiesLayout->addWidget( maxLabel, 1, 0 );

    maxSpin = new QSpinBox( attributeProperties );
    attributePropertiesLayout->addWidget( maxSpin, 1, 1 );

    childLabel = new QLabel( attributeProperties );
    attributePropertiesLayout->addWidget( childLabel, 2, 0 );

    childEdit = new QLineEdit( attributeProperties );
    attributePropertiesLayout->addWidget( childEdit, 2, 1 );

    attributeDocumentation = new QTextBrowser( splitter2 );
    attributeDocumentation->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum);

    schemaBrowserLayout->addWidget( splitter1 );
    languageChange();
    resize( QSize(740, 346).expandedTo(minimumSizeHint()) );
//    clearWState( WState_Polished ); // TODO: Commented out while  qt4 porting
    connect(attributeList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(updateCurrentAttribute()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
SchemaBrowser::~SchemaBrowser()
{
    // no need to delete child widgets, Qt does it all for us
}

void SchemaBrowser::addAttribute(const std::string & scope, const std::string & name, const std::string & type)
{
    QList<QTreeWidgetItem *> scopeItems = attributeList->findItems(scope.c_str(),Qt::MatchExactly);
    QTreeWidgetItem * scopeItem = 0;
    if (scopeItems.size()==0)
    {
	    scopeItem = new QTreeWidgetItem( attributeList);
	    scopeItem->setText( 0, scope.c_str() );
	    scopeItem->setIcon( 0, scopeIcon );
	    attributeList->expandItem(scopeItem);
    }
	else
		scopeItem = scopeItems[0];

    QTreeWidgetItem * item = new QTreeWidgetItem( scopeItem);
    item->setText( 0, name.c_str() );
    item->setIcon( 0, attributeIcon );
    item->setText( 1, type.c_str() );
}
/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SchemaBrowser::languageChange()
{
    setWindowTitle( tr( "Schema Browser" ) );
    attributeList->headerItem()->setText( 0, tr( "Attribute" ) );
    attributeList->headerItem()->setText( 1, tr( "Type" ) );
    attributeList->clear();

    minLabel->setText( tr( "Minimum" ) );
    maxLabel->setText( tr( "Maximum" ) );
    childLabel->setText( tr( "Child scope" ) );

	updateCurrentAttribute();
}

typedef std::list<CLAM_Annotator::SchemaAttribute> SchemaAttributes;

QTreeWidgetItem * SchemaBrowser::findAttributeItem(const QString & scope, const QString & name)
{
	QTreeWidgetItem * scopeItem = findScopeItem(scope);
	if (!scopeItem) return 0;
	for (int i=0; i<scopeItem->childCount(); i++)
	{
		QTreeWidgetItem * attributeItem = scopeItem->child(i);
		if (attributeItem->text(0)==name) return attributeItem;
	}
	return 0;
}

QTreeWidgetItem * _findScopeItem( QTreeWidgetItem * item, const QString & scope)
{
	if (item->text(0)==scope) return item;
	for (int i=0; i<item->childCount(); i++)
	{
		QTreeWidgetItem * attribute = item->child(i);
		for (int j=0; j<attribute->childCount(); j++)
		{
			QTreeWidgetItem * candidate =_findScopeItem(attribute->child(j), scope);
			if (candidate) return candidate;
		}
	}
	return 0;
}

QTreeWidgetItem * SchemaBrowser::findScopeItem(const QString & scope)
{
	for (int i=0; i<attributeList->topLevelItemCount(); i++)
	{
		QTreeWidgetItem * result = _findScopeItem( attributeList->topLevelItem(i), scope );
		if (result) return result;
	}
	return 0;
}

void SchemaBrowser::setSchema(CLAM_Annotator::Schema & schema)
{
	attributeProperties->hide();
	mSchema = &schema;
	attributeList->clear();
	SchemaAttributes & attributes = schema.GetAttributes();
	for (SchemaAttributes::iterator it = attributes.begin();
			it!=attributes.end();
			it++)
	{
		addAttribute(it->GetScope(), it->GetName(), it->GetType());
	}
	for (SchemaAttributes::iterator it = attributes.begin();
			it!=attributes.end();
			it++)
	{
		if (! it->HasChildScope() ) continue;
		QTreeWidgetItem * parent = findAttributeItem(it->GetScope().c_str(), it->GetName().c_str() );
		if (!parent) continue;
		QTreeWidgetItem * child = findScopeItem(it->GetChildScope().c_str());
		if (!child) continue;
		CLAM_ASSERT(parent, "The attribute should have been inserted");
		attributeList->takeTopLevelItem(attributeList->indexOfTopLevelItem(child));
		parent->insertChild(0,child);
		parent->setExpanded(true);
		child->setExpanded(true);
	}
	attributeList->show();
	attributeList->resizeColumnToContents(0);
	attributeList->resizeColumnToContents(1);
	attributeList->show();
}

void SchemaBrowser::updateCurrentAttribute()
{
	QTreeWidgetItem * current = attributeList->currentItem();
	if (!current) 
	{
		attributeDocumentation->setHtml(tr("<p><em>(No attribute or scope selected)</em></p>"));
		return;
	}
	unsigned level = 0;
	for ( QTreeWidgetItem * parent = current->parent(); parent ; level++)
		parent=parent->parent();
	if (! (level&1)) // Scope
	{
		QString documentation = tr("<h2>Scope '%1'</h2>").arg(current->text(0));
		attributeDocumentation->setHtml(documentation);
		return;
	}
	// Attribute
	QTreeWidgetItem * parent = current->parent();
	const CLAM_Annotator::SchemaAttribute & attributeSchema = mSchema->GetAttribute(parent->text(0).toStdString(),current->text(0).toStdString());
	QString url = "http://mtg100.upf.es/simac/DescriptionSchemeWeb";
	QString documentation = tr("<h2>Attribute '%1::%2'</h2>").arg(parent->text(0)).arg(current->text(0));
	documentation+="<ul>";
//	documentation+=tr("<li><b>Documentation url:</b> <a href='%1'>%2</a>.</li>").arg(url).arg(url);
	documentation+=tr("<li><b>Type:</b> %1</li>").arg(attributeSchema.GetType().c_str());
	if (attributeSchema.HasEnumerationValues())
	{
		documentation+=tr("<li><b>Available values:</b> ");
		std::list<std::string> & values = attributeSchema.GetEnumerationValues();
		const char * separator = "";
		for (std::list<std::string>::iterator it =values.begin(); it!= values.end(); it++)
		{
			documentation+=separator;
			documentation+="<tt>";
			documentation+=it->c_str();
			documentation+="</tt>";
			separator =", ";
		}
		documentation+=".</li>";
	}
	if (attributeSchema.HasBinLabels())
	{
		documentation+=tr("<li><b>Array bins labels:</b> ");
		std::list<std::string> & values = attributeSchema.GetBinLabels();
		const char * separator = "";
		for (std::list<std::string>::iterator it =values.begin(); it!= values.end(); it++)
		{
			documentation+=separator;
			documentation+="<tt>";
			documentation+=it->c_str();
			documentation+="</tt>";
			separator =", ";
		}
		documentation+=".</li>";
	}
	if (attributeSchema.HasFirstBinOffset())
	{
		documentation+=tr("<li><b>First Bin Offset:</b> %1</li>").arg(attributeSchema.GetFirstBinOffset());
	}
	if (attributeSchema.HasBinGap())
	{
		documentation+=tr("<li><b>Bin Gap:</b> %1</li>").arg(attributeSchema.GetBinGap());
	}
	if (attributeSchema.HasNBins())
	{
		documentation+=tr("<li><b>Number of Bins:</b> %1</li>").arg(attributeSchema.GetNBins());
	}
	if (attributeSchema.HasUnits())
	{
		documentation+=tr("<li><b>Units:</b> %1</li>").arg(attributeSchema.GetUnits().c_str());
	}
	if (attributeSchema.HasMinValue())
	{
		documentation+=tr("<li><b>Minimum value:</b> %1</li>").arg(attributeSchema.GetMinValue());
	}
	if (attributeSchema.HasMaxValue())
	{
		documentation+=tr("<li><b>Maximum value:</b> %1</li>").arg(attributeSchema.GetMaxValue());
	}
	if (attributeSchema.HasChildScope())
	{
		QString childScope = attributeSchema.GetChildScope().c_str();
		if (childScope=="") childScope = tr("No child scope");
		documentation+=tr("<li><b>Child scope:</b> %1</li>").arg(childScope);
	}
	if (attributeSchema.HasSegmentationPolicy())
	{
		documentation+=tr("<li><b>Segmentation policy:</b> %1</li>").arg(attributeSchema.GetSegmentationPolicy().GetString().c_str());
	}
	documentation += "</ul>";
	documentation += tr("<h2>Description</h2>");
	documentation += "<div class='descriptorDocumentation'>";
	if (attributeSchema.HasDocumentation())
		documentation += attributeSchema.GetDocumentation().c_str();
	else
		documentation += tr("<p><em>(The schema has no embeded documentation for the attribute)</em></p>");
	documentation += "</div>";
	attributeDocumentation->setHtml(documentation);
}


