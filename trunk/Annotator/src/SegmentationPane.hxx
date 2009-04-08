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
		, _tableControler(0)
	{
		setOrientation(Qt::Horizontal);
		setMidLineWidth(4);
		_segmentEditor = new CLAM::VM::AudioPlot(this);
		_segmentEditor->setFrameShape(QFrame::StyledPanel);
		_segmentEditor->setFrameShadow(QFrame::Raised);
		_segmentEditor->setFocusPolicy(Qt::WheelFocus);
		QSizePolicy segmentEditorSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		segmentEditorSizePolicy.setHorizontalStretch(2);
		segmentEditorSizePolicy.setVerticalStretch(0);
		_segmentEditor->setSizePolicy(segmentEditorSizePolicy);
		_segmentEditor->setMinimumSize(700,150);
		_segmentEditor->setAutoFillBackground(true);
		_segmentEditor->setWhatsThis(
			tr("<p>The <b>segmentation editor</b> allows you to change song level segmentations.</p>\n"
				"<ul>\n"
				"<li>Click Body: Set as current segment</li>\n"
				"<li>Dragging segment limit: Moves it</li>\n"
				"<li>Insert + Click: Inserts a segment</li>\n"
				"<li>Delete: Deletes the current segment</li>\n"
				"<li>Dragging play positon: Moves it.</li>\n"
				"<li>'r'+Click: Set the current play position.</li>\n"
				"</ul>\n"));

		QWidget * propertiesPane = new QWidget(this);
		QVBoxLayout * propertiesPaneLayout = new QVBoxLayout(propertiesPane);
		propertiesPaneLayout->setSpacing(6);
		propertiesPaneLayout->setMargin(0);
		_segmentDescriptorsTable = new QTableWidget(propertiesPane);
		_segmentDescriptorsTable->setMinimumSize(QSize(200, 0));
		_segmentDescriptorsTable->setFocusPolicy(Qt::WheelFocus);
		_segmentDescriptorsTable->setSelectionMode(QAbstractItemView::NoSelection);
		_segmentDescriptorsTable->setRowCount(0);
		_segmentDescriptorsTable->setColumnCount(0);
		_segmentDescriptorsTable->setWhatsThis(
			tr("<p>The <b>segmentation level descriptors table</b>\n"
			"allows you to edit values for descriptors\n"
			"that are attached to child scope of the\n"
			"currently selected segmentation.\n"
			"</p>\n"
			"<p>You can edit the values by clicking\n"
			"to the proper cell.</p>\n"));
		propertiesPaneLayout->addWidget(_segmentDescriptorsTable);

		_segmentationSelection = new QComboBox(propertiesPane);
		_segmentationSelection->setWhatsThis(
			tr("This combo box allows you to change the segmentation"
			" that is currently displayed on the segmentation view."));
		propertiesPaneLayout->addWidget(_segmentationSelection);

		addWidget(propertiesPane);

		_tableControler = new CLAM_Annotator::DescriptorTableController(_segmentDescriptorsTable, _project);

		// Changing the current segmentation descriptor
		connect(_segmentationSelection, SIGNAL(activated(const QString&)),
				this, SLOT(refreshSegmentation()));
		// Apply segment descriptors changes
		connect(_tableControler, SIGNAL(contentEdited(int) ),
			this, SIGNAL(dataChanged() ) );
		// Segment editing
		connect(_segmentEditor, SIGNAL(segmentOnsetChanged(unsigned,double)),
				this, SLOT(updateSegmentations()));
		connect(_segmentEditor, SIGNAL(segmentOffsetChanged(unsigned,double)),
				this, SLOT(updateSegmentations()));
		connect(_segmentEditor, SIGNAL(currentSegmentChanged()),
				this, SLOT(changeCurrentSegment()));
		connect(_segmentEditor, SIGNAL(segmentDeleted(unsigned)),
				this, SLOT(removeSegment(unsigned)));
		connect(_segmentEditor, SIGNAL(segmentInserted(unsigned)),
				this, SLOT(insertSegment(unsigned)));
		connect(_segmentEditor, SIGNAL(selectedRegion(double,double)),
			this, SIGNAL(playRegionChanged(double,double)));

		// zoom change
		connect(_segmentEditor, SIGNAL(visibleXRangeChanged(double,double)),
			this, SIGNAL(visibleXRangeChanged(double,double)));
	}
	~SegmentationPane()
	{
		if (_segmentation) delete _segmentation;
		if (_tableControler) delete _tableControler;
	}
	void setCurrentSegmentFollowsPlay(bool enabled)
	{
		_segmentEditor->setCurrentSegmentFollowsPlay(enabled);
	}
	void updateLocator(double timeMilliseconds, bool paused)
	{
		_segmentEditor->updateLocator(timeMilliseconds, paused);
	}
	void updateLocator(double timeMilliseconds)
	{
		_segmentEditor->updateLocator(timeMilliseconds);
	}
	
	void setVisibleXRange(double xmin, double xmax)
	{
		_segmentEditor->setVisibleXRange(xmin, xmax);
	}
	
	void redrawAudio()
	{
		if (not _audio) return;
		_segmentEditor->SetData(*_audio, /*updating*/ true);
	}
	void setData(CLAM::DescriptionDataPool * pool, const CLAM::Audio & audio)
	{
		_segmentEditor->SetData(audio, /*updating*/ false);
		_audio = & audio;
		_pool=pool;	
		refreshSegmentation();
	}
	// Kludge to provide access
	const CLAM::Segmentation * getSegmentation() const
	{
		return _segmentation;
	}
	void adaptToSchema()
	{
		_segmentationSelection->clear();
		typedef std::list<std::string> Names;
		const Names & segmentationNames =
			_project.GetNamesByScopeAndType("Song", "Segmentation");
		QStringList segmentations;
		for (Names::const_iterator it=segmentationNames.begin(); it!=segmentationNames.end(); it++)
			_segmentationSelection->addItem(it->c_str());
	}
signals:
	void segmentationSelectionChanged();
	void dataChanged();
	void playRegionChanged(double startMiliseconds, double endMiliseconds);
	void visibleXRangeChanged(double min, double max);
public slots:
	void changeCurrentSegment()
	{
		std::cout << "Segment changed to " << _segmentation->current() << std::endl;
		// TODO: Some widgets may have half edited information. Need update.
		// TODO: Some times is not worth to update the information (deleted segment)
		_tableControler->refreshData(_segmentation->current(),_pool);
	}
	void refreshSegmentation()
	{
		if (_segmentationSelection->currentText()==QString::null) return; // No segmentation
		if (!_audio) return;
		CLAM::TData audioDuration = _audio->GetSize() / _audio->GetSampleRate();
		std::string currentSegmentation = _segmentationSelection->currentText().toStdString();
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
				theSegmentation = new CLAM::UnsizedSegmentation;
			} break;
			case CLAM_Annotator::SegmentationPolicy::eContinuous:
			{
				theSegmentation = new CLAM::ContiguousSegmentation;
			} break;
			case CLAM_Annotator::SegmentationPolicy::eOverlapping:
				// Not yet implemented, using Discontinuous by now
			case CLAM_Annotator::SegmentationPolicy::eDiscontinuous:
			{
				theSegmentation = new CLAM::DiscontinuousSegmentation;
			} break;
			default: CLAM_ASSERT(false, "Unsupported kind of segmentation");
		}
		if (_segmentation) delete _segmentation;
		_segmentation = theSegmentation;
		_segmentation->maxPosition(audioDuration);
		const CLAM::TData * marks = nMarks? (&descriptorsMarks[0]) : 0;
		_segmentation->takeArray(marks, marks+nMarks);
		_segmentation->xUnits("s");
		_segmentEditor->SetSegmentation(_segmentation);

		std::string childScope = _project.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
		_tableControler->refreshSchema(childScope);
		emit segmentationSelectionChanged();
	}

	void updateSegmentations()
	{
		if (!_pool) return;
		std::string currentSegmentation = _segmentationSelection->currentText().toStdString();
		CLAM::DataArray & descriptorMarks = 
			_pool->GetWritePool<CLAM::DataArray>("Song",currentSegmentation)[0];
		_segmentation->fillArray(descriptorMarks);
		emit dataChanged();
	}
	void insertSegment(unsigned index)
	{
		std::cout << tr("Inserting segment at ").toStdString() << index << std::endl;
		std::string currentSegmentation = _segmentationSelection->currentText().toStdString();
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
		std::string currentSegmentation = _segmentationSelection->currentText().toStdString();
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
	CLAM::VM::AudioPlot * _segmentEditor;
	QTableWidget *_segmentDescriptorsTable;
	QComboBox *_segmentationSelection;
	CLAM_Annotator::Project & _project;
	CLAM::Segmentation * _segmentation;
	const CLAM::Audio * _audio;
	CLAM::DescriptionDataPool * _pool;
	CLAM_Annotator::DescriptorTableController * _tableControler;
};


#endif//SegmentationPane_hxx

