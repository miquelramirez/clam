#include "DataTypes.hxx"
#include "BPFEditorController.hxx"
#include "BPFEditorDisplaySurface.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFEditorDisplaySurface::BPFEditorDisplaySurface(QWidget* parent)
			: QGLWidget(parent), 
			  mController(0),
			  mWidth(0),mHeight(0),
			  mDoResize(false)
		{
			setMouseTracking(true);
			setFocusPolicy(StrongFocus);
			setAutoBufferSwap(false);
			SetBackgroundColor(0.0f,0.0f,0.0f);
		}

		BPFEditorDisplaySurface::~BPFEditorDisplaySurface()
		{
			if(mController) delete mController;
		}

		void BPFEditorDisplaySurface::SetController(BPFEditorController* controller)
		{
			mController = controller;
			
			connect(mController,SIGNAL(requestRefresh()),this,SLOT(updateGL()));
			connect(mController,SIGNAL(viewChanged(GLView)),this,SLOT(updateView(GLView)));
			connect(mController,SIGNAL(cursorChanged(QCursor)),this,SLOT(changeCursor(QCursor)));
		}

		void BPFEditorDisplaySurface::SetBackgroundColor(const float& r, const float& g, const float& b)
		{
			mRed = r;
			mGreen = g;
			mBlue = b;

			updateGL();
		}

		void BPFEditorDisplaySurface::updateView(GLView view)
		{
			mView.mLeft = view.mLeft;
			mView.mRight = view.mRight;
			mView.mBottom = view.mBottom;
			mView.mTop = view.mTop;
		}

		void BPFEditorDisplaySurface::changeCursor(QCursor cursor)
		{
			setCursor(cursor);
		}

		void BPFEditorDisplaySurface::paintGL()
		{
			if(!mController) return;
			if(mDoResize)
			{
				glViewport(0,0,mWidth,mHeight);
				mDoResize=false;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(mView.mLeft,mView.mRight,mView.mBottom,mView.mTop,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			glClearColor(GLfloat(mRed),GLfloat(mGreen),GLfloat(mBlue),1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			mController->Draw();
			swapBuffers();
		}

		void BPFEditorDisplaySurface::mousePressEvent(QMouseEvent* e)
		{
			if(mController)
			{
				if(e->button() == LeftButton)
				{
					mController->SetLeftButtonPressed(true);
					double xcoord = double(e->x());
					xcoord *= (mView.mRight-mView.mLeft);
					xcoord /= double(width());
					xcoord += mView.mLeft;
					double ycoord = double(-e->y())+double(height());
					ycoord *= (mView.mTop-mView.mBottom);
					ycoord /= double(height());
					ycoord += mView.mBottom;
					mController->SetPoint(TData(xcoord),TData(ycoord));
				}

				if(e->button() == RightButton)
				{
					mController->SetRightButtonPressed(true);
				}
			}
		}

		void BPFEditorDisplaySurface::mouseReleaseEvent(QMouseEvent* e)
		{
			if(mController)
			{
				if(e->button() == LeftButton)
				{
					mController->SetLeftButtonPressed(false);
					double xcoord = double(e->x());
					xcoord *= (mView.mRight-mView.mLeft);
					xcoord /= double(width());
					xcoord += mView.mLeft;
					double ycoord = double(-e->y())+double(height());
					ycoord *= (mView.mTop-mView.mBottom);
					ycoord /= double(height());
					ycoord += mView.mBottom;
					mController->SetPoint(TData(xcoord),TData(ycoord));			
				}

				if(e->button() == RightButton)
				{
					mController->SetRightButtonPressed(false);
				}
			}
		}

		void BPFEditorDisplaySurface::mouseMoveEvent(QMouseEvent* e)
		{
			if(mController)
			{
				double xcoord = double(e->x());
				xcoord *= (mView.mRight-mView.mLeft);
				xcoord /= double(width());
				xcoord += mView.mLeft;
				double ycoord = double(-e->y())+double(height());
				ycoord *= (mView.mTop-mView.mBottom);
				ycoord /= double(height());
				ycoord += mView.mBottom;
				mController->UpdatePoint(TData(xcoord),TData(ycoord));
			}
		}

		void BPFEditorDisplaySurface::keyReleaseEvent(QKeyEvent * e)
		{
			if (!mController) return;
			int step = (e->state() & ControlButton) ? 1:5;
			switch (e->key())
			{
				case Qt::Key_Up:
					mController->MoveCurrentPointDelta(0,step);
					break;
				case Qt::Key_Down:
					mController->MoveCurrentPointDelta(0,-step);
					break;
				case Qt::Key_Right:
					mController->MoveCurrentPointDelta(step,0);
					break;
				case Qt::Key_Left:
					mController->MoveCurrentPointDelta(-step,0);
					break;
				case Qt::Key_Prior:
					mController->ChooseCurrentPointByJumping(-1);
					break;
				case Qt::Key_Next:
					mController->ChooseCurrentPointByJumping(1);
					break;
			}
		}

		void BPFEditorDisplaySurface::leaveEvent(QEvent* e)
		{
			if(mController)
			{
				mController->MouseOverDisplay(false);
			}
		}

		void BPFEditorDisplaySurface::enterEvent(QEvent* e)
		{
			if(mController)
			{
				mController->MouseOverDisplay(true);
			}
		}
		
		void BPFEditorDisplaySurface::resizeEvent(QResizeEvent *e)
		{
			mWidth = e->size().width();
			mHeight = e->size().height();
			mDoResize = true;
			if(mController) mController->DisplayDimensions(mWidth,mHeight);
		}
	}
}

// END
