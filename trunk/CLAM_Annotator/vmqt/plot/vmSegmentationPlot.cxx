#include <QtGui/QGridLayout>
#include "vmRuler.hxx"
#include "vmPlot2D.hxx"
#include "vmScrollGroup.hxx"
#include "vmSegmentation.hxx"
#include "vmSegmentEditor.hxx"
#include "vmLocatorRenderer.hxx"
#include "vmSegmentationPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		SegmentationPlot::SegmentationPlot(QWidget* parent)
			: WPlot(parent)
			, mCurrentSegmentFollowsPlay(true)
		{
			InitSegmentationPlot();
		}

		SegmentationPlot::~SegmentationPlot()
		{
		}

		void SegmentationPlot::SetXRange(double xmin, double xmax, ERulerScale scale)
		{
			mPlot->SetXRange(xmin,xmax);
			mXRuler->SetRange(xmin,xmax);
			mXRuler->SetScale(scale);
		}

		void SegmentationPlot::SetYRange(double ymin, double ymax, ERulerScale scale)
		{
			mPlot->SetYRange(ymin,ymax);
			mYRuler->SetRange(ymin,ymax);
			mYRuler->SetScale(scale);
			AdjustYRulerWidth(ymin,ymax);
		}

		void SegmentationPlot::SetSegmentation(Segmentation* s)
		{
			mSegmentation->SetSegmentation(s);
		}

		void SegmentationPlot::SetZoomSteps(int hsteps, int vsteps)
		{
			mPlot->SetZoomSteps(hsteps,vsteps);
		}

		void SegmentationPlot::backgroundWhite()
		{
			setPalette(Qt::white);
			mXRuler->SetBackgroundColor(Color(255,255,255));
			mXRuler->SetForegroundColor(Color(0,0,0));
			mYRuler->SetBackgroundColor(Color(255,255,255));
			mYRuler->SetForegroundColor(Color(0,0,0));
			mHScroll->setPalette(Qt::white);
			mVScroll->setPalette(Qt::white);
			mPlot->SetBackgroundColor(Color(255,255,255));
			mLocator->SetLocatorColor(Color(250,160,30));
		}

		void SegmentationPlot::backgroundBlack()
		{
			setPalette(Qt::black);
			mXRuler->SetBackgroundColor(Color(0,0,0));
			mXRuler->SetForegroundColor(Color(255,255,255));
			mYRuler->SetBackgroundColor(Color(0,0,0));
			mYRuler->SetForegroundColor(Color(255,255,255));
			mHScroll->setPalette(Qt::darkGreen);
			mVScroll->setPalette(Qt::darkGreen);
			mPlot->SetBackgroundColor(Color(0,0,0));
			mLocator->SetLocatorColor(Color(255,0,0));
		}

		void SegmentationPlot::setCurrentSegmentFollowsPlay(bool active)
		{
			mCurrentSegmentFollowsPlay=active;
			mSegmentation->allowChangeCurrent(active);
		}

		void SegmentationPlot::updateLocator(double value)
		{
			mLocator->updateLocator(value);
			mSegmentation->checkCurrent(value);
		}

		void SegmentationPlot::updateLocator(double value, bool flag)
		{
			mLocator->updateLocator(value,flag);
			mSegmentation->checkCurrent(value);
		}

		void SegmentationPlot::setMaxVScroll(int value)
		{
			int max = value-mPlot->height();
			if(max < 0) max=0;
			if(mVScroll->GetMaxScrollValue() == max) return;		
			mVScroll->setMaxScrollValue(max);
		}

		void SegmentationPlot::setMaxHScroll(int value)
		{
			int max = value-mPlot->width();
			if(max < 0) max=0;;
			if(mHScroll->GetMaxScrollValue() == max) return;
			mHScroll->setMaxScrollValue(max);
		}

		void SegmentationPlot::InitSegmentationPlot()
		{
			mXRuler = new Ruler(this,CLAM::VM::eTop);
			mYRuler = new Ruler(this,CLAM::VM::eLeft);

			QFontMetrics fm(mYRuler->GetFont());
			int yruler_width = fm.width("-0.0e+00")+12;

			mXRuler->setFixedHeight(40);
			mYRuler->setFixedWidth(yruler_width);

			mHScroll = new ScrollGroup(CLAM::VM::eHorizontal,this);
			mVScroll = new ScrollGroup(CLAM::VM::eVertical,this);

			mPlot = new Plot2D(this);
			mLocator =  new Locator();
			mPlot->AddRenderer("locator", mLocator);
			mSegmentation = new SegmentEditor();
			mPlot->AddRenderer("segmentation", mSegmentation);
			mPlot->BringToFront("locator");

			mLayout = new QGridLayout(this);
			mLayout->setMargin(0);
			mLayout->setSpacing(0);
			mLayout->addWidget(mXRuler,0,1);
			mLayout->addWidget(mYRuler,1,0);
			mLayout->addWidget(mPlot,1,1); 
			mLayout->addWidget(mVScroll,1,2);
			mLayout->addWidget(mHScroll,2,1);

			backgroundWhite();

			connect(mPlot,SIGNAL(xRulerRange(double,double)),mXRuler,SLOT(updateRange(double,double)));
			connect(mPlot,SIGNAL(yRulerRange(double,double)),mYRuler,SLOT(updateRange(double,double)));
	
			connect(mVScroll,SIGNAL(zoomIn()),mPlot,SLOT(vZoomIn()));
			connect(mVScroll,SIGNAL(zoomOut()),mPlot,SLOT(vZoomOut()));
			connect(mVScroll,SIGNAL(scrollValueChanged(int)),mPlot,SLOT(updateVScrollValue(int)));

			connect(mHScroll,SIGNAL(zoomIn()),mPlot,SLOT(hZoomIn()));
			connect(mHScroll,SIGNAL(zoomOut()),mPlot,SLOT(hZoomOut()));
			connect(mHScroll,SIGNAL(scrollValueChanged(int)),mPlot,SLOT(updateHScrollValue(int)));
			
			connect(mPlot,SIGNAL(vZoomRatio(QString)),mVScroll,SLOT(updateZoomRatio(QString)));
			connect(mPlot,SIGNAL(vScrollValue(int)),mVScroll,SLOT(updateScrollValue(int)));
			connect(mPlot,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));

			connect(mPlot,SIGNAL(hZoomRatio(QString)),mHScroll,SLOT(updateZoomRatio(QString)));
			connect(mPlot,SIGNAL(hScrollValue(int)),mHScroll,SLOT(updateScrollValue(int)));
			connect(mPlot,SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));

			connect(mLocator,SIGNAL(selectedRegion(double,double)),this,SIGNAL(selectedRegion(double,double)));

			connect(mSegmentation,SIGNAL(segmentOnsetChanged(unsigned,double)),
					this,SIGNAL(segmentOnsetChanged(unsigned,double)));
			connect(mSegmentation,SIGNAL(segmentOffsetChanged(unsigned,double)),
					this,SIGNAL(segmentOffsetChanged(unsigned,double)));
			connect(mSegmentation,SIGNAL(segmentInserted(unsigned)),this,SIGNAL(segmentInserted(unsigned)));
			connect(mSegmentation,SIGNAL(segmentDeleted(unsigned)),this,SIGNAL(segmentDeleted(unsigned)));
			connect(mSegmentation,SIGNAL(currentSegmentChanged()),this,SIGNAL(currentSegmentChanged()));
		}

		void SegmentationPlot::AdjustYRulerWidth(double min, double max)
		{
			if(mYRuler->GetScale() == CLAM::VM::eLogScale) return;
			
			int length_min = QString::number(min,'f',2).length();
			int length_max = QString::number(max,'f',2).length();

			QFontMetrics fm(mYRuler->GetFont());

			int width = (length_min > length_max) 
				? fm.width(QString::number(min,'f',2)) 
				: fm.width(QString::number(max,'f',2));
			width += 12;
			if(mYRuler->width() < width) mYRuler->setFixedWidth(width);
		}
	}
}

// END
