#ifndef ANNOTATOR_BROWSER_GL
#define ANNOTATOR_BROWSER_GL

#include <qgl.h>

class AnnotatorBrowserGL : public QGLWidget
{
    Q_OBJECT
   public:
       AnnotatorBrowserGL( QWidget *parent, const char *name ):QGLWidget(parent,name){}
   private:
       int frame;
       bool play;
       void paintBoundaries();
       void paintPlay();
       void paintWave();
       void paintBase();
       void paintMark();
       void paintWindow(double,double);
       void paintDiracs();
protected:	
    void initializeGL();
    void paintGL(void);
    void resizeGL(int w, int h);
public slots:
    void playStop();
    void update();
};

#endif
