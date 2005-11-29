#include <qlayout.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qtooltip.h>
#include "PlotController.hxx"
#include "DisplaySurface.hxx"
#include "Ruler.hxx"
#include "HScrollGroup.hxx"
#include "VScrollGroup.hxx"
#include "SingleDisplayPlot.hxx"
#include "EditTagDialog.hxx"

namespace CLAM
{
    namespace VM
    {
		SingleDisplayPlot::SingleDisplayPlot(QWidget* parent, const char * name, WFlags f) 
			: QtPlot(parent,name,f)
			, mController(0)
			, editTagDlg(0)
			, mIsEditTagDialogEnabled(true)
		{
			Init();
		}
		
		SingleDisplayPlot::~SingleDisplayPlot()
		{
		}

		void SingleDisplayPlot::SetBackgroundColor(Color c)
		{
			double r = double(c.r)/255.0;
			double g = double(c.g)/255.0;
			double b = double(c.b)/255.0;

			mDisplaySurface->SetBackgroundColor(r,g,b);
		}

		void SingleDisplayPlot::SetDialColor(Color c)
		{
			mController->SetDialColor(c);
		}

		void SingleDisplayPlot::SetController(PlotController* controller)
		{
			mController = controller;
			mDisplaySurface->SetController(mController);

			// Connections vertical scroll and zoom
			connect(mVScrollBar,SIGNAL(zoomIn()),mController,SLOT(vZoomIn()));
			connect(mVScrollBar,SIGNAL(zoomOut()),mController,SLOT(vZoomOut()));
			connect(mVScrollBar,SIGNAL(scrollValueChanged(int)),mController,SLOT(updateVScrollValue(int)));

			// Connections horizontal scroll and zoom group
			connect(mHScrollBar,SIGNAL(zoomIn()),mController,SLOT(hZoomIn()));
			connect(mHScrollBar,SIGNAL(zoomOut()),mController,SLOT(hZoomOut()));
			connect(mHScrollBar,SIGNAL(scrollValueChanged(int)),mController,SLOT(updateHScrollValue(int)));
			
			// vertical scroll and zoom ratio management 
			connect(mController,SIGNAL(vZoomRatio(double)),mVScrollBar,SLOT(updateZoomRatio(double)));
			connect(mController,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
			connect(mController,SIGNAL(vScrollValue(int)),mVScrollBar,SLOT(updateScrollValue(int)));

			// horizontal scroll and zoom ratio management
			connect(mController,SIGNAL(hZoomRatio(double)),mHScrollBar,SLOT(updateZoomRatio(double)));
			connect(mController,SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));
			connect(mController,SIGNAL(hScrollValue(int)),mHScrollBar,SLOT(updateScrollValue(int)));

			// connections to emit signals for segmentation marks management
			connect(mController,SIGNAL(insertedMark(unsigned)),this,SIGNAL(insertedMark(unsigned)));
			connect(mController,SIGNAL(removedMark(int,unsigned)),this,SIGNAL(removedMark(int,unsigned)));
			connect(mController,SIGNAL(updatedMark(int,unsigned)),this,SIGNAL(updatedMark(int,unsigned)));

			// connect rulers
			connect(mController,SIGNAL(xRulerRange(double,double)),mXRuler,SLOT(updateRange(double,double)));
			connect(mController,SIGNAL(yRulerRange(double,double)),mYRuler,SLOT(updateRange(double,double)));

			// selected x pos
			connect(mController,SIGNAL(selectedXPos(double)),this,SIGNAL(selectedXPos(double)));

			// to show edit tag dialog
			connect(mController,SIGNAL(requestSegmentationTag()),this,SLOT(showEditTagDialog()));

			// signal updated tag 
			connect(mController,SIGNAL(updatedTag(int,QString)),this,SIGNAL(updatedTag(int,QString)));

			// to adjust left margin
			connect(mController,SIGNAL(initialYRulerRange(double,double)),this,SLOT(initialYRulerRange(double,double)));

			// connect signals from SegmentEditor
			connect(mController,SIGNAL(segmentOnsetChanged(unsigned,double)),
					this,SIGNAL(segmentOnsetChanged(unsigned,double)));
			connect(mController,SIGNAL(segmentOffsetChanged(unsigned,double)),
					this,SIGNAL(segmentOffsetChanged(unsigned,double)));
			connect(mController,SIGNAL(segmentInserted(unsigned)),
					this,SIGNAL(segmentInserted(unsigned)));
			connect(mController,SIGNAL(segmentDeleted(unsigned)),
					this,SIGNAL(segmentDeleted(unsigned)));
			connect(mController,SIGNAL(currentSegmentChanged(unsigned)),
					this,SIGNAL(currentSegmentChanged(unsigned)));
		}

		void SingleDisplayPlot::Init()
		{
			QFont f("Sans",8);
			f.setBold(true);

			// top area (x ruler)
			mXRuler = new Ruler(this,CLAM::VM::Top);

			QFont ref = mXRuler->Font();
			QFontMetrics fm(ref);
			int initial_yruler_width=fm.width("X:-0.0e+00");
	    
			topLeftHole = new QFrame(this);
			topLeftHole->setFixedSize(initial_yruler_width,40);
			mToggleColor = new QPushButton(topLeftHole);
			mToggleColor->setFont(f);
			mToggleColor->setFixedSize(25,25);
			mToggleColor->setGeometry(12,8,25,25);
			mToggleColor->setToggleButton(true);
			mToggleColor->setText("CM");
			QToolTip::add(mToggleColor,"switch display colors");
			SwitchDisplayColors(false);

			mXRuler->setFixedHeight(topLeftHole->height());
	    
			QFrame* topRightHole = new QFrame(this);
			topRightHole->setFixedSize(20,40);

			// middle area (y ruler, display area, vertical scroll and zoom group)
			mYRuler = new Ruler(this,CLAM::VM::Left);
			mYRuler->setFixedWidth(initial_yruler_width);
			
			mDisplaySurface = new DisplaySurface(this);
			mDisplaySurface->setMinimumSize(200,20);
			
			mVScrollBar = new VScrollGroup(this);
			
			// bottom area (horizontal scroll and zoom group)
			bottomLeftHole = new QFrame(this);
			bottomLeftHole->setFixedSize(initial_yruler_width,20);
			
			mHScrollBar = new HScrollGroup(this);
			
			QFrame* bottomRightHole = new QFrame(this);
			bottomRightHole->setFixedSize(20,20);

			// layout
			QGridLayout* innerLayout = new QGridLayout;
			innerLayout->addWidget(topLeftHole,0,0);
			innerLayout->addWidget(mXRuler,0,1);
			innerLayout->addWidget(topRightHole,0,2);
			innerLayout->addWidget(mYRuler,1,0);
			innerLayout->addWidget(mDisplaySurface,1,1);
			innerLayout->addWidget(mVScrollBar,1,2);
			innerLayout->addWidget(bottomLeftHole,2,0);
			innerLayout->addWidget(mHScrollBar,2,1);
			innerLayout->addWidget(bottomRightHole,2,2);

			mMainLayout = new QVBoxLayout(this);
			mMainLayout->setMargin(2);
			mMainLayout->addLayout(innerLayout);

			// toggle button to change display colors automatically
			connect(mToggleColor,SIGNAL(clicked()),this,SLOT(switchColors()));

			// connect ruler signals
			connect(mXRuler,SIGNAL(valueChanged(double,double)),this,SIGNAL(xRulerRange(double,double)));
			connect(mYRuler,SIGNAL(valueChanged(double,double)),this,SIGNAL(yRulerRange(double,double)));

		}

		void SingleDisplayPlot::setMaxHScroll(int value)
		{
			int max = value-mDisplaySurface->width();
			if(max < 0) max=0;;
			if(max >= 0 && mHScrollBar->GetMaxScrollValue() != max)
			{
				mHScrollBar->setMaxScrollValue(max);
			}
		}

		void SingleDisplayPlot::setMaxVScroll(int value)
		{
			int max = value-mDisplaySurface->height();
			if(max < 0) max=0;
			if(max >= 0 && mVScrollBar->GetMaxScrollValue() != max)
			{
				mVScrollBar->setMaxScrollValue(max);
			}
		}

		void SingleDisplayPlot::AddToMainLayout(QLayout* layout)
		{
			mMainLayout->addLayout(layout);
		}

		void SingleDisplayPlot::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void SingleDisplayPlot::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y,w,h);
		}

		void SingleDisplayPlot::Show()
		{
			show();
		}

		void SingleDisplayPlot::Hide()
		{
			hide();
		}

		void SingleDisplayPlot::SwitchDisplayColors(bool b)
		{
			(b) ? mToggleColor->show() : mToggleColor->hide();
		}

		void SingleDisplayPlot::switchColors()
		{
			if(mToggleColor->isOn())
			{
				DisplayBackgroundBlack();
			}
			else
			{
				DisplayBackgroundWhite();	
			}
			emit switchColorsRequested();
		}

		void SingleDisplayPlot::SetToggleColorOn(bool b)
		{
			mToggleColor->setOn(b);
		}

		int SingleDisplayPlot::YRulerWidth()
		{
			return mYRuler->width();
		}
    
		void SingleDisplayPlot::initialYRulerRange(double min, double max)
		{
			int length_min = QString::number(min,'f',2).length();
			int length_max = QString::number(max,'f',2).length();

			QFontMetrics fm(mYRuler->Font());

			int width = (length_min > length_max) ? fm.width(QString::number(min,'f',2)) : fm.width(QString::number(max,'f',2));
			width += 10;

			if(width > YRulerWidth())
			{
				int bt_x = (width-mToggleColor->width())/2;
				topLeftHole->setFixedSize(width,topLeftHole->height());
				mToggleColor->setGeometry(bt_x,mToggleColor->y(),mToggleColor->width(),mToggleColor->height());
				mYRuler->setFixedWidth(width);
				bottomLeftHole->setFixedSize(width,bottomLeftHole->height());
			}
		}

		void SingleDisplayPlot::showEditTagDialog()
		{
			if(mIsEditTagDialogEnabled)
			{
				if(editTagDlg)
				{
					delete editTagDlg;
					editTagDlg=0;    
				}

				editTagDlg = new EditTagDialog(mController->GetTag(),this);
				if( editTagDlg->exec() == QDialog::Accepted )
				{
					mController->SetSegmentationTag(editTagDlg->text());
				}
			}
			else
			{
				emit requestSegmentationTag(mController->GetMarkIndex());
			}
		}

		void SingleDisplayPlot::keyPressEvent(QKeyEvent* e)
		{
			mController->KeyPressEvent(e);
		}

		void SingleDisplayPlot::keyReleaseEvent(QKeyEvent* e)
		{
			mController->KeyReleaseEvent(e);
		}

		void SingleDisplayPlot::SetMarks(std::vector<unsigned>& marks)
		{
			mController->SetMarks(marks);
		}
	    
		std::vector<unsigned>& SingleDisplayPlot::GetMarks()
		{
			return mController->GetMarks();
		}

		void SingleDisplayPlot::SetMarksColor(Color c)
		{
			mController->SetMarksColor(c);
		}

		std::vector<QString> SingleDisplayPlot::GetSegmentationTags()
		{
			return mController->GetTags();
		}
		
		void SingleDisplayPlot::setSelectedXPos(double xpos)
		{
			mController->setSelectedXPos(xpos);
		}
		
		void SingleDisplayPlot::showEvent(QShowEvent* e)
		{
			mController->enableRendering();
			QWidget::showEvent(e);
		}

		void SingleDisplayPlot::hideEvent(QHideEvent* e)
		{
			mController->disableRendering();
			QWidget::hideEvent(e);
		}

		void SingleDisplayPlot::SetEditTagDialogEnabled(bool e)
		{
			mIsEditTagDialogEnabled = e;
		}

		void SingleDisplayPlot::SetSegmentation(Segmentation* s)
		{
			mController->SetSegmentation(s);
		}
    }
}

// END

