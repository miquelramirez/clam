#include <qlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include "Assert.hxx"
#include "XRuler.hxx"
#include "HScrollGroup.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtAudioPlayer.hxx"
#include "QtAudioPlot.hxx"
#include "QtStereoAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtStereoAudioPlot::QtStereoAudioPlot(QWidget* parent) : QtPlot(parent)
		{
			InitStereoAudioPlot();
		}

		QtStereoAudioPlot::~QtStereoAudioPlot()
		{
		}

		void QtStereoAudioPlot::InitStereoAudioPlot()
		{
			QVBoxLayout* main = new QVBoxLayout(this);
			main->setMargin(5);
			main->setSpacing(0);

			//create player
			_player = new QtAudioPlayer(this);
			_player->setFixedSize(75,30);

			// top area (x ruler)
			QHBoxLayout* top = new QHBoxLayout(main);
			QFrame* topLeftHole = new QFrame(this);
			topLeftHole->setFixedSize(75,40);
			_btoggle_color = new QPushButton(topLeftHole);
			_btoggle_color->setFixedSize(25,25);
			_btoggle_color->setGeometry(0,8,25,25);
			_btoggle_color->setToggleButton(true);
			_btoggle_color->setText("CM");
			QToolTip::add(_btoggle_color,"switch display colors");
			SwitchDisplayColors(false);
			top->addWidget(topLeftHole,0);
			_xRuler = new XRuler(this);
			top->addWidget(_xRuler);

			// middle area - channels left and right
			QHBoxLayout* leftChannel = new QHBoxLayout(main);
			
			QVBoxLayout* left = new QVBoxLayout(leftChannel);
			QLabel* leftlabel= new QLabel(this);
			leftlabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			leftlabel->setFixedSize(25,20);
			leftlabel->setText("L");
			leftlabel->setAlignment(Qt::AlignCenter);
			QToolTip::add(leftlabel,"Left Channel");
			left->addWidget(leftlabel);

			_muteLeft = new QPushButton(this);
			_muteLeft->setFixedSize(25,20);
			_muteLeft->setToggleButton(true);
			_muteLeft->setDown(false);
			_muteLeft->setOn(false);
			_muteLeft->setText("M");
			QToolTip::add(_muteLeft,"Mute Left Channel");
			left->addWidget(_muteLeft);
			left->addStretch(1);

			_leftChannel = new QtAudioPlot(this);
			_leftChannel->SetFlag(true);
			_leftChannel->RemoveXRuler();
			_leftChannel->RemoveVScrollGroup();
			_leftChannel->RemoveHScrollGroup();
			_leftChannel->RemovePlayPanel();
			leftChannel->addWidget(_leftChannel);

			QHBoxLayout* rightChannel = new QHBoxLayout(main);

			QVBoxLayout* right = new QVBoxLayout(rightChannel);
			QLabel* rightlabel = new QLabel(this);
			rightlabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			rightlabel->setFixedSize(25,20);
			rightlabel->setText("R");
			rightlabel->setAlignment(Qt::AlignCenter);
			QToolTip::add(rightlabel,"Right Channel");
			right->addWidget(rightlabel);

			_muteRight = new QPushButton(this);
			_muteRight->setFixedSize(25,20);
			_muteRight->setToggleButton(true);
			_muteRight->setDown(false);
			_muteRight->setOn(false);
			_muteRight->setText("M");
			QToolTip::add(_muteRight,"Mute Right Channel");
			right->addWidget(_muteRight);

			right->addStretch(1);

			_rightChannel = new QtAudioPlot(this);
			_rightChannel->SetFlag(true);
			_rightChannel->RemoveXRuler();
			_rightChannel->RemoveVScrollGroup();
			_rightChannel->RemoveHScrollGroup();
			_rightChannel->RemovePlayPanel();
			rightChannel->addWidget(_rightChannel);

			// bottom area (horizontal scroll and zoom group)
			QHBoxLayout* bottom = new QHBoxLayout(main);
			QFrame* bottomLeftHole = new QFrame(this);
			bottomLeftHole->setFixedSize(75,20);
			bottom->addWidget(bottomLeftHole,0);

			_hs = new HScrollGroup(this);
			bottom->addWidget(_hs);

			// play panel
			QHBoxLayout* panel = new QHBoxLayout(main);
			
			QFrame* leftpphole = new QFrame(this);
			leftpphole->setFixedSize(75,30);
			panel->addWidget(leftpphole);

			// add player to panel
			panel->addWidget(_player);

			panel->addStretch(1);

			_labelsGroup = new TimeSegmentLabelsGroup(this);
			_labelsGroup->setFixedSize(186,25);
			panel->addWidget(_labelsGroup);
			
			// connection to switch colors 
			connect(_btoggle_color,SIGNAL(clicked()),this,SLOT(switchColors()));

			// connections for select channel buttons
			connect(_muteLeft,SIGNAL(clicked()),this,SLOT(leftChannelClicked()));
			connect(_muteRight,SIGNAL(clicked()),this,SLOT(rightChannelClicked()));

			// xRuler
			connect(_leftChannel,SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));

			// horizontal zoom ratio and horizontal scroll bar max value
			connect(_leftChannel,SIGNAL(HZoomRatio(int)),this,SLOT(receivedHZoomRatio(int)));

			// zoom in/out
			connect(_leftChannel,SIGNAL(HZoomIn()),_rightChannel,SLOT(hZoomIn()));
			connect(_leftChannel,SIGNAL(HZoomOut()),_rightChannel,SLOT(hZoomOut()));

			// max and updated horizontal scroll values
			connect(_rightChannel,SIGNAL(HMaxScroll(int)),this,SLOT(hMaxScroll(int)));
			connect(_rightChannel,SIGNAL(UpdatedHScroll(int)),this,SLOT(updateHScroll(int)));

			// Connections horizontal scroll and zoom group
			connect(_hs,SIGNAL(zoomIn()),this,SLOT(hZoomIn()));
		    connect(_hs,SIGNAL(zoomOut()),this,SLOT(hZoomOut()));
			connect(_hs,SIGNAL(scrollValueChanged(int)),this,SLOT(hScrollValue(int)));

			// region time labels
			connect(_leftChannel,SIGNAL(regionTime(MediaTime)),this,SLOT(updateRegion(MediaTime)));
			connect(_rightChannel,SIGNAL(regionTime(MediaTime)),this,SLOT(updateRegion(MediaTime)));

			// sel pos sync
			connect(_leftChannel,SIGNAL(selPos(TData)),_rightChannel,SLOT(setSelPos(TData)));
			connect(_rightChannel,SIGNAL(selPos(TData)),_leftChannel,SLOT(setSelPos(TData)));
		}

		void QtStereoAudioPlot::SetData(std::vector<Audio> data)
		{
			CLAM_ASSERT(data.size()==2,"Number of channels must be two.");
			CLAM_ASSERT(data[0].GetSize() == data[1].GetSize(),"Size of channels left and right must be the same!");
			_leftChannel->SetData(data[0]);
			_rightChannel->SetData(data[1]);
			SetPData(data);
		}

		void QtStereoAudioPlot::SetBackgroundColor(Color c)
		{
			_leftChannel->SetBackgroundColor(c);
			_rightChannel->SetForegroundColor(c);
		}

		void QtStereoAudioPlot::SetForegroundColor(Color c)
		{
			_leftChannel->SetForegroundColor(c);
			_rightChannel->SetForegroundColor(c);
		}

		void QtStereoAudioPlot::SetDialColor(Color c)
		{
			_leftChannel->SetDialColor(c);
			_rightChannel->SetDialColor(c);
		}

		void QtStereoAudioPlot::SetRegionColor(Color c)
		{
			_leftChannel->SetRegionColor(c);
			_rightChannel->SetRegionColor(c);
		}

		void QtStereoAudioPlot::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void QtStereoAudioPlot::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y,w,h);
		}

		void QtStereoAudioPlot::Show()
		{
			show();
		}

		void QtStereoAudioPlot::Hide()
		{
			hide();
		}

		void QtStereoAudioPlot::hZoomIn()
		{
			_leftChannel->hZoomIn();
			DirtyTrick();
		}

		void QtStereoAudioPlot::hZoomOut()
		{
			_leftChannel->hZoomOut();
			DirtyTrick();
		}

		void QtStereoAudioPlot::hScrollValue(int value)
		{
			_leftChannel->hScrollValue(value);
			_rightChannel->hScrollValue(value);	
		}

		void QtStereoAudioPlot::hMaxScroll(int value)
		{
			if(value >=0 && _hs->GetMaxScrollValue() != value)
			{
				_hs->setMaxScrollValue(value);
			}
		}

		void QtStereoAudioPlot::updateHScroll(int value)
		{
			if(_hs->GetScrollValue() != value)
			{
				_hs->updateScrollValue(value);
			}
		}

		void QtStereoAudioPlot::receivedHZoomRatio(int zr)
		{
			// update horizontal zoom ratio indicator
			_hs->updateZoomRatio(zr);
		}

		void QtStereoAudioPlot::setXRulerScaleDiv(QwtScaleDiv div)
		{
			_xRuler->SetScaleDiv(div);
		}
		
		void QtStereoAudioPlot::leftChannelClicked()
		{
			((QtAudioPlayer*)_player)->SetLeftChannelMuted(_muteLeft->isOn());
		}

		void QtStereoAudioPlot::rightChannelClicked()
		{
			((QtAudioPlayer*)_player)->SetRightChannelMuted(_muteRight->isOn());
		}

		void QtStereoAudioPlot::updateRegion(MediaTime time)
		{
			_player->stop();
			_player->SetPlaySegment(time);
			_labelsGroup->UpdateLabels(time);
		}

		void QtStereoAudioPlot::keyPressEvent(QKeyEvent* e)
		{
			_leftChannel->SetKeyPressed(e);
			_rightChannel->SetKeyPressed(e);
		}

		void QtStereoAudioPlot::keyReleaseEvent( QKeyEvent* e)
		{
			_leftChannel->SetKeyReleased(e);
			_rightChannel->SetKeyReleased(e);
		}

		void QtStereoAudioPlot::closeEvent(QCloseEvent* e)
		{
			RemoveFromPlayList();
			QtPlot::closeEvent(e);
		}

		void QtStereoAudioPlot::SetPData(std::vector<Audio> data)
		{
			((QtAudioPlayer*)_player)->SetData(data);
		}

		void QtStereoAudioPlot::DirtyTrick()
		{
			if(isMaximized())
			{
				_leftChannel->resize(_leftChannel->width()+1,_leftChannel->height());
				_rightChannel->resize(_rightChannel->width()+1,_rightChannel->height());
				_leftChannel->resize(_leftChannel->width()-1,_leftChannel->height());
				_rightChannel->resize(_rightChannel->width()-1,_rightChannel->height());
			}
			else
			{
				resize(width()+1,height());
				resize(width()-1,height());
			}
		}

		void QtStereoAudioPlot::SwitchDisplayColors(bool b)
		{
			(b) ? _btoggle_color->show() : _btoggle_color->hide();
		}
		
		void QtStereoAudioPlot::switchColors()
		{
			_leftChannel->SetToggleColorOn(_btoggle_color->isOn());
			_rightChannel->SetToggleColorOn(_btoggle_color->isOn());
			_leftChannel->switchColors();
			_rightChannel->switchColors();
		}

		void QtStereoAudioPlot::SetMarks(std::list<unsigned>& marks)
		{
			_leftChannel->SetMarks(marks);
			_rightChannel->SetMarks(marks);
		}

		void QtStereoAudioPlot::SetMarksColor(Color c)
		{
			_leftChannel->SetMarksColor(c);
			_leftChannel->SetMarksColor(c);
		}
	}	
}

// END

