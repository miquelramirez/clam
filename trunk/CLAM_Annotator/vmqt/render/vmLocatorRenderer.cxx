#include "CLAMGL.hxx"
#include "vmLocatorRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		Locator::Locator()
			: mUpdatingLocator(false)
			, mLocatorPos(0.0)
			, mRegion(0.0,0.0)
			, mLocatorColor(0,0,0)
			, mRegionColor(185,185,185)
			, mEditionMode(Idle)
		{
		}

		Locator::~Locator()
		{
		}

		void Locator::SetLocatorColor(const Color& c)
		{
			mLocatorColor = c;
		}

		void Locator::SetRegionColor(const Color& c)
		{
			mRegionColor = c;
		}

		void Locator::updateLocator(double value)
		{
			if(!mUpdatingLocator) mUpdatingLocator = true;
			mLocatorPos = value;
			emit requestRefresh();
		}

		void Locator::updateLocator(double value, bool flag)
		{
			mUpdatingLocator = false;
			mLocatorPos = (flag) ? value : mRegion.min;
			double zoom_ref;
			if(mRegion.min < mRegion.max)
			{
				if(flag)
				{
					mRegion.min = mLocatorPos;
					emit selectedRegion(mRegion.min,mRegion.max);
				}
				zoom_ref = mRegion.min+mRegion.Span()/2.0;
			}
			else
			{
				if(flag) 
				{
					mRegion.min = mRegion.max = mLocatorPos;
					emit selectedRegion(mLocatorPos,mXRange.max);
				}
				zoom_ref = mLocatorPos;
			}
			emit hZoomRef(zoom_ref);
			emit requestRefresh();
		}

		void Locator::updateRegion(double begin, double end, bool isEnd)
		{
			if(begin==end)
			{
				mLocatorPos = begin;
				mRegion.min = mRegion.max = mLocatorPos;
				emit hZoomRef(mLocatorPos);
			}
			else
			{
				mRegion.min = begin;
				mRegion.max = end;
				(isEnd) ? mLocatorPos = end : mLocatorPos = begin;
				emit hZoomRef(mRegion.min+mRegion.Span()/2.0);
			}
			emit requestRefresh();
		}

		void Locator::Render()
		{
			if(!mEnabled) return;
			DrawRegion(DrawRegionMode());
			DrawLocator();
		}

		void Locator::SetXRange(double xmin, double xmax)
		{
			Renderer2D::SetXRange(xmin,xmax);
			mLocatorPos = mXRange.min;
		}

		void Locator::MousePressEvent(double x, double y)
		{
			if(!mEnabled || !mCatchEvents) return;
			if(mUpdatingLocator) return;
			if(x == mRegion.min && x == mRegion.max) return;		
			if(mKeyboard.key_shift && mKeyboard.key_r)
			{
				if(mRegion.min != mRegion.max)
				{
					double ref = mRegion.min+mRegion.Span()/2.0;
					if(x <= ref) 
					{
						mRegion.min = x;  
						mLocatorPos = mRegion.min;
					}
					else
					{
						mRegion.max = x;
						mLocatorPos = mRegion.max;
					}
				}
				else
				{
					if(x < mRegion.min)  
					{
						mRegion.min = x;
						mLocatorPos = mRegion.min;
					}
					else
					{
						mRegion.max = x;
						mLocatorPos = mRegion.max;
					}
				}
				emit regionChanged(mRegion.min,mRegion.max,mLocatorPos > mRegion.min);
				emit hZoomRef(mRegion.min+mRegion.Span()/2.0);
				emit requestRefresh();
				return;
			}
			if(mKeyboard.key_r)
			{
				mLocatorPos = x;
				mRegion.min = mRegion.max = mLocatorPos;
				emit regionChanged(mRegion.min,mRegion.max,false);
				emit hZoomRef(mLocatorPos);
				emit requestRefresh();
				return;
			}
			double tolerance = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);
			if(mRegion.min == mRegion.max) 
			{
				if(PickBeginRegion(x,tolerance))
				{
					mEditionMode = DraggingLocator;
				}
				return;
			}
			if(PickBeginRegion(x,tolerance)) 
			{
				mEditionMode = DraggingBeginRegion;
				return;
			}
			if(PickEndRegion(x,tolerance)) 
			{
				mEditionMode = DraggingEndRegion;
				return;
			}
		}

		void Locator::MouseReleaseEvent(double x, double y)
		{
			if(!mEnabled || !mCatchEvents) return;
			if(mUpdatingLocator) return;
			mEditionMode = Idle;
			emit working(mKey,false);
			emit cursorChanged(QCursor(Qt::ArrowCursor));
			double zoom_ref = (mRegion.min == mRegion.max) ? mLocatorPos : mRegion.min+mRegion.Span()/2.0;
			emit hZoomRef(zoom_ref);
			(zoom_ref == mLocatorPos) 
				? emit selectedRegion(mLocatorPos,mXRange.max) 
				: emit selectedRegion(mRegion.min,mRegion.max);
		}

		void Locator::MouseMoveEvent(double x, double y)
		{
			if(!mEnabled || !mCatchEvents) return;
			if(mUpdatingLocator) return;
			QString ttip("");
			if(mKeyboard.key_r) 
			{
				mEditionMode = Idle;
				emit working(mKey,true);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::ArrowCursor));
				return;
			}
			switch (mEditionMode)
			{
				case DraggingBeginRegion:
				{
					if(x < mXRange.min) x = mXRange.min;
					if(x > mXRange.max) x = mXRange.max;
					bool flag = mRegion.min == mLocatorPos;
					mRegion.min = x;
					if(mRegion.min >= mRegion.max) mRegion.min = mRegion.max;
					if(mRegion.min < mXRange.min) mRegion.min = mXRange.min;
					if(flag)
						mLocatorPos = mRegion.min;
					else
						if(mLocatorPos <= mRegion.min) mLocatorPos = mRegion.min;
					ttip = "Cue:"+QString::number(mRegion.min,'f',2);
					emit toolTip(ttip);
					emit regionChanged(mRegion.min,mRegion.max,mLocatorPos > mRegion.min);
					emit requestRefresh();	
					return;
				}
				case DraggingEndRegion:
				{
					if(x < mXRange.min) x = mXRange.min;
					if(x > mXRange.max) x = mXRange.max;
					bool flag = mRegion.max == mLocatorPos;
					mRegion.max = x;
					if(mRegion.max <= mRegion.min) mRegion.max = mRegion.min;
					if(mRegion.max > mXRange.max) mRegion.max = mXRange.max;
					if(flag)
						mLocatorPos = mRegion.max;
					else
						if(mLocatorPos >= mRegion.max) mLocatorPos = mRegion.max;
					ttip = "Cue:"+QString::number(mRegion.max,'f',2);
					emit toolTip(ttip);
					emit regionChanged(mRegion.min,mRegion.max,mLocatorPos > mRegion.min);
					emit requestRefresh();	
					return;
				}
				case DraggingLocator:
				{
					if(x < mXRange.min) x = mXRange.min;
					if(x > mXRange.max) x = mXRange.max;
					mRegion.min = mRegion.max = mLocatorPos = x;
					ttip = "Cue:"+QString::number(mLocatorPos,'f',2);
					emit toolTip(ttip);
					emit regionChanged(mLocatorPos,mLocatorPos,false);
					return;
				}
				default:
					emit working(mKey,false);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}
			double tolerance = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);
			if(mRegion.min == mRegion.max) 
			{
				if(PickBeginRegion(x,tolerance))
				{
					emit working(mKey,true);
					ttip = "Cue:"+QString::number(mLocatorPos,'f',2);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::SizeHorCursor));
				}
				else
				{
					mEditionMode = Idle;
					emit working(mKey,false);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
				}
				return;
			}
			if(PickBeginRegion(x,tolerance)) 
			{
				emit working(mKey,true);
				ttip = "Cue:"+QString::number(mRegion.min,'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			if(PickEndRegion(x,tolerance)) 
			{
				emit working(mKey,true);
				ttip = "Cue:"+QString::number(mRegion.max,'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
		}

		void Locator::KeyPressEvent(int key)
		{
			if(!mEnabled || !mCatchEvents) return;
			if(mUpdatingLocator) return;
			switch(key)
			{
				case Qt::Key_Shift:
					mKeyboard.key_shift = true;
					break;
				case Qt::Key_R:
					mKeyboard.key_r = true;
					emit working(mKey,true);
					break;
				default:
					break;
			}
		}

		void Locator::KeyReleaseEvent(int key)
		{
			if(!mEnabled || !mCatchEvents) return;
			if(mUpdatingLocator) return;
			switch(key)
			{
				case Qt::Key_Shift:
					mKeyboard.key_shift = false;
					break;
				case Qt::Key_R:
					mKeyboard.key_r = false;
					break;
				default:
					break;
			}
		}

		void Locator::LeaveEvent()
		{
			emit toolTip("");
		}

		int Locator::DrawRegionMode()
		{
			// 0 -> neither the points nor the region are visible
			// 1 -> the region is visible but the points are not  
			// 2 -> the first point is visible but the second is not
			// 3 -> the second point is visible but the first is not
			// 4 -> both points are visible
			double p1 = mRegion.min;
			double p2 = mRegion.max;
			if(p1 < mView.left && p2 > mView.right) return 1;
			if(p1 >= mView.left && p2 > mView.right) return 2;
			if(p1 < mView.left && p2 <= mView.right) return 3;
			if(p1 >= mView.left && p2 <= mView.right) return 4;
			return 0;
		}

		void Locator::DrawRegion(int mode)
		{
			if(mRegion.min == mRegion.max) return;
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor4ub(mRegionColor.r,mRegionColor.g,mRegionColor.b,175);
			glBegin(GL_QUADS);
			switch(mode)
			{
				case 1:
					// the region is visible but the points are not
					glVertex2d(mView.left,mView.top);
					glVertex2d(mView.right,mView.top);
					glVertex2d(mView.right,mView.bottom);
					glVertex2d(mView.left,mView.bottom);
					break;
				case 2:
					// the first point is visible but the second is not
					glVertex2d(mRegion.min,mView.top);
					glVertex2d(mView.right,mView.top);
					glVertex2d(mView.right,mView.bottom);
					glVertex2d(mRegion.min,mView.bottom);
					break;
				case 3:
					// the second point is visible but the first is not
					glVertex2d(mView.left,mView.top);
					glVertex2d(mRegion.max,mView.top);
					glVertex2d(mRegion.max,mView.bottom);
					glVertex2d(mView.left,mView.bottom);
					break;
				case 4:
					// both points are visible
					glVertex2d(mRegion.min,mView.top);
					glVertex2d(mRegion.max,mView.top);
					glVertex2d(mRegion.max,mView.bottom);
					glVertex2d(mRegion.min,mView.bottom);
					break;
				default:
					// neither the points nor the region are visible
					break;
			}
			glEnd();
			glDisable(GL_BLEND);
		}

		void Locator::DrawLocator()
		{
			if(mLocatorPos < mView.left || mLocatorPos > mView.right) return;
			glLineWidth(2);
			glColor3ub(mLocatorColor.r,mLocatorColor.g,mLocatorColor.b);
			glBegin(GL_LINES);
			glVertex2d(mLocatorPos,mView.top);
			glVertex2d(mLocatorPos,mView.bottom);
			glEnd();
		}
		
		bool Locator::PickBeginRegion(double x, double tolerance)
		{
			return (mRegion.min >= (x-tolerance) && mRegion.min <= (x+tolerance));
		}

		bool Locator::PickEndRegion(double x, double tolerance)
		{
			return (mRegion.max >= (x-tolerance) && mRegion.max <= (x+tolerance));
		}
	}
}

// END
