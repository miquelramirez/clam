#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include "Ruler.hxx"
#include "BPFEditorController.hxx"
#include "BPFEditorDisplaySurface.hxx"
#include "BPFEditor.hxx"

namespace CLAM
{
    namespace VM
    {
	BPFEditor::BPFEditor(QWidget* parent)
	    : QWidget(parent)
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
	}

	BPF& BPFEditor::GetData()
	{
	    return mController->GetData();
	}


	void BPFEditor::SetXRange(const double& min, const double& max)
	{
	    mController->SetXRange(min,max);
	}

	void BPFEditor::SetYRange(const double& min, const double& max)
	{
	    mController->SetYRange(min,max);
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
		    mController->SetKeyInsertPressed(true); 
		    break;
				
		case Qt::Key_Delete:
		    mController->SetKeyDeletePressed(true); 
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
		    mController->SetKeyInsertPressed(false); 
		    break;
				
		case Qt::Key_Delete:
		    mController->SetKeyDeletePressed(false); 
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
	    mController = new BPFEditorController();
	    
	    // main layout
	    QBoxLayout* mainLayout = new QVBoxLayout(this);

	    // top area: left ruler and display surface
	    QBoxLayout* topLayout = new QHBoxLayout(mainLayout);

	    mYRuler = new Ruler(this,CLAM::VM::Left);
	    
	    mDisplaySurface = new BPFEditorDisplaySurface(this);
	    mDisplaySurface->setMinimumSize(200,100);
	    mDisplaySurface->SetController(mController);

	    topLayout->addWidget(mYRuler);
	    topLayout->addWidget(mDisplaySurface);

	    // bottom area:info labels and bottom ruler
	    QBoxLayout* bottomLayout = new QHBoxLayout(mainLayout);

	    mXRuler = new Ruler(this,CLAM::VM::Bottom);
	    
	    labelsContainer = new QFrame(this);
	    labelsContainer->setFixedWidth(mYRuler->width());
	    labelsContainer->setFixedHeight(mXRuler->height());

	    QFont labelFont;
	    labelFont.setFamily("fixed");
	    labelFont.setPointSize(10);
	    labelFont.setBold(true);
	    labelFont.setStyleHint(QFont::Courier,QFont::NoAntialias);

	    QFontMetrics fm(labelFont);
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
	    bottomLayout->addWidget(mXRuler);

	    // connections
	    connect(mController,SIGNAL(xRulerRange(double,double)),mXRuler,SLOT(updateRange(double,double)));
	    connect(mController,SIGNAL(yRulerRange(double,double)),mYRuler,SLOT(updateRange(double,double)));
	    connect(mController,SIGNAL(labelsText(QString,QString)),this,SLOT(updateLabels(QString,QString)));

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
	    
	    mColorScheme = EWhiteOverBlack;
	}

	void BPFEditor::BlackOverWhite()
	{
	    if(mColorScheme==EBlackOverWhite) return;

	    mController->SetDataColor(VMColor::Black());
	    mController->SetHandlersColor(VMColor::Red());
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

	    mColorScheme = EBlackOverWhite;
	}
    }
}

// END

