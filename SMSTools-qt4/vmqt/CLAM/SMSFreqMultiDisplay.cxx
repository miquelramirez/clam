#include <qlayout.h>
#include <qframe.h>
#include <CLAM/Ruler.hxx>
#include <CLAM/VScrollGroup.hxx>
#include <CLAM/DisplaySurface.hxx>
#include <CLAM/SpectrumPlotController.hxx>
#include <CLAM/SMSFreqMultiDisplay.hxx>

namespace CLAM
{
	namespace VM
	{
		SMSFreqMultiDisplay::SMSFreqMultiDisplay(QWidget* parent, const char * name, WFlags f )
			: MultiDisplayPlot(parent,name,f)
			, mShowOnNewData(true)
			, mHasMasterData(false)
		{
			InitSMSFreqPlot();
		}

		SMSFreqMultiDisplay::~SMSFreqMultiDisplay()
		{
			mYRulers.clear();
		}

		void SMSFreqMultiDisplay::InitSMSFreqPlot()
		{
			// x ruler
			CreateXRuler();

			// surfaces
			CreateSurfaces();

			QFont ref = mYRulers[0]->Font();
			QFontMetrics fm(ref);       
			int y_ruler_width = fm.width("X:-0.0e+00");
			
			SetYRulersWidth(y_ruler_width);
			
			// holes
			QFrame* topHole = new QFrame(this);
			QFrame* leftHole = new QFrame(this);
			QFrame* rightHole = new QFrame(this);
			QFrame* bottomHole = new QFrame(this);

			topHole->setFixedSize(20,GetXRuler()->height());
			leftHole->setFixedSize(y_ruler_width,20);
			rightHole->setFixedSize(20,20);
			bottomHole->setFixedHeight(5);

			// toggle frame
			CreateToggleColorFrame();
			SetToggleColorFrameWidth(y_ruler_width,true);

			// scrolling
			CreateHScrollGroup();
			mVScrollBar = new VScrollGroup(this);

			QBoxLayout* displayLayout = new QHBoxLayout;
			QBoxLayout* rulersLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* surferLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* scrollLayout = new QVBoxLayout(displayLayout); 
			// add widgets
			for(unsigned i=0; i < mYRulers.size(); i++)
			{
				rulersLayout->addWidget(mYRulers[i]);
				surferLayout->addWidget(mSurfaces[i]);
			}
			scrollLayout->addWidget(mVScrollBar);

			// layout
			QGridLayout* innerLayout = new QGridLayout(this,4,3,1);
			innerLayout->addWidget(GetToggleColorFrame(),0,0);
			innerLayout->addWidget(GetXRuler(),0,1);
			innerLayout->addWidget(topHole,0,2);
			innerLayout->addMultiCellLayout(displayLayout,1,1,0,2);
			innerLayout->addWidget(leftHole,2,0);
			innerLayout->addWidget(GetHScrollGroup(),2,1);
			innerLayout->addWidget(rightHole,2,2);
			innerLayout->addMultiCellWidget(bottomHole,3,3,0,2);

			// bind controllers to surfaces
			CreateControllers();
			BindToSurfaces();

			// connections
			ConnectXRuler();
			ConnectHScrollGroup();
			ConnectVScrollGroup();
			ConnectEditTagSlot();
			ConnectDataTypeSignal();
			SynchronizeHZoom();
			SynchronizeVZoom();
			SynchronizeDialPos();
			SynchronizeInsertMark();
			SynchronizeRemoveMark();
			SynchronizeUpdateMark();
			SynchronizeUpdateTag();
			SynchronizeYRulers();
			SynchronizeFocusIn();
			
			HideDisplays();
		}

		void SMSFreqMultiDisplay::SetSpectrumAndPeaks(const Spectrum& spec, const SpectralPeakArray& peaks, bool update)
		{
			if(update)
			{
				((SpectrumPlotController*)mControllers[MASTER])->UpdateData(spec,peaks);
			}
			else
			{
				((SpectrumPlotController*)mControllers[MASTER])->SetData(spec,peaks);
			}
			mHasMasterData = true;
			if(mShowOnNewData) ShowDisplay(MASTER);
		}

		void SMSFreqMultiDisplay::SetSinusoidalSpectrum(const Spectrum& spec, bool update)
		{
			if(!mHasMasterData) 
			{
				// dummy
				((SpectrumPlotController*)mControllers[MASTER])->SetData(spec);
				mHasMasterData = true;
			}
			if(update)
			{
				((SpectrumPlotController*)mControllers[SINUSOIDAL])->UpdateData(spec);
			}
			else
			{
				((SpectrumPlotController*)mControllers[SINUSOIDAL])->SetData(spec);
			}
			if(mShowOnNewData) ShowDisplay(SINUSOIDAL);
		}

		void SMSFreqMultiDisplay::SetResidualSpectrum(const Spectrum& spec, bool update)
		{
			if(!mHasMasterData) 
			{
				// dummy
				((SpectrumPlotController*)mControllers[MASTER])->SetData(spec);
			}
			if(update)
			{
				((SpectrumPlotController*)mControllers[RESIDUAL])->UpdateData(spec);
			}
			else
			{
				((SpectrumPlotController*)mControllers[RESIDUAL])->SetData(spec);
			}
			if(mShowOnNewData) ShowDisplay(RESIDUAL);
		}

		void SMSFreqMultiDisplay::SetForegroundColor(Color c)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				((SpectrumPlotController*)mControllers[i])->SetDataColor(c);
			}
		}

		void SMSFreqMultiDisplay::SetPeaksColor(Color cline, Color cpoint)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				((SpectrumPlotController*)mControllers[i])->SetPeaksColor(cline,cpoint);
			}
		}

		void SMSFreqMultiDisplay::ShowDisplay(int id)
		{
			if(id < 0 || id > 2) return;
			mControllers[id]->enableRendering();
			mYRulers[id]->show();
			mSurfaces[id]->show();
			if(!isVisible()) show();
		}

		void SMSFreqMultiDisplay::HideDisplay(int id)
		{
			if(id < 0 || id > 2) return;
			mControllers[id]->disableRendering();
			mYRulers[id]->hide();
			mSurfaces[id]->hide();
			CheckVisibility();
		}

		void SMSFreqMultiDisplay::ShowDisplayOnNewData(bool show)
		{
			mShowOnNewData = show;
		}

		void SMSFreqMultiDisplay::Flush()
		{
			mHasMasterData = false;
			HideDisplays();
			for(int i=0; i < 3; i++) mControllers[i]->ClearMarks();
		}

		void SMSFreqMultiDisplay::CreateControllers()
		{
			for(int i=0; i < 3; i++)
			{
				mControllers.push_back(new SpectrumPlotController());
				mControllers[i]->SetSegmentationMarksEnabled(false); // disable segmentation marks 
			}
			SetMasterId(MASTER);
		}

		void SMSFreqMultiDisplay::CreateSurfaces()
		{
			for(int i=0; i < 3; i++)
			{
				mSurfaces.push_back(new DisplaySurface(this));
				mYRulers.push_back(new Ruler(this,CLAM::VM::Left));
				mSurfaces[i]->setMinimumSize(200,20);
			}
		}

		void SMSFreqMultiDisplay::SynchronizeDialPos()
		{
			for(int i=0; i < 3; i++)
			{
				SyncDialPos(i);
			}
		}

		void SMSFreqMultiDisplay::SynchronizeInsertMark()
		{
			for(int i=0; i < 3; i++)
			{
				SyncInsertMarkBySampleIndex(i);
			}
		}

		void SMSFreqMultiDisplay::SynchronizeRemoveMark()
		{
			for(int i=0; i < 3; i++)
			{
				SyncRemoveMarkBySampleIndex(i);
			}
		}

		void SMSFreqMultiDisplay::SynchronizeUpdateMark()
		{
			for(int i=0; i < 3; i++)
			{
				SyncUpdateMarkBySampleIndex(i);
			}
		}

		void SMSFreqMultiDisplay::SynchronizeUpdateTag()
		{
			for(int i=0; i < 3; i++)
			{
				SyncUpdateTag(i);
			}
		}

		void SMSFreqMultiDisplay::DisplayBackgroundBlack()
		{
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
			SetPeaksColor(VMColor::Yellow(),VMColor::Red());
			SetBackgroundColor(VMColor::Black());
		}

		void SMSFreqMultiDisplay::DisplayBackgroundWhite()
		{
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetMarksColor(VMColor::Red());
			SetPeaksColor(VMColor::Cyan(),VMColor::Red());
			SetBackgroundColor(VMColor::White());
		}

		void SMSFreqMultiDisplay::SynchronizeVZoom()
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				if(i == unsigned(MASTER)) continue;
				connect(mControllers[MASTER],SIGNAL(yRulerRange(double,double)),
						mControllers[i],SLOT(setVBounds(double,double)));
			}
		}

		void SMSFreqMultiDisplay::CheckVisibility()
		{
		    bool flag = false;
			for(unsigned i=0; i < mYRulers.size(); i++)
			{
				if(mYRulers[i]->isVisible()) 
				{
					flag = true;
					break;
				}
			}
			if(!flag) hide();
		}

		void SMSFreqMultiDisplay::SetYRulersWidth(int w)
		{
			for(unsigned i=0; i < mYRulers.size(); i++)
			{
				mYRulers[i]->setFixedWidth(w);
			}
		}

		void SMSFreqMultiDisplay::ConnectVScrollGroup()
		{		
			connect(mVScrollBar,SIGNAL(zoomIn()),mControllers[MASTER],SLOT(vZoomIn()));
			connect(mVScrollBar,SIGNAL(zoomOut()),mControllers[MASTER],SLOT(vZoomOut()));
			connect(mVScrollBar,SIGNAL(scrollValueChanged(int)),mControllers[MASTER],SLOT(updateVScrollValue(int)));

			connect(mControllers[MASTER],SIGNAL(vZoomRatio(double)),mVScrollBar,SLOT(updateZoomRatio(double)));
			connect(mControllers[MASTER],SIGNAL(vScrollValue(int)),mVScrollBar,SLOT(updateScrollValue(int)));
			connect(mControllers[MASTER],SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
		}

		void SMSFreqMultiDisplay::SynchronizeYRulers()
		{
			for(unsigned i=0; i < mYRulers.size(); i++)
			{
				SyncYRuler(i);
			}
		}

		void SMSFreqMultiDisplay::SyncYRuler(int id)
		{
			for(unsigned i=0; i < mYRulers.size(); i++)
			{
				if(i==(unsigned)id) continue;
				connect(mControllers[id],SIGNAL(yRulerRange(double,double)),
						mYRulers[i],SLOT(updateRange(double,double)));
			}
		}

		void SMSFreqMultiDisplay::setMaxVScroll(int value)
		{
			int max = value-mSurfaces[MASTER]->height();
			if(max < 0) max=0;
			if(max >= 0 && mVScrollBar->GetMaxScrollValue() != max)
			{
				mVScrollBar->setMaxScrollValue(max);
			}
		}

		void SMSFreqMultiDisplay::HideDisplays()
		{
			for(unsigned i=0; i < mSurfaces.size(); i++)
			{
				HideDisplay(i);
			}
		}

		void SMSFreqMultiDisplay::spectrumAndPeaks(bool over)
		{
			(over) ? emit dataType("spectrum and peaks") : emit dataType("");
		}

		void SMSFreqMultiDisplay::sinusoidalSpectrum(bool over)
		{
			(over) ? emit dataType("sinusoidal spectrum") : emit dataType("");
		}

		void SMSFreqMultiDisplay::residualSpectrum(bool over)
		{
			(over) ? emit dataType("residual spectrum") : emit dataType("");
		}
	
		void SMSFreqMultiDisplay::ConnectDataTypeSignal()
		{
			connect(mControllers[MASTER],SIGNAL(mouseOverDisplay(bool)),this,SLOT(spectrumAndPeaks(bool)));
			connect(mControllers[SINUSOIDAL],SIGNAL(mouseOverDisplay(bool)),this,SLOT(sinusoidalSpectrum(bool)));
			connect(mControllers[RESIDUAL],SIGNAL(mouseOverDisplay(bool)),this,SLOT(residualSpectrum(bool)));
		}

	}
}

// END

