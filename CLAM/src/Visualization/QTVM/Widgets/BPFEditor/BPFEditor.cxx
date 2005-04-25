#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include "VScrollGroup.hxx"
#include "Ruler.hxx"
#include "BPFEditorController.hxx"
#include "BPFEditorDisplaySurface.hxx"
#include "QtBPFPlayer.hxx"
#include "BPFEditor.hxx"

namespace CLAM
{
    namespace VM
    {
	BPFEditor::BPFEditor(QWidget* parent,const char* name,int eFlags)
	    : QWidget(parent,name),
	      mEFlags(eFlags),
	      mController(0),
	      mDisplaySurface(0),
	      mColorScheme(EBlackOverWhite),
	      mVScroll(0),
	      bottomRightHole(0),
	      mWhiteOverBlackScheme(true)
	{
	    InitBPFEditor();
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

	void BPFEditor::BPFEditor::SetData(const BPF& bpf)
	{
	    mController->SetData(bpf);
	    if(_player) ((QtBPFPlayer*)_player)->SetData(bpf);
	}

	BPF& BPFEditor::GetData()
	{
	    return mController->GetData();
	}

	Melody& BPFEditor::GetMelody() 
	{
	    if(_player) mMelody = ((QtBPFPlayer*)_player)->GetMelody();
	    return mMelody;
	}

	MIDIMelody& BPFEditor::GetMIDIMelody() 
	{
	    if(_player) mMIDIMelody = ((QtBPFPlayer*)_player)->GetMIDIMelody();
	    return mMIDIMelody;;
	}

	void BPFEditor::SetXRange(const double& min, const double& max)
	{
	    mController->SetXRange(min,max);
	    if(_player) ((QtBPFPlayer*)_player)->SetDuration(max);
	    AdjustLeft(min,max,false);
	}

	void BPFEditor::SetYRange(const double& min, const double& max)
	{
	    mController->SetYRange(min,max);
	    AdjustLeft(min,max);
	}
	    
	void BPFEditor::Show()
	{
	    show();
	}

	void BPFEditor::Hide()
	{
	    hide();
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
				    
		default:
		    break;
	    }
	}
	
	void BPFEditor::keyReleaseEvent(QKeyEvent* e)
	{
	    int ascii_code = e->ascii();
	    if(ascii_code==87 || ascii_code==119)
	    {
		BlackOverWhite();
	    }
	    if(ascii_code==66 || ascii_code==98)
	    {
		WhiteOverBlack();
	    }

	    switch(e->key())
	    {
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
				    
		default:
		    break;
	    }
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

	    // top area: left ruler and display surface
	    topLayout = new QHBoxLayout(mainLayout);

	    mYRuler = new Ruler(this,CLAM::VM::Left);
	    mYRuler->setFixedWidth(10);
	    
	    mDisplaySurface = new BPFEditorDisplaySurface(this);
	    mDisplaySurface->setMinimumSize(200,100);
	    mDisplaySurface->SetController(mController);

	    topLayout->addWidget(mYRuler);
	    topLayout->addWidget(mDisplaySurface);

	    // bottom area:info labels and bottom ruler
	    bottomLayout = new QHBoxLayout(mainLayout);

	    QFontMetrics fm(labelFont);

	    int middle_panel_height=0;
	    
	    mXRuler = new Ruler(this,CLAM::VM::Bottom);
	    if(mEFlags & CLAM::VM::HasPlayer)
	    {
		mXRuler->setFixedHeight(fm.height()+10);
		_player = new QtBPFPlayer(this);
		middle_panel_height = mXRuler->height()+_player->height();
	    }
	    else
	    {
		mXRuler->setFixedHeight(fm.height()+30);
		middle_panel_height = mXRuler->height();
	    }

	    labelsContainer = new QFrame(this);
	    labelsContainer->setFixedWidth(mYRuler->width());
	    labelsContainer->setFixedHeight(middle_panel_height);

	    labelFont.setFamily("fixed");
	    labelFont.setPointSize(10);
	    labelFont.setBold(true);
	    labelFont.setStyleHint(QFont::Courier,QFont::NoAntialias);

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
	    middlePanel->addWidget(mXRuler);
	    if(_player) middlePanel->addWidget(_player);

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

	    connect(mController,SIGNAL(selectedXPos(double)),this,SIGNAL(selectedXPos(double)));

	    if(_player)
	    {
		connect(mController,SIGNAL(xValueChanged(int, float)),((QtBPFPlayer*)_player),SLOT(updateNoteDuration(int, float)));
		connect(mController,SIGNAL(yValueChanged(int, float)),((QtBPFPlayer*)_player),SLOT(updateNotePitch(int, float)));
		connect(mController,SIGNAL(elementAdded(int, float, float)),((QtBPFPlayer*)_player),SLOT(addNote(int, float, float)));
		connect(mController,SIGNAL(elementRemoved(int)),((QtBPFPlayer*)_player),SLOT(removeNote(int)));
	    }
	    
	    // set color scheme
	    WhiteOverBlack();
				      
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
	    if(mColorScheme==EWhiteOverBlack) return;

	    mController->SetDataColor(VMColor::White());
	    mController->SetHandlersColor(VMColor::Cyan());
	    mController->SetRectColor(VMColor::White());

	    mXRuler->SetBackgroundColor(VMColor::Black());
	    mXRuler->SetForegroundColor(VMColor::White());
	    mYRuler->SetBackgroundColor(VMColor::Black());
	    mYRuler->SetForegroundColor(VMColor::White());

	    labelsContainer->setPaletteBackgroundColor(QColor(0,0,0));

	    fixed_x_label->setPaletteBackgroundColor(QColor(0,0,0));
	    fixed_x_label->setPaletteForegroundColor(QColor(255,255,255));
	    fixed_y_label->setPaletteBackgroundColor(QColor(0,0,0));
	    fixed_y_label->setPaletteForegroundColor(QColor(255,255,255));

	    mXLabelInfo->setPaletteBackgroundColor(QColor(0,0,0));
	    mXLabelInfo->setPaletteForegroundColor(QColor(255,255,255));
	    mYLabelInfo->setPaletteBackgroundColor(QColor(0,0,0));
	    mYLabelInfo->setPaletteForegroundColor(QColor(255,255,255));

	    mDisplaySurface->SetBackgroundColor(0.0f,0.0f,0.0f);

	    if(bottomRightHole) bottomRightHole->setPaletteBackgroundColor(QColor(0,0,0));
	    if(_player) ((QtBPFPlayer*)_player)->SetColorMap(CLAM::VM::BlackBackground);
	    
	    mColorScheme = EWhiteOverBlack;
	}

	void BPFEditor::BlackOverWhite()
	{
	    if(mColorScheme==EBlackOverWhite) return;

	    mController->SetDataColor(VMColor::Black());
	    mController->SetHandlersColor(VMColor::Blue());
	    mController->SetRectColor(VMColor::Black());

	    mXRuler->SetBackgroundColor(VMColor::White());
	    mXRuler->SetForegroundColor(VMColor::Black());
	    mYRuler->SetBackgroundColor(VMColor::White());
	    mYRuler->SetForegroundColor(VMColor::Black());
	    
	    labelsContainer->setPaletteBackgroundColor(QColor(255,255,255));

	    fixed_x_label->setPaletteBackgroundColor(QColor(255,255,255));
	    fixed_x_label->setPaletteForegroundColor(QColor(0,0,0));
	    fixed_y_label->setPaletteBackgroundColor(QColor(255,255,255));
	    fixed_y_label->setPaletteForegroundColor(QColor(0,0,0));

	    mXLabelInfo->setPaletteBackgroundColor(QColor(255,255,255));
	    mXLabelInfo->setPaletteForegroundColor(QColor(0,0,0));
	    mYLabelInfo->setPaletteBackgroundColor(QColor(255,255,255));
	    mYLabelInfo->setPaletteForegroundColor(QColor(0,0,0));

	    mDisplaySurface->SetBackgroundColor(1.0f,1.0f,1.0f);
	    
	    if(bottomRightHole) bottomRightHole->setPaletteBackgroundColor(QColor(255,255,255));
	    if(_player) ((QtBPFPlayer*)_player)->SetColorMap(CLAM::VM::WhiteBackground);

	    mColorScheme = EBlackOverWhite;
	}

	void BPFEditor::AdjustLeft(const double& min, const double& max, bool y_axis)
	{
	     int length_min = QString::number(min,'f',2).length();
	     int length_max = QString::number(max,'f',2).length();

	     QFontMetrics fm(labelFont);

	     int width = (length_min > length_max) ? fm.width(QString::number(min,'f',2)) : fm.width(QString::number(max,'f',2));
	   
	     width += (fixed_x_label->width()+2);

	     if(y_axis)
	     {
		 mYRuler->setFixedWidth(width);
		 if(labelsContainer->width() > mYRuler->width()) 
		 {
		     mYRuler->setFixedWidth(labelsContainer->width());
		 }
		 else
		 {
		     labelsContainer->setFixedWidth(mYRuler->width());
		 }
	    }
	    else
	    {
		
		if(width > mYRuler->width())
		{
		    mYRuler->setFixedWidth(width);
		    labelsContainer->setFixedWidth(width);
		}
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
	    topLayout->addWidget(mVScroll);
	    bottomRightHole = new QFrame(this);
	    if(_player)
	    {
		bottomRightHole->setFixedSize(mVScroll->width(),mXRuler->height()+_player->height());
	    }
	    else
	    {
		bottomRightHole->setFixedSize(mVScroll->width(),mXRuler->height());
	    }
	    bottomLayout->addWidget(bottomRightHole);

	    // connections
	    connect(mVScroll,SIGNAL(zoomIn()),mController,SLOT(vZoomIn()));
	    connect(mVScroll,SIGNAL(zoomOut()),mController,SLOT(vZoomOut()));
	    connect(mVScroll,SIGNAL(scrollValueChanged(int)),mController,SLOT(updateVScrollValue(int)));
	    connect(mController,SIGNAL(vZoomRatio(int)),mVScroll,SLOT(updateZoomRatio(int)));
	    connect(mController,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
	    connect(mController,SIGNAL(vScrollValue(int)),mVScroll,SLOT(updateScrollValue(int)));

	}

	void BPFEditor::setMaxVScroll(int value)
	{
	    int max = value-mDisplaySurface->height();
	    if(max < 0) max=0;
	    mVScroll->setMaxScrollValue(max);
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
	    if(_player) ((QtBPFPlayer*)_player)->SetPlaySegment(time);
	}

	void BPFEditor::setRegionTime(float begin, float end)
	{
	    if(_player)
	    {
		MediaTime time;
		time.SetBegin(TData(begin));
		time.SetEnd(TData(end));
		((QtBPFPlayer*)_player)->SetPlaySegment(time);
	    }
	}

	void BPFEditor::stopPendingTasks()
	{
	    if(!_player) return;
	    ((QtBPFPlayer*)_player)->StopThread();
	}

    }
}


// END

