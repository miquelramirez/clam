#ifndef CLIPBOARDPLUGIN_HXX
#define CLIPBOARDPLUGIN_HXX

#include <qwidgetplugin.h>
#include <qwidget.h>
#include <qstring.h>
#include <qiconset.h>
#include <qstringlist.h>

class ClipBoardPlugin : public QWidgetPlugin
{
public:
  ClipBoardPlugin();
  QStringList keys() const;
  QWidget* create( const QString &classname, QWidget* parent = 0, const char* name = 0 );
  QString group( const QString& ) const;
  QIconSet iconSet( const QString& ) const;
  QString includeFile( const QString& ) const;
  QString toolTip( const QString& ) const;
  QString whatsThis( const QString& ) const;
  bool isContainer( const QString& ) const;
};

Q_EXPORT_PLUGIN( ClipBoardPlugin )

#endif
