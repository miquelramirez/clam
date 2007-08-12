#ifndef SegmentationPane_hxx
#define SegmentationPane_hxx
#include <QtGui/QSplitter>
#include "Project.hxx"
#include "vmAudioPlot.hxx"
#include "DescriptorTableController.hxx"
#include <QtGui/QTableWidget>
#include <QtGui/QComboBox>
#include <QtGui/QVBoxLayout>
#include <CLAM/UnsizedSegmentation.hxx>
#include <CLAM/DiscontinuousSegmentation.hxx>
#include <CLAM/ContiguousSegmentation.hxx>
#include <CLAM/Pool.hxx>

class SegmentationPane : public QSplitter
{
	Q_OBJECT
public:
	SegmentationPane(QWidget * parent, CLAM_Annotator::Project & project)
		: QSplitter(parent)
		, _project(project)
		, _segmentation(0)
		, _audio(0)
		, _pool(0)
		, mSegmentDescriptors(0)
	{
		setOrientation(Qt::Horizontal);
		setMidLineWidth(4);
		mSegmentEditor = new CLAM::VM::AudioPlot(this);
		mSegmentEditor->setFrameShape(QFrame::StyledPanel);
		mSegmentEditor->setFrameShadow(QFrame::Raised);
		mSegmentEditor->setFocusPolicy(Qt::WheelFocus);
		QSizePolicy segmentEditorSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		segmentEditorSizePolicy.setHorizontalStretch(2);
		segmentEditorSizePolicy.setVerticalStretch(0);
		mSegmentEditor->setSizePolicy(segmentEditorSizePolicy);
		mSegmentEditor->setMinimumSize(700,150);
		mSegmentEditor->setAutoFillBackground(true);

		_propertiesPane = new QWidget(this);
		_propertiesPaneLayout = new QVBoxLayout(_propertiesPane);
		_propertiesPaneLayout->setSpacing(6);
		_propertiesPaneLayout->setMargin(0);
		mSegmentDescriptorsTable = new QTableWidget(_propertiesPane);
		mSegmentDescriptorsTable->setMinimumSize(QSize(200, 0));
		mSegmentDescriptorsTable->setFocusPolicy(Qt::WheelFocus);
		mSegmentDescriptorsTable->setSelectionMode(QAbstractItemView::NoSelection);
		mSegmentDescriptorsTable->setRowCount(0);
		mSegmentDescriptorsTable->setColumnCount(0);

		_propertiesPaneLayout->addWidget(mSegmentDescriptorsTable);

		mSegmentationSelection = new QComboBox(_propertiesPane);

		_propertiesPaneLayout->addWidget(mSegmentationSelection);

		addWidget(_propertiesPane);

		mSegmentDescriptors = new CLAM_Annotator::DescriptorTableController(mSegmentDescriptorsTable, _project);

		// Changing the current segmentation descriptor
		connect(mSegmentationSelection, SIGNAL(activated(const QString&)),
				this, SLOT(refreshSegmentation()));
		// Apply segment descriptors changes
		connect(mSegmentDescriptors, SIGNAL(contentEdited(int) ),
				this, SLOT(notifyChanges() ) );
		// Segment editing
		connect(mSegmentEditor, SIGNAL(segmentOnsetChanged(unsigned,double)),
				this, SLOT(notifyChanges()));
		connect(mSegmentEditor, SIGNAL(segmentOffsetChanged(unsigned,double)),
				this, SLOT(notifyChanges()));
		connect(mSegmentEditor, SIGNAL(currentSegmentChanged()),
				this, SLOT(changeCurrentSegment()));
		connect(mSegmentEditor, SIGNAL(segmentDeleted(unsigned)),
				this, SLOT(removeSegment(unsigned)));
		connect(mSegmentEditor, SIGNAL(segmentInserted(unsigned)),
				this, SLOT(insertSegment(unsigned)));
	}
	~SegmentationPane()
	{
		if (_segmentation) delete _segmentation;
		if (mSegmentDescriptors) delete mSegmentDescriptors;
	}
	void setCurrentSegmentFollowsPlay(bool enabled)
	{
		mSegmentEditor->setCurrentSegmentFollowsPlay(enabled);
	}
	void updateLocator(double timeMilliseconds, bool loque)
	{
		mSegmentEditor->updateLocator(timeMilliseconds, loque);
	}
	void updateLocator(double timeMilliseconds)
	{
		mSegmentEditor->updateLocator(timeMilliseconds);
	}
	void setAudio(const CLAM::Audio & audio, bool updating)
	{
		mSegmentEditor->SetData(audio, updating);
		_audio = & audio;
	}
	void setData(CLAM::DescriptionDataPool * pool)
	{
		_pool=pool;	
	}
	// Kludge to provide access
	const CLAM::Segmentation * getSegmentation() const
	{
		return _segmentation;
	}
signals:
	void segmentationSelectionChanged();
	void segmentationDataChanged();
public slots:
	void notifyChanges()
	{
		emit segmentationDataChanged();
	}
	void changeCurrentSegment()
	{
		std::cout << "Segment changed to " << _segmentation->current() << std::endl;
		// TODO: Some widgets may have half edited information. Need update.
		// TODO: Some times is not worth to update the information (deleted segment)
		mSegmentDescriptors->refreshData(_segmentation->current(),_pool);
	}
	void refreshSegmentation()
	{
		if (mSegmentationSelection->currentText()==QString::null) return; // No segmentation
		if (!_audio) return;
		CLAM::TData audioDuration = _audio->GetSize() / _audio->GetSampleRate();
		std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
		CLAM_Annotator::SegmentationPolicy policy = 
			_project.GetAttributeScheme("Song",currentSegmentation).GetSegmentationPolicy();

		CLAM::DataArray nullSegmentation; // A null segmentation to be passed when no data available
		const CLAM::DataArray & descriptorsMarks = _pool?
			_pool->GetReadPool<CLAM::DataArray>("Song",currentSegmentation)[0]
			: nullSegmentation;
		unsigned nMarks = descriptorsMarks.Size();
		CLAM::Segmentation * theSegmentation=0;
		switch (policy)
		{
			case CLAM_Annotator::SegmentationPolicy::eUnsized:
			{
				theSegmentation = 
					new CLAM::UnsizedSegmentation(
							audioDuration,
							&descriptorsMarks[0],
							&descriptorsMarks[0]+nMarks);
			} break;
			case CLAM_Annotator::SegmentationPolicy::eContinuous:
			{
				for (unsigned i=0; i<nMarks; i++)
					std::cout << descriptorsMarks[i] << std::endl;
				std::cout << audioDuration << std::endl;
				theSegmentation = 
					new CLAM::ContiguousSegmentation(
							audioDuration,
							&descriptorsMarks[0],
							&descriptorsMarks[0]+nMarks);
			} break;
			case CLAM_Annotator::SegmentationPolicy::eOverlapping:
				// Not yet implemented, using Discontinuous by now
			case CLAM_Annotator::SegmentationPolicy::eDiscontinuous:
			{
				theSegmentation = 
					new CLAM::DiscontinuousSegmentation(
							audioDuration,
							&descriptorsMarks[0],
							&descriptorsMarks[0]+nMarks);
			} break;
		}
		if (_segmentation) delete _segmentation;
		_segmentation = theSegmentation;
		_segmentation->xUnits("s");
		mSegmentEditor->SetSegmentation(_segmentation);

		std::string childScope = _project.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
		mSegmentDescriptors->refreshSchema(childScope);
		emit segmentationSelectionChanged();
	}

	void updateSegmentations()
	{
		if (!_pool) return;
		std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
		CLAM::DataArray & descriptorMarks = 
			_pool->GetWritePool<CLAM::DataArray>("Song",currentSegmentation)[0];
		CLAM_Annotator::SegmentationPolicy policy = 
			_project.GetAttributeScheme("Song",currentSegmentation).GetSegmentationPolicy();
		const std::vector<double> & onsets = _segmentation->onsets();
		const std::vector<double> & offsets = _segmentation->offsets();
		unsigned nSegments = onsets.size();
		switch (policy)
		{
			case CLAM_Annotator::SegmentationPolicy::eUnsized:
			{
				descriptorMarks.Resize(nSegments);
				descriptorMarks.SetSize(nSegments);
				for (unsigned i=0; i<nSegments; i++)
					descriptorMarks[i] = onsets[i];
			} break;
			case CLAM_Annotator::SegmentationPolicy::eContinuous:
			{
				descriptorMarks.Resize(nSegments-1);
				descriptorMarks.SetSize(nSegments-1);
				for (unsigned i=1; i<nSegments; i++)
					descriptorMarks[i] = onsets[i];
			} break;
			case CLAM_Annotator::SegmentationPolicy::eOverlapping:
				// Not yet implemented, using Discontinuous by now
			case CLAM_Annotator::SegmentationPolicy::eDiscontinuous:
			{
				descriptorMarks.Resize(nSegments*2);
				descriptorMarks.SetSize(nSegments*2);
				for (unsigned i=0; i<nSegments; i++)
				{
					descriptorMarks[i*2] = onsets[i];
					descriptorMarks[i*2+1] = offsets[i];
				}
			} break;
		}
		std::cout << "TODO: call annotator's markCurrentSongChanged" << std::endl;
		// TODO: This should be called from the annotator class
		// markCurrentSongChanged(true);
		std::cout << "TODO: Warn other segment panes" << std::endl;
		std::cout << "TODO: Update player" << std::endl;
	}
	void updateSchema()
	{
		mSegmentationSelection->clear();
		typedef std::list<std::string> Names;
		const Names & segmentationNames =
			_project.GetNamesByScopeAndType("Song", "Segmentation");
		QStringList segmentations;
		for (Names::const_iterator it=segmentationNames.begin(); it!=segmentationNames.end(); it++)
			mSegmentationSelection->addItem(it->c_str());
	}
	void insertSegment(unsigned index)
	{
		std::cout << tr("Inserting segment at ").toStdString() << index << std::endl;
		std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
		std::string childScope = _project.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
		if (childScope!="")
		{
			CLAM_ASSERT(index<=_pool->GetNumberOfContexts(childScope),
					"Invalid position to insert a segment");
			_pool->Insert(childScope, index);
			_project.InitInstance(childScope, index, *_pool);
		}
		updateSegmentations();
	}
	void removeSegment(unsigned index)
	{
		std::cout << tr("Removing segment at ").toStdString() << index << std::endl;
		std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
		std::string childScope = _project.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
		if (childScope!="")
		{
			CLAM_ASSERT(index<_pool->GetNumberOfContexts(childScope),
					"Invalid segment to be removed");
			_pool->Remove(childScope, index);
		}
		updateSegmentations();
	}

private:
	CLAM::VM::AudioPlot * mSegmentEditor;
	QTableWidget *mSegmentDescriptorsTable;
	QComboBox *mSegmentationSelection;
	QVBoxLayout *_propertiesPaneLayout;
	QWidget *_propertiesPane;
	CLAM_Annotator::Project & _project;
	CLAM::Segmentation * _segmentation;
	const CLAM::Audio * _audio;
	CLAM::DescriptionDataPool * _pool;
	CLAM_Annotator::DescriptorTableController * mSegmentDescriptors;
};


#endif//SegmentationPane_hxx

