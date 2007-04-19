#ifndef __BPFEDITORDISPLAYSURFACE__
#define __BPFEDITORDISPLAYSURFACE__

#include <qgl.h>
#include <qcursor.h>
#include <CLAM/GLView.hxx>

class QTimer;

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

		signals:
			void focusIn();
			void focusOut();
	
		private slots:
			void updateView(GLView);
			void changeCursor(QCursor);
			void startTimer();
			void stopTimer();

		protected:
			void paintGL(); 
			void mousePressEvent(QMouseEvent* e);
			void mouseReleaseEvent(QMouseEvent* e);
			void mouseMoveEvent(QMouseEvent* e);
			void keyReleaseEvent(QKeyEvent * e);
			void resizeEvent(QResizeEvent *e);
			void enterEvent(QEvent *e); 
			void leaveEvent(QEvent *e); 
			void focusInEvent(QFocusEvent* e);
			void focusOutEvent(QFocusEvent* e);

		private:
			GLView               mView;
			float                mRed;
			float                mGreen; 
			float                mBlue;
			BPFEditorController* mController;
			int                  mWidth;
			int                  mHeight;
			volatile bool        mDoResize;
			QTimer*              mTimer;
			bool                 mIsPlaying;

			enum { TIMER_INTERVAL=10 };
		};
    }
}

#endif

