#include "ClipBoardPlugin.hxx"
#include "ClipBoardWidget.hxx"
#include "ClipBoard.xpm"
#include <qpixmap.h>

ClipBoardPlugin::ClipBoardPlugin()
{
}

QStringList ClipBoardPlugin::keys() const
{
	QStringList list;
	list << "ClipBoardWidget";
	return list;
}

QWidget* ClipBoardPlugin::create( const QString &classname, QWidget* parent , const char* name ) 
{
	if ( classname == "ClipBoardWidget")
		return new ClipBoardWidget( parent, name );
	return 0; 
}

QString  ClipBoardPlugin::group( const QString& feature) const
{
	if ( feature == "ClipBoardWidget" )
		return "Logic";
	return QString::null;
}

QIconSet ClipBoardPlugin::iconSet( const QString& feature) const
{
	if (feature == "ClipBoardWidget")
	{
		return QIconSet( QPixmap( static_cast<const char**>(ClipBoard_xpm )) );
	}
	return QIconSet();
}

QString ClipBoardPlugin::includeFile( const QString& feature) const
{
	if ( feature == "ClipBoardWidget" )
		return "ClipBoardWidget.hxx";
	return QString::null;
}

QString ClipBoardPlugin::toolTip( const QString& feature) const
{
	if ( feature =="ClipBoardWidget" )
		return "ClipBoardWidget";
	return QString::null;
}

QString ClipBoardPlugin::whatsThis( const QString& feature) const
{
	if ( feature == "ClipBoardWidget")
		return "ClipBoard Widget is a singleton where all the application widgets can paste, cut and copy SONGS";
	return QString::null;
}

bool ClipBoardPlugin::isContainer( const QString& feature) const
{
	return false;
}
