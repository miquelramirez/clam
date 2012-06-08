#include <qframe.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <CLAM/Assert.hxx>
#include <CLAM/Ruler.hxx>
#include <CLAM/HScrollGroup.hxx>
#include <CLAM/PlotController.hxx>
#include <CLAM/DisplaySurface.hxx>
#include <CLAM/EditTagDialog.hxx>
#include <CLAM/MultiDisplayPlot.hxx>

namespace CLAM
{
	namespace VM
	{
		MultiDisplayPlot::MultiDisplayPlot(QWidget* parent,  const char * name, WFlags f)
			: QtPlot(parent,name,f)
			, mMasterId(-1)
			, mXRuler(0)
			, mHScrollBar(0)
			, mToggleContainer(0)
			, mToggleButton(0)
			, editTagDlg(0)
		{
		}

		MultiDisplayPlot::~MultiDisplayPlot()
		{
			mControllers.clear();
			mSurfaces.clear();
		}

		void MultiDisplayPlot::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void MultiDisplayPlot::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y,w,h);
		}

		void MultiDisplayPlot::Show()
		{
			show();
		}

		void MultiDisplayPlot::Hide()
		{
			hide();
		}

		void MultiDisplayPlot::SetBackgroundColor(Color c)
		{
			double r = double(c.r)/255.0;
			double g = double(c.g)/255.0;
			double b = double(c.b)/255.0;

			for(unsigned i=0; i < mSurfaces.size(); i++)
			{
				mSurfaces[i]->SetBackgroundColor(r,g,b);
			}
		}

		void MultiDisplayPlot::SetDialColor(Color c)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				mControllers[i]->SetDialColor(c);
			}
		}

		void MultiDisplayPlot::SetMarks(std::vector<unsigned>& marks)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				mControllers[i]->SetMarks(marks);
			}
		}

		std::vector<unsigned>& MultiDisplayPlot::GetMarks()
		{
			CLAM_ASSERT( mMasterId >=0 && mMasterId < (int)mControllers.size(), 
						 "MultiDisplayPlot::GetMarks: Invalid masterId." );
			return mControllers[mMasterId]->GetMarks();
		}

		void MultiDisplayPlot::SetMarksColor(Color c)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				mControllers[i]->SetMarksColor(c);
			}
		}

		std::vector<QString> MultiDisplayPlot::GetSegmentationTags()
		{
			CLAM_ASSERT( mMasterId >=0 && mMasterId < (int)mControllers.size(), 
						 "MultiDisplayPlot::GetSegmentationTags: Invalid masterId." );
			return mControllers[mMasterId]->GetTags();
		}

		void MultiDisplayPlot::SetToggleColorOn(bool on)
		{
			if(!mToggleButton) return;
			mToggleButton->setOn(on);
		}

		void MultiDisplayPlot::SwitchDisplayColors(bool sw)
		{
			(sw) ? mToggleButton->show() : mToggleButton->hide();
		}

		void MultiDisplayPlot::switchColors()
		{
			if(mToggleButton->isOn())
			{
				DisplayBackgroundBlack();
			}
			else
			{
				DisplayBackgroundWhite();	
			}
			emit switchColorsRequested();
		}

		void MultiDisplayPlot::SyncDialPos(const int& senderId)
		{
			CLAM_ASSERT( senderId >=0 && senderId < (int)mControllers.size(), 
						 "MultiDisplayPlot::GetSyncDialPos: invalid senderId." );
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				if(i == unsigned(senderId)) continue;
				connect(mControllers[senderId],SIGNAL(selectedXPos(double)),
						mControllers[i],SLOT(setSelectedXPos(double)));	
			}
		}

		void MultiDisplayPlot::SyncInsertMarkBySampleIndex(const int& senderId)
		{
			CLAM_ASSERT( senderId >=0 && senderId < (int)mControllers.size(), 
						 "MultiDisplayPlot::GetSyncInsertMarkBySampleIndex: invalid senderId." );
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				if(i == unsigned(senderId)) continue;
				connect(mControllers[senderId],SIGNAL(insertedMark(unsigned)),
						mControllers[i],SLOT(insertMark(unsigned)));
			}
			
		}

		void MultiDisplayPlot::SyncRemoveMarkBySampleIndex(const int& senderId)
		{
			CLAM_ASSERT( senderId >=0 && senderId < (int)mControllers.size(), 
						 "MultiDisplayPlot::GetSyncRemoveMarkBySampleIndex: invalid senderId." );
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				if(i == unsigned(senderId)) continue;
				connect(mControllers[senderId],SIGNAL(removedMark(int,unsigned)),
						mControllers[i],SLOT(removeMark(int,unsigned)));
			}
					
		}

		void MultiDisplayPlot::SyncUpdateMarkBySampleIndex(const int& senderId)
		{
			CLAM_ASSERT( senderId >=0 && senderId < (int)mControllers.size(), 
						 "MultiDisplayPlot::GetSyncUpdateMarkBySampleIndex: invalid senderId." );
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				if(i == unsigned(senderId)) continue;
				connect(mControllers[senderId],SIGNAL(updatedMark(int,unsigned)),
						mControllers[i],SLOT(updateMark(int,unsigned)));
			}
		}

		void MultiDisplayPlot::SyncUpdateTag(const int& senderId)
		{
			CLAM_ASSERT( senderId >=0 && senderId < (int)mControllers.size(), 
						 "MultiDisplayPlot::GetSyncUpdateTag: invalid senderId." );
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				if(i == unsigned(senderId)) continue;
				connect(mControllers[senderId],SIGNAL(updatedTag(int,QString)),
						mControllers[i],SLOT(updateTag(int,QString)));
			}
		}

		void MultiDisplayPlot::SetMasterId(const int& mid)
		{
			CLAM_ASSERT( mControllers.size() > 0, 
						 "MultiDisplayPlot::SetMasterId: master controller has not been created yet." );
			CLAM_ASSERT( mid >=0 && mid < (int)mControllers.size(), 
						 "MultiDisplayPlot::SetMasterId: invalid masterId." );
			mMasterId = mid;
		}

		void MultiDisplayPlot::BindToSurfaces()
		{
			CLAM_ASSERT( mControllers.size() > 0, 
						 "MultiDisplayPlot::BindToSurfaces: thre are not controllers to bind." );
			CLAM_ASSERT( mControllers.size() == mSurfaces.size(), 
						 "MultiDisplayPlot::BindToSurfaces: the number of controllers and surfaces must be the same." );
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				mSurfaces[i]->SetController(mControllers[i]);
			}
		}

		void MultiDisplayPlot::SynchronizeHZoom()
		{
			CLAM_ASSERT(mMasterId >= 0,"MultiDisplayPlot::SyncHZoom: the master controller has not been created yet." );
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				if(i == unsigned(mMasterId)) continue;
				connect(mControllers[mMasterId],SIGNAL(xRulerRange(double,double)),
						mControllers[i],SLOT(setHBounds(double,double)));
			}
		}

		void MultiDisplayPlot::SynchronizeFocusIn()
		{
			for(unsigned i=0; i < mSurfaces.size(); i++)
			{
				connect(mSurfaces[i],SIGNAL(focusIn()),SIGNAL(focusIn()));
			}
		}

		void MultiDisplayPlot::keyPressEvent(QKeyEvent* e)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				mControllers[i]->KeyPressEvent(e);
			}
		}

		void MultiDisplayPlot::keyReleaseEvent( QKeyEvent* e)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				mControllers[i]->KeyReleaseEvent(e);
			}
		}

		void MultiDisplayPlot::CreateToggleColorFrame()
		{
			if(mToggleContainer) return;
		   
			QFont f("Sans",8);
			f.setBold(true);
			
			mToggleContainer = new QFrame(this);
			mToggleContainer->setFixedSize(XRULER_HEIGHT,XRULER_HEIGHT);
			mToggleButton = new QPushButton(mToggleContainer);
			mToggleButton->setFont(f);
			mToggleButton->setFixedSize(25,25);
			mToggleButton->setGeometry(0,8,25,25);
			mToggleButton->setToggleButton(true);
			mToggleButton->setText("CM");
			QToolTip::add(mToggleButton,"switch display colors");
			SwitchDisplayColors(false);

			// connect
			connect(mToggleButton,SIGNAL(clicked()),this,SLOT(switchColors()));
		}

		void MultiDisplayPlot::SetToggleColorFrameWidth(const int& w, bool center)
		{
			if(!mToggleContainer) return;
			mToggleContainer->setFixedWidth(w);
			if(!center) return;
			int x = (mToggleContainer->width()-mToggleButton->width())/2;
			mToggleButton->setGeometry(x,mToggleButton->y(),mToggleButton->width(),mToggleButton->height());
		}
			
		void MultiDisplayPlot::CreateXRuler()
		{
			if(mXRuler) return;
			mXRuler = new Ruler(this,CLAM::VM::Top);
			mXRuler->setFixedHeight(XRULER_HEIGHT);
		}

		void MultiDisplayPlot::CreateHScrollGroup()
		{
			if(mHScrollBar) return;
			mHScrollBar = new HScrollGroup(this);
		}

		void MultiDisplayPlot::ConnectXRuler()
		{
			CLAM_ASSERT( mXRuler,"MultiDisplayPlot::ConnectXRuler: mXRuler has not been created yet." );
			CLAM_ASSERT( mMasterId >=0, "MultiDisplayPlot::ConnectXRuler: invalid masterId." );
			connect(mControllers[mMasterId],SIGNAL(xRulerRange(double,double)),mXRuler,SLOT(updateRange(double,double)));
		}

		void MultiDisplayPlot::ConnectHScrollGroup()
		{
			CLAM_ASSERT( mHScrollBar,"MultiDisplayPlot::ConnectHScrollGroup: mHScrollBar has not been created yet." );
			CLAM_ASSERT( mMasterId >=0, "MultiDisplayPlot::ConnectHScrollGroup: invalid masterId." );

			connect(mHScrollBar,SIGNAL(zoomIn()),mControllers[mMasterId],SLOT(hZoomIn()));
			connect(mHScrollBar,SIGNAL(zoomOut()),mControllers[mMasterId],SLOT(hZoomOut()));
			connect(mHScrollBar,SIGNAL(scrollValueChanged(int)),mControllers[mMasterId],SLOT(updateHScrollValue(int)));

			connect(mControllers[mMasterId],SIGNAL(hZoomRatio(double)),mHScrollBar,SLOT(updateZoomRatio(double)));
			connect(mControllers[mMasterId],SIGNAL(hScrollValue(int)),mHScrollBar,SLOT(updateScrollValue(int)));
			connect(mControllers[mMasterId],SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));

		}

		void MultiDisplayPlot::ConnectEditTagSlot()
		{
			for(unsigned i = 0; i < mControllers.size(); i++)
			{
				connect(mControllers[i],SIGNAL(requestSegmentationTag()),this,SLOT(showEditTagDialog()));
			}

		}

		QWidget* MultiDisplayPlot::GetToggleColorFrame()
		{
			CLAM_ASSERT( mToggleContainer,"MultiDisplayPlot::GetToggleColorFrame: frame has not been created yet." );
			return mToggleContainer;
		}

		QWidget* MultiDisplayPlot::GetXRuler()
		{
			CLAM_ASSERT( mXRuler,"MultiDisplayPlot::GetXRuler: mXRuler has not been created yet." );
			return mXRuler;
		}

		QWidget* MultiDisplayPlot::GetHScrollGroup()
		{
			CLAM_ASSERT( mHScrollBar,"MultiDisplayPlot::GetHScrollBar: mHScrollBar has not been created yet." );
			return mHScrollBar;
		}

		void MultiDisplayPlot::setMaxHScroll(int value)
		{
			int max = value-mSurfaces[0]->width();
			if(max < 0) max=0;;
			if(max >= 0 && mHScrollBar->GetMaxScrollValue() != max)
			{
				mHScrollBar->setMaxScrollValue(max);
			}
		}

		void MultiDisplayPlot::showEditTagDialog()
		{
			if(editTagDlg)
			{
				delete editTagDlg;
				editTagDlg=0;    
			}

			editTagDlg = new EditTagDialog(mControllers[mMasterId]->GetTag(),this);
			if( editTagDlg->exec() == QDialog::Accepted )
			{
				mControllers[mMasterId]->SetSegmentationTag(editTagDlg->text());
			}
		}
	  
	}
}

// END

