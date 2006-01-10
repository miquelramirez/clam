#include "Annotator.hxx"
#include "Project.hxx"

#include <qaction.h>
#include <qthread.h>
#include <qprocess.h>
#include <qmessagebox.h>
#include <qtabwidget.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <qsplitter.h>
#include <qtabbar.h>

#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>

//xamat
#include <time.h>

#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>

#include <CLAM/AudioFile.hxx>
#include <CLAM/IndexArray.hxx>
#include <CLAM/Text.hxx>
#include <CLAM/XMLStorage.hxx>

#include <CLAM/BPFEditor.hxx>
#include <CLAM/QtAudioPlot.hxx>

#include "AudioLoadThread.hxx"

#include <CLAM/ContiguousSegmentation.hxx>
#include <CLAM/DiscontinuousSegmentation.hxx>
#include "SchemaBrowser.hxx"
#include <CLAM/QtSingleBPFPlayerExt.hxx>
#include <qstatusbar.h>

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

void Annotator::computeSongDescriptors()
{
	if (!mProjectOverview->selectedItem()) return;
	QString filename = mProjectOverview->selectedItem()->text(0);
	filename  = projectToAbsolutePath(filename.ascii()).c_str();
	if (!std::ifstream(filename.utf8()))
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Unable to open selected file '%1'</b></p>.").arg(filename));
		return;
	}
	if (!mProject.HasExtractor() || mProject.GetExtractor()=="")
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: No extractor defined for the project.</b></p>"
					"<p>Unable to extract descriptors from song. "
					"Define the extractor first, please.</p>"));
		return;
	}
	mStatusBar << "Launching Extractor..." << mStatusBar;
	QProcess extractor(this);
	QDir projectPath(mProjectFileName.c_str());
	projectPath.cdUp();
	extractor.setWorkingDirectory(projectPath);
	extractor.addArgument(mProject.GetExtractor().c_str());
	extractor.addArgument(filename);
	if (!extractor.start())
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: Unable to launch the extractor.</b></p>"
					"<p>Check that the extractor is well configured and you have permissions to run it.</p>"
					"<p>The configured command was:</p><tt>%1</tt>")
				.arg(mProject.GetExtractor().c_str())
				);
		return;
	}
	while (extractor.isRunning())
	{
		while (extractor.canReadLineStdout())
			mStatusBar << extractor.readLineStdout() << mStatusBar;
		while (extractor.canReadLineStderr())
			mStatusBar << extractor.readLineStderr() << mStatusBar;
	}
	while (extractor.canReadLineStdout())
		mStatusBar << extractor.readLineStdout() << mStatusBar;
	while (extractor.canReadLineStderr())
		mStatusBar << extractor.readLineStderr() << mStatusBar;
	if (!extractor.normalExit())
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: The extractor was terminated with an error.</b></p>"));
		return;
	}
	loadDescriptorPool();
}

Annotator::Annotator(const std::string & nameProject = "")
	: AnnotatorBase( 0, "annotator", WDestructiveClose)
	, mCurrentIndex(0)
	, mpDescriptorPool(0)
	, mFrameDescriptorsNeedUpdate(false)
	, mDescriptorsNeedSave(false)
	, mMustUpdateMarkedAudio(false)
	, mpAudioPlot(0)
	, mAudioRefreshTimer(new QTimer(this))
	, mAudioLoaderThread(0)
	, mGlobalDescriptors(mDescriptorsTable, mProject)
	, mSegmentDescriptors(mSegmentDescriptorsTable, mProject)
	, mBPFEditor(0)
	, mCurrentBPFIndex(-1)
	, mSegmentation(0)
	, mPlayer(0)
	, mStatusBar(statusBar())
{
	loadSettings();
	initInterface();
	setMenuAudioItemsEnabled(false);
	connect (mAudioRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshAudioData()) );
	if (nameProject!="") mProjectFileName = nameProject;
	if (mProjectFileName=="") return;
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

void Annotator::loadSettings()
{
	QSettings settings;
	settings.setPath( "clam.iua.upf.es", "MusicAnnotator", QSettings::User);
	int posX = settings.readNumEntry("MusicAnnotator/LastSession/PosX", pos().x());
	int posY = settings.readNumEntry("MusicAnnotator/LastSession/PosY", pos().y());
	int sizeX = settings.readNumEntry("MusicAnnotator/LastSession/SizeX", size().width());
	int sizeY = settings.readNumEntry("MusicAnnotator/LastSession/SizeY", size().height());
	resize(QSize(sizeX,sizeY));
	move(QPoint(posX,posY));
	mProjectFileName = settings.readEntry( "MusicAnnotator/LastSession/ProjectFile", "").ascii();

	mRecentOpenedProjects.clear();
	QStringList recents;
	recents = settings.readListEntry("MusicAnnotator/LastSession/RecentOpenedProjects");
	for(QStringList::Iterator it = recents.begin(); it!=recents.end(); it++ )
		appendRecentOpenedProject((*it).ascii());
}

void Annotator::saveSettings()
{
	QSettings settings;
	settings.setPath("clam.iua.upf.es", "MusicAnnotator", QSettings::User);
	settings.writeEntry("MusicAnnotator/LastSession/ProjectFile", mProjectFileName.c_str()  );
	settings.writeEntry("MusicAnnotator/LastSession/PositionX", pos().x());
	settings.writeEntry("MusicAnnotator/LastSession/PositionY", pos().y());
	settings.writeEntry("MusicAnnotator/LastSession/SizeX", size().width());
	settings.writeEntry("MusicAnnotator/LastSession/SizeY", size().height());

	QStringList recents;
	for (unsigned i=0; i<mRecentOpenedProjects.size(); i++)
		recents <<  mRecentOpenedProjects[i].c_str();
	settings.writeEntry("MusicAnnotator/LastSession/RecentOpenedProjects", recents);
}

void Annotator::appendRecentOpenedProject(const std::string & projectFilename)
{
	std::deque<std::string>::iterator found = 
		std::find(mRecentOpenedProjects.begin(), mRecentOpenedProjects.end(), projectFilename);
	if (found != mRecentOpenedProjects.end()) mRecentOpenedProjects.erase(found);
	mRecentOpenedProjects.push_front(projectFilename);
	while (mRecentOpenedProjects.size()>4)
		mRecentOpenedProjects.pop_back();
}

Annotator::~Annotator()
{
	saveSettings();
	abortLoader();
	if (mSegmentation) delete mSegmentation;
}

void Annotator::initInterface()
{
	mProjectOverview->setSorting(-1); // Unordered

	QVBoxLayout * frameLevelContainerLayout = new QVBoxLayout(mFrameLevelContainer);
	mFrameLevelTabBar = new QTabBar(mFrameLevelContainer);
	frameLevelContainerLayout->add(mFrameLevelTabBar);

	mBPFEditor = new CLAM::VM::BPFEditor(CLAM::VM::AllowVerticalEdition|CLAM::VM::HasVerticalScroll,
			mFrameLevelContainer);
	frameLevelContainerLayout->add(mBPFEditor);
	mBPFEditor->UseFocusColors();
//	mBPFEditor->Hide();

	mpAudioPlot = new QtAudioPlot(mAudioPlotContainer,0,0,false);
	QVBoxLayout * audioPlotContainerLayout = new QVBoxLayout(mAudioPlotContainer);
	audioPlotContainerLayout->addWidget(mpAudioPlot);
	mpAudioPlot->Label("Audio");
	mCurrentAudio.SetSize(20000);
//	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->SetEditTagDialogEnabled(false);
//	mpAudioPlot->setFocus();
	mpAudioPlot->UseFocusColors();
//	mpAudioPlot->Hide();


	mSchemaBrowser = new SchemaBrowser;
	mMainTabWidget->addTab(mSchemaBrowser, "Description Schema");

	mPlayer = new CLAM::VM::QtSingleBPFPlayerExt(this);
	mPlayer->hide();

	resetTabOrder();
	makeConnections();
}

void Annotator::resetTabOrder()
{
	setTabOrder(mFrameLevelTabBar, mBPFEditor);
	setTabOrder(mBPFEditor,mpAudioPlot);
	setTabOrder(mpAudioPlot,mSegmentationSelection);
	setTabOrder(mSegmentationSelection,mSegmentDescriptorsTable);

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
		mProject.LoadScheme(projectToAbsolutePath(mProject.GetSchema()));
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
	appendRecentOpenedProject(mProjectFileName);
}

void Annotator::adaptInterfaceToCurrentSchema()
{
	mStatusBar << "Adapting Interface to Song level descriptors..." << mStatusBar;
	mGlobalDescriptors.refreshSchema("Song");
	mStatusBar << "Adapting Interface to Frame level descriptors..." << mStatusBar;
	adaptEnvelopesToCurrentSchema();
	mStatusBar << "Adapting Interface to Segmentations..." << mStatusBar;
	adaptSegmentationsToCurrentSchema();
	mStatusBar << "Updating schema browser..." << mStatusBar;
	mSchemaBrowser->setSchema(mProject.GetAnnotatorSchema());
	mStatusBar << "User interface adapted to the new schema." << mStatusBar;
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
		mStatusBar << "Adding Segmentation: " << it->c_str() << mStatusBar;
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
			theSegmentation = 
				new CLAM::DiscontinuousSegmentation(
					mCurrentAudio.GetSize(),
					&descriptorsMarks[0],
					&descriptorsMarks[0]+nMarks);
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
	mStatusBar << "Removing segment at " << index << mStatusBar;
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to shrink
	CLAM_ASSERT(index<mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid segment to be removed");
	mpDescriptorPool->Remove(childScope, index);
}

void Annotator::insertSegment(unsigned index)
{
	mStatusBar << "Inserting segment at " << index << mStatusBar;
	std::string currentSegmentation = mSegmentationSelection->currentText().ascii();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to grow up
	CLAM_ASSERT(index<mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid position to insert a segment");
	mpDescriptorPool->Insert(childScope, index);
}

void Annotator::adaptEnvelopesToCurrentSchema()
{
	while (mFrameLevelTabBar->count())
		mFrameLevelTabBar->removeTab(mFrameLevelTabBar->tabAt(0));

	const std::list<std::string>& names = mProject.GetNamesByScopeAndType("Frame", "Float");
	const unsigned nTabs = names.size();
	std::list<std::string>::const_iterator name = names.begin();
	for (unsigned i = 0; i<nTabs; name++, i++)
	{
		mFrameLevelTabBar->addTab(new QTab(name->c_str()));
	}
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
	connect(mFrameLevelTabBar, SIGNAL(selected(int)),
		this, SLOT(changeFrameLevelDescriptor(int)));

	connect( mBPFEditor, SIGNAL(yValueChanged(int, float)),
		 this, SLOT(frameDescriptorsChanged(int, float)));
	connect( mBPFEditor, SIGNAL(yValueChanged(int, float)),
		 mPlayer, SLOT(updateYValue(int, float)));
	connect( mBPFEditor, SIGNAL(selectedXPos(double)),
		 mpAudioPlot, SLOT(setSelectedXPos(double)));

	connect(mpAudioPlot, SIGNAL(xRulerRange(double,double)),
		mBPFEditor, SLOT(setHBounds(double,double)));
	connect(mpAudioPlot, SIGNAL(selectedXPos(double)),
		mBPFEditor, SLOT(selectPointFromXCoord(double)));
	connect(mpAudioPlot, SIGNAL(regionTime(float,float)),
		mPlayer, SLOT(setRegionTime(float,float)));
/*
	connect(mpAudioPlot, SIGNAL(switchColorsRequested()),
		mBPFEditor, SLOT(switchColors()));
*/

	connect(mPlayer, SIGNAL(playingTime(float)),
		mBPFEditor, SLOT(setCurrentPlayingTime(float)));
	connect(mPlayer, SIGNAL(stopPlaying(float)),
		mBPFEditor, SLOT(receivedStopPlaying(float)));
	connect(mPlayer, SIGNAL(playingTime(float)),
		mpAudioPlot, SLOT(setCurrentPlayingTime(float)));
	connect( mPlayer, SIGNAL(stopPlaying(float)),
		 mpAudioPlot, SLOT(receivedStopPlaying(float)));

}

void Annotator::linkCurrentSegmentToPlayback(bool enabled)
{
	if (enabled)
	{
		int answer = QMessageBox::warning(this, 
				tr("Linking Current Segment to Playback"),
				tr("<p>This feature is still experimental and it may hang the application.</p>"
					"<p>Are you sure you want to activate it?</p>"),
				QMessageBox::Yes | QMessageBox::Default,
				QMessageBox::Cancel | QMessageBox::Escape);
		if (answer==QMessageBox::Cancel)
		{
			playbackLinkCurrentSegmentToPlaybackAction->setOn(false);
			return;
		}
	}
	mpAudioPlot->ChangeSegmentOnPlay(enabled);
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
	mStatusBar << "Segment changed to " << mSegmentation->current() << mStatusBar;
	// TODO: Some widgets may have half edited information. Need update.
	// TODO: Some times is not worth to update the information (deleted segment)
	mSegmentDescriptors.refreshData(mSegmentation->current(),mpDescriptorPool);
}

void Annotator::frameDescriptorsChanged(int pointIndex,float newValue)
{
	/*TODO: right now, no matter how many points have been edited all descriptors are updated. This
	  is not too smart/efficient but doing it otherwise would mean having a dynamic list of slots 
	  in the class.*/
	int index = mFrameLevelTabBar->indexOf(mFrameLevelTabBar->currentTab());
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
	QListViewItem *lastItem = 0;
	for ( std::vector<CLAM_Annotator::Song>::const_iterator it = songs.begin() ; it != songs.end() ; it++)
	{
		QListViewItem * item = new QListViewItem(
			mProjectOverview, lastItem,
			it->GetSoundFile().c_str(),
			tr("Yes"), tr("No") );
		lastItem = item;
	}
}

void Annotator::closeEvent ( QCloseEvent * e ) 
{
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
	markProjectChanged(true);
}

void Annotator::addSongsToProject()
{
	QStringList files = QFileDialog::getOpenFileNames(
		"Songs (*.wav *.mp3 *.ogg)",
		projectToAbsolutePath(".").c_str(),
		this,
		"Add files to the project",
		"Select one or more files to add" );
	QStringList::Iterator it = files.begin();
	for (; it != files.end(); it++ )
	{
		mProject.AppendSong(absoluteToProjectPath((*it).ascii()));
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
	loadSchema();
	initProject();
	markProjectChanged(true);
}

void Annotator::fileSaveAs()
{
	QString qFileName = QFileDialog::getSaveFileName(QString::null,"*.pro");
	if(qFileName == QString::null) return;

	mProjectFileName = qFileName.ascii();
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
	appendRecentOpenedProject(mProjectFileName);
}

void  Annotator::loadSchema()
{
	QString qFileName = QFileDialog::getOpenFileName(
			projectToAbsolutePath(".").c_str(),
			"Description Schemes (*.sc)",
			this,
			"SchemaChooser",
			"Choose an Schema");
	if(qFileName == QString::null) return;

	std::string schemaFile = absoluteToProjectPath(qFileName.ascii());
	mProject.SetSchema(schemaFile);
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

	CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",projectToAbsolutePath(mCurrentDescriptorsPoolFileName));

	mDescriptorsNeedSave = false;
}

std::string Annotator::projectToAbsolutePath(const std::string & file)
{
	QString projectPath = QDir::cleanDirPath((mProjectFileName+"/../").c_str());
	mProject.SetBasePath(projectPath.ascii());
	QDir qdir = QString(file.c_str());
	if (qdir.isRelative())
		return QDir::cleanDirPath( QDir(projectPath).filePath(file.c_str()) ).ascii();
	return file;
}

std::string Annotator::absoluteToProjectPath(const std::string & file)
{
	QDir qdir = QString(file.c_str());
	if (qdir.isRelative()) return file;
	const std::string & basePath = mProject.GetBasePath();
	if (file.substr(0,basePath.length()+1)==(basePath+"/"))
		return file.substr(mProject.GetBasePath().length()+1);
	return file;
}

void Annotator::currentSongChanged()
{
	stopPlaying();
	mStatusBar << "Saving Previous Song Descriptors..." << mStatusBar;
	saveDescriptors();
	QListViewItem * item = mProjectOverview->currentItem();

	if (item == 0) return;

	setCursor(Qt::waitCursor);

	const char * filename = item->text(0).ascii();
	mCurrentIndex = songIndexInTable(filename);
	if (mCurrentIndex <0) return;
	CLAM_Annotator::Song & currentSong = mProject.GetSongs()[mCurrentIndex];
	mCurrentSoundFileName = currentSong.GetSoundFile();
	if (currentSong.HasPoolFile())
		mCurrentDescriptorsPoolFileName = currentSong.GetPoolFile();
	else 
		mCurrentDescriptorsPoolFileName = mCurrentSoundFileName + ".pool";
	mStatusBar << "Loading descriptors..." << mStatusBar;
	loadDescriptorPool();
	mStatusBar << "Filling Global Descriptors..." << mStatusBar;
	refreshGlobalDescriptorsTable();
	mStatusBar << "Drawing Audio..." << mStatusBar;
	mAudioRefreshTimer->stop();
	drawAudio(projectToAbsolutePath(filename).c_str());
	mStatusBar << "Drawing LLD..." << mStatusBar;
	refreshEnvelopes();
	mStatusBar << "Done" << mStatusBar;
	loaderLaunch();
	setCursor(Qt::arrowCursor);
	mAudioRefreshTimer->start(4000, true);
}

void Annotator::refreshEnvelopes()
{
	if (!mpDescriptorPool) return;

	mStatusBar << "Loading LLD Data..." << mStatusBar;

	std::list<std::string>::const_iterator it;
	const std::list<std::string>& descriptorsNames = mProject.GetNamesByScopeAndType("Frame", "Float");

	mBPFs.clear();
	mBPFEditor->SetAudioPtr(&mCurrentAudio);
	mBPFEditor->SetXRange(0.0,double(mCurrentAudio.GetDuration())/1000.0);
	mPlayer->SetAudioPtr(&mCurrentAudio,CLAM::VM::BOTH_CHANNELS);
	mPlayer->SetDuration(double(mCurrentAudio.GetDuration())/1000.0);
	mPlayer->SetSampleRate(mCurrentAudio.GetSampleRate());
	mCurrentBPFIndex = -1;
	mFrameLevelTabBar->setCurrentTab(mFrameLevelTabBar->tabAt(0));

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
	if(mBPFs.size()) mPlayer->SetData(mBPFs[0].second);
}

void Annotator::refreshAudioData()
{
	mStatusBar << "Refresing audio..." << mStatusBar;
	bool finished= loaderFinished();
	if (finished)
	{
		mStatusBar << "Last refresh, updating segment auralization..." << mStatusBar;
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
	mBPFEditor->Hide();
	setMenuAudioItemsEnabled(false);
	loaderCreate(mCurrentAudio, filename);
	setMenuAudioItemsEnabled(true);

	refreshSegmentation();
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->Show();
	mBPFEditor->Show();
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
	std::string poolFile = projectToAbsolutePath(mCurrentDescriptorsPoolFileName);
	try
	{
		CLAM::XMLStorage::Restore(*tempPool,poolFile);
		std::ostringstream os;
		os <<"Read data did not validate with schema."<<std::endl;
		if (!mProject.ValidateDataPool(*tempPool, os))
		{
			std::cerr << os.str() << std::endl;
			QMessageBox::warning(this, tr("Error Loading Descriptors Pool File"),
					os.str().c_str());
			delete tempPool;
			return;
		}
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,tr("Error Loading Descriptors Pool File"), 
			constructFileError(poolFile,e));
		delete tempPool;
		return;
	}

	//Create Descriptors Pool
	if (mpDescriptorPool) delete mpDescriptorPool;
	mpDescriptorPool = tempPool;
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
		file.OpenExisting(RESOURCES_BASE"/sounds/click.wav");
		CLAM_ASSERT(file.IsReadable(), "The application requires the file '"
				RESOURCES_BASE"/sounds/click.wav' which couldn't be open.");
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

void Annotator::updateAuralizationOptions()
{
	bool playOnsets = playbackAuralizeSegmentOnsetsAction->isOn();
	bool playLLDs = playbackAuralizeFrameLevelDescriptorsAction->isOn();
	mPlayer->SetAudioPtr(&mCurrentAudio,CLAM::VM::BOTH_CHANNELS);
	if (playLLDs)
		mPlayer->SetAudioPtr(0,CLAM::VM::LEFT_CHANNEL);
	if (playOnsets)
		mPlayer->SetAudioPtr(&mCurrentMarkedAudio,CLAM::VM::RIGHT_CHANNEL);
}

void Annotator::setMenuAudioItemsEnabled(bool enabled)
{
	playbackAuralizeSegmentOnsetsAction->setOn(false); 
	playbackAuralizeFrameLevelDescriptorsAction->setOn(false);
	playbackAuralizeSegmentOnsetsAction->setEnabled(enabled);
	playbackAuralizeFrameLevelDescriptorsAction->setEnabled(enabled);
}

QString Annotator::constructFileError(const std::string& fileName,const CLAM::XmlStorageErr& e)
{
	return tr(
		"<p><b>XML loading Error: %1</b>/p>"
		"<p>Check that your file '<tt>%2</tt>'\n"
		"is well formed and folllows the specifications"
		"</p>"
		).arg(e.what()).arg(fileName.c_str());
}

void Annotator::onStopPlaying(float time)
{
	stopPlaying();
	if(!mMustUpdateMarkedAudio) return;
	mMustUpdateMarkedAudio = false;
	auralizeMarks();
}

bool Annotator::isPlaying()
{
	return (mPlayer->IsPlaying());
}

void Annotator::changeFrameLevelDescriptor(int current)
{
	unsigned index = mFrameLevelTabBar->indexOf(current);
	if (index >= (int)mBPFs.size()) return; // No valid descriptor
	if (index == mCurrentBPFIndex) return; // No change
	mCurrentBPFIndex = index;
	mBPFEditor->Hide();
	double min_y = mBPFs[index].first.first;
	double max_y = mBPFs[index].first.second;
	mBPFEditor->SetData(mBPFs[index].second);
	bool scale_log = (fabs(min_y) > 9999.99 || fabs(max_y) > 9999.99 || max_y-min_y < TData(5E-2));
	CLAM::EScale scale = (scale_log) ? CLAM::EScale::eLog : CLAM::EScale::eLinear;
	mBPFEditor->SetYRange(min_y,max_y,scale);
	mBPFEditor->Show();
	mPlayer->SetData(mBPFs[index].second);
}

void Annotator::startPlaying()
{
	if(!mPlayer) return;
	mPlayer->Play();
}

void Annotator::pausePlaying()
{
	if(!mPlayer) return;
	mPlayer->Pause();
}

void Annotator::stopPlaying()
{
	if(!mPlayer) return;
	mPlayer->Stop();
}


