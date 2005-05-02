#ifndef __BPFEDITORDISPLAYSURFACE__
#define __BPFEDITORDISPLAYSURFACE__

#include <qgl.h>
#include <qcursor.h>
#include "GLView.hxx"

namespace CLAM
{
    namespace VM
    {
	class BPFEditorController;

	class BPFEditorDisplaySurface : public QGLWidget
	{
	    Q_OBJECT

	public:
	    BPFEditorDisplaySurface(QWidget* parent=0);
	    ~BPFEditorDisplaySurface();

	    void SetController(BPFEditorController* controller);
	    void SetBackgroundColor(const float& r, const float& g, const float& b);

	private slots:
	    void updateView(GLView);
	    void changeCursor(QCursor);

	protected:
	    void paintGL(); 

	    void mousePressEvent(QMouseEvent* e);
	    void mouseReleaseEvent(QMouseEvent* e);
	    void mouseMoveEvent(QMouseEvent* e);
	    void keyReleaseEvent(QKeyEvent * e);

	    void resizeEvent(QResizeEvent *e);

	private:
	    GLView mView;
	    float mRed, mGreen, mBlue;
	    BPFEditorController* mController;
	    int mWidth, mHeight;
	    volatile bool mDoResize;
	};
    }
}

#endif

