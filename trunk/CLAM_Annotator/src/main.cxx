#include <qapplication.h>
#include <qcursor.h>
#include <qsplashscreen.h>

#include "AnnotatorEnter.hxx"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QSplashScreen splash( QPixmap::fromMimeSource("simac.png") );
    splash.setCursor( QCursor(Qt::WaitCursor) );
    splash.show();
    splash.message("Loading data ... ");
    AnnotatorEnter w;
    w.show();
    splash.finish(&w);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
