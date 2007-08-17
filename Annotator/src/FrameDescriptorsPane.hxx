#ifndef FrameDescriptorsPane_hxx
#define FrameDescriptorsPane_hxx

#include <QtGui/QSplitter>
#include <QtGui/QListWidget>
#include "FrameDivision.hxx"
#include "vmBPFPlot.hxx"
#include "Project.hxx"
#include "EquidistantPointsFunction.hxx"
#include "Audio.hxx"
#include <CLAM/Pool.hxx>

class FrameDescriptorsPane : public QSplitter
{
	Q_OBJECT

	CLAM::VM::BPFPlot *mBPFEditor;
	QListWidget *mFrameLevelAttributeList;
 
private:
	CLAM_Annotator::Project& _mProject;
	std::vector<CLAM::EquidistantPointsFunction> _EPFs;
	const CLAM::Audio * _audio;
	CLAM::DescriptionDataPool * _pool;

//public slots:
//	void changeFrameLevelDescriptor(int current);

public:
	FrameDescriptorsPane(QWidget * parent,
	 					CLAM_Annotator::Project& mProject,
						std::vector<CLAM::EquidistantPointsFunction> mEPFs)
	
		: QSplitter(parent)
		, _mProject(mProject)
		, _EPFs(mEPFs)
		, _audio(0)
		, _pool(0)
	{
		setOrientation(Qt::Horizontal);
		mBPFEditor = new CLAM::VM::BPFPlot(this);
		QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		sizePolicy3.setHorizontalStretch(2);
		sizePolicy3.setVerticalStretch(0);
		sizePolicy3.setHeightForWidth(mBPFEditor->sizePolicy().hasHeightForWidth());
		mBPFEditor->setSizePolicy(sizePolicy3);
		mBPFEditor->setMinimumSize(QSize(700, 150));
		mBPFEditor->setFocusPolicy(Qt::WheelFocus);
		mBPFEditor->setFrameShape(QFrame::StyledPanel);
		mBPFEditor->setFrameShadow(QFrame::Raised);
		addWidget(mBPFEditor);
		mFrameLevelAttributeList = new QListWidget(this);
		mFrameLevelAttributeList->setMinimumSize(QSize(200, 0));
		mFrameLevelAttributeList->setAlternatingRowColors(true);
		mFrameLevelAttributeList->setResizeMode(QListView::Adjust);
		addWidget(mFrameLevelAttributeList);

		// Changing the current frame level descriptor
		connect(mFrameLevelAttributeList, SIGNAL(currentRowChanged(int)),
				this, SLOT(changeFrameLevelDescriptor(int)));
		// Apply frame descriptor changes
		connect( mBPFEditor, SIGNAL(yValueChanged(unsigned, double)),
				this, SLOT(frameDescriptorsChanged(unsigned, double)));
		// Current position update
		connect(mBPFEditor, SIGNAL(selectedRegion(double,double)),
				this, SLOT(setCurrentTime(double,double)));

// TODO: move elsewhere!
//		mVSplit->addWidget(this);

		mBPFEditor->setWhatsThis(
			tr("Annotator", "<p>The <b>frame level descriptors editor</b> allows editing\n"
			"			 floating point descriptors that has an occurrence for each frame.</p>\n"
			"			 <ul>\n"
			"			 <li>Dragging Points: Moving them.</li>\n"
			"			 <li>Page up/down: Set focus prev/next point</li>\n"
			"			 <li>Hover point: Set focus to point</li>\n"
			"			 <li>Dragging play positon: Moves it.</li>\n"
			"			 <li>'r'+Click: Set the current play position.</li>\n"
			"			 <li>Arrows: Change focused point values</li>\n"
			"			 </ul>")
		);

		mBPFEditor->SetFlags(CLAM::VM::eAllowVerEdition);//|CLAM::VM::eHasVerticalScroll); // QTPORT: What about this flag
		mBPFEditor->SetZoomSteps(5,5);
		mBPFEditor->SetXRange(0.0,2.0);

		mBPFEditor->setAutoFillBackground(true);
	}
	
	void setData(CLAM::DescriptionDataPool * pool, const CLAM::Audio & audio)
	{
		_audio = & audio;
		_pool=pool;
		refreshEnvelopes();
	}
	
	void adaptEnvelopesToCurrentSchema()
	{
		mFrameLevelAttributeList->clear();

		const std::list<std::string>& names = _mProject.GetNamesByScopeAndType("Frame", "Float");
		const unsigned nTabs = names.size();
		std::list<std::string>::const_iterator name = names.begin();
		for (unsigned i = 0; i<nTabs; name++, i++)
		{
			mFrameLevelAttributeList->addItem(name->c_str());
		}
	}
	
	void refreshEnvelopes()
	{
		std::cout<<"Loading frame level data..."<< std::endl;

		// TODO: Not all the things should be done here
		_EPFs.clear();
		mBPFEditor->SetXRange(0.0,double(_audio->GetDuration())/1000.0);	

		if (!_pool) return;

		const std::list<std::string>& divisionNames = _mProject.GetNamesByScopeAndType("Song", "FrameDivision");

		std::list<std::string>::const_iterator divisionName;
		for(divisionName = divisionNames.begin();divisionName != divisionNames.end(); divisionName++)
		{
			const CLAM_Annotator::FrameDivision & division = _pool->GetReadPool<CLAM_Annotator::FrameDivision>("Song",*divisionName)[0];

			const std::string & frameDivisionChildScope = _mProject.GetAttributeScheme("Song", *divisionName).GetChildScope();
			const std::list<std::string>& descriptorsNames = _mProject.GetNamesByScopeAndType(frameDivisionChildScope, "Float");
			std::list<std::string>::const_iterator it;
			for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++)
			{
				_EPFs.push_back(CLAM::EquidistantPointsFunction());
				CLAM::TData firstCenter = division.GetFirstCenter();
				CLAM::TData interCenterGap = division.GetInterCenterGap();
				const CLAM::TData* values = _pool->GetReadPool<CLAM::TData>(frameDivisionChildScope,*it);
				CLAM::TData sr = _audio->GetSampleRate();
				int nFrames = _pool->GetNumberOfContexts(frameDivisionChildScope);
				_EPFs.back().setDivision(firstCenter/sr, interCenterGap/sr, "s");
				_EPFs.back().setValues(values, nFrames, "");
			}
		}
		changeFrameLevelDescriptor(mFrameLevelAttributeList->currentRow());
	}
	
	void updateLocator(double timeMilliseconds, bool paused)
	{
		mBPFEditor->updateLocator(timeMilliseconds, paused);
	}
	void updateLocator(double timeMilliseconds)
	{
		mBPFEditor->updateLocator(timeMilliseconds);
	}

public slots:
	void changeFrameLevelDescriptor(int current)
	{
		unsigned index = mFrameLevelAttributeList->currentRow();
		if (index >= _EPFs.size()) return; // No valid descriptor
		double minValue = _EPFs[index].getMin();
		double maxValue = _EPFs[index].getMax();
		// TODO: Move this margin to the widget
		double span = maxValue-minValue;
		minValue -= span*0.1;
		maxValue += span*0.1;
		mBPFEditor->SetData(&_EPFs[index].GetBPF());
		CLAM::VM::ERulerScale scale = CLAM::VM::eLinearScale;
		if (fabs(minValue) > 9999.99) scale=CLAM::VM::eLogScale;
		if (fabs(maxValue) > 9999.99) scale=CLAM::VM::eLogScale;
		if (maxValue-minValue < CLAM::TData(5E-2)) scale=CLAM::VM::eLogScale;
		mBPFEditor->SetYRange(minValue, maxValue, scale);

		mBPFEditor->show();
	}

};

#endif
