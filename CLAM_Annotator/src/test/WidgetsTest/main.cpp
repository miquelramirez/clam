#include <qapplication.h>
#include "MusicAnnotatorWidgetsTestDialog.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    MusicAnnotatorWidgetsTestDialog w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
