/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "CLAMGL.hxx"
#include "CLAM_Math.hxx"
#include "vmGrid.hxx"
#include "vmBPFEditor.hxx"
#include "vmPlot2D.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFEditor::BPFEditor()
			: mglList(0)
			, mFlags(0xff)
			, mCurrentIndex(0)
			, mRebuildglList(false)
			, mBPF(0)
			, mEditionMode(Idle)

		{
		}

		BPFEditor::~BPFEditor()
		{
			if(mglList) glDeleteLists(mglList, mglList);
		}

		void BPFEditor::SetData(BPF* bpf)
		{
			mBPF = bpf;
			mCurrentIndex = 0;
			mRebuildglList = true;
			mEditionMode = Idle;
			emit requestUpdate();
		}

		void BPFEditor::SetColors(const QColor& cline, const QColor& chandler)
		{
			mLineColor = cline;
			mHandlerColor = chandler;
			mRebuildglList = true;
			emit requestRefresh();
		}

		void BPFEditor::SetFlags(int flags)
		{
			mFlags = flags;
		}

		int BPFEditor::GetFlags() const
		{
			return mFlags;
		}

		const CLAM::BPF& BPFEditor::GetData() const
		{
			return *mBPF;
		}

		void BPFEditor::Render()
		{
			if(!mEnabled) return;
			if(!mBPF) return;
			if(!mBPF->Size()) return;

			if(!mglList) mglList = GetglList();
			if(mRebuildglList)
			{
				std::pair<long, long> bounds = GetBounds();
				glNewList(mglList,GL_COMPILE_AND_EXECUTE);
				// lines
				glLineWidth(LINE_WIDTH);
				glColor3ub(mLineColor.red(), mLineColor.green(), mLineColor.blue());
				glBegin(GL_LINE_STRIP);
				for(long i=bounds.first; i < bounds.second; i++)
				{
					glVertex2d(GLdouble(mBPF->GetXValue(i)),GLdouble(mBPF->GetValueFromIndex(i)));
				}
				glEnd();
				// points
				glPointSize(POINT_SIZE);
				glColor3ub(mHandlerColor.red(), mHandlerColor.green(), mHandlerColor.blue());
				glBegin(GL_POINTS);
				for(long i=bounds.first; i < bounds.second; i++)
				{
					glVertex2d(GLdouble(mBPF->GetXValue(i)),GLdouble(mBPF->GetValueFromIndex(i)));
				}
				glEnd();
				if(mCurrentIndex < 0 || mCurrentIndex >= mBPF->Size()) return;
				glPointSize(POINT_SIZE+1);
				glColor3ub(250,160,30);
				glBegin(GL_POINTS);
				glVertex2d(GLdouble(mBPF->GetXValue(mCurrentIndex)),
						   GLdouble(mBPF->GetValueFromIndex(mCurrentIndex)));
				glEnd();
				glEndList();
				mRebuildglList = false;
			}
			else
			{
				glCallList(mglList);
			}
		}

		void BPFEditor::SetHBounds(double left, double right)
		{
			Renderer2D::SetHBounds(left,right);
			mRebuildglList = true;
		}

		void BPFEditor::MousePressEvent(double x, double y)
		{
			if(!mFlags) return;
			if(!mEnabled) return;
			if(!mBPF || !mCatchEvents) return;
			if(mKeyboard.key_insert)
			{
				if(!(mFlags & CLAM::VM::eAllowInsertions)) return;
				long index = Add(x,y);
				ChooseCurrentPoint(index);
				emit elementAdded(mKey,index,x,y);
				return;
			}
			long index = PickPoint(x,y);
			if(index != -1)
			{
				mEditionMode = DraggingPoint;
				ChooseCurrentPoint(index);
			}
		}

		void BPFEditor::MouseReleaseEvent(double x, double y)
		{
			if(!mFlags) return;
			if(!mEnabled) return;
			if(!mBPF || !mCatchEvents) return;
			int mode = mEditionMode;
			mEditionMode = Idle;
			emit toolTip("");
			_container->setCursor(Qt::ArrowCursor);
			if(mode == DraggingPoint) 
			{
				if(mFlags & CLAM::VM::eAllowHorEdition)
				{
					emit xValueChanged(mKey,mCurrentIndex,mBPF->GetXValue(mCurrentIndex));
				}
				if(mFlags & CLAM::VM::eAllowVerEdition)
				{
					emit yValueChanged(mKey,mCurrentIndex,mBPF->GetValueFromIndex(mCurrentIndex));
				}
			}
		}

		void BPFEditor::MouseMoveEvent(double x, double y)
		{
			if(!mBPF) return;
			if(!mCatchEvents) return;
			if(!mBPF->Size()) return;
			if(!mEnabled) return;
	   
			switch (mEditionMode)
			{
				case DraggingPoint:
					Update(x,y);
					emit toolTip(GetToolTip(double(mBPF->GetXValue(mCurrentIndex)),
											double(mBPF->GetValueFromIndex(mCurrentIndex))));
					emit requestRefresh();	
					return;
				default:
					// Just continue below
					emit working(mKey,false);
					emit toolTip("");
					_container->setCursor(Qt::ArrowCursor);
					break;
			}
	
			long index = PickPoint(x,y);
			if(index != -1) 
			{
				emit working(mKey,true);
				ChooseCurrentPoint(index);
				emit toolTip(GetToolTip(double(mBPF->GetXValue(mCurrentIndex)),
										double(mBPF->GetValueFromIndex(mCurrentIndex))));
				if(mFlags & CLAM::VM::eAllowHorEdition && mFlags & CLAM::VM::eAllowVerEdition)
				{
					_container->setCursor(Qt::SizeAllCursor);
				}
				else if(mFlags & CLAM::VM::eAllowHorEdition)
				{
					_container->setCursor(Qt::SizeHorCursor);
				}
				else if(mFlags & CLAM::VM::eAllowVerEdition)
				{
					_container->setCursor(Qt::SizeVerCursor);
				}
				else
				{
					_container->setCursor(Qt::ArrowCursor);
				}
				return;
			}
			
			if(mKeyboard.key_insert)
			{
				emit working(mKey,true);	
			}
				   
		}

		void BPFEditor::KeyPressEvent(int key)
		{
			if(!mFlags) return;
			if(!mEnabled) return;
			if(!mBPF || !mCatchEvents) return;
			switch(key)
			{
				case Qt::Key_Insert:
					emit working(mKey,true);
					mKeyboard.key_insert = true; 
					break;
				case Qt::Key_Delete:
					mKeyboard.key_delete = true; 
					if(!(mFlags & CLAM::VM::eAllowDeletions)) return;
					Remove();
					if(mBPF->Size() > 1)
					{
						if(mCurrentIndex > 0)  ChooseCurrentPoint(mCurrentIndex-1);
					}
					else
					{
						ChooseCurrentPoint(0);
					}
					emit requestRefresh();
					break;
				case Qt::Key_Control:
					mKeyboard.key_ctrl = true;
					break;
				case Qt::Key_Up:
					mKeyboard.key_up = true;
					MoveCurrentPointdy(GetYStep());
					break;
				case Qt::Key_Down:
					mKeyboard.key_down = true;
					MoveCurrentPointdy(-GetYStep());
					break;
				case Qt::Key_Left:
					mKeyboard.key_left = true;
					MoveCurrentPointdx(-GetXStep());
					break;
				case Qt::Key_Right:
					mKeyboard.key_right = true;
					MoveCurrentPointdx(GetXStep());
					break;
				case Qt::Key_PageUp:
					mKeyboard.key_prior = true;
					if(mCurrentIndex <= 0) return;
					ChooseCurrentPoint(mCurrentIndex-1);
					break;
				case Qt::Key_PageDown:
					mKeyboard.key_next = true;
					if(mCurrentIndex >= mBPF->Size()-1) return;
					ChooseCurrentPoint(mCurrentIndex+1);
					break;
				default:
					break;
			}
		}

		void BPFEditor::KeyReleaseEvent(int key)
		{
			if(!mFlags) return;
			if(!mEnabled) return;
			if(!mBPF || !mCatchEvents) return;
			switch(key)
			{
				case Qt::Key_Insert:
					emit working(mKey,false);
					mKeyboard.key_insert = false; 
					break;
				case Qt::Key_Delete:
					mKeyboard.key_delete = false; 
					break;
				case Qt::Key_Control:
					mKeyboard.key_ctrl = false;
					break;
				case Qt::Key_Up:
					mKeyboard.key_up = false;
					break;
				case Qt::Key_Down:
					mKeyboard.key_down = false;
					break;
				case Qt::Key_Left:
					mKeyboard.key_left = false;
					break;
				case Qt::Key_Right:
					mKeyboard.key_right = false;
					break;
				case Qt::Key_PageUp:
					mKeyboard.key_prior = false;
					break;
				case Qt::Key_PageDown:
					mKeyboard.key_next = false;
					break;
				default:
					break;
			}
		}

		void BPFEditor::LeaveEvent()
		{
			emit toolTip("");
		}

		void BPFEditor::Remove()
		{
			if(!mBPF->Size()) return;
			if(mCurrentIndex < 0 || mCurrentIndex >= mBPF->Size()) return;
			mRebuildglList = true;
			mBPF->DeleteIndex(mCurrentIndex);
			emit elementRemoved(mKey,mCurrentIndex);
		}

		long BPFEditor::Add(double x, double y)
		{
			mRebuildglList = true;
			if(mGrid)
			{
				if(mGrid->ShowGrid())
				{
					if(mGrid->SnapToGrid())
					{
						x = Round((x-mXRange.min)/mGrid->XStep())*mGrid->XStep()+mXRange.min;
						y = Round((y-mYRange.min)/mGrid->YStep())*mGrid->YStep()+mYRange.min;
					}
				}
			}
			return mBPF->Insert(x,y);
		}

		void BPFEditor::Update(double x, double y)
		{
			mRebuildglList = true;
			double threshold = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);
			if(mCurrentIndex != 0)
			{
				double prior_x = mBPF->GetXValue(mCurrentIndex-1);
				if(x < prior_x+threshold) x = prior_x+threshold;
			}
			if(mCurrentIndex != mBPF->Size()-1)
			{
				double next_x = mBPF->GetXValue(mCurrentIndex+1);
				if(x > next_x-threshold) x = next_x-threshold;
			}

			if(x < mXRange.min) x = mXRange.min;
			if(x > mXRange.max) x = mXRange.max;
			if(y < mYRange.min) y = mYRange.min;
			if(y > mYRange.max) y = mYRange.max;

			if(mGrid)
			{
				if(mGrid->ShowGrid())
				{
					if(mGrid->SnapToGrid())
					{
						x = Round((x-mXRange.min)/mGrid->XStep())*mGrid->XStep()+mXRange.min;
						y = Round((y-mYRange.min)/mGrid->YStep())*mGrid->YStep()+mYRange.min;
					}
				}
			}

			if(mFlags & CLAM::VM::eAllowHorEdition)
			{
				mBPF->SetXValue(mCurrentIndex,x);
			}
			if(mFlags & CLAM::VM::eAllowVerEdition)
			{
				mBPF->SetValue(mCurrentIndex,y);
			}				
		}

		long BPFEditor::PickPoint(double x, double y)
		{
			long index = -1;
			double xtolerance = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);
			double ytolerance = double(TOLERANCE)*(mView.top-mView.bottom)/double(mViewport.h);
			long left_index = 0;
			long right_index = mBPF->Size()-1;
			while(left_index <= right_index)
			{
				const long current_index = (left_index+right_index)/2;
				
				if(double(mBPF->GetXValue(current_index)) >= (x-xtolerance) && 
				   double(mBPF->GetXValue(current_index)) <= (x+xtolerance))
				{
					index = current_index;
					break;
				}
				if(x < double(mBPF->GetXValue(current_index)))
				{
					right_index = current_index-1;
				}
				else if(x > double(mBPF->GetXValue(current_index)))
				{
					left_index = current_index+1;
				}
			}
			if(index==-1) return -1;
			if(double(mBPF->GetValueFromIndex(index)) >= (y-ytolerance) && 
			   double(mBPF->GetValueFromIndex(index)) <= (y+ytolerance))
			{
			    return index;
			}
			return -1;
		}

		long BPFEditor::GetBound(double search_value)
		{
			if(search_value <= mBPF->GetXValue(0)) return 0;
			if(search_value >= mBPF->GetXValue(mBPF->Size()-1)) return mBPF->Size()-1;

			long index=0;
			long left = 0;
			long right = mBPF->Size()-1;
			while(left <= right)
			{
				const long current = (left+right)/2;
				if(current == mBPF->Size()-1)
				{
					if(mBPF->GetXValue(current) <= search_value) 
					{
						index = current;
						break;
					}
					continue;
				}
				if(double(mBPF->GetXValue(current+1)) >= search_value && 
				   double(mBPF->GetXValue(current)) <= search_value)
				{
					index = current;
					break;
				}
				if(search_value < double(mBPF->GetXValue(current)))
				{
					right = current-1;
				}
				else if(search_value > double(mBPF->GetXValue(current)))
				{
					left = current+1;
				}
			}
			return index;
		}

		std::pair<long, long> BPFEditor::GetBounds()
		{
			if(!mBPF) return std::make_pair(0,0);
			if(!mBPF->Size()) return std::make_pair(0,0);
			if(mBPF->Size()==1) return std::make_pair(0,1);

			long left = GetBound(mView.left);
			long right = GetBound(mView.right);

			if(right < mBPF->Size()-1) right+=2;
			else if(right < mBPF->Size()) right++;

			return std::make_pair(left,right);
		}

		double BPFEditor::Round(double x) 
		{
			double i=double(int(x));
			double frac=x-i;
			return (frac >= 0.5) ? i+1.0 : i;
		}

		QString BPFEditor::GetToolTip(double x, double y)
		{
			return "P("+QString::number(x,'f',2)+","+QString::number(y,'f',2)+")";
		}

		void BPFEditor::ChooseCurrentPoint(long index)
		{
			if(!mBPF) return;
			if(!mEnabled || !mBPF->Size()) return;
			mRebuildglList = true;
			mCurrentIndex = index;
			emit requestRefresh();
		}

		void BPFEditor::MoveCurrentPointdx(double dx)
		{
			if(!mBPF) return;
			if(!mEnabled || !mBPF->Size()) return;
			if(!(mFlags & CLAM::VM::eAllowHorEdition)) return;
			mRebuildglList = true;
			double threshold = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);
			double x = mBPF->GetXValue(mCurrentIndex)+dx;
			if(mCurrentIndex != 0)
			{
				double prior_x = mBPF->GetXValue(mCurrentIndex-1);
				if(x < prior_x+threshold) x = prior_x+threshold;
			}
			if(mCurrentIndex != mBPF->Size()-1)
			{
				double next_x = mBPF->GetXValue(mCurrentIndex+1);
				if(x > next_x-threshold) x = next_x-threshold;
			}
			if(x < mXRange.min) x = mXRange.min;
			if(x > mXRange.max) x = mXRange.max;			
			if(mGrid)
			{
				if(mGrid->ShowGrid())
				{
					if(mGrid->SnapToGrid())
					{
						x = Round((x-mXRange.min)/mGrid->XStep())*mGrid->XStep()+mXRange.min;
					}
				}
			}
			mBPF->SetXValue(mCurrentIndex,x);
			emit requestRefresh();
			emit xValueChanged(mKey,mCurrentIndex,mBPF->GetXValue(mCurrentIndex));
		}

		void BPFEditor::MoveCurrentPointdy(double dy)
		{
			if(!mBPF) return;
			if(!mEnabled || !mBPF->Size()) return;
			mRebuildglList = true;
			if(!(mFlags & CLAM::VM::eAllowVerEdition)) return;
			double y = mBPF->GetValueFromIndex(mCurrentIndex)+dy;
			if(y < mYRange.min) y = mYRange.min;
			if(y > mYRange.max) y = mYRange.max;
			if(mGrid)
			{
				if(mGrid->ShowGrid())
				{
					if(mGrid->SnapToGrid())
					{
						y = Round((y-mYRange.min)/mGrid->YStep())*mGrid->YStep()+mYRange.min;
					}
				}
			}
			mBPF->SetValue(mCurrentIndex,y);
			emit requestRefresh();
			emit yValueChanged(mKey,mCurrentIndex,mBPF->GetValueFromIndex(mCurrentIndex));
		}

		double BPFEditor::GetXStep()
		{
			double dx;
			if(!mGrid)
			{
				(mKeyboard.key_ctrl) 
					? dx = (mView.right-mView.left)/double(mViewport.w)
					: dx = 5.0*(mView.right-mView.left)/double(mViewport.w);
				
			}
			else
			{
				if(mGrid->ShowGrid())
				{
					if(mGrid->SnapToGrid())
					{
						dx = mGrid->XStep();
					}
					else
					{
						(mKeyboard.key_ctrl) 
							? dx = (mView.right-mView.left)/double(mViewport.w)
							: dx = 5.0*(mView.right-mView.left)/double(mViewport.w);
					}
				}
				else
				{
					(mKeyboard.key_ctrl) 
						? dx = (mView.right-mView.left)/double(mViewport.w)
						: dx = 5.0*(mView.right-mView.left)/double(mViewport.w);
				}
			}
			return dx;
		}
		
		double BPFEditor::GetYStep()
		{
			double dy;
			if(!mGrid)
			{
				(mKeyboard.key_ctrl)
					? dy = (mView.top-mView.bottom)/double(mViewport.h)
					: dy = 5.0*(mView.top-mView.bottom)/double(mViewport.h);

			}
			else
			{
				if(mGrid->ShowGrid())
				{
					if(mGrid->SnapToGrid())
					{
						dy = mGrid->YStep();
					}
					else
					{
						(mKeyboard.key_ctrl)
							? dy = (mView.top-mView.bottom)/double(mViewport.h)
							: dy = 5.0*(mView.top-mView.bottom)/double(mViewport.h);
					}
				}
				else
				{
					(mKeyboard.key_ctrl)
						? dy = (mView.top-mView.bottom)/double(mViewport.h)
						: dy = 5.0*(mView.top-mView.bottom)/double(mViewport.h);
				}
			}
			return dy;
		}
	}
}

// END

