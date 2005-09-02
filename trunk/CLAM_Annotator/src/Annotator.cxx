#include "Annotator.hxx"
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

void Annotator::LoadSchema(const std::string & filename)
{
	if (filename!="")
	{
		try
		{
			CLAM::XMLStorage::Restore(mSchema,filename);
		}
		catch (CLAM::XmlStorageErr e)
		{
			QMessageBox::warning(this,"Error Loading Schema File", 
				constructFileError(filename,e));
			return;
		}
	}
}

void Annotator::initProject()
{
	updateSongListWidget();

	LoadSchema(mProject.GetSchema());
	AdaptInterfaceToCurrentSchema();
	
	markProjectChanged(false);
	mLLDChanged = false;
	mHLDChanged = false;
	mSegmentsChanged = false;
}

void Annotator::AdaptInterfaceToCurrentSchema()
{
	AdaptDescriptorsTableToCurrentHLDSchema();
	AdaptEnvelopesToCurrentLLDSchema();
}

void Annotator::initAudioWidget()
{
	if(mpAudioPlot) delete mpAudioPlot;
	mpAudioPlot = new QtAudioPlot(frame3);
	mpTabLayout = new QVBoxLayout(frame3);
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

void Annotator::AdaptEnvelopesToCurrentLLDSchema()
{
	removeLLDTabs();

	std::list<std::string>& names = mSchema.GetLLDSchema().GetLLDNames();
	const unsigned nTabs = names.size();
	const int eFlags = CLAM::VM::AllowVerticalEdition | CLAM::VM::HasVerticalScroll | CLAM::VM::HasPlayer;
	mTabPages.resize(nTabs);
	mBPFEditors.resize(nTabs);
	std::list<std::string>::iterator name = names.begin();
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

void Annotator::AdaptDescriptorsTableToCurrentHLDSchema()
{
	mDescriptorsTable->setLeftMargin(0);
	mDescriptorsTable->setLeftMargin(0);
	mDescriptorsTable->setNumRows(0);
	mDescriptorsTable->setNumCols(2);
	mDescriptorsTable->setRowMovingEnabled(false);
	mDescriptorsTable->setColumnMovingEnabled(false);
	mDescriptorsTable->setReadOnly(false);
	mDescriptorsTable->setSelectionMode(QTable::NoSelection);
	mDescriptorsTable->horizontalHeader()->setLabel( 0, tr( "Descriptor" ) );
	mDescriptorsTable->horizontalHeader()->setLabel( 1, tr( "Value" ) );

	std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
	mDescriptorsTable->setNumRows(hlds.size());
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it = hlds.begin();
	for(int i = 0 ; it != hlds.end(); it++, i++)
	{
		mDescriptorsTable->setItem(i,0,
			new TableItem(mDescriptorsTable,
				TableItem::Never,
				QString((*it).GetName().c_str())));
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
		connect( (*it), SIGNAL(yValueChanged(int, float)), this, 
			SLOT(descriptorsBPFChanged(int, float)));
		connect((*it),SIGNAL(selectedXPos(double)),mpAudioPlot,SLOT(setSelectedXPos(double)));
		connect(mpAudioPlot,SIGNAL(xRulerRange(double,double)),(*it),
			SLOT(setHBounds(double,double)));
		connect(mpAudioPlot,SIGNAL(selectedXPos(double)),(*it),SLOT(selectPointFromXCoord(double)));
		connect(mpAudioPlot,SIGNAL(switchColorsRequested()),(*it),SLOT(switchColors()));
		connect(mpAudioPlot,SIGNAL(regionTime(float,float)),(*it),SLOT(setRegionTime(float,float)));
		connect(mpAudioPlot,SIGNAL(currentPlayingTime(float)),(*it),SLOT(setCurrentPlayingTime(float)));
		connect(mpAudioPlot,SIGNAL(stopPlayingTime(float)),(*it),SLOT(receivedStopPlaying(float)));
		connect((*it),SIGNAL(currentPlayingTime(float)),mpAudioPlot,SLOT(setCurrentPlayingTime(float)));
		connect((*it),SIGNAL(stopPlaying(float)),mpAudioPlot,SLOT(receivedStopPlaying(float)));
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
	//We first take the HLDSchema element where we will find all necessary info
	CLAM_Annotator::HLDSchemaElement hldSchemaElement;
	getHLDSchemaElementFromIndex(row, hldSchemaElement);

	std::string descriptorName = hldSchemaElement.GetName();
	std::string descriptorType = hldSchemaElement.GetType();
	std::string descriptorValue;
	descriptorValue = std::string( mDescriptorsTable->text(row, column).ascii() );
	setHLDescriptorPoolFromString(descriptorName, descriptorType, descriptorValue);
	changeCurrentFile();
	mDescriptorsTable->adjustColumn(0);
	mDescriptorsTable->adjustColumn(1);
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
	CLAM::IndexArray* descriptorMarks = 
		mpDescriptorPool->GetWritePool<CLAM::IndexArray>("Song","Segments");
	marks = mpAudioPlot->GetMarks();
	int nMarks = marks.size();
	descriptorMarks->Resize(nMarks);
	descriptorMarks->SetSize(nMarks);
	for (int i=0; i<nMarks; i++)
	{
		(*descriptorMarks)[i] = marks[i];
	} 
	mSegmentsChanged = true;
	auralizeMarks();
}

void Annotator::updateSongListWidget()
{
	mProjectOverview->clear();
	std::vector< CLAM_Annotator::Song> songs = mProject.GetSongs();
	for ( std::vector<CLAM_Annotator::Song>::const_iterator it = songs.begin() ; it != songs.end() ; it++)
	{
		ListViewItem * item = new ListViewItem(
			mProjectOverview->childCount(), mProjectOverview, 
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
			if (mLLDChanged) generateDescriptorsFromEnvelopes();
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

void Annotator::deleteAllSongsFromProject()
{
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
	mProject.SetSchema("");
	mProject.GetSongs().resize(0);
	mSchema.GetLLDSchema().GetLLDNames().resize(0);
	mSchema.GetHLDSchema().GetHLDs().resize(0);
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

	mProject.SetSchema(std::string(qFileName.ascii()));
	LoadSchema(qFileName.ascii());

	initInterface();
	initProject();
}

void  Annotator::loadDescriptors()
{
	QString qFileName = QFileDialog::getOpenFileName(QString::null,"*.pool");
	if(qFileName == QString::null) return;

	mCurrentDescriptorsPoolFileName = (std::string(qFileName.ascii()));
	try
	{
		CLAM::XMLStorage::Restore(*mpDescriptorPool,mCurrentDescriptorsPoolFileName);
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,"Error Loading Descriptors File", 
			constructFileError(mCurrentDescriptorsPoolFileName,e));
		return;
	}
	//TODO: Does loading the descriptors affect all this
	initInterface();
	initProject();
}

void  Annotator::saveDescriptors()
{
	if(QMessageBox::question(this,QString("Save Descriptors"),
		QString("Do you want to save current song's descriptors?"),
		QString("OK"),QString("Cancel")) != 0) return;

	QString qFileName;
	qFileName = QFileDialog::getSaveFileName(QString(mCurrentDescriptorsPoolFileName.c_str()),
			"*.pool");
	if(qFileName == QString::null) return;

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
			generateDescriptorsFromEnvelopes();
		saveDescriptors();
	}

	if (item == 0) return;

	std::cout << "Loading Song Descriptors..." << std::endl;
	const char * filename = item->text(0).ascii();
	mCurrentIndex = getIndexFromFileName(filename);
	if (mCurrentIndex <0) return;
	mCurrentSoundFileName = mProject.GetSongs()[mCurrentIndex].GetSoundFile();
	if (mProject.GetSongs()[mCurrentIndex].HasPoolFile())
	{
		mCurrentDescriptorsPoolFileName = 
			mProject.GetSongs()[mCurrentIndex].GetPoolFile();
	}
	else 
		mCurrentDescriptorsPoolFileName = mCurrentSoundFileName + ".pool";
	loadDescriptorPool();
	std::cout << "Filling Global Descriptors..." << std::endl;
	fillGlobalDescriptors( mCurrentIndex );
	std::cout << "Drawing Audio..." << std::endl;
	mAudioRefreshTimer->stop();
	drawAudio(filename);
	std::cout << "Drawing LLD..." << std::endl;
	drawLLDescriptors(mCurrentIndex);
	std::cout << "Done" << std::endl;
	loaderLaunch();
	mAudioRefreshTimer->start(0, false);
}

void Annotator::drawLLDescriptors(int index)
{
	std::cout << "Loading LLD Data..." << std::endl;
	generateEnvelopesFromDescriptors();
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
	if (!loaderFinished())
		mAudioRefreshTimer->start(2000, false);
	else
		auralizeMarks();
	std::cout << "Refreshing..." << std::endl;
	mpAudioPlot->SetData(mCurrentAudio);
}

void Annotator::drawAudio(const char * filename)
{
	mpAudioPlot->Hide();
	hideBPFEditors();
	setMenuAudioItemsEnabled(false);
	loaderCreate(mCurrentAudio, filename);
	setMenuAudioItemsEnabled(true);

	const CLAM::IndexArray* descriptorsMarks = 
		mpDescriptorPool->GetReadPool<CLAM::IndexArray>("Song","Segments");
	int nMarks = descriptorsMarks->Size();
	std::vector<unsigned> marks(nMarks);
	for(int i=0;i<nMarks;i++)
	{
		marks[i] = (unsigned)(*descriptorsMarks)[i];
	}
	mpAudioPlot->SetMarks(marks);
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->Show();
	auralizeMarks();
}

void Annotator::generateEnvelopesFromDescriptors()
{
	std::vector<CLAM::VM::BPFEditor*>::iterator ed_it = mBPFEditors.begin();
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();

	for(it = descriptorsNames.begin();it != descriptorsNames.end(); ed_it++, it++)
	{
		CLAM::BPF transcribed;
		fillEnvelopeWithLLDValues(transcribed, *it);
		(*ed_it)->SetData( transcribed );
		(*ed_it)->SetAudioPtr(&mCurrentAudio);
	}

}
  
void Annotator::fillEnvelopeWithLLDValues(CLAM::BPF & bpf, const std::string& descriptorName)
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

void Annotator::generateDescriptorsFromEnvelopes()
{
	mLLDChanged = false;
	unsigned i=0, editors_size = mBPFEditors.size();
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();

	for(it = descriptorsNames.begin() ;i < editors_size; i++, it++)
	{
		generateDescriptorFromEnvelope(i, mpDescriptorPool->GetWritePool<float>("Frame",(*it)));
	}
}

void Annotator::generateDescriptorFromEnvelope(int bpfIndex, float* descriptor)
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

	//Create Descriptors Pool Scheme and add attributes following loaded schema

	//First HLD's
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = mSchema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
	mDescriptionScheme = CLAM::DescriptionScheme();//we need to initialize everything
	for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
		const std::string & name = it2->GetName();
		const std::string & type = it2->GetType();
		if(type=="Float")
		{
			mDescriptionScheme.AddAttribute <float>("Song",name);
		}
		else if(type=="Int")
		{
			mDescriptionScheme.AddAttribute <int>("Song",name);
		}
		else if(type=="RestrictedString")
		{
			mDescriptionScheme.AddAttribute <CLAM_Annotator::RestrictedString>("Song",name);
		}
		else
		{
			mDescriptionScheme.AddAttribute <std::string>("Song",name);
		}
	}
	//And now LLD's  
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();
	for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
	{
		mDescriptionScheme.AddAttribute <CLAM::TData>("Frame", (*it));
	}

	//Finally segmentation marks
	mDescriptionScheme.AddAttribute<CLAM::IndexArray>("Song","Segments");

	//Create Descriptors Pool
	if(mpDescriptorPool) delete mpDescriptorPool;
	mpDescriptorPool = new CLAM::DescriptionDataPool(mDescriptionScheme);

	//Load Descriptors Pool
	if(mCurrentDescriptorsPoolFileName!="")
	{
		try
		{
			CLAM::XMLStorage::Restore((*mpDescriptorPool),mCurrentDescriptorsPoolFileName);
		}
		catch (CLAM::XmlStorageErr e)
		{
			QMessageBox::warning(this,"Error Loading Descriptors Pool File", 
				constructFileError(mCurrentDescriptorsPoolFileName,e));
			return;
		}
	}
    
  
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


void Annotator::drawHLD(int songIndex, const std::string& descriptorName, const std::string& value, 
			bool computed)
{
	QString qvalue = QString(value.c_str());
	if(!computed) qvalue = "?";
	mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
		new TableItem(mDescriptorsTable,TableItem::WhenCurrent,qvalue));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, 
			const CLAM_Annotator::RestrictedString& value, bool computed)
{
	QString qvalue = QString(value.GetString().c_str());
	if(!computed) qvalue = "?";
	QStringList qrestrictionStrings;
	std::list<std::string> restrictionStrings;
	restrictionStrings = mSchema.GetHLDSchema().FindElement(descriptorName).GetRestrictionValues();
	std::list<std::string>::iterator it;
	for(it = restrictionStrings.begin();it != restrictionStrings.end(); it++)
	{
		qrestrictionStrings << QString((*it).c_str());
	}

	std::vector<QStringList> qrestrictionStringslist;
	qrestrictionStringslist.push_back( qrestrictionStrings );
	mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
		new ComboTableItem(mDescriptorsTable,qrestrictionStringslist,false));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, float value, Range<float> range, bool computed)
{
	std::ostringstream s;
	s<<value;
	QString qvalue = QString(s.str().c_str());
	if(!computed) qvalue = "?";
	mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
		new RangeSelectionTableItem(mDescriptorsTable,
			TableItem::WhenCurrent,qvalue,range));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, int value, Range<int> range, bool computed)
{
	std::ostringstream s;
	s<<value;
	QString qvalue = QString(s.str().c_str());
	if(!computed) qvalue = "?";
	mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
		new RangeSelectionTableItem(mDescriptorsTable,
			TableItem::WhenCurrent,qvalue,range));

}

void Annotator::drawDescriptorsValue( int index, bool computed = true)
{
	std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
	for(it = hlds.begin() ; it != hlds.end(); it++)
	{
		if ((*it).GetType() == "String")
		{
			drawHLD(index,(*it).GetName(),*mpDescriptorPool->
			GetReadPool<std::string>("Song",(*it).GetName()),computed);
		}
		if ((*it).GetType() == "RestrictedString")
		{
			drawHLD(index,(*it).GetName(),*mpDescriptorPool->
			GetReadPool<CLAM_Annotator::RestrictedString>("Song",(*it).GetName()),computed);
		}
		if ((*it).GetType() == "Float")
		{
			drawHLD(index,(*it).GetName(),*mpDescriptorPool->
			GetReadPool<float>("Song",(*it).GetName()),(*it).GetfRange(),computed);
		}
		if ((*it).GetType() == "Int")
		{
			drawHLD(index,(*it).GetName(),*mpDescriptorPool->
			GetReadPool<int>("Song",(*it).GetName()),(*it).GetiRange(),computed);
		}

	}

}

void Annotator::setHLDescriptorPoolFromString(const std::string& descriptorName, 
					      const std::string& descriptorType,
					      const std::string& descriptorValue)
{
	QString qValue(descriptorValue.c_str());
	if (descriptorType == "String")
	{
		*(mpDescriptorPool->GetWritePool<std::string>("Song",descriptorName)) = descriptorValue;
	}
	if (descriptorType == "RestrictedString")
	{
		CLAM_Annotator::RestrictedString* rString = mpDescriptorPool->
		GetWritePool<CLAM_Annotator::RestrictedString>("Song",descriptorName);
		rString->SetString(descriptorValue);
	}
	if (descriptorType == "Float")
	{
		*(mpDescriptorPool->GetWritePool<float>("Song",descriptorName)) = qValue.toFloat();
	}
	if (descriptorType == "Int")
	{
		*(mpDescriptorPool->GetWritePool<int>("Song",descriptorName)) = qValue.toInt();
	}
}

void Annotator::fillGlobalDescriptors( int index)
{
	mDescriptorsTable->show();
	drawDescriptorsValue(index);
	mDescriptorsTable->adjustColumn(0);
	mDescriptorsTable->adjustColumn(1);
} 

int Annotator::findHLDescriptorIndex(const std::string& name)
{
	//TODO: should find a more efficient search algorithm

	std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it = hlds.begin();
	for(int i = 0 ; it != hlds.end(); it++, i++)
	{
		if ((*it).GetName() == name) return i;
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
	std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
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
		if((*it).GetSoundFile() == fileName) return i;
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
	std::string errorMessage = "XML Error:";
	errorMessage += e.what();
	errorMessage += "\n";
	errorMessage += "Check that your file ";
	errorMessage += mProjectFileName;
	errorMessage += "\n";
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


