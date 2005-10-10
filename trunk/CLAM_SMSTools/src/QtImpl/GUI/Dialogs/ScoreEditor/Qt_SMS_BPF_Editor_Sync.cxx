#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qwhatsthis.h>
#include "BPFEditor.hxx"
#include "Qt_SMS_BPF_Editor_Sync.hxx"

namespace QtSMS
{
	Qt_SMS_BPF_Editor_Sync::Qt_SMS_BPF_Editor_Sync(QWidget* parent)
		: QWidget(parent)
	{
		Init();
	}

	Qt_SMS_BPF_Editor_Sync::~Qt_SMS_BPF_Editor_Sync()
	{
	}

	void Qt_SMS_BPF_Editor_Sync::InitPoints(double yvalue)
	{
		CLAM::BPF bpf;
		bpf.Insert(0.0,yvalue);
		bpf.Insert(1.0,yvalue);
		mBPFEditor->SetData(bpf);
		emit pointsChanged();
	}

	void Qt_SMS_BPF_Editor_Sync::InitPoints(const CLAM::BPF& originalBPF)
	{
		mBPFEditor->SetData(originalBPF);
		emit pointsChanged();
	}

	void Qt_SMS_BPF_Editor_Sync::Clear()
	{
		CLAM::BPF bpf; // empty bpf
		mBPFEditor->SetData(bpf);
		emit pointsChanged();
	}

	void Qt_SMS_BPF_Editor_Sync::SetGridWidth(double xwidth, double ywidth)
	{
		mBPFEditor->SetGridSteps(xwidth,ywidth);
	}

	void Qt_SMS_BPF_Editor_Sync::SetHorizontalRange(double xmin, double xmax)
	{
		mBPFEditor->SetXRange(xmin,xmax);
	}

	void Qt_SMS_BPF_Editor_Sync::SetVerticalRange(double ymin, double ymax)
	{
		mBPFEditor->SetYRange(ymin,ymax);
	}

	CLAM::BPF& Qt_SMS_BPF_Editor_Sync::GetBPF() const
	{
		return mBPFEditor->GetData();
	}

	void Qt_SMS_BPF_Editor_Sync::SetSyncToMasterChecked(bool checked)
	{
		mSyncToMaster->setChecked(checked);
		syncToMasterChanged();
	}

	void Qt_SMS_BPF_Editor_Sync::WhatsThis(const char* text)
	{
		QWhatsThis::add(mBPFEditor,text);
	}


	void Qt_SMS_BPF_Editor_Sync::updateXValue(int index, float new_value)
	{
		if(!mSyncToMaster->isChecked()) return;
		mBPFEditor->updateXValue(index, new_value);
	}

	void Qt_SMS_BPF_Editor_Sync::updateYValue(int index, float new_value)
	{
		if(!mSyncToMaster->isChecked()) return;
		mBPFEditor->updateYValue(index, new_value);
	}

	void Qt_SMS_BPF_Editor_Sync::addElement(int index, float xvalue, float yvalue)
	{
		if(!mSyncToMaster->isChecked()) return;
		mBPFEditor->addElement(index,xvalue,yvalue);
	}

	void Qt_SMS_BPF_Editor_Sync::removeElement(int index)
	{
		if(!mSyncToMaster->isChecked()) return;
		mBPFEditor->removeElement(index);
	}


	void Qt_SMS_BPF_Editor_Sync::showGrid()
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

	void Qt_SMS_BPF_Editor_Sync::snapToGrid()
	{
		bool snap = (mSnapToGrid->isChecked()) ? true : false;
		mBPFEditor->SnapToGrid(snap);
	}

	void Qt_SMS_BPF_Editor_Sync::syncToMasterChanged()
	{
		bool allow_edit_points = (mSyncToMaster->isChecked()) ? false : true;
		mBPFEditor->AllowInsertPoints(allow_edit_points);
		mBPFEditor->AllowDeletePoints(allow_edit_points);
		mBPFEditor->AllowModifyPoints(allow_edit_points);
	}

	void Qt_SMS_BPF_Editor_Sync::updateColorScheme(int index)
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

	void Qt_SMS_BPF_Editor_Sync::Init()
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
		mSyncToMaster = new QCheckBox(this);
		mColorScheme = new QComboBox(this);

		mShowGrid->setChecked(true);
		mShowGrid->setText("show grid");
		mSnapToGrid->setChecked(false);
		mSnapToGrid->setText("snap to grid");
		mSyncToMaster->setChecked(false);
		mSyncToMaster->setText("sync to master");
		mColorScheme->insertItem("Background black");
		mColorScheme->insertItem("Background white");

		QBoxLayout* checkControls = new QVBoxLayout;
		checkControls->addWidget(mShowGrid);
		checkControls->addWidget(mSnapToGrid);
		checkControls->addWidget(mSyncToMaster);

		QBoxLayout* bottomLayout = new QHBoxLayout;
		bottomLayout->addLayout(checkControls);
		bottomLayout->addWidget(mColorScheme);

		QBoxLayout* mainLayout = new QVBoxLayout(this);
		mainLayout->addWidget(mBPFEditor);
		mainLayout->addLayout(bottomLayout);

		connect(mBPFEditor,SIGNAL(pointsChanged()),this,SIGNAL(pointsChanged()));
		connect(mShowGrid,SIGNAL(clicked()),SLOT(showGrid()));
		connect(mSnapToGrid,SIGNAL(clicked()),SLOT(snapToGrid()));
		connect(mSyncToMaster,SIGNAL(clicked()),SLOT(syncToMasterChanged()));
		connect(mColorScheme,SIGNAL(activated(int)),SLOT(updateColorScheme(int)));
	}
}

// END

