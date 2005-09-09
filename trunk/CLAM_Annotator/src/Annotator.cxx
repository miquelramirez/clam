#include "Annotator.hxx"
#include "Project.hxx"
#include "RangeSelectionTableItem.hxx"
#include <qaction.h>
#include <qsplitter.h>
#include <qtextbrowser.h>
#include <qtoolbutton.h>
#include <qmessagebox.h>
#include <algorithm>
#include <qpopupmenu.h>
#include <qprogressdialog.h>
#include <qapplication.h>
#include <qeventloop.h>
#include <qfiledialog.h>
#include <qthread.h>
#include <qprocess.h>


#include <algorithm>
#include <iostream>
#include <utility>

//xamat
#include <time.h>

#include "QtAudioPlot.hxx"
#include <qtabwidget.h>
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "IndexArray.hxx"
#include "Text.hxx"


#include "XMLStorage.hxx"

#include "BPFEditor.hxx"

using CLAM::VM::QtAudioPlot;

using CLAM::TData;
using CLAM::TIndex;

class AudioLoadThread : public QThread
{
	bool mAborted;
	CLAM::Audio & mAudio;
	const CLAM::TSize readSize;
	unsigned nSamples;
	CLAM::AudioFile file;
	std::vector<CLAM::Audio> audioFrameVector;
	unsigned mNumber;
public:
	static unsigned number()
	{
		static unsigned number = 0;
		return number++;
	}
	AudioLoadThread(CLAM::Audio & audio, const std::string audioFileName)
		: mAborted(false)
		, mAudio(audio)
		, readSize(4096)
		, mNumber(number())
	{
		std::cout << "AudioLoader " << mNumber << " created..." << std::endl;
		file.OpenExisting(audioFileName);
		int nChannels = file.GetHeader().GetChannels();
		float samplingRate = file.GetHeader().GetSampleRate();
		nSamples = file.GetHeader().GetLength()/1000.0*samplingRate;
		audioFrameVector.resize(nChannels);
		for (int i=0;i<nChannels;i++)
			audioFrameVector[i].SetSize(readSize);
		mAudio.SetSize(0);
		mAudio.SetSize(nSamples);
		mAudio.SetSampleRate(samplingRate);
	}
	~AudioLoadThread()
	{
		std::cout << "Aborting audio loader " << mNumber << "..." << std::endl;
		mAborted=true;
		wait();
		std::cout << "AudioLoader " << mNumber << " destroyed..." << std::endl;
	}
	void run()
	{
		std::cout << "AudioLoader " << mNumber << " running..." << std::endl;
		CLAM::MultiChannelAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MultiChannelAudioFileReader reader(cfg);
		reader.Start();
		int beginSample=0;
		while(reader.Do(audioFrameVector))
		{
			mAudio.SetAudioChunk(beginSample,audioFrameVector[0]);
			beginSample+=readSize;
			if (mAborted) break;
			if (beginSample+readSize>nSamples) break;
		}
//		mAudio.SetSize(beginSample);
		reader.Stop();
		std::cout << "AudioLoader " << mNumber << " finished..." << std::endl;
	}
};

void Annotator::abortLoader()
{
	if (!mAudioLoaderThread) return;
	delete mAudioLoaderThread;
	mAudioLoaderThread=0;
}
void Annotator::loaderCreate(CLAM::Audio & audio, const char * filename)
{
	abortLoader();
	mAudioLoaderThread = new AudioLoadThread(audio, filename);
}

void Annotator::computeSongDescriptors()
{
	std::cout << "Launching Extractor..." << std::endl;
	if (!mProjectOverview->selectedItem()) return;
	QString filename = mProjectOverview->selectedItem()->text(0);
	QProcess extractor(this);
	extractor.addArgument("../buildTest/Test");
	extractor.addArgument(filename);
	if (!extractor.start())
	{
		std::cout << "Launch failed..." << std::endl;
		return;
	}
	while (extractor.isRunning())
	{
		while (extractor.canReadLineStdout())
			std::cout << extractor.readLineStdout() << std::endl;
		while (extractor.canReadLineStderr())
			std::cout << extractor.readLineStderr() << std::endl;
	}
	while (extractor.canReadLineStdout())
		std::cout << extractor.readLineStdout() << std::endl;
	while (extractor.canReadLineStderr())
		std::cout << extractor.readLineStderr() << std::endl;
}

void Annotator::loaderLaunch()
{
	CLAM_ASSERT(mAudioLoaderThread, "Launching a loader when none created");
	mAudioLoaderThread->start();
}
bool Annotator::loaderFinished()
{
	if (!mAudioLoaderThread) return true;
	if (!mAudioLoaderThread->finished()) return false;
	abortLoader();
	return true;
}

Annotator::Annotator(const std::string & nameProject = "")
	: AnnotatorBase( 0, "annotator", WDestructiveClose)
	, mCurrentIndex(0)
	, mpDescriptorPool(0)
	, mHLDChanged(false)
	, mLLDChanged(false)
	, mSegmentsChanged(false)
	, mBPFEditors(0)
	, mpTabLayout(0)
	, mpAudioPlot(0)
	, mAudioRefreshTimer(new QTimer(this))
	, mAudioLoaderThread(0)
{
	mDescriptorsTable->setLeftMargin(0);
	initAudioWidget();
	initInterface();
	setMenuAudioItemsEnabled(false);
	connect (mAudioRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshAudioData()) );
}

Annotator::~Annotator()
{
	abortLoader();
}

void Annotator::initInterface()
{
	if (mpAudioPlot) mpAudioPlot->Hide();
	mProjectOverview->setSorting(-1); // Unordered
	makeConnections();
}

void Annotator::markProjectChanged(bool changed)
{
	mGlobalChanges = changed;
	fileSave_projectAction->setEnabled(changed);
}

void Annotator::initProject()
{
	updateSongListWidget();

	try
	{
		mProject.LoadScheme(mProject.GetSchema());
	}
	catch (CLAM::XmlStorageErr & e)
	{
		QMessageBox::warning(this,"Error Loading Schema File",
			constructFileError(mProject.GetSchema(),e));
		return;
	}
	AdaptInterfaceToCurrentSchema();
	
	markProjectChanged(false);
	mLLDChanged = false;
	mHLDChanged = false;
	mSegmentsChanged = false;
}

void Annotator::AdaptInterfaceToCurrentSchema()
{
	AdaptDescriptorsTableToCurrentSchema(mDescriptorsTable, "Song");
	AdaptEnvelopesToCurrentSchema();
	AdaptSegmentationsToCurrentSchema();
}

void Annotator::initAudioWidget()
{
	if(mpAudioPlot) delete mpAudioPlot;
	mpAudioPlot = new QtAudioPlot(mAudioPlotContainer);
	mpTabLayout = new QVBoxLayout(mAudioPlotContainer);
	mpTabLayout->addWidget(mpAudioPlot);
	mpAudioPlot->Label("Audio");
	mCurrentAudio.SetSize(20000);
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->SwitchDisplayColors(true);
	mpAudioPlot->SetToggleColorOn(true);
	mpAudioPlot->switchColors();
	mpAudioPlot->setFocus();
	mpAudioPlot->Hide();
}

void Annotator::AdaptSegmentationsToCurrentSchema()
{
	mSegmentationSelection->clear();
	const std::list<std::string> & segmentationNames =
		mProject.GetSongSegmentationNames();
	for (std::list<std::string>::const_iterator it =  segmentationNames.begin();
		it != segmentationNames.end();
		it++)
	{
		mSegmentationSelection->insertItem(it->c_str());
	}
}

void Annotator::AdaptEnvelopesToCurrentSchema()
{
	removeLLDTabs();

	const std::list<std::string>& names = mProject.GetFrameScopeAttributeNames();
	const unsigned nTabs = names.size();
	const int eFlags = CLAM::VM::AllowVerticalEdition | CLAM::VM::HasVerticalScroll | CLAM::VM::HasPlayer;
	mTabPages.resize(nTabs);
	mBPFEditors.resize(nTabs);
	std::list<std::string>::const_iterator name = names.begin();
	for (unsigned i = 0; i<nTabs; name++, i++)
	{
		if (i==0)
		{
			mTabPages[0] = tabWidget2->page(0);
		}
		else
		{
			mTabPages[i] = new QWidget( tabWidget2, "Dummy");
			tabWidget2->insertTab( mTabPages[i], QString("") );
		}

		tabWidget2->changeTab( mTabPages[i], (*name).c_str() );
		QVBoxLayout* tabLayout = new QVBoxLayout( mTabPages[i]);
		mBPFEditors[i] = new CLAM::VM::BPFEditor(eFlags, mTabPages[i]);
		mBPFEditors[i]->SetActivePlayer(false);
		mBPFEditors[i]->Hide();
		tabLayout->addWidget(mBPFEditors[i]);
	}
	connectBPFs();
}

void Annotator::AdaptDescriptorsTableToCurrentSchema(QTable * table, const std::string & scope)
{
	CLAM_Annotator::Project::SongScopeSchema hlds = mProject.GetSongScopeSchema();
	table->setNumRows(hlds.size());
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it = hlds.begin();
	for(int i = 0 ; it != hlds.end(); it++, i++)
	{
		table->setItem(i, 0,
			new TableItem(table,
				TableItem::Never,
				QString(it->GetName().c_str())));
	}
}

void Annotator::removeLLDTabs()
{
	const unsigned nTabs = mTabPages.size();
	if (!nTabs) return;
	for (unsigned i = mTabPages.size(); --i; ) // reverse, 0 not included
	{
		tabWidget2->removePage(tabWidget2->page(i));
	}
	tabWidget2->changeTab(tabWidget2->page(0), tr("No Low Level Descriptors") );
	mTabPages.resize(0);
}


void Annotator::makeConnections()
{
	connect(helpAboutAction,SIGNAL(activated()),&mAbout,SLOT(show()));
	connect(mDescriptorsTable, SIGNAL(valueChanged( int, int) ) , this, 
		SLOT(descriptorsTableChanged(int, int) ) );
	connect(mpAudioPlot, SIGNAL(updatedMark(int, unsigned)),this,
		SLOT(segmentationMarksChanged(int, unsigned)));

}

void Annotator::connectBPFs()
{
	std::vector<CLAM::VM::BPFEditor*>::iterator it;
	for(it = mBPFEditors.begin(); it != mBPFEditors.end(); it++)
	{
		CLAM::VM::BPFEditor* bpfEditor = *it;
		connect( bpfEditor, SIGNAL(yValueChanged(int, float)),
			this, SLOT(descriptorsBPFChanged(int, float)));

		connect( bpfEditor, SIGNAL(selectedXPos(double)),
			mpAudioPlot, SLOT(setSelectedXPos(double)));

		connect(mpAudioPlot, SIGNAL(xRulerRange(double,double)),
			bpfEditor, SLOT(setHBounds(double,double)));

		connect(mpAudioPlot, SIGNAL(selectedXPos(double)),
			bpfEditor, SLOT(selectPointFromXCoord(double)));

		connect(mpAudioPlot, SIGNAL(switchColorsRequested()),
			bpfEditor, SLOT(switchColors()));

		connect(mpAudioPlot, SIGNAL(regionTime(float,float)),
			bpfEditor, SLOT(setRegionTime(float,float)));

		connect(mpAudioPlot, SIGNAL(currentPlayingTime(float)),
			bpfEditor, SLOT(setCurrentPlayingTime(float)));

		connect(mpAudioPlot, SIGNAL(stopPlayingTime(float)),
			bpfEditor, SLOT(receivedStopPlaying(float)));

		connect(bpfEditor, SIGNAL(currentPlayingTime(float)),
			mpAudioPlot, SLOT(setCurrentPlayingTime(float)));

		connect(bpfEditor, SIGNAL(stopPlaying(float)),
			mpAudioPlot, SLOT(receivedStopPlaying(float)));
	}
}

void Annotator::changeCurrentFile()
{
	QListViewItemIterator it( mProjectOverview );
	for ( ; it.current() && !it.current()->isSelected() ; it++ );
	if ( it.current() )
	{
		it.current()->setText(2, "Yes");	
	}
}


void Annotator::descriptorsTableChanged(int row, int column)
{
	mHLDChanged = true;
	updateDescriptorTableData(mDescriptorsTable, "Song", 0, row);
	changeCurrentFile();
}

void Annotator::updateDescriptorTableData(QTable * table, const std::string & scope, unsigned element, int row)
{
	std::string name = table->text(row,0).ascii();
	const CLAM_Annotator::HLDSchemaElement & hldSchemaElement = 
		mProject.GetAttributeScheme(scope,name);

	const std::string & type = hldSchemaElement.GetType();
	QString qValue = table->text(row, 1);
	const std::string & value = qValue.ascii();

	if (type == "String")
	{
		mpDescriptorPool->GetWritePool<CLAM::Text>(scope,name)[element] = value;
	}
	if (type == "RestrictedString")
	{
		mpDescriptorPool->GetWritePool<CLAM_Annotator::RestrictedString>(scope,name)[element].SetString(value);
	}
	if (type == "Float")
	{
		mpDescriptorPool->GetWritePool<float>(scope,name)[element] = qValue.toFloat();
	}
	if (type == "Int")
	{
		mpDescriptorPool->GetWritePool<int>(scope,name)[element] = qValue.toInt();
	}
	table->adjustColumn(0);
	table->adjustColumn(1);
}

void Annotator::descriptorsBPFChanged(int pointIndex,float newValue)
{
	/*TODO: right now, no matter how many points have been edited all descriptors are updated. This
	  is not too smart/efficient but doing it otherwise would mean having a dynamic list of slots 
	  in the class.*/
	mLLDChanged = true;
} 

void Annotator::segmentationMarksChanged(int, unsigned)
{
	std::vector<unsigned int> marks;
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	CLAM::IndexArray & descriptorMarks = 
		mpDescriptorPool->GetWritePool<CLAM::IndexArray>("Song",currentSegmentation)[0];
	marks = mpAudioPlot->GetMarks();
	int nMarks = marks.size();
	descriptorMarks.Resize(nMarks);
	descriptorMarks.SetSize(nMarks);
	for (int i=0; i<nMarks; i++)
	{
		descriptorMarks[i] = marks[i];
	} 
	mSegmentsChanged = true;
	auralizeMarks();
}

void Annotator::updateSongListWidget()
{
	mProjectOverview->clear();
	std::vector< CLAM_Annotator::Song> songs = mProject.GetSongs();
	unsigned i = 0;
	for ( std::vector<CLAM_Annotator::Song>::const_iterator it = songs.begin() ; it != songs.end() ; it++, i++)
	{
		ListViewItem * item = new ListViewItem(
			i, mProjectOverview, 
			QString( it->GetSoundFile().c_str() ),
			tr("Yes"), tr("No") );
	}
}

void Annotator::closeEvent ( QCloseEvent * e ) 
{
	std::vector<CLAM::VM::BPFEditor*>::iterator it = mBPFEditors.begin();
	for(; it != mBPFEditors.end(); it++)
	{
		(*it)->stopPendingTasks();
	}

	if(mLLDChanged||mHLDChanged||mSegmentsChanged)
	{
		if(QMessageBox::question(this, "Descriptors Changed", 
			"Do you want to save the changes to current descriptors?", 
			QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
		{
			if (mLLDChanged) updateEnvelopesData();
			saveDescriptors();
		}
	}
	if ( mGlobalChanges )
	{
		if(QMessageBox::question(this, "Close project", 
			"Do you want to save changes to the project?", 
			QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
		{
			saveDescriptors();
		}

	}
	e->accept();
}

void Annotator::markAllSongsUnchanged()
{
	QListViewItemIterator it (mProjectOverview);
	for ( ; it.current() ; ++it )
	{
		it.current()->setText(2, "No");
	}
}

void Annotator::deleteSongsFromProject()
{
	std::vector< QListViewItem * > toBeDeleted;
	
	for ( QListViewItemIterator it(mProjectOverview);
			it.current(); it++ )
		if ( it.current()->isSelected() )
			toBeDeleted.push_back(*it);

	
	for ( std::vector< QListViewItem* >::iterator it = toBeDeleted.begin();
			it!= toBeDeleted.end(); it++ )
		delete *it;
}

void Annotator::addSongsToProject()
{
	QStringList files = QFileDialog::getOpenFileNames(
		"Songs (*.wav *.mp3 *.ogg)",
		"/home",
		this,
		"open files dialog",
		"Select one or more files to open" );
	QStringList list = files;
	QStringList::Iterator it = list.begin();
	for (; it != list.end(); it++ )
	{
		mProject.AppendSong((*it).ascii());
	}
	updateSongListWidget();
	markProjectChanged(true);
}

void Annotator::fileOpen()
{
	QString qFileName = QFileDialog::getOpenFileName(QString::null,"*.pro");
	if(qFileName == QString::null) return;

	mProjectFileName = std::string(qFileName.ascii());
	try
	{
		CLAM::XMLStorage::Restore(mProject,mProjectFileName);
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,"Error Loading Project File", 
			constructFileError(mProjectFileName,e));
		return;
	}
	initInterface();
	initProject();
}

void Annotator::fileNew()
{
	mProjectFileName = "";
	mProject = CLAM_Annotator::Project();
	initInterface();
	initProject();
	markProjectChanged(true);
}

void Annotator::fileSaveAs()
{
	QString qFileName = QFileDialog::getSaveFileName(QString::null,"*.pro");
	if(qFileName == QString::null) return;

	mProjectFileName = std::string(qFileName.ascii());
	fileSave();
}

void Annotator::fileSave()
{
	if(mProjectFileName=="")
	{
		fileSaveAs();
		return;
	}
	CLAM::XMLStorage::Dump(mProject,"Project",mProjectFileName);
	markAllSongsUnchanged();
	markProjectChanged(false);
}

void  Annotator::loadSchema()
{
	QString qFileName = QFileDialog::getOpenFileName(QString::null,"*.sc");
	if(qFileName == QString::null) return;

	try
	{
		mProject.LoadScheme(qFileName.ascii());
	}
	catch (CLAM::XmlStorageErr & e)
	{
		QMessageBox::warning(this,"Error Loading Schema File",
			constructFileError(qFileName.ascii(),e));
		return;
	}

	initInterface();
	initProject();
}

void  Annotator::saveDescriptors()
{
	if (QMessageBox::question(this,QString("Save Descriptors"),
		QString("Do you want to save current song's descriptors?"),
		QString("OK"),QString("Cancel")) != 0) return;

	QString qFileName;
	qFileName = QFileDialog::getSaveFileName(QString(mCurrentDescriptorsPoolFileName.c_str()),
			"*.pool");
	if (qFileName == QString::null) return;

	mCurrentDescriptorsPoolFileName = (std::string(qFileName.ascii()));
	CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",mCurrentDescriptorsPoolFileName);
	mLLDChanged = false;
	mHLDChanged = false;
	mSegmentsChanged = false;
}


void Annotator::songsClicked( QListViewItem * item)
{
	/* before doing anything with the new selected file we must update information for previously
	selected song */
	if(mLLDChanged||mHLDChanged||mSegmentsChanged) 
	{
		std::cout << "Saving Previous Song Descriptors..." << std::endl;
		if(mLLDChanged) 
			updateEnvelopesData();
		saveDescriptors();
	}

	if (item == 0) return;

	const char * filename = item->text(0).ascii();
	mCurrentIndex = getIndexFromFileName(filename);
	if (mCurrentIndex <0) return;
	CLAM_Annotator::Song & currentSong = mProject.GetSongs()[mCurrentIndex];
	mCurrentSoundFileName = currentSong.GetSoundFile();
	if (currentSong.HasPoolFile())
		mCurrentDescriptorsPoolFileName = currentSong.GetPoolFile();
	else 
		mCurrentDescriptorsPoolFileName = mCurrentSoundFileName + ".pool";
	loadDescriptorPool();
	std::cout << "Filling Global Descriptors..." << std::endl;
	refreshGlobalDescriptorsTable();
	std::cout << "Drawing Audio..." << std::endl;
	mAudioRefreshTimer->stop();
	drawAudio(filename);
	std::cout << "Drawing LLD..." << std::endl;
	drawLLDescriptors(mCurrentIndex);
	std::cout << "Done" << std::endl;
	loaderLaunch();
	mAudioRefreshTimer->start(4000, true);
}

void Annotator::drawLLDescriptors(int index)
{
	if (!mpDescriptorPool) return;
	std::cout << "Loading LLD Data..." << std::endl;
	refreshEnvelopes();
	std::cout << "Adjusting BPF's..." << std::endl;

	std::vector<CLAM::VM::BPFEditor*>::iterator editors_it = mBPFEditors.begin();
	for(int i=0; editors_it != mBPFEditors.end(); editors_it++, i++)
	{
		CLAM::EScale scale;
		std::pair<TData, TData> minmaxy = GetMinMaxY((*editors_it)->GetData());
		TData min_y = minmaxy.first;
		TData max_y = minmaxy.second;
		bool scale_log = (fabs(min_y) > 9999.99 || fabs(max_y) > 9999.99 || max_y-min_y < TData(5E-2));
		scale = (scale_log) ? CLAM::EScale::eLog : CLAM::EScale::eLinear;
		(*editors_it)->SetXRange(0.0,double(mCurrentAudio.GetDuration())/1000.0);
		(*editors_it)->SetYRange(min_y,max_y,scale);
		(*editors_it)->Geometry(0,0,tabWidget2->page(i)->width(),tabWidget2->page(i)->height());
		(*editors_it)->Show();
	}
}

void Annotator::refreshAudioData()
{
	std::cout << "Refresing audio..." << std::endl;
	bool finished= loaderFinished();
	if (finished)
	{
		std::cout << "Last refresh, updating marks..." << std::endl;
		mAudioRefreshTimer->stop();
		auralizeMarks();
	}
	mpAudioPlot->UpdateData(mCurrentAudio);

	if (!finished)
		mAudioRefreshTimer->start(2000, true);
}

void Annotator::drawAudio(const char * filename)
{
	mpAudioPlot->Hide();
	hideBPFEditors();
	setMenuAudioItemsEnabled(false);
	loaderCreate(mCurrentAudio, filename);
	setMenuAudioItemsEnabled(true);

	refreshMarksView();
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->Show();
}

void Annotator::refreshMarksView()
{
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	if (!mpDescriptorPool) return;
	const CLAM::IndexArray & descriptorsMarks = 
		mpDescriptorPool->GetReadPool<CLAM::IndexArray>("Song",currentSegmentation)[0];
	int nMarks = descriptorsMarks.Size();
	std::vector<unsigned> marks(nMarks);
	for(int i=0;i<nMarks;i++)
	{
		marks[i] = (unsigned)descriptorsMarks[i];
	}
	mpAudioPlot->SetMarks(marks);
	auralizeMarks();
}

void Annotator::refreshEnvelopes()
{
	std::vector<CLAM::VM::BPFEditor*>::iterator ed_it = mBPFEditors.begin();
	std::list<std::string>::const_iterator it;
	const std::list<std::string>& descriptorsNames = mProject.GetFrameScopeAttributeNames();

	for(it = descriptorsNames.begin();it != descriptorsNames.end(); ed_it++, it++)
	{
		CLAM::BPF transcribed;
		refreshEnvelope(transcribed, *it);
		(*ed_it)->SetData( transcribed );
		(*ed_it)->SetAudioPtr(&mCurrentAudio);
	}

}
  
void Annotator::refreshEnvelope(CLAM::BPF & bpf, const std::string& descriptorName)
{
	const CLAM::TData* values = mpDescriptorPool->GetReadPool<CLAM::TData>("Frame",descriptorName);

	int audioSize=mCurrentAudio.GetSize();
	TData sr = mCurrentAudio.GetSampleRate();

	int nFrames = mpDescriptorPool->GetNumberOfContexts("Frame");
	int frameSize = audioSize/nFrames;

	bpf.Resize(nFrames);
	bpf.SetSize(nFrames);
	for(int i=0, x=0; i<nFrames ; x+=frameSize, i++)
	{
		bpf.SetXValue(i,TData(x)/sr);
		bpf.SetValue(i,TData(values[i]));
	}
}

void Annotator::updateEnvelopesData()
{
	mLLDChanged = false;
	unsigned i=0, editors_size = mBPFEditors.size();
	std::list<std::string>::const_iterator it;
	const std::list<std::string>& descriptorsNames = mProject.GetFrameScopeAttributeNames();

	for(it = descriptorsNames.begin() ;i < editors_size; i++, it++)
	{
		updateEnvelopeData(i, mpDescriptorPool->GetWritePool<float>("Frame",*it));
	}
}

void Annotator::updateEnvelopeData(int bpfIndex, float* descriptor)
{
	int nPoints = mBPFEditors[bpfIndex]->GetData().Size();
	for (int i=0; i<nPoints; i++)
	{
		descriptor[i] = mBPFEditors[bpfIndex]->GetData().GetValueFromIndex(i);
	}
}


void Annotator::loadDescriptorPool()
{
	mLLDChanged = false;
	mHLDChanged = false;
	mSegmentsChanged = false;

	CLAM::DescriptionDataPool * tempPool = new CLAM::DescriptionDataPool(mProject.GetDescriptionScheme());

	//Load Descriptors Pool
	CLAM_ASSERT(mCurrentDescriptorsPoolFileName!="", "Empty file name");
	try
	{
		CLAM::XMLStorage::Restore(*tempPool,mCurrentDescriptorsPoolFileName);
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,"Error Loading Descriptors Pool File", 
			constructFileError(mCurrentDescriptorsPoolFileName,e));
		delete tempPool;
		return;
	}

	//Create Descriptors Pool
	if (mpDescriptorPool) delete mpDescriptorPool;
	mpDescriptorPool = tempPool;

}

bool Annotator::event(QEvent* e)
{
	if(!mpAudioPlot) return QWidget::event(e);

	QKeyEvent* keyEvent=(QKeyEvent*)(e);
	if(!keyEvent) return QWidget::event(e);
	if(mpAudioPlot->hasFocus()) return QWidget::event(e);

	switch(keyEvent->key())
	{
		case Qt::Key_Shift:
		case Qt::Key_Insert:
		case Qt::Key_Delete:
			mpAudioPlot->setFocus();
			break;
		default:
			break;
	}
	return QWidget::event(e);
}


void Annotator::drawHLD(QTable* table, int row, const CLAM::Text & value)
{
	QString qvalue = QString(value.c_str());
	table->setItem(row,1,
		new TableItem(table,TableItem::WhenCurrent,qvalue));
}

void Annotator::drawHLD(QTable* table, int row, 
			const CLAM_Annotator::RestrictedString& value, 
			const std::list<std::string> & options)
{
	QString qvalue = value.GetString().c_str();
	QStringList qrestrictionStrings;
	std::list<std::string>::const_iterator it;
	for(it = options.begin();it != options.end(); it++)
	{
		qrestrictionStrings << QString(it->c_str());
	}

	std::vector<QStringList> qrestrictionStringslist;
	qrestrictionStringslist.push_back( qrestrictionStrings );
	ComboTableItem * item = new ComboTableItem(table,qrestrictionStringslist,false);
	item->setCurrentItem(qvalue);
	table->setItem(row,1,item);
}

void Annotator::drawHLD(QTable* table, int row, float value, Range<float> range)
{
	std::ostringstream s;
	s<<value;
	QString qvalue = QString(s.str().c_str());
	table->setItem(row,1,
		new RangeSelectionTableItem(table,
			TableItem::WhenCurrent,qvalue,range));
}

void Annotator::drawHLD(QTable* table, int row, int value, Range<int> range)
{
	std::ostringstream s;
	s<<value;
	QString qvalue = QString(s.str().c_str());
	table->setItem(row,1,
		new RangeSelectionTableItem(table,
			TableItem::WhenCurrent,qvalue,range));

}

void Annotator::refreshDescriptorsTable(QTable * table, const std::string & scope, unsigned element)
{
	CLAM_Annotator::Project::SongScopeSchema hlds = mProject.GetSongScopeSchema();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
	for(it = hlds.begin() ; it != hlds.end(); it++)
	{
		const std::string & type = it->GetType();
		const std::string & name = it->GetName();
		unsigned row = findHLDescriptorIndex(name);
		if (type == "String")
		{
			drawHLD(table, row,
				mpDescriptorPool->GetReadPool<CLAM::Text>(scope,name)[element]
				);
		}
		if (type == "RestrictedString")
		{
			const std::list<std::string> & options = it->GetRestrictionValues();
			drawHLD(table, row,
				mpDescriptorPool->GetReadPool<CLAM_Annotator::RestrictedString>(scope,name)[element],
				options);
		}
		if (type == "Float")
		{
			drawHLD(table, row,
				mpDescriptorPool->GetReadPool<float>(scope,name)[element],
				it->GetfRange());
		}
		if (type == "Int")
		{
			drawHLD(table, row,
				mpDescriptorPool->GetReadPool<int>(scope,name)[element],
				it->GetiRange());
		}
	}
	table->adjustColumn(0);
	table->adjustColumn(1);
}

void Annotator::refreshGlobalDescriptorsTable()
{
	if (!mpDescriptorPool) return;
	refreshDescriptorsTable(mDescriptorsTable, "Song", 0);
	mDescriptorsTable->show();
}

int Annotator::findHLDescriptorIndex(const std::string& name)
{
	//TODO: should find a more efficient search algorithm

	CLAM_Annotator::Project::SongScopeSchema hlds = mProject.GetSongScopeSchema();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it = hlds.begin();
	for(int i = 0 ; it != hlds.end(); it++, i++)
	{
		if (it->GetName() == name) return i;
	}
	return -1;
}

std::string Annotator::getHLDescriptorNameFromIndex(int index)
{
	CLAM_Annotator::HLDSchemaElement hldSchemaElement;
	getHLDSchemaElementFromIndex(index, hldSchemaElement);
	return hldSchemaElement.GetName();
}

void Annotator::getHLDSchemaElementFromIndex(int index, CLAM_Annotator::HLDSchemaElement& element)
{
	/* TODO: This is not very efficient, wouldn't it be better to have HLDs as a vector instead of
	a list? */
	CLAM_Annotator::Project::SongScopeSchema hlds = mProject.GetSongScopeSchema();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it = hlds.begin();
	for(int i = 0 ; it != hlds.end(); it++, i++)
	{
		if (i==index) {
			element = (*it);
			return;
		}
	}
	CLAM_ASSERT(false, "Index does not exist for HLDSchema");;

}

std::pair<double,double> Annotator::GetMinMaxY(const CLAM::BPF& bpf)
{
	double min_value=1E9;
	double max_value=-1E9;
	for(TIndex i=0; i < bpf.Size(); i++)
	{
		double current = double(bpf.GetValueFromIndex(i));
		if(current > max_value)
		{
			max_value = current;
		}
		else if(current < min_value)
		{
			min_value = current;
		}
	}
	double span = max_value-min_value;
	min_value -= span*0.1;
	max_value += span*0.1;
	return std::make_pair(min_value, max_value);
}


int Annotator::getIndexFromFileName(const std::string& fileName)
{
	//TODO: have to optimize these tasks maybe by using a map or at least std::find
	std::vector<CLAM_Annotator::Song> fileNames = mProject.GetSongs();
	std::vector<CLAM_Annotator::Song>::iterator it = fileNames.begin();
	for (int i=0 ; it != fileNames.end(); it++, i++)
	{
		if (it->GetSoundFile() == fileName) return i;
	}
	return -1;
}

void Annotator::auralizeMarks()
{
	if(mClick.size()==0)
	{
		CLAM::AudioFile file;
		file.OpenExisting("click.mp3");
		int nChannels = file.GetHeader().GetChannels();
		mClick.resize(nChannels);
		for (int i=0; i<nChannels; i++)
			mClick[i].SetSize(5000);

		CLAM::MultiChannelAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MultiChannelAudioFileReader reader(cfg);
		reader.Start();
		int beginSample=0;
		reader.Do(mClick);
		reader.Stop();
	}
	const std::vector<unsigned int> & marks = mpAudioPlot->GetMarks();
	int nMarks = marks.size();
	mCurrentMarkedAudio = mCurrentAudio;
	int size = mCurrentMarkedAudio.GetSize();
	for (int i=0; i<nMarks; i++)
	{
		if(marks[i]<size)
			mCurrentMarkedAudio.SetAudioChunk((int)marks[i],mClick[0]);
	} 

}

void Annotator::playMarks(bool playThem)
{
	CLAM::Audio * audioToPlay = playThem ? &mCurrentMarkedAudio : &mCurrentAudio;
	mpAudioPlot->SetData(*audioToPlay,false);
	for(unsigned i=0; i < mBPFEditors.size(); i++)
	{
		mBPFEditors[i]->SetAudioPtr(audioToPlay);
	}
}

QString Annotator::constructFileError(const std::string& fileName,const CLAM::XmlStorageErr& e)
{
	std::string errorMessage = "<p><b>XML Error: ";
	errorMessage += e.what();
	errorMessage += "</b></p>";
	errorMessage += "Check that your file '<tt>";
	errorMessage += mProjectFileName;
	errorMessage += "</tt>'\n";
	errorMessage += "is well formed and folllows the specifications";
	return QString(errorMessage.c_str());
}

void Annotator::playOriginalAudioAndLLD(bool both)
{
	for(unsigned i=0; i < mBPFEditors.size(); i++)
	{
		mBPFEditors[i]->playSimultaneously(both);
	}
}

void Annotator::setMenuAudioItemsEnabled(bool enabled)
{
	audioAuralize_Segmentation_MarksAction->setOn(false); 
	audioOriginal_Audio__LLDAction->setOn(false);
	audioAuralize_Segmentation_MarksAction->setEnabled(enabled);
	audioOriginal_Audio__LLDAction->setEnabled(enabled);
}

void Annotator::hideBPFEditors()
{
	for(unsigned i=0; i < mBPFEditors.size(); i++)
		mBPFEditors[i]->Hide();
}


