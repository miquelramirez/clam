#include "Annotator.hxx"
#include "Project.hxx"

#include <qaction.h>
#include <qthread.h>
#include <qprocess.h>
#include <qmessagebox.h>
#include <qtabwidget.h>
#include <qfiledialog.h>

#include <algorithm>
#include <iostream>
#include <utility>

//xamat
#include <time.h>

#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"

#include "AudioFile.hxx"
#include "IndexArray.hxx"
#include "Text.hxx"
#include "XMLStorage.hxx"

#include "BPFEditor.hxx"
#include "QtAudioPlot.hxx"

#include "AudioLoadThread.hxx"

#include "ContiguousSegmentation.hxx"
#include "DiscontinuousSegmentation.hxx"


#ifndef RESOURCES_BASE
#define RESOURCES_BASE "../resources"
#endif

using CLAM::VM::QtAudioPlot;

using CLAM::TData;
using CLAM::TIndex;

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
	extractor.addArgument("./runExtractor.sh");
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

	loadDescriptorPool();
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
	, mFrameDescriptorsNeedUpdate(false)
	, mDescriptorsNeedSave(false)
	, mMustUpdateMarkedAudio(false)
	, mpTabLayout(0)
	, mpAudioPlot(0)
	, mAudioRefreshTimer(new QTimer(this))
	, mAudioLoaderThread(0)
	, mGlobalDescriptors(mDescriptorsTable, mProject)
	, mSegmentDescriptors(mSegmentDescriptorsTable, mProject)
	, mBPFEditor(0)
	, mCurrentBPFIndex(-1)
	, mSegmentation(0)
{
	initAudioWidget();
	initInterface();
	setMenuAudioItemsEnabled(false);
	connect (mAudioRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshAudioData()) );
}

Annotator::~Annotator()
{
	abortLoader();
	if (mSegmentation) delete mSegmentation;
}

void Annotator::initInterface()
{
	if (mpAudioPlot) mpAudioPlot->Hide();
	mProjectOverview->setSorting(-1); // Unordered
	makeConnections();
}

void Annotator::markProjectChanged(bool changed)
{
	mProjectNeedsSave = changed;
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
	adaptInterfaceToCurrentSchema();
	
	markProjectChanged(false);
	mDescriptorsNeedSave = false;
}

void Annotator::adaptInterfaceToCurrentSchema()
{
	std::cout << "Adapting Interface to Song level descriptors..." << std::endl;
	mGlobalDescriptors.refreshSchema("Song");
	std::cout << "Adapting Interface to Frame level descriptors..." << std::endl;
	adaptEnvelopesToCurrentSchema();
	std::cout << "Adapting Interface to Segmentations..." << std::endl;
	adaptSegmentationsToCurrentSchema();
	std::cout << "User interface adaptation ended." << std::endl;
}

void Annotator::initAudioWidget()
{
	if(mpAudioPlot) delete mpAudioPlot;
	mpAudioPlot = new QtAudioPlot(mAudioPlotContainer);
	mpTabLayout = new QVBoxLayout(mAudioPlotContainer);
	mpTabLayout->addWidget(mpAudioPlot);
	mpAudioPlot->Label("Audio");
	mCurrentAudio.SetSize(20000);
//	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->SetEditTagDialogEnabled(false);
	mpAudioPlot->SwitchDisplayColors(true);
//	mpAudioPlot->SetToggleColorOn(true);
//	mpAudioPlot->switchColors();
	mpAudioPlot->setFocus();
	mpAudioPlot->Hide();
}

void Annotator::segmentDescriptorsTableChanged(int row, int column)
{
	mSegmentDescriptors.updateData(row, mpDescriptorPool);
	markCurrentSongChanged();
}

void Annotator::globalDescriptorsTableChanged(int row, int column)
{
	mGlobalDescriptors.updateData(row, mpDescriptorPool);
	markCurrentSongChanged();
}

void Annotator::adaptSegmentationsToCurrentSchema()
{
	mSegmentationSelection->clear();
	const std::list<std::string> & segmentationNames =
		mProject.GetNamesByScopeAndType("Song", "Segmentation");
	for (std::list<std::string>::const_iterator it =  segmentationNames.begin();
		it != segmentationNames.end();
		it++)
	{
		std::cout << "Adding: " << it->c_str() << std::endl;
		mSegmentationSelection->insertItem(it->c_str());
	}
}

void Annotator::refreshSegmentation()
{
	if (!mpDescriptorPool) return;
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	const CLAM::IndexArray & descriptorsMarks = 
		mpDescriptorPool->GetReadPool<CLAM::IndexArray>("Song",currentSegmentation)[0];
	int nMarks = descriptorsMarks.Size();
	CLAM_Annotator::SegmentationPolicy policy = 
		mProject.GetAttributeScheme("Song",currentSegmentation).GetSegmentationPolicy();
	CLAM::Segmentation * theSegmentation=0;
	switch (policy)
	{
		case CLAM_Annotator::SegmentationPolicy::eUnsized:
			// Not yet implemented, using Continuous by now
		case CLAM_Annotator::SegmentationPolicy::eContinuous:
		{
			theSegmentation = 
				new CLAM::ContiguousSegmentation(
					mCurrentAudio.GetSize(),
					&descriptorsMarks[0],
					&descriptorsMarks[0]+nMarks);
		} break;
		case CLAM_Annotator::SegmentationPolicy::eOverlapping:
			// Not yet implemented, using Discontinuous by now
		case CLAM_Annotator::SegmentationPolicy::eDiscontinuous:
		{
			CLAM::DiscontinuousSegmentation * segmentation = 
				new CLAM::DiscontinuousSegmentation(mCurrentAudio.GetSize());
			for (unsigned i=0; i<nMarks; i++)
			{
				segmentation->insert(descriptorsMarks[i]);
			}
			theSegmentation = segmentation;
		} break;
	}
	if (mSegmentation) delete mSegmentation;
	mSegmentation = theSegmentation;
	mpAudioPlot->SetSegmentation(mSegmentation);
	auralizeMarks();

	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	mSegmentDescriptors.refreshSchema(childScope);
}

void Annotator::updateSegmentations()
{
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	CLAM::IndexArray & descriptorMarks = 
		mpDescriptorPool->GetWritePool<CLAM::IndexArray>("Song",currentSegmentation)[0];
	const std::vector<double> & marks = mSegmentation->onsets();
	int nMarks = marks.size();
	descriptorMarks.Resize(nMarks);
	descriptorMarks.SetSize(nMarks);
	for (int i=0; i<nMarks; i++)
	{
		descriptorMarks[i] = marks[i];
	} 
	mDescriptorsNeedSave = true;
}

void Annotator::removeSegment(unsigned index)
{
	std::cout << "Removing segment at " << index << std::endl;
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to shrink
	CLAM_ASSERT(index<mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid segment to be removed");
	mpDescriptorPool->Remove(childScope, index);
}

void Annotator::insertSegment(unsigned index)
{
	std::cout << "Inserting segment at " << index << std::endl;
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to grow up
	CLAM_ASSERT(index<mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid position to insert a segment");
	mpDescriptorPool->Insert(childScope, index);
}

void Annotator::adaptEnvelopesToCurrentSchema()
{
	if(mBPFEditor)
	{
		delete mBPFEditor;
		mBPFEditor=0;
	}
	mBPFEditor = new CLAM::VM::BPFEditor(CLAM::VM::AllowVerticalEdition|CLAM::VM::HasVerticalScroll|CLAM::VM::HasPlayer);
	mBPFEditor->switchColors();
	mBPFEditor->SetActivePlayer(false);
	mBPFEditor->Hide();
	
	tabWidget2->hide();

	tabWidget2->insertTab(new QWidget(tabWidget2, "Dummy"), tr("No Low Level Descriptors"), 0);
	while (tabWidget2->count()>1)
		delete tabWidget2->page(1);

	const std::list<std::string>& names = mProject.GetNamesByScopeAndType("Frame", "Float");
	const unsigned nTabs = names.size();
	mTabPages.resize(nTabs);
	std::list<std::string>::const_iterator name = names.begin();
	for (unsigned i = 0; i<nTabs; name++, i++)
	{
		mTabPages[i] = new QWidget( tabWidget2, "Dummy");
		tabWidget2->insertTab( mTabPages[i], name->c_str() );
		QVBoxLayout* tabLayout = new QVBoxLayout( mTabPages[i]);
	}
	if (nTabs)
		delete tabWidget2->page(0);

	connectBPFs();
	tabWidget2->show();
}

void Annotator::makeConnections()
{
	connect(helpAboutAction,SIGNAL(activated()),
		&mAbout,SLOT(show()));
	connect(mDescriptorsTable, SIGNAL(valueChanged( int, int) ) ,
		this, SLOT(globalDescriptorsTableChanged(int, int) ) );
	connect(mSegmentDescriptorsTable, SIGNAL(valueChanged( int, int) ) ,
		this, SLOT(segmentDescriptorsTableChanged(int, int) ) );
	connect(mpAudioPlot, SIGNAL(segmentOnsetChanged(unsigned,double)),
		this, SLOT(segmentationMarksChanged(unsigned, double)));
	connect(mpAudioPlot, SIGNAL(currentSegmentChanged(unsigned)),
		this, SLOT(changeCurrentSegment(unsigned)));
	connect(mpAudioPlot, SIGNAL(segmentDeleted(unsigned)),
		this, SLOT(removeSegment(unsigned)));
	connect(mpAudioPlot, SIGNAL(segmentInserted(unsigned)),
		this, SLOT(insertSegment(unsigned)));
	connect(mpAudioPlot, SIGNAL(stopPlayingTime(float)),
			this, SLOT(onStopPlaying(float)));

}

void Annotator::connectBPFs()
{
	connect( mBPFEditor, SIGNAL(yValueChanged(int, float)),
			 this, SLOT(frameDescriptorsChanged(int, float)));

	connect( mBPFEditor, SIGNAL(selectedXPos(double)),
			 mpAudioPlot, SLOT(setSelectedXPos(double)));

	connect(mpAudioPlot, SIGNAL(xRulerRange(double,double)),
			mBPFEditor, SLOT(setHBounds(double,double)));

	connect(mpAudioPlot, SIGNAL(selectedXPos(double)),
			mBPFEditor, SLOT(selectPointFromXCoord(double)));

	connect(mpAudioPlot, SIGNAL(switchColorsRequested()),
			mBPFEditor, SLOT(switchColors()));

	connect(mpAudioPlot, SIGNAL(regionTime(float,float)),
			mBPFEditor, SLOT(setRegionTime(float,float)));

	connect(mpAudioPlot, SIGNAL(currentPlayingTime(float)),
			mBPFEditor, SLOT(setCurrentPlayingTime(float)));

	connect(mpAudioPlot, SIGNAL(stopPlayingTime(float)),
			mBPFEditor, SLOT(receivedStopPlaying(float)));

	connect(mBPFEditor, SIGNAL(currentPlayingTime(float)),
			mpAudioPlot, SLOT(setCurrentPlayingTime(float)));

	connect( mBPFEditor, SIGNAL(stopPlaying(float)),
			 mpAudioPlot, SLOT(receivedStopPlaying(float)));
}

void Annotator::markCurrentSongChanged()
{
	mDescriptorsNeedSave = true;
	QListViewItemIterator it( mProjectOverview );
	for ( ; it.current() && !it.current()->isSelected() ; it++ );
	if ( it.current() )
	{
		it.current()->setText(2, "Yes");	
	}
}


void Annotator::changeCurrentSegment(unsigned current)
{
	std::cout << "Segment changed to " << mSegmentation->current() << std::endl;
	// TODO: Some widgets may have half edited information. Need update.
	// TODO: Some times is not worth to update the information (deleted segment)
	mSegmentDescriptors.refreshData(mSegmentation->current(),mpDescriptorPool);
}

void Annotator::frameDescriptorsChanged(int pointIndex,float newValue)
{
	/*TODO: right now, no matter how many points have been edited all descriptors are updated. This
	  is not too smart/efficient but doing it otherwise would mean having a dynamic list of slots 
	  in the class.*/
	int index = tabWidget2->currentPageIndex();
	mBPFs[index].second.SetValue(pointIndex,TData(newValue));
	mFrameDescriptorsNeedUpdate = true;
}

void Annotator::segmentationMarksChanged(unsigned, double)
{
	updateSegmentations();
	if(isPlaying())
		mMustUpdateMarkedAudio = true;
	else
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
	if(mBPFEditor) mBPFEditor->stopPendingTasks();

	saveDescriptors();

	if ( mProjectNeedsSave )
	{
		if(QMessageBox::question(this, "Close project", 
			"Do you want to save changes to the project?", 
			QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
		{
			fileSave();
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
	initProject();
}

void Annotator::fileNew()
{
	mProjectFileName = "";
	mProject = CLAM_Annotator::Project();
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

	initProject();
}

void  Annotator::saveDescriptors()
{
	if (mFrameDescriptorsNeedUpdate)
	{
		updateEnvelopesData();
		mFrameDescriptorsNeedUpdate=false;
		mDescriptorsNeedSave=true;
	}
	if (!mDescriptorsNeedSave) return;

	if (QMessageBox::question(this,QString("Save Descriptors"),
		QString("Do you want to save current song's descriptors?"),
		QString("Save Changes"),QString("Discard Them")) != 0) return;

	CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",mCurrentDescriptorsPoolFileName);

	mDescriptorsNeedSave = false;
}


void Annotator::songsClicked( QListViewItem * item)
{
	std::cout << "Saving Previous Song Descriptors..." << std::endl;
	saveDescriptors();

	if (item == 0) return;

	setCursor(Qt::busyCursor);

	const char * filename = item->text(0).ascii();
	mCurrentIndex = songIndexInTable(filename);
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
	refreshEnvelopes();
	std::cout << "Done" << std::endl;
	loaderLaunch();
	setCursor(Qt::arrowCursor);
	mAudioRefreshTimer->start(4000, true);
}

void Annotator::refreshEnvelopes()
{
	if (!mpDescriptorPool) return;

	std::cout << "Loading LLD Data..." << std::endl;

	std::list<std::string>::const_iterator it;
	const std::list<std::string>& descriptorsNames = mProject.GetNamesByScopeAndType("Frame", "Float");

	mBPFs.clear();
	mBPFEditor->SetAudioPtr(&mCurrentAudio);
	mBPFEditor->SetXRange(0.0,double(mCurrentAudio.GetDuration())/1000.0);
	mCurrentBPFIndex = -1;
	tabWidget2->setCurrentPage(0);

	for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++/*, i++*/)
	{
		CLAM::BPF transcribed;
		refreshEnvelope(transcribed, *it);
		std::pair<TData, TData> minmaxy = GetMinMaxY(transcribed);
		BPFInfo bpf_info;
		bpf_info.first=minmaxy;
		bpf_info.second=transcribed;
		mBPFs.push_back(bpf_info);
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

	refreshSegmentation();
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->Show();
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
	mFrameDescriptorsNeedUpdate = false;
	unsigned i=0, editors_size = mBPFs.size();
	std::list<std::string>::const_iterator it;
	const std::list<std::string>& descriptorsNames = mProject.GetNamesByScopeAndType("Frame", "Float");

	for(it = descriptorsNames.begin() ;i < editors_size; i++, it++)
	{
		updateEnvelopeData(i, mpDescriptorPool->GetWritePool<CLAM::TData>("Frame",*it));
	}
}

void Annotator::updateEnvelopeData(int bpfIndex, CLAM::TData* descriptor)
{
	int nPoints = mBPFs[bpfIndex].second.Size();
	for (int i=0; i<nPoints; i++)
	{
		descriptor[i] = mBPFs[bpfIndex].second.GetValueFromIndex(i);
	}
}


void Annotator::loadDescriptorPool()
{
	mFrameDescriptorsNeedUpdate = false;
	mDescriptorsNeedSave = false;

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

void Annotator::refreshGlobalDescriptorsTable()
{
	if (!mpDescriptorPool) return;
	mGlobalDescriptors.refreshData(0,mpDescriptorPool);
	mDescriptorsTable->show();
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


int Annotator::songIndexInTable(const std::string& fileName)
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
	if (!mSegmentation) return;
	if(mClick.size()==0)
	{
		CLAM::AudioFile file;
		file.OpenExisting(RESOURCES_BASE"/sounds/click.mp3");
		int nChannels = file.GetHeader().GetChannels();
		mClick.resize(nChannels);
		for (int i=0; i<nChannels; i++)
			mClick[i].SetSize(5000);

		CLAM::MultiChannelAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MultiChannelAudioFileReader reader(cfg);
		reader.Start();
		// Unused variable: int beginSample=0;
		reader.Do(mClick);
		reader.Stop();
	}
	const std::vector<double> & marks = mSegmentation->onsets();
	int nMarks = marks.size();
	mCurrentMarkedAudio.SetSize(0);
	mCurrentMarkedAudio.SetSize(mCurrentAudio.GetSize());
	mCurrentMarkedAudio.SetSampleRate(mCurrentAudio.GetSampleRate());
	int size = mCurrentMarkedAudio.GetSize();
	for (int i=0; i<nMarks; i++)
	{
		if(marks[i]<size)
			mCurrentMarkedAudio.SetAudioChunk((int)marks[i],mClick[0]);
	} 
}

void Annotator::playMarks(bool playThem)
{
	if(audioOriginal_Audio__LLDAction->isOn() && playThem) audioOriginal_Audio__LLDAction->setOn(false);
	CLAM::Audio* audio_to_play = (playThem) ? &mCurrentMarkedAudio : &mCurrentAudio;
	mpAudioPlot->SetData(*audio_to_play,false);
	mBPFEditor->SetAudioPtr(&mCurrentMarkedAudio);
	mBPFEditor->playSimultaneously(playThem);
}

void Annotator::playOriginalAudioAndLLD(bool both)
{
	if(audioAuralize_Segmentation_MarksAction->isOn() && both) audioAuralize_Segmentation_MarksAction->setOn(false);
	mBPFEditor->SetAudioPtr(&mCurrentAudio);
	mBPFEditor->playSimultaneously(both);
}

void Annotator::hideBPFEditors()
{
	mBPFEditor->Hide();
}

void Annotator::setMenuAudioItemsEnabled(bool enabled)
{
	audioAuralize_Segmentation_MarksAction->setOn(false); 
	audioOriginal_Audio__LLDAction->setOn(false);
	audioAuralize_Segmentation_MarksAction->setEnabled(enabled);
	audioOriginal_Audio__LLDAction->setEnabled(enabled);
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

void Annotator::onStopPlaying(float time)
{
	if(!mMustUpdateMarkedAudio) return;
	mMustUpdateMarkedAudio = false;
	auralizeMarks();
}

bool Annotator::isPlaying()
{
	return (mpAudioPlot->IsPlaying() || mBPFEditor->IsPlaying());
}

void Annotator::onSelectPageLLD(QWidget* w)
{
	int index = tabWidget2->currentPageIndex();
	if (!mBPFs.size()) return; // No Bpf to choose
	if (index > (int)mBPFs.size()-1) return; // Illegal bpf
	if (index == mCurrentBPFIndex) return; // No change
	mCurrentBPFIndex = index;
	removeFromCurrentLayout();
	double min_y = mBPFs[index].first.first;
	double max_y = mBPFs[index].first.second;
	mBPFEditor->reparent(w,QPoint(0,0));
	mBPFEditor->SetData(mBPFs[index].second);
	bool scale_log = (fabs(min_y) > 9999.99 || fabs(max_y) > 9999.99 || max_y-min_y < TData(5E-2));
	CLAM::EScale scale = (scale_log) ? CLAM::EScale::eLog : CLAM::EScale::eLinear;
	mBPFEditor->SetYRange(min_y,max_y,scale);
	w->layout()->add(mBPFEditor);
	mBPFEditor->Show();
}

void Annotator::removeFromCurrentLayout()
{
	for(unsigned i=0; i < mTabPages.size(); i++)
	{
		if(!mTabPages[i]->layout()->isEmpty()) 
		{
			mTabPages[i]->layout()->remove(mBPFEditor);
			break;
		}
	}
}


