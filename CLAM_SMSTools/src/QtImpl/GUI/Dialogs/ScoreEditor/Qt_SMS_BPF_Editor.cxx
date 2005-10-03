#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include "BPFEditor.hxx"
#include "Qt_SMS_BPF_Editor.hxx"

namespace QtSMS
{
	Qt_SMS_BPF_Editor::Qt_SMS_BPF_Editor(QWidget* parent)
		: QWidget(parent)
	{
		Init();
	}

	Qt_SMS_BPF_Editor::~Qt_SMS_BPF_Editor()
	{
	}

	void Qt_SMS_BPF_Editor::InitPoints(double yvalue)
	{
		CLAM::BPF bpf;
		bpf.Insert(0.0,yvalue);
		bpf.Insert(1.0,yvalue);
		mBPFEditor->SetData(bpf);
	}

	void Qt_SMS_BPF_Editor::InitPoints(const CLAM::BPF& originalBPF)
	{
		mBPFEditor->SetData(originalBPF);
	}

	void Qt_SMS_BPF_Editor::Clear()
	{
		CLAM::BPF bpf; // empty bpf
		mBPFEditor->SetData(bpf);
	}

	void Qt_SMS_BPF_Editor::SetGridWidth(double xwidth, double ywidth)
	{
		mBPFEditor->SetGridSteps(xwidth,ywidth);
	}

	void Qt_SMS_BPF_Editor::SetHorizontalRange(double xmin, double xmax)
	{
		mBPFEditor->SetXRange(xmin,xmax);
	}

	void Qt_SMS_BPF_Editor::SetVerticalRange(double ymin, double ymax)
	{
		mBPFEditor->SetYRange(ymin,ymax);
	}

	CLAM::BPF& Qt_SMS_BPF_Editor::GetBPF() const
	{
		return mBPFEditor->GetData();
	}

	void Qt_SMS_BPF_Editor::showGrid()
	{
		if(mShowGrid->isChecked())
		{
			mBPFEditor->ShowGrid(true);
			mSnapToGrid->setEnabled(true);
		}
		else
		{
			mBPFEditor->ShowGrid(false);
			mBPFEditor->SnapToGrid(false);
			mSnapToGrid->setChecked(false);
			mSnapToGrid->setEnabled(false);
		}
	}

	void Qt_SMS_BPF_Editor::snapToGrid()
	{
		bool snap = (mSnapToGrid->isChecked()) ? true : false;
		mBPFEditor->SnapToGrid(snap);
	}

	void Qt_SMS_BPF_Editor::updateColorScheme(int index)
	{
		switch(index)
		{
			case 0:
				mBPFEditor->WhiteOverBlack();
				break;
			case 1:
				mBPFEditor->BlackOverWhite();
				break;
			default:
				break;
		}
	}

	void Qt_SMS_BPF_Editor::Init()
	{
		mBPFEditor = new CLAM::VM::BPFEditor(CLAM::VM::AllowInsert |
											 CLAM::VM::AllowRemove |
											 CLAM::VM::AllowHorizontalEdition | 
											 CLAM::VM::AllowVerticalEdition |
											 CLAM::VM::HasHorizontalScroll | 
											 CLAM::VM::HasVerticalScroll,
											 this);
		mBPFEditor->ActiveLocator(false);
		mBPFEditor->ShowGrid(true);
		mBPFEditor->SnapToGrid(false);

		mShowGrid = new QCheckBox(this);
		mSnapToGrid = new QCheckBox(this);
		mColorScheme = new QComboBox(this);

		mShowGrid->setChecked(true);
		mShowGrid->setText("show grid");
		mSnapToGrid->setChecked(false);
		mSnapToGrid->setText("snap to grid");
		mColorScheme->insertItem("Background black");
		mColorScheme->insertItem("Background white");

		QBoxLayout* checkControls = new QVBoxLayout;
		checkControls->addWidget(mShowGrid);
		checkControls->addWidget(mSnapToGrid);

		QBoxLayout* bottomLayout = new QHBoxLayout;
		bottomLayout->addLayout(checkControls);
		bottomLayout->addWidget(mColorScheme);

		QBoxLayout* mainLayout = new QVBoxLayout(this);
		mainLayout->addWidget(mBPFEditor);
		mainLayout->addLayout(bottomLayout);

		connect(mBPFEditor,SIGNAL(pointsChanged()),this,SIGNAL(pointsChanged()));
		connect(mShowGrid,SIGNAL(clicked()),SLOT(showGrid()));
		connect(mSnapToGrid,SIGNAL(clicked()),SLOT(snapToGrid()));
		connect(mColorScheme,SIGNAL(activated(int)),SLOT(updateColorScheme(int)));
	}
}

// END

