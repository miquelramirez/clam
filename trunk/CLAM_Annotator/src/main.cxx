#include <qapplication.h>
#include <qcursor.h>
#include <qsplashscreen.h>

#include "Annotator.hxx"

int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    QSplashScreen splash( QPixmap::fromMimeSource("simac.png") );
    splash.setCursor( QCursor(Qt::WaitCursor) );
    splash.show();
    splash.message("Loading data ... ");
    Annotator w;
    w.show();
    splash.finish(&w);
    QObject::connect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );
    return app.exec();
}
