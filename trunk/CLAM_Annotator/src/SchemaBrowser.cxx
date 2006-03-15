#include "SchemaBrowser.hxx"

#include "Schema.hxx"

#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QTextBrowser>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
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
    childLabel->setText( tr( "Child Scope" ) );

    QString documentation = tr(
		    "<b>No item selected</b>"
		    );
    attributeDocumentation->setHtml(documentation);
}

typedef std::list<CLAM_Annotator::SchemaAttribute> SchemaAttributes;

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
	attributeList->show();
	attributeList->resizeColumnToContents(0);
	attributeList->resizeColumnToContents(1);
	attributeList->show();
}

void SchemaBrowser::updateCurrentAttribute()
{
	QTreeWidgetItem * current = attributeList->currentItem();
	QTreeWidgetItem * parent = current->parent();
	if (!parent) // Scope
	{
		QString documentation = "<h2>Scope '" + current->text(0) + "'</h2>";
		attributeDocumentation->setHtml(documentation);
	}
	else // Attribute
	{
		const CLAM_Annotator::SchemaAttribute & attributeSchema = mSchema->GetAttribute(parent->text(0).toStdString(),current->text(0).toStdString());
		QString url = "http://mtg100.upf.es/simac/DescriptionSchemeWeb";
		QString documentation = "<h2>Attribute '" + parent->text(0) + "::" + current->text(0) + "'</h2>";
		documentation += "<ul>";
		documentation+="<li><b>Type:</b> ";
		documentation+=QString(attributeSchema.GetType().c_str()) + "</li>";
		if (attributeSchema.HasEnumerationValues())
		{
			documentation+="<li><b>Available Values:</b> ";
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
			documentation+=".</ul></li>";
		}
		if (attributeSchema.HasfRange())
		{
			documentation+="<li><b>Minimum value:</b> " + QString::number(attributeSchema.GetfRange().GetMin()) + "</li>";
			documentation+="<li><b>Maximum value:</b> " + QString::number(attributeSchema.GetfRange().GetMax()) + "</li>";
		}
		if (attributeSchema.HasiRange())
		{
			documentation+="<li><b>Minimum value:</b> " + QString::number(attributeSchema.GetiRange().GetMin()) + "</li>";
			documentation+="<li><b>Maximum value:</b> " + QString::number(attributeSchema.GetiRange().GetMax()) + "</li>";
		}
		if (attributeSchema.HasChildScope())
		{
			QString childScope = attributeSchema.GetChildScope().c_str();
			if (childScope=="") childScope = tr("No child scope");
			documentation+="<li><b>Child Scope:</b> " + childScope + "</li>";
		}
		if (attributeSchema.HasSegmentationPolicy())
		{
			documentation+="<li><b>Segmentation Policy:</b> " + QString(attributeSchema.GetSegmentationPolicy().GetString().c_str()) + "</li>";
		}
		documentation += "</ul>";
		documentation += "<div class='descriptorDocumentation'>";
		if (attributeSchema.HasDocumentation())
			documentation += attributeSchema.GetDocumentation().c_str();
		else
			documentation += "<p>The schema has no embeded documentation for the attribute.</p>";
		documentation += "</div>";
		documentation += "<p>Get further information on <a href='" + url + "'>" +url+"</a>.</p>";
		attributeDocumentation->setHtml(documentation);
	}
}


