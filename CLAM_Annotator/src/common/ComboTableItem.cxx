#include "ComboTableItem.hxx"
#include <qstyle.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qrect.h>
#include <qapplication.h>
#include <QComboBox>

#include <iostream>

QComboBox *ComboTableItem::fakeCombo = 0;
QWidget *ComboTableItem::fakeComboWidget = 0;
int ComboTableItem::fakeRef = 0;

ComboTableItem::ComboTableItem( Q3Table *table, std::vector<QStringList> &list, bool editable )
	: TableItem( table, WhenCurrent, "" ), entries( list ), current( 0 ), edit( editable )
{
	setReplaceable( FALSE );
	if ( !ComboTableItem::fakeCombo ) {
		ComboTableItem::fakeComboWidget = new QWidget( 0, 0 );
		ComboTableItem::fakeCombo = new QComboBox( FALSE, ComboTableItem::fakeComboWidget, 0 );
		ComboTableItem::fakeCombo->hide();
	}
	++ComboTableItem::fakeRef;
	if ( entries[0].count() )
		setText( entries[0].at( current ) );
}

ComboTableItem::~ComboTableItem()
{
	if (--ComboTableItem::fakeRef <= 0) {
		delete ComboTableItem::fakeComboWidget;
		ComboTableItem::fakeComboWidget = 0;
		ComboTableItem::fakeCombo = 0;
	}
}

void ComboTableItem::setStringList( const QStringList &l )
{
	entries[0] = l;
	current = 0;
	if ( entries[0].count() )
		setText( entries[0].at( current ) );
	if ( table()->cellWidget( row(), col() ) ) {
		cb->clear();
		cb->insertStringList( entries[0] );
	}
	table()->updateCell( row(), col() );
}

QWidget *ComboTableItem::createEditor() const
{
	( (ComboTableItem*)this )->cb = new QComboBox( edit, table()->viewport(), "qt_editor_cb" );
	cb->insertStringList( entries[0] );
	cb->setCurrentItem( current );
	QObject::connect( cb, SIGNAL( activated(int) ), table(), SLOT( doValueChanged() ) );
	return cb;
}

void ComboTableItem::setContentFromEditor( QWidget *w )
{
	QComboBox *cb = dynamic_cast<QComboBox*>(w); // QTPORT Was a qt_cast
	if ( cb ) {
		entries[0].clear();
		for ( int i = 0; i < cb->count(); ++i )
			entries[0] << cb->text( i );
		current = cb->currentItem();
		setText( entries[0].at( current ) );
	}
}

void ComboTableItem::paint( QPainter *p, const QColorGroup &cg,
			   const QRect &cr, bool selected )
{
	fakeCombo->resize( cr.width()/entries.size(), cr.height() );

	QColorGroup c( cg );
	if ( selected )
	{
		c.setBrush( QColorGroup::Base, cg.brush( QColorGroup::Highlight ) );
		c.setColor( QColorGroup::Text, cg.highlightedText() );
		
		QStyleOptionComboBox options;
		options.state = QStyle::State_None;
		if(isEnabled() && table()->isEnabled())
			options.state |= QStyle::State_Enabled;
		for (int i=0;i<entries.size();i++)
		{
			table()->style()->drawComplexControl( QStyle::CC_ComboBox, &options, p, fakeCombo);
		}

		p->save();
		QRect textR = table()->style()->subControlRect(QStyle::CC_ComboBox, &options, QStyle::SC_ComboBoxEditField, fakeCombo);
		int align = alignment(); 
		p->drawText( textR, wordWrap() ? ( align | Qt::TextWordWrap ) : align, entries[0].at( current ) );
		p->restore();
	}
	else
	{
		setText(currentText());
		TableItem::paint(p,cg,cr,selected);
	}
}

void ComboTableItem::setCurrentItem( int i )
{
	QWidget *w = table()->cellWidget( row(), col() );
	QComboBox *cb = dynamic_cast<QComboBox*>(w);  // QTPORT Was a qt_cast
	if ( cb )
	{
		cb->setCurrentItem( i );
		current = i;
		setText( cb->currentText() );
	}
	else
	{
		current = i;
		setText( entries[0].at( i ) );
		table()->updateCell( row(), col() );
	}
}

void ComboTableItem::setCurrentItem( const QString &s )
{
	int i = entries[0].findIndex( s );
	if ( i != -1 )
	setCurrentItem( i );
}

int ComboTableItem::currentItem() const
{
	QWidget *w = table()->cellWidget( row(), col() );
	QComboBox *cb = dynamic_cast<QComboBox*>(w);  // QTPORT Was a qt_cast
	if ( cb )
		return cb->currentItem();
	return current;
}

QString ComboTableItem::currentText() const
{
	QWidget *w = table()->cellWidget( row(), col() );
	QComboBox *cb = dynamic_cast<QComboBox*>(w);  // QTPORT Was a qt_cast
	if ( cb )
		return cb->currentText();
	return entries[0].at( current );
}

int ComboTableItem::count() const
{
	QWidget *w = table()->cellWidget( row(), col() );
	QComboBox *cb = dynamic_cast<QComboBox*>(w);  // QTPORT Was a qt_cast
	if ( cb )
		return cb->count();
	return (int)entries[0].count();   
}

QString ComboTableItem::text( int i ) const
{
	QWidget *w = table()->cellWidget( row(), col() );
	QComboBox *cb = dynamic_cast<QComboBox*>(w);  // QTPORT Was a qt_cast
	if ( cb )
		return cb->text( i );
	return entries[0].at( i );
}

void ComboTableItem::setEditable( bool b )
{
	edit = b;
}

bool ComboTableItem::isEditable() const
{
	return edit;
}

int ComboTableItem::RTTI = 1;

int ComboTableItem::rtti() const
{
	return RTTI;
}

QSize ComboTableItem::sizeHint() const
{
	fakeCombo->insertItem( currentText() );
	fakeCombo->setCurrentItem( fakeCombo->count() - 1 );
	QSize sh = fakeCombo->sizeHint();
	fakeCombo->removeItem( fakeCombo->count() - 1 );
	return sh.expandedTo( QApplication::globalStrut() );
}

