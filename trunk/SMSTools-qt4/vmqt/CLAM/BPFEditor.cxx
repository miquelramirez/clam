#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpopupmenu.h>
#include <CLAM/VScrollGroup.hxx>
#include <CLAM/HScrollGroup.hxx>
#include <CLAM/Ruler.hxx>
#include <CLAM/BPFEditorController.hxx>
#include <CLAM/BPFEditorDisplaySurface.hxx>
#include <CLAM/QtBPFPlayer.hxx>
#include <CLAM/ListItemChooser.hxx>
#include <CLAM/BPFEditor.hxx>

namespace CLAM
{
	namespace VM
	{
		BPFEditor::BPFEditor(int eFlags, QWidget* parent,const char* name, WFlags f)
			: QWidget(parent,name,f)
			, mEFlags(eFlags)
			, mActivePlayer(true)
			, mXRuler(0)
			, mYRuler(0)
			, mController(0)
			, mDisplaySurface(0)
			, mColorScheme(EWhiteOverBlack)
			, mVScroll(0)
			, mHScroll(0)
			, topLeftHole(0)
			, topRightHole(0)
			, bottomRightHole(0)
			, playerHole(0)
			, mWhiteOverBlackScheme(false)
			, mUseFocusColors(false)
			, mPopupMenu(0)
			, mChooseBPFDialog(0)
		{
			mSlotPlayingTimeReceived.Wrap(this,&BPFEditor::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&BPFEditor::StopPlaying);

			InitBPFEditor();

			SetActivePlayer(false);
		}

		BPFEditor::~BPFEditor()
		{
		}

		void BPFEditor::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void BPFEditor::Geometry(int x, int y, int w, int h)
		{
			setGeometry(x,y,w,h);
		}

		void BPFEditor::SetData(const BPF& bpf)
		{
			mController->AddData("default",bpf);
			if(mPlayer) 
			{
				((QtBPFPlayer*)mPlayer)->AddData("default",bpf);
			}
		}

		BPF& BPFEditor::GetData()
		{
			return mController->GetData("default");
		}

		Melody& BPFEditor::GetMelody() 
		{
			if(mPlayer) mMelody = ((QtBPFPlayer*)mPlayer)->GetMelody("default");
			return mMelody;
		}

		MIDIMelody& BPFEditor::GetMIDIMelody() 
		{
			if(mPlayer) mMIDIMelody = ((QtBPFPlayer*)mPlayer)->GetMIDIMelody("default");
			return mMIDIMelody;
		}

		void BPFEditor::AddData(const std::string& key, const BPF& bpf)
		{
			mController->AddData(key,bpf);
			if(mPlayer) 
			{
				((QtBPFPlayer*)mPlayer)->AddData(key,bpf);	
			}
		}

		void BPFEditor::SetDataColor(const std::string& key, 
									 const Color& lines_color, 
									 const Color& handlers_color)
		{
			mController->SetDataColor(key, lines_color, handlers_color);
		}

		BPF& BPFEditor::GetData(const std::string& key)
		{
			return mController->GetData(key);
		}

		Melody& BPFEditor::GetMelody(const std::string& key)
		{
			if(mPlayer) mMelody = ((QtBPFPlayer*)mPlayer)->GetMelody(key);
			return mMelody;
		}

		MIDIMelody& BPFEditor::GetMIDIMelody(const std::string& key)
		{
			if(mPlayer) mMIDIMelody = ((QtBPFPlayer*)mPlayer)->GetMIDIMelody(key);
			return mMIDIMelody;
		}

		void BPFEditor::SetXRange(const double& min, const double& max, const EScale& scale)
		{
			mController->SetXRange(min,max,scale);
			mXRuler->SetScale(scale);
			if(mPlayer)
			{
				MediaTime time;
				time.SetBegin(min);
				time.SetEnd(max);
				((QtBPFPlayer*)mPlayer)->SetPlaySegment(time);
				((QtBPFPlayer*)mPlayer)->SetDuration(max-min);
			}
			AdjustLeft(min,max,false);
		}

		void BPFEditor::SetYRange(const double& min, const double& max, const EScale& scale)
		{
			mController->SetYRange(min,max,scale);
			mYRuler->SetScale(scale);
			AdjustLeft(min,max);
		}

		void BPFEditor::SetXScale(const EScale& scale)
		{
			mXRuler->SetScale(scale);
			mController->SetXScale(scale);
		}

		void BPFEditor::SetYScale(const EScale& scale)
		{
			mYRuler->SetScale(scale);
			mController->SetYScale(scale);
		}
			
		void BPFEditor::Show()
		{
			show();
		}

		void BPFEditor::Hide()
		{
			hide();
		}

		void BPFEditor::SetActivePlayer(bool active)
		{
			if(mPlayer)
			{
				if(active)
				{
					ShowPlayer();
				}
				else
				{
					HidePlayer();
				}
			}
		}

		void BPFEditor::SetAudioPtr(const Audio* audio)
		{
			if(mPlayer) ((QtBPFPlayer*)mPlayer)->SetAudioPtr(audio);
		}

		void BPFEditor::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Insert:
					if(mEFlags & CLAM::VM::AllowInsert)
					{
						mController->SetKeyInsertPressed(true); 
					}
					break;
								
				case Qt::Key_Delete:
					if(mEFlags & CLAM::VM::AllowRemove)
					{
						mController->SetKeyDeletePressed(true); 
					}
					break;
					
				case Qt::Key_Control:
					mController->SetKeyControlPressed(true);
					break;
									
				default:
					break;
			}
		}
		
		void BPFEditor::keyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_B:
					WhiteOverBlack();
					break;

				case Qt::Key_W:
					BlackOverWhite();
					break;

				case Qt::Key_Insert:
					if(mEFlags & CLAM::VM::AllowInsert)
					{
						mController->SetKeyInsertPressed(false); 
					}
					break;
								
				case Qt::Key_Delete:
					if(mEFlags & CLAM::VM::AllowRemove)
					{
						mController->SetKeyDeletePressed(false); 
					}
					break;
					
				case Qt::Key_Control:
					mController->SetKeyControlPressed(false);
					break;
									
				default:
					break;
			}
		}

		void BPFEditor::showEvent(QShowEvent* e)
		{
			mController->ActiveRendering(true);
			QWidget::showEvent(e);
		}

		void BPFEditor::hideEvent(QHideEvent* e)
		{
			mController->ActiveRendering(false);
		    if(mPlayer) ((QtBPFPlayer*)mPlayer)->stop();
		    QWidget::hideEvent(e);
		}

		void BPFEditor::updateLabels(QString xLabel, QString yLabel)
		{
			mXLabelInfo->setText(xLabel);
			mYLabelInfo->setText(yLabel);
		}

		void BPFEditor::InitBPFEditor()
		{
			// controller
			mController = new BPFEditorController(mEFlags);
			
			// main layout
			mainLayout = new QVBoxLayout(this);

			if((mEFlags & CLAM::VM::HasHorizontalScroll) 
			   && !(mEFlags & CLAM::VM::AllowZoomByMouse))
			{
			    // top area xRuler
			    topLayout = new QHBoxLayout(mainLayout);
			    mXRuler = new Ruler(this,CLAM::VM::Top);
			    topLeftHole = new QFrame(this);
			    topLayout->addWidget(topLeftHole);
			    topLayout->addWidget(mXRuler);
			    if((mEFlags & CLAM::VM::HasVerticalScroll) 
			       && !(mEFlags & CLAM::VM::AllowZoomByMouse))
			    {
					topRightHole = new QFrame(this);
					topLayout->addWidget(topRightHole);
			    }
			
			}

			// middle area: left ruler and display surface
			middleLayout = new QHBoxLayout(mainLayout);

			mYRuler = new Ruler(this,CLAM::VM::Left);

			labelFont = mYRuler->Font();

			QFontMetrics fm(labelFont);

			int initial_width=fm.width("X:-0.0e+00");

			mYRuler->setFixedWidth(initial_width);
			
			mDisplaySurface = new BPFEditorDisplaySurface(this);
			mDisplaySurface->setMinimumSize(200,100);
			mDisplaySurface->SetController(mController);

			middleLayout->addWidget(mYRuler);
			middleLayout->addWidget(mDisplaySurface);

			// bottom area:info labels and bottom ruler
			bottomLayout = new QHBoxLayout(mainLayout);
			
			int middle_panel_height=0;
			
			if(mXRuler)
			{
			    CreateHScroll();
			}
			else
			{
			    mXRuler = new Ruler(this,CLAM::VM::Bottom);
			}
			
			if(mEFlags & CLAM::VM::HasPlayer)
			{
				mPlayer = new QtBPFPlayer(this);
				((QtBPFPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
				((QtBPFPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
				AddToPlayList();
				if(mHScroll)
				{
				    mXRuler->setFixedHeight(fm.height()+30);
				    middle_panel_height = mXRuler->height();
				}
				else
				{
				    mXRuler->setFixedHeight(fm.height()+10);
				    middle_panel_height = mXRuler->height()+mPlayer->height();
				}
			}
			else
			{
				mXRuler->setFixedHeight(fm.height()+30);
				middle_panel_height = mXRuler->height();
			}

			if(topLeftHole) topLeftHole->setFixedSize(mYRuler->width(),mXRuler->height());

			labelsContainer = new QFrame(this);
			labelsContainer->setFixedSize(mYRuler->width(),middle_panel_height);
			

			int fixed_label_width = fm.width("X:");
			int fixed_label_height = fm.height();

			fixed_x_label = new QLabel(labelsContainer);
			fixed_x_label->setFont(labelFont);
			fixed_x_label->setText("X:");
			fixed_x_label->setGeometry(2, 10, fixed_label_width, fixed_label_height);

			fixed_y_label = new QLabel(labelsContainer);
			fixed_y_label->setFont(labelFont);
			fixed_y_label->setText("Y:");
			fixed_y_label->setGeometry(fixed_x_label->x(),
									   fixed_x_label->y()+fixed_x_label->height()+3,
									   fixed_x_label->width(),
									   fixed_x_label->height());

			mXLabelInfo = new QLabel(labelsContainer);
			mXLabelInfo->setFont(labelFont);
			mXLabelInfo->setAlignment(Qt::AlignRight);
			mXLabelInfo->setGeometry(fixed_x_label->x()+fixed_x_label->width(),
									 fixed_x_label->y(),
									 labelsContainer->width()-fixed_x_label->width()-2,
									 fixed_x_label->height());

			mYLabelInfo = new QLabel(labelsContainer);
			mYLabelInfo->setFont(labelFont);
			mYLabelInfo->setAlignment(Qt::AlignRight);
			mYLabelInfo->setGeometry(fixed_y_label->x()+fixed_y_label->width(),
									 fixed_y_label->y(),
									 mXLabelInfo->width(),
									 fixed_y_label->height());

			bottomLayout->addWidget(labelsContainer);

			QBoxLayout* middlePanel = new QVBoxLayout(bottomLayout);
			if(mHScroll)
			{
			    playerHole = new QFrame(this);
			    playerHole->setFixedHeight(labelsContainer->height()-mHScroll->height()+10);
			    middlePanel->addWidget(mHScroll);
			    middlePanel->addWidget(playerHole);
			}
			else
			{
			    middlePanel->addWidget(mXRuler);
			}

			if(mPlayer) middlePanel->addWidget(mPlayer);

			if((mEFlags & CLAM::VM::HasVerticalScroll) 
			   && !(mEFlags & CLAM::VM::AllowZoomByMouse))
			{
				CreateVScroll();
			}

			// connections
			connect(mController,SIGNAL(xRulerRange(double,double)),mXRuler,SLOT(updateRange(double,double)));
			connect(mController,SIGNAL(yRulerRange(double,double)),mYRuler,SLOT(updateRange(double,double)));
			connect(mController,SIGNAL(labelsText(QString,QString)),this,SLOT(updateLabels(QString,QString)));
			
			connect(mController,SIGNAL(xValueChanged(int, float)),this,SIGNAL(xValueChanged(int, float)));
			connect(mController,SIGNAL(yValueChanged(int, float)),this,SIGNAL(yValueChanged(int, float)));
			connect(mController,SIGNAL(elementAdded(int,float,float)),this,SIGNAL(elementAdded(int,float,float)));
			connect(mController,SIGNAL(elementRemoved(int)),this,SIGNAL(elementRemoved(int)));

			connect(mController,SIGNAL(selectedXPos(double)),this,SIGNAL(selectedXPos(double)));
			connect(mController,SIGNAL(rightButtonPressed()),this,SLOT(showPopupMenu()));
			connect(mController,SIGNAL(pointsChanged()),this,SIGNAL(pointsChanged()));

			connect(mDisplaySurface,SIGNAL(focusIn()),SLOT(focusInColor()));
			connect(mDisplaySurface,SIGNAL(focusOut()),SLOT(focusOutColor()));

			if(mPlayer)
			{
				connect(mController,SIGNAL(xValueChanged(int, float)),
						((QtBPFPlayer*)mPlayer),SLOT(updateNoteDuration(int, float)));

				connect(mController,SIGNAL(yValueChanged(int, float)),
						((QtBPFPlayer*)mPlayer),SLOT(updateNotePitch(int, float)));

				connect(mController,SIGNAL(elementAdded(int, float, float)),
						((QtBPFPlayer*)mPlayer),SLOT(addNote(int, float, float)));

				connect(mController,SIGNAL(elementRemoved(int)),((QtBPFPlayer*)mPlayer),SLOT(removeNote(int)));
				connect(mController,SIGNAL(currentPlayingTime(float)),this,SIGNAL(currentPlayingTime(float)));
				connect(mController,SIGNAL(stopPlaying(float)),this,SIGNAL(stopPlaying(float)));
			}
			
			// set color scheme
			BlackOverWhite();						  
		}

		void BPFEditor::SetScheme(int scheme)
		{
			switch(scheme)
			{
				case EWhiteOverBlack:
					WhiteOverBlack();
					break;
				case EBlackOverWhite:
					BlackOverWhite();
					break;
				default:
					break;
			}
		}

		void BPFEditor::WhiteOverBlack()
		{
			if(mUseFocusColors) return;
			if(mColorScheme==EWhiteOverBlack) return;

			setPaletteBackgroundColor(Qt::black);

			mController->SetDataColor(VMColor::White(),VMColor::Cyan());
			mController->SetRectColor(VMColor::White());
			mController->SetDialColor(VMColor::Red());
			mController->SetGridColor(VMColor::LightGray());

			mXRuler->SetBackgroundColor(VMColor::Black());
			mXRuler->SetForegroundColor(VMColor::White());
			mYRuler->SetBackgroundColor(VMColor::Black());
			mYRuler->SetForegroundColor(VMColor::White());

			labelsContainer->setPaletteBackgroundColor(Qt::black);

			fixed_x_label->setPaletteBackgroundColor(Qt::black);
			fixed_x_label->setPaletteForegroundColor(Qt::white);
			fixed_y_label->setPaletteBackgroundColor(Qt::black);
			fixed_y_label->setPaletteForegroundColor(Qt::white);

			mXLabelInfo->setPaletteBackgroundColor(Qt::black);
			mXLabelInfo->setPaletteForegroundColor(Qt::white);
			mYLabelInfo->setPaletteBackgroundColor(Qt::black);
			mYLabelInfo->setPaletteForegroundColor(Qt::white);

			mDisplaySurface->SetBackgroundColor(0.0f,0.0f,0.0f);

			if(bottomRightHole) bottomRightHole->setPaletteBackgroundColor(Qt::black);
			if(mPlayer) ((QtBPFPlayer*)mPlayer)->SetColorMap(CLAM::VM::BlackBackground);
			
			mColorScheme = EWhiteOverBlack;
		}

		void BPFEditor::BlackOverWhite()
		{
			if(!mUseFocusColors && mColorScheme==EBlackOverWhite) return;
			
			setPaletteBackgroundColor(Qt::white);

			mController->SetDataColor(VMColor::Black(),VMColor::Blue());
			mController->SetRectColor(VMColor::Black());
			mController->SetDialColor(VMColor::Black());
			mController->SetGridColor(VMColor::Brown());

			mXRuler->SetBackgroundColor(VMColor::White());
			mXRuler->SetForegroundColor(VMColor::Black());
			mYRuler->SetBackgroundColor(VMColor::White());
			mYRuler->SetForegroundColor(VMColor::Black());
			
			labelsContainer->setPaletteBackgroundColor(Qt::white);

			fixed_x_label->setPaletteBackgroundColor(Qt::white);
			fixed_x_label->setPaletteForegroundColor(Qt::black);
			fixed_y_label->setPaletteBackgroundColor(Qt::white);
			fixed_y_label->setPaletteForegroundColor(Qt::black);

			mXLabelInfo->setPaletteBackgroundColor(Qt::white);
			mXLabelInfo->setPaletteForegroundColor(Qt::black);
			mYLabelInfo->setPaletteBackgroundColor(Qt::white);
			mYLabelInfo->setPaletteForegroundColor(Qt::black);

			mDisplaySurface->SetBackgroundColor(1.0f,1.0f,1.0f);
			
			if(bottomRightHole) bottomRightHole->setPaletteBackgroundColor(Qt::white);
			if(mPlayer) ((QtBPFPlayer*)mPlayer)->SetColorMap(CLAM::VM::WhiteBackground);

			mColorScheme = EBlackOverWhite;
		}

		void BPFEditor::AdjustLeft(const double& min, const double& max, bool y_axis)
		{
			if(y_axis)
			{
				if(mYRuler->GetScale() == EScale::eLog) return;
			}
			
			int length_min = QString::number(min,'f',2).length();
			int length_max = QString::number(max,'f',2).length();

			QFontMetrics fm(labelFont);

			int width = (length_min > length_max) ? fm.width(QString::number(min,'f',2)) : fm.width(QString::number(max,'f',2));
		   
			width += (fixed_x_label->width()+2);

			if(width > mYRuler->width())
			{
				mYRuler->setFixedWidth(width);
				labelsContainer->setFixedWidth(width);
				if(topLeftHole) topLeftHole->setFixedWidth(width);
			}

			mXLabelInfo->setGeometry(fixed_x_label->x()+fixed_x_label->width(),
									 fixed_x_label->y(),
									 labelsContainer->width()-fixed_x_label->width()-2,
									 fixed_x_label->height());

			mYLabelInfo->setGeometry(fixed_y_label->x()+fixed_y_label->width(),
									 fixed_y_label->y(),
									 mXLabelInfo->width(),
									 fixed_y_label->height());
		} 

		void BPFEditor::setHBounds(double left, double right)
		{
			mController->SetHBounds(left,right);
		}

		void BPFEditor::selectPointFromXCoord(double xcoord)
		{
			mController->SelectPointFromXCoord(xcoord);
		}

		void BPFEditor::CreateVScroll()
		{
			mVScroll = new VScrollGroup(this);
			middleLayout->addWidget(mVScroll);
			bottomRightHole = new QFrame(this);
			bottomRightHole->setFixedSize(mVScroll->width(),labelsContainer->height());
			bottomLayout->addWidget(bottomRightHole);

			if(topRightHole) topRightHole->setFixedSize(mVScroll->width(),mXRuler->height());

			// connections
			connect(mVScroll,SIGNAL(zoomIn()),mController,SLOT(vZoomIn()));
			connect(mVScroll,SIGNAL(zoomOut()),mController,SLOT(vZoomOut()));
			connect(mVScroll,SIGNAL(scrollValueChanged(int)),mController,SLOT(updateVScrollValue(int)));
			connect(mController,SIGNAL(vZoomRatio(double)),mVScroll,SLOT(updateZoomRatio(double)));
			connect(mController,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
			connect(mController,SIGNAL(vScrollValue(int)),mVScroll,SLOT(updateScrollValue(int)));

		}

		void BPFEditor::CreateHScroll()
		{
			mHScroll = new HScrollGroup(this);

			// connections
			connect(mHScroll,SIGNAL(zoomIn()),mController,SLOT(hZoomIn()));
			connect(mHScroll,SIGNAL(zoomOut()),mController,SLOT(hZoomOut()));
			connect(mHScroll,SIGNAL(scrollValueChanged(int)),mController,SLOT(updateHScrollValue(int)));
			connect(mController,SIGNAL(hZoomRatio(double)),mHScroll,SLOT(updateZoomRatio(double)));
			connect(mController,SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));
			connect(mController,SIGNAL(hScrollValue(int)),mHScroll,SLOT(updateScrollValue(int)));

		}

		void BPFEditor::setMaxVScroll(int value)
		{
			int max = value-mDisplaySurface->height();
			if(max < 0) max=0;
			mVScroll->setMaxScrollValue(max);
		}

		void BPFEditor::setMaxHScroll(int value)
		{
			int max = value-mDisplaySurface->width();
			if(max < 0) max=0;
			mHScroll->setMaxScrollValue(max);
		}
		
		void BPFEditor::switchColors()
		{
			mWhiteOverBlackScheme = !mWhiteOverBlackScheme;
			if(mWhiteOverBlackScheme)
			{
				WhiteOverBlack();
			}
			else
			{
				BlackOverWhite();
			}
		}

		void BPFEditor::setRegionTime(MediaTime time)
		{
			if(mPlayer) ((QtBPFPlayer*)mPlayer)->SetPlaySegment(time);
		}

		void BPFEditor::setRegionTime(float begin, float end)
		{
			if(mPlayer)
			{
				MediaTime time;
				time.SetBegin(TData(begin));
				time.SetEnd(TData(end));
				((QtBPFPlayer*)mPlayer)->SetPlaySegment(time);
			}
		}

		void BPFEditor::stopPendingTasks()
		{
			if(!mPlayer) return;
			((QtBPFPlayer*)mPlayer)->StopThread();
		}

		void BPFEditor::playSimultaneously(bool both)
		{
			if(mPlayer)
			{
				((QtBPFPlayer*)mPlayer)->PlaySimultaneously(both);
			}
			
		}

		void BPFEditor::ShowPlayer()
		{
			QFont f=mXRuler->Font();
			QFontMetrics fm(f);

			int middle_panel_height=labelsContainer->height();
			if(!mHScroll)
			{
			    mXRuler->setFixedHeight(fm.height()+10);
			    middle_panel_height = mXRuler->height()+mPlayer->height();
			    labelsContainer->setFixedHeight(middle_panel_height);
			}
			else
			{
			    playerHole->hide();
			}
			
			if(bottomRightHole) bottomRightHole->setFixedHeight(middle_panel_height);
			mPlayer->show();
			mActivePlayer = true;
		}

		void BPFEditor::HidePlayer()
		{
			((QtBPFPlayer*)mPlayer)->stop();

			mPlayer->hide();
			
			QFont f=mXRuler->Font();
			QFontMetrics fm(f);

			int middle_panel_height = labelsContainer->height();
			if(!mHScroll)
			{
			    mXRuler->setFixedHeight(fm.height()+30);
			    middle_panel_height = mXRuler->height();
			    labelsContainer->setFixedHeight(middle_panel_height);
			}
			else
			{
			    playerHole->show();
			}
			if(bottomRightHole) bottomRightHole->setFixedHeight(middle_panel_height);
			mActivePlayer = false;
			
		}

		void BPFEditor::InitPopupMenu()
		{
			if(mPopupMenu) 
			{
				delete mPopupMenu;
				mPopupMenu = 0;
			}
			mPopupMenu = new QPopupMenu();
			mPopupMenu->setCheckable(true);
			if(mPlayer)
			{
				mPopupMenu->insertItem("Auralize",this,SLOT(activePlayer()),0,0);
				if(mActivePlayer) mPopupMenu->setItemChecked(0,true);
			}
			if(mController->HasMultipleBPF())
			{
				mPopupMenu->insertItem("Select...",this,SLOT(showChooseBPFDlg()),0,1);
			}
		}

		void BPFEditor::showPopupMenu()
		{
			if(!mPlayer && !mController->HasMultipleBPF()) return;
			InitPopupMenu();
			mPopupMenu->exec(QCursor::pos());
		}

		void BPFEditor::activePlayer()
		{
			mActivePlayer = !mActivePlayer;
			SetActivePlayer(mActivePlayer);
		}

		void BPFEditor::PlayingTime(TData time)
		{
		    mController->UpdateTimePos(time);
		}

		void BPFEditor::StopPlaying(TData time)
		{
		    mController->StopPlaying(time);
		}

		void BPFEditor::setCurrentPlayingTime(float t)
		{
		    mController->UpdateTimePos(TData(t));
		}

		void BPFEditor::receivedStopPlaying(float t)
		{
		    mController->StopPlaying(TData(t));
		}

		void BPFEditor::showChooseBPFDlg()
		{
			if(mChooseBPFDialog)
			{
				delete mChooseBPFDialog;
				mChooseBPFDialog=0;    
			}

			mChooseBPFDialog = new ListItemChooser("Choose BPF","Available BPFs",mController->GetBPFNamesList(),this);
			if( mChooseBPFDialog->exec() == QDialog::Accepted )
			{
				mController->SetCurrentBPF(mChooseBPFDialog->selection().ascii());
				if(mPlayer) ((QtBPFPlayer*)mPlayer)->SetCurrentBPF(mChooseBPFDialog->selection().ascii());
			}
		}

		void BPFEditor::ActiveLocator(bool active)
		{
			mController->ActiveDial(active);
		}

		void BPFEditor::ShowGrid(bool show)
		{
			mController->ShowGrid(show);
		}

		void BPFEditor::SnapToGrid(bool snap)
		{
			mController->SnapToGrid(snap);
		}

		void BPFEditor::SetGridSteps(double xstep, double ystep)
		{
			mController->SetGridSteps(xstep,ystep);
		}

		void BPFEditor::updateXValue(int index, float new_value)
		{
			mController->updateXValue(index,new_value);
		}

		void BPFEditor::updateYValue(int index, float new_value)
		{
			mController->updateYValue(index,new_value);
		}

		void BPFEditor::addElement(int index, float xvalue, float yvalue)
		{
			mController->addElement(index,xvalue,yvalue);
		}

		void BPFEditor::removeElement(int index)
		{
			mController->removeElement(index);
		}

		void BPFEditor::AllowInsertPoints(bool allow)
		{
			mController->AllowInsertPoints(allow);
		}

		void BPFEditor::AllowDeletePoints(bool allow)
		{
			mController->AllowDeletePoints(allow);
		}

		void BPFEditor::AllowModifyPoints(bool allow)
		{
			mController->AllowModifyPoints(allow);
		}

		void BPFEditor::UseFocusColors()
		{
			mUseFocusColors = true;
		}

		void BPFEditor::focusInColor()
		{
			if(!mUseFocusColors) return;
			Color c(250,225,240);

			mXRuler->SetBackgroundColor(c);
			mXRuler->SetForegroundColor(VMColor::Black());
			mYRuler->SetBackgroundColor(c);
			mYRuler->SetForegroundColor(VMColor::Black());
		
			QColor fc(QColor(c.r,c.g,c.b));

			setPaletteBackgroundColor(fc);
			
			labelsContainer->setPaletteBackgroundColor(fc);

			fixed_x_label->setPaletteBackgroundColor(fc);
			fixed_x_label->setPaletteForegroundColor(Qt::black);
			fixed_y_label->setPaletteBackgroundColor(fc);
			fixed_y_label->setPaletteForegroundColor(Qt::black);

			mXLabelInfo->setPaletteBackgroundColor(fc);
			mXLabelInfo->setPaletteForegroundColor(Qt::black);
			mYLabelInfo->setPaletteBackgroundColor(fc);
			mYLabelInfo->setPaletteForegroundColor(Qt::black);
			
			if(bottomRightHole) bottomRightHole->setPaletteBackgroundColor(fc);
		}

		void BPFEditor::focusOutColor()
		{
			if(!mUseFocusColors) return;
			BlackOverWhite();
		}
	}
}

// END

