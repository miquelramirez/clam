#define QTPORT

#include "Annotator.hxx"
#include "Project.hxx"
#include "FrameDivision.hxx"

#include <QtCore/QSettings>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QTabBar>
#include <QtGui/QFileDialog>
#include <QtGui/QTextBrowser>
#include <QtGui/QCloseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSplashScreen>
#include <QtGui/QWhatsThis>
#include <QtGui/QFocusFrame>

#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <QTimer>
#include "TaskRunner.hxx"

//xamat
#include <time.h>

#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>

#include <CLAM/AudioFile.hxx>
#include <CLAM/Array.hxx>
#include <CLAM/Text.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/CLAM_Math.hxx>

#include <vmBPFPlot.hxx>
#include <vmAudioPlot.hxx>

#include "AudioLoadThread.hxx"

#include "vmContiguousSegmentation.hxx"
#include "vmDiscontinuousSegmentation.hxx"
#include "vmUnsizedSegmentation.hxx"
#include "SchemaBrowser.hxx"
#include <vmBPFPlayer.hxx>
#include "ui_About.hxx"
#define VERSION "2.1"

#ifndef RESOURCES_BASE
#define RESOURCES_BASE "../resources"
#endif

using CLAM::VM::AudioPlot;
using CLAM::VM::BPFPlot;

using CLAM::TData;
using CLAM::TIndex;

void Annotator::abortLoader()
{
	if (!mAudioLoaderThread) return;
	delete mAudioLoaderThread;
	mAudioLoaderThread=0;
}
bool Annotator::loaderCreate(CLAM::Audio & audio, const std::string & filename)
{
	abortLoader();
	mAudioLoaderThread = new AudioLoadThread(audio, filename);
	return mCurrentAudio.GetSize()!=0;
}

void Annotator::loaderLaunch()
{
	CLAM_ASSERT(mAudioLoaderThread, "Launching a loader when none created");
	mAudioLoaderThread->start();
}
bool Annotator::loaderFinished()
{
	if (!mAudioLoaderThread) return true;
	if (!mAudioLoaderThread->isFinished()) return false;
	abortLoader();
	return true;
}

void Annotator::computeSongDescriptors()
{
	if (!mSongListView->currentItem()) return;
	QString filename = mSongListView->currentItem()->text(0);
	filename  = projectToAbsolutePath(filename.toStdString()).c_str();
	if (!std::ifstream(filename.toUtf8()))
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
	TaskRunner * runner = new TaskRunner();
	addDockWidget( Qt::BottomDockWidgetArea, runner);
	// Wait the window to be redrawn after the reconfiguration
	// before loading the cpu with the extractor
	qApp->processEvents();
	QDir projectPath(mProjectFileName.c_str());
	projectPath.cdUp();
	bool ok = runner->run(QString(mProject.GetExtractor().c_str()),
			QStringList() << filename,
			projectPath.absolutePath());
	if (!ok)
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: Unable to launch the extractor.</b></p>"
					"<p>Check that the extractor is well configured and you have permissions to run it.</p>"
					"<p>The configured command was:</p><tt>%1</tt>")
				.arg(mProject.GetExtractor().c_str())
				);
		delete runner;
		return;
	}
	return;
	/*
	while (extractor.isRunning())
	{
	}
	if (!extractor.normalExit())
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: The extractor was terminated with an error.</b></p>"));
		return;
	}
	*/
	loadDescriptorPool(); // TODO: This should be done thru an slot from the runner
}

Annotator::Annotator(const std::string & nameProject = "")
	: QMainWindow( 0 )
	, Ui::Annotator( )
	, mpDescriptorPool(0)
	, mFrameDescriptorsNeedUpdate(false)
	, mDescriptorsNeedSave(false)
	, mMustUpdateMarkedAudio(false)
	, mAudioRefreshTimer(new QTimer(this))
	, mAudioLoaderThread(0)
	, mGlobalDescriptors(0)
	, mSegmentDescriptors(0)
	, mSegmentation(0)
	, mPlayer(0)
	, mStatusBar(statusBar())
{

	QSplashScreen * splash = new QSplashScreen( QPixmap(":/logos/images/annotator-splash1.png") );
	splash->setCursor( QCursor(Qt::WaitCursor) );
	splash->show();
	splash->showMessage("Loading data ... ");

	setupUi(this);
	mGlobalDescriptors = new CLAM_Annotator::DescriptorTableController(mDescriptorsTable, mProject);
	mSegmentDescriptors = new CLAM_Annotator::DescriptorTableController(mSegmentDescriptorsTable, mProject);
	mAbout = new QDialog(this);
	Ui::About aboutUi;
	aboutUi.setupUi(mAbout);
	statusBar();
	initInterface();
	setMenuAudioItemsEnabled(false);
	loadSettings();
	mAudioRefreshTimer->setSingleShot(true);
	connect (mAudioRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshAudioData()) );
	if (nameProject!="") // Project on command line
		loadProject(nameProject);
	else if (mProjectFileName!="") // Last openend project on settings
		loadProject(mProjectFileName);
	updateAuralizationOptions();
	QTimer::singleShot(1000, splash, SLOT(close()));
}

void Annotator::loadSettings()
{
	QSettings settings;
	int posX = settings.value(VERSION "/LastSession/PosX", pos().x()).toInt();
	int posY = settings.value(VERSION "/LastSession/PosY", pos().y()).toInt();
	int sizeX = settings.value(VERSION "/LastSession/SizeX", size().width()).toInt();
	int sizeY = settings.value(VERSION "/LastSession/SizeY", size().height()).toInt();
	resize(QSize(sizeX,sizeY));
	move(QPoint(posX,posY));
	mProjectFileName = settings.value(VERSION "/LastSession/ProjectFile", "").toString().toStdString();

	mRecentOpenedProjects.clear();
	QStringList recents = settings.value(VERSION "/LastSession/RecentOpenedProjects").toStringList();
	for(QStringList::Iterator it = recents.begin(); it!=recents.end(); it++ )
		appendRecentOpenedProject(it->toStdString());
}

void Annotator::saveSettings()
{
	QSettings settings;
	settings.setValue(VERSION "/LastSession/ProjectFile", mProjectFileName.c_str()  );
	settings.setValue(VERSION "/LastSession/PositionX", pos().x());
	settings.setValue(VERSION "/LastSession/PositionY", pos().y());
	settings.setValue(VERSION "/LastSession/SizeX", size().width());
	settings.setValue(VERSION "/LastSession/SizeY", size().height());

	QStringList recents;
	for (unsigned i=0; i<mRecentOpenedProjects.size(); i++)
		recents <<  mRecentOpenedProjects[i].c_str();
	settings.setValue(VERSION "/LastSession/RecentOpenedProjects", recents);
}

void Annotator::appendRecentOpenedProject(const std::string & projectFilename)
{
	std::deque<std::string>::iterator found = 
		std::find(mRecentOpenedProjects.begin(), mRecentOpenedProjects.end(), projectFilename);
	if (found != mRecentOpenedProjects.end()) mRecentOpenedProjects.erase(found);
	mRecentOpenedProjects.push_front(projectFilename);

	while (mRecentOpenedProjects.size()>MaxRecentFiles)
		mRecentOpenedProjects.pop_back();
	updateRecentFilesMenu();

}

void Annotator::updateRecentFilesMenu()
{
	mRecentFilesMenuSeparator->setVisible(mRecentOpenedProjects.size() > 0);
	for (unsigned i = 0; i < mRecentOpenedProjects.size(); ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(mRecentOpenedProjects[i].c_str());
		mRecentFilesActions[i]->setText(text);
		mRecentFilesActions[i]->setData(mRecentOpenedProjects[i].c_str());
		mRecentFilesActions[i]->setVisible(true);
	}
	for (unsigned i = mRecentOpenedProjects.size(); i < MaxRecentFiles; ++i)
		mRecentFilesActions[i]->setVisible(false);
}

static CLAM::Audio dummyAudio;

Annotator::~Annotator()
{
	std::cout << "Annotator destructor" << std::endl;
	std::cout << "Saving Settings" << std::endl;
	saveSettings();
	std::cout << "Aborting Loader" << std::endl;
	abortLoader();
	std::cout << "Deleting segmentation" << std::endl;
	if (mSegmentation) delete mSegmentation;
	std::cout << "Default destructors" << std::endl;
}

void Annotator::initInterface()
{
	mSongListView->setSortingEnabled(false); // Unordered

	mProjectDocumentation = new QTextBrowser;
	mMainTabWidget->insertTab(0, mProjectDocumentation, "Project Documentation");
	mMainTabWidget->setCurrentIndex(0);

	mBPFEditor->SetFlags(CLAM::VM::eAllowVerEdition);//|CLAM::VM::eHasVerticalScroll); // QTPORT: What about this flag
	mBPFEditor->SetZoomSteps(5,5);
	mBPFEditor->SetXRange(0.0,2.0);

	mCurrentAudio.SetSize(2.0);
	mSegmentEditor->SetData(mCurrentAudio);

#if QT_VERSION >= 0x040100 // QTPORT TODO: 4.0 backport
	mBPFEditor->setAutoFillBackground(true);
	mSegmentEditor->setAutoFillBackground(true);
#endif

	mSchemaBrowser = new SchemaBrowser;
	mMainTabWidget->addTab(mSchemaBrowser, "Description Schema");

	mPlayer = new CLAM::VM::BPFPlayer(this);

	resetTabOrder();
	makeConnections();
}

void Annotator::resetTabOrder()
{
#ifndef QTPORT
	setTabOrder(mBPFEditor,mSegmentEditor);
	setTabOrder(mSegmentEditor,mSegmentationSelection);
	setTabOrder(mSegmentationSelection,mSegmentDescriptorsTable);
#endif//QTPORT
}

void Annotator::markProjectChanged(bool changed)
{
	mProjectNeedsSave = changed;
	fileSave_projectAction->setEnabled(changed);
	updateApplicationTitle();
}
void Annotator::markCurrentSongChanged(bool changed)
{
	mDescriptorsNeedSave = changed;
	updateApplicationTitle();
}

void Annotator::updateApplicationTitle()
{
	QString title(tr("Music Annotator"));
	if (mProjectNeedsSave)
		title+=tr(" [modified project]");
	if (mDescriptorsNeedSave)
		title+=tr(" [modified song]");
	setWindowTitle(title);
}

void Annotator::initProject()
{
	updateSongListWidget();

	QString projectDescription = "<h1>Project Documentation</h1>\n";
	if (mProject.HasDescription())
		projectDescription += mProject.GetDescription().c_str();
	else
		projectDescription += "<p>No project documentation available</p>";
	mProjectDocumentation->setHtml(projectDescription);

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
	
	markCurrentSongChanged(false);
	markProjectChanged(false);
	appendRecentOpenedProject(mProjectFileName);
}

void Annotator::adaptInterfaceToCurrentSchema()
{
	mStatusBar << "Adapting Interface to Song level descriptors..." << mStatusBar;
	mGlobalDescriptors->refreshSchema("Song");
	mStatusBar << "Adapting Interface to Frame level descriptors..." << mStatusBar;
	adaptEnvelopesToCurrentSchema();
	mStatusBar << "Adapting Interface to Segmentations..." << mStatusBar;
	adaptSegmentationsToCurrentSchema();
	mStatusBar << "Updating schema browser..." << mStatusBar;
	mSchemaBrowser->setSchema(mProject.GetAnnotatorSchema());
	mStatusBar << "User interface adapted to the new schema." << mStatusBar;
}

void Annotator::segmentDescriptorsTableChanged(int row)
{
	markCurrentSongChanged(true);
}

void Annotator::globalDescriptorsTableChanged(int row)
{
	markCurrentSongChanged(true);
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
		mSegmentationSelection->addItem(it->c_str());
	}
}

void Annotator::refreshSegmentation()
{
	if (!mpDescriptorPool) return;
	if (mSegmentationSelection->currentText()==QString::null) return; // No segmentation
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	const CLAM::DataArray & descriptorsMarks = 
		mpDescriptorPool->GetReadPool<CLAM::DataArray>("Song",currentSegmentation)[0];
	int nMarks = descriptorsMarks.Size();
	CLAM_Annotator::SegmentationPolicy policy = 
		mProject.GetAttributeScheme("Song",currentSegmentation).GetSegmentationPolicy();
	CLAM::Segmentation * theSegmentation=0;
	switch (policy)
	{
		case CLAM_Annotator::SegmentationPolicy::eUnsized:
		{
			theSegmentation = 
				new CLAM::UnsizedSegmentation(
					mCurrentAudio.GetSize(),
					&descriptorsMarks[0],
					&descriptorsMarks[0]+nMarks);
		} break;
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
	mSegmentation->xUnits("s");
	mSegmentEditor->SetSegmentation(mSegmentation);
	auralizeMarks();

	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	mSegmentDescriptors->refreshSchema(childScope);
}

void Annotator::updateSegmentations()
{
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	CLAM::DataArray & descriptorMarks = 
		mpDescriptorPool->GetWritePool<CLAM::DataArray>("Song",currentSegmentation)[0];
	const std::vector<double> & marks = mSegmentation->onsets();
	int nMarks = marks.size();
	descriptorMarks.Resize(nMarks);
	descriptorMarks.SetSize(nMarks);
	for (int i=0; i<nMarks; i++)
	{
		descriptorMarks[i] = marks[i];
	} 
	markCurrentSongChanged(true);
}

void Annotator::removeSegment(unsigned index)
{
	mStatusBar << "Removing segment at " << index << mStatusBar;
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to shrink
	CLAM_ASSERT(index<mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid segment to be removed");
	mpDescriptorPool->Remove(childScope, index);
}

void Annotator::insertSegment(unsigned index)
{
	mStatusBar << "Inserting segment at " << index << mStatusBar;
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to grow up
	CLAM_ASSERT(index<=mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid position to insert a segment");
	mpDescriptorPool->Insert(childScope, index);
}

void Annotator::adaptEnvelopesToCurrentSchema()
{
	mFrameLevelAttributeList->clear();

	const std::list<std::string>& names = mProject.GetNamesByScopeAndType("Frame", "Float");
	const unsigned nTabs = names.size();
	std::list<std::string>::const_iterator name = names.begin();
	for (unsigned i = 0; i<nTabs; name++, i++)
	{
		mFrameLevelAttributeList->addItem(name->c_str());
	}
}

void Annotator::makeConnections()
{
	// Action Signals
	connect(fileExitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(fileNew_projectAction, SIGNAL(triggered()), this, SLOT(fileNew()));
	connect(fileOpen_projectAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileSave_project_asAction, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	connect(fileSave_projectAction, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(addSongAction, SIGNAL(triggered()), this, SLOT(addSongsToProject()));
	connect(removeSongAction, SIGNAL(triggered()), this, SLOT(deleteSongsFromProject()));
	connect(songComputeDescriptorsAction, SIGNAL(triggered()), this, SLOT(computeSongDescriptors()));
	connect(songSaveDescriptorsAction, SIGNAL(triggered()), this, SLOT(saveDescriptors()));
	connect(mAuralizeSegmentOnsetsAction, SIGNAL(toggled(bool)), this, SLOT(updateAuralizationOptions()));
	connect(mAuralizeFrameLevelDescriptorsAction, SIGNAL(toggled(bool)), this, SLOT(updateAuralizationOptions()));
	connect(mLinkCurrentSegmentToPlaybackAction, SIGNAL(toggled(bool)), this, SLOT(linkCurrentSegmentToPlayback(bool)));
	connect(helpAboutAction,SIGNAL(triggered()), mAbout,SLOT(show()));
	connect(helpAboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(helpWhatsThisAction, SIGNAL(triggered()), this, SLOT(whatsThis()));

	mRecentFilesMenuSeparator = mFileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i) {
		mRecentFilesActions.push_back(new QAction(this));
		mRecentFilesActions[i]->setVisible(false);
		connect(mRecentFilesActions[i], SIGNAL(triggered()), this, SLOT(fileOpenRecent()));
		mFileMenu->addAction(mRecentFilesActions[i]);
	}

	// Changing the current song
	connect(mSongListView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
			this, SLOT(currentSongChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
	// Changing the current frame level descriptor
	connect(mFrameLevelAttributeList, SIGNAL(currentRowChanged(int)),
			this, SLOT(changeFrameLevelDescriptor(int)));
	// Changing the current segmentation descriptor
	connect(mSegmentationSelection, SIGNAL(activated(const QString&)),
			this, SLOT(refreshSegmentation()));
	// Apply global descriptors changes
	connect(mGlobalDescriptors, SIGNAL(contentEdited(int) ),
			this, SLOT(globalDescriptorsTableChanged(int) ) );
	// Apply segment descriptors changes
	connect(mSegmentDescriptors, SIGNAL(contentEdited(int) ),
			this, SLOT(segmentDescriptorsTableChanged(int) ) );
	// Apply frame descriptor changes
	connect( mBPFEditor, SIGNAL(yValueChanged(unsigned, double)),
		 this, SLOT(frameDescriptorsChanged(unsigned, double)));

	// Segment editing
	connect(mSegmentEditor, SIGNAL(segmentOnsetChanged(unsigned,double)),
		this, SLOT(segmentationMarksChanged(unsigned, double)));
	connect(mSegmentEditor, SIGNAL(segmentOffsetChanged(unsigned,double)),
		this, SLOT(segmentationMarksChanged(unsigned, double)));
	connect(mSegmentEditor, SIGNAL(currentSegmentChanged()),
		this, SLOT(changeCurrentSegment()));
	connect(mSegmentEditor, SIGNAL(segmentDeleted(unsigned)),
		this, SLOT(removeSegment(unsigned)));
	connect(mSegmentEditor, SIGNAL(segmentInserted(unsigned)),
		this, SLOT(insertSegment(unsigned)));

	// BPF editing
/*
	connect( mBPFEditor, SIGNAL(yValueChanged(unsigned, double)),
		 mPlayer, SLOT(updateYValue(int, double)));
*/

	// Interplot viewport syncronization
/*
	connect(mSegmentEditor, SIGNAL(xRulerRange(double,double)),
		mBPFEditor, SLOT(setHBounds(double,double)));
	connect( mBPFEditor, SIGNAL(selectedXPos(double)),
		 mSegmentEditor, SLOT(setSelectedXPos(double)));
	connect(mSegmentEditor, SIGNAL(selectedXPos(double)),
		mBPFEditor, SLOT(selectPointFromXCoord(double)));
*/
	// Interplot locator syncronization
	connect(mSegmentEditor, SIGNAL(selectedRegion(double,double)), // Was xRulerRange
		mBPFEditor, SLOT(updateLocator(double)));
	connect(mBPFEditor, SIGNAL(selectedRegion(double,double)), // Was xRulerRange
		mSegmentEditor, SLOT(updateLocator(double)));
	// Playhead update
	connect(mPlayer, SIGNAL(playingTime(double)),
		mSegmentEditor, SLOT(updateLocator(double)), Qt::DirectConnection);
	connect(mPlayer, SIGNAL(playingTime(double)),
		mBPFEditor, SLOT(updateLocator(double)), Qt::DirectConnection);
	connect(mPlayer, SIGNAL(stopTime(double,bool)),
		mBPFEditor, SLOT(updateLocator(double,bool)));
	connect( mPlayer, SIGNAL(stopTime(double,bool)),
		 mSegmentEditor, SLOT(updateLocator(double,bool)));
	connect(mSegmentEditor, SIGNAL(selectedRegion(double,double)),
		mPlayer, SLOT(timeBounds(double,double)));
	connect(mBPFEditor, SIGNAL(selectedRegion(double,double)),
		mPlayer, SLOT(timeBounds(double,double)));

	connect(mBPFEditor, SIGNAL(selectedRegion(double,double)),
		mSegmentEditor, SLOT(updateLocator(double,double)));
/*
	connect(mSegmentEditor, SIGNAL(stopPlayingTime(double)),
		this, SLOT(onStopPlaying(double)));
*/
	connect(mFrameEditorSplit, SIGNAL(splitterMoved(int,int)),
			this, SLOT(syncronizeSplits()));
	connect(mSegmentEditorSplit, SIGNAL(splitterMoved(int,int)),
			this, SLOT(syncronizeSplits()));
}
void Annotator::fileOpenRecent()
{
	// This hack is from the qt example, don't ask me...
	QAction *action = qobject_cast<QAction *>(sender());
	if (!action) return;
	loadProject( action->data().toString().toStdString() );
}

void Annotator::loadProject(const std::string & projectName)
{
	CLAM_Annotator::Project temporaryProject;
	try
	{
		CLAM::XMLStorage::Restore(temporaryProject,projectName);
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,"Error Loading Project File",
		constructFileError(projectName,e));
		return;
	}
	mProjectFileName = projectName;
	mProject = temporaryProject;
	initProject();
}

void Annotator::syncronizeSplits()
{
	if (sender()==mSegmentEditorSplit)
		mFrameEditorSplit->setSizes(mSegmentEditorSplit->sizes());
	if (sender()==mFrameEditorSplit)
		mSegmentEditorSplit->setSizes(mFrameEditorSplit->sizes());
}

void Annotator::linkCurrentSegmentToPlayback(bool enabled)
{
	mSegmentEditor->setCurrentSegmentFollowsPlay(enabled);
}


void Annotator::changeCurrentSegment()
{
	mStatusBar << "Segment changed to " << mSegmentation->current() << mStatusBar;
	// TODO: Some widgets may have half edited information. Need update.
	// TODO: Some times is not worth to update the information (deleted segment)
	mSegmentDescriptors->refreshData(mSegmentation->current(),mpDescriptorPool);
}

void Annotator::frameDescriptorsChanged(unsigned pointIndex,double newValue)
{
	/*TODO: right now, no matter how many points have been edited all descriptors are updated. This
	  is not too smart/efficient but doing it otherwise would mean having a dynamic list of slots 
	  in the class.*/
	unsigned index = mFrameLevelAttributeList->currentRow();
	mStatusBar << "Frame " << pointIndex 
		<< " changed value from " << mEPFs[index].GetBPF().GetValue(pointIndex) << " to " << newValue << mStatusBar;
	mEPFs[index].GetBPF().SetValue(pointIndex,TData(newValue));
	mFrameDescriptorsNeedUpdate = true;
}

void Annotator::segmentationMarksChanged(unsigned, double)
{
	updateSegmentations();
	if(mPlayer->IsPlaying())
		mMustUpdateMarkedAudio = true;
	else
		auralizeMarks();
}

void Annotator::updateSongListWidget()
{
	mSongListView->clear();
	std::vector< CLAM_Annotator::Song> songs = mProject.GetSongs();
	for ( std::vector<CLAM_Annotator::Song>::const_iterator it = songs.begin() ; it != songs.end() ; it++)
	{
#if QT_VERSION >= 0x040100 // QTPORT TODO: 4.0 backport
		QTreeWidgetItem * item = new QTreeWidgetItem(
			mSongListView,
			QStringList()
				<< it->GetSoundFile().c_str());
#else
		QTreeWidgetItem * item = new QTreeWidgetItem(mSongListView);
		item->setText(0, it->GetSoundFile().c_str());
#endif
	}
	mSongListView->show();
	mSongListView->resizeColumnToContents(0);
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

void Annotator::deleteSongsFromProject()
{
	if (QMessageBox::question(this, tr( "Removing songs from project"),
				tr("Are you sure you want to remove selected songs from the project?\n"
				"(Audio files and extracted descriptors files won't be removed from the disk)"),
				QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)
		return;
	QList< QTreeWidgetItem * > toBeDeleted = mSongListView->selectedItems();
	for ( QList< QTreeWidgetItem* >::iterator it = toBeDeleted.begin();
			it!= toBeDeleted.end(); it++ )
		mProject.RemoveSong((*it)->text(0).toStdString());
	markProjectChanged(true);
	updateSongListWidget();
}

void Annotator::addSongsToProject()
{
	QStringList files = QFileDialog::getOpenFileNames(this,
		"Add files to the project",
		projectToAbsolutePath(".").c_str(),
		"Songs (*.wav *.mp3 *.ogg)");
	QStringList::Iterator it = files.begin();
	for (; it != files.end(); it++ )
	{
		mProject.AppendSong(absoluteToProjectPath(it->toStdString()));
	}
	updateSongListWidget();
	markProjectChanged(true);
}

void Annotator::fileOpen()
{
	QString qFileName = QFileDialog::getOpenFileName(this, tr("Choose a project to work with"), QString::null, "*.pro");
	if(qFileName == QString::null) return;
	loadProject(qFileName.toStdString());
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
	QString qFileName = QFileDialog::getSaveFileName(this, tr("Saving the project"), QString::null,"*.pro");
	if(qFileName == QString::null) return;

	mProjectFileName = qFileName.toStdString();
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
	markProjectChanged(false);
	appendRecentOpenedProject(mProjectFileName);
}

void  Annotator::loadSchema()
{
	QString qFileName = QFileDialog::getOpenFileName(
			this,
			tr("Choose a Schema"),
			projectToAbsolutePath(".").c_str(),
			tr("Description Schemes (*.sc)"));
	if(qFileName == QString::null) return;

	std::string schemaFile = absoluteToProjectPath(qFileName.toStdString());
	mProject.SetSchema(schemaFile);
	initProject();
}

void  Annotator::saveDescriptors()
{
	if (mFrameDescriptorsNeedUpdate)
	{
		updateEnvelopesData();
		mFrameDescriptorsNeedUpdate=false;
		markCurrentSongChanged(true);
	}
	if (!mDescriptorsNeedSave) return;

	if (QMessageBox::question(this,QString("Save Descriptors"),
		QString("Do you want to save current song's descriptors?"),
		QString("Save Changes"),QString("Discard Them")) != 0) return;

	CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",projectToAbsolutePath(mCurrentDescriptorsPoolFileName));
	markCurrentSongChanged(false);
}

std::string Annotator::projectToAbsolutePath(const std::string & file)
{
	QString projectPath = QDir::cleanPath((mProjectFileName+"/../").c_str());
	mProject.SetBasePath(projectPath.toStdString());
	QDir qdir = QString(file.c_str());
	if (qdir.isRelative())
		return QDir::cleanPath( QDir(projectPath).filePath(file.c_str()) ).toStdString();
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

void Annotator::currentSongChanged(QTreeWidgetItem * current, QTreeWidgetItem *previous)
{
	if (mPlayer) mPlayer->stop();
	mStatusBar << "Saving Previous Song Descriptors..." << mStatusBar;
	if (previous) saveDescriptors();
	if (!current) return;

	setCursor(Qt::WaitCursor);

	mStatusBar << "Loading descriptors..." << mStatusBar;
	const std::string & songFilename = current->text(0).toStdString();
	mCurrentDescriptorsPoolFileName = mProject.GetDescriptorsFileName(songFilename);
	if (mCurrentDescriptorsPoolFileName=="") return; // TODO: fill with default data
	loadDescriptorPool();
	mStatusBar << "Filling Global Descriptors..." << mStatusBar;
	refreshGlobalDescriptorsTable();
	mStatusBar << "Drawing Audio..." << mStatusBar;
	mAudioRefreshTimer->stop();
	
	mSegmentEditor->hide();
	mBPFEditor->hide();
	setMenuAudioItemsEnabled(false);
	const std::string absolutePath = projectToAbsolutePath(songFilename).c_str();
	if (!loaderCreate(mCurrentAudio, absolutePath))
	{
		setCursor(Qt::ArrowCursor);
		QMessageBox::critical(this, tr("Error opening audio file"), absolutePath.c_str());
		return;
	}
	setMenuAudioItemsEnabled(true);

	refreshSegmentation();
	mSegmentEditor->SetData(mCurrentAudio);
	mSegmentEditor->show();
	mBPFEditor->show();
	mStatusBar << "Drawing LLD..." << mStatusBar;
	refreshEnvelopes();
	mStatusBar << "Done" << mStatusBar;
	loaderLaunch();
	setCursor(Qt::ArrowCursor);
	mAudioRefreshTimer->start(4000);
}

void Annotator::refreshEnvelopes()
{
	if (!mpDescriptorPool) return;

	mStatusBar << "Loading LLD Data..." << mStatusBar;

	// TODO: Not all the things should be done here
	mEPFs.clear();
	mBPFEditor->SetXRange(0.0,double(mCurrentAudio.GetDuration())/1000.0);

	mPlayer->SetAudioPtr(&mCurrentAudio);
	mPlayer->SetDuration(double(mCurrentAudio.GetDuration())/1000.0);

	const std::list<std::string>& divisionNames = mProject.GetNamesByScopeAndType("Song", "FrameDivision");

	std::list<std::string>::const_iterator divisionName;
	for(divisionName = divisionNames.begin();divisionName != divisionNames.end(); divisionName++)
	{
		const CLAM_Annotator::FrameDivision & division = mpDescriptorPool->GetReadPool<CLAM_Annotator::FrameDivision>("Song",*divisionName)[0];
		
		const std::string & frameDivisionChildScope = mProject.GetAttributeScheme("Song", *divisionName).GetChildScope();
		const std::list<std::string>& descriptorsNames = mProject.GetNamesByScopeAndType(frameDivisionChildScope, "Float");
		std::list<std::string>::const_iterator it;
		for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++)
		{
			mEPFs.push_back(CLAM::EquidistantPointsFunction());
			refreshEnvelope(mEPFs.back(), frameDivisionChildScope, *it, division);
		}
	}
	changeFrameLevelDescriptor(mFrameLevelAttributeList->currentRow());
}

void Annotator::refreshAudioData()
{
	mStatusBar << "Refreshing audio..." << mStatusBar;
	bool finished= loaderFinished();
	if (finished)
	{
		mStatusBar << "Last refresh, updating segment auralization..." << mStatusBar;
		mAudioRefreshTimer->stop();
		auralizeMarks();
	}
	mSegmentEditor->SetData(mCurrentAudio,true);

	if (!finished)
		mAudioRefreshTimer->start(2000);
}

void Annotator::refreshEnvelope(CLAM::EquidistantPointsFunction & epf, const std::string& scope, const std::string& descriptorName, const CLAM_Annotator::FrameDivision & division)
{
	CLAM::TData firstCenter = division.GetFirstCenter();
	CLAM::TData interCenterGap = division.GetInterCenterGap();
	const CLAM::TData* values = mpDescriptorPool->GetReadPool<CLAM::TData>(scope,descriptorName);
	TData sr = mCurrentAudio.GetSampleRate();
	int nFrames = mpDescriptorPool->GetNumberOfContexts(scope);
	epf.setDivision(firstCenter/sr, interCenterGap/sr, "s");
	epf.setValues(values, nFrames, "");
}

void Annotator::updateEnvelopesData()
{
	// TODO: Any child scope of any FrameDivision in Song not just Frame, which may not even exist
	mFrameDescriptorsNeedUpdate = false;
	unsigned nEPFEditors = mEPFs.size();
	const std::list<std::string>& descriptorsNames = mProject.GetNamesByScopeAndType("Frame", "Float");
	std::list<std::string>::const_iterator it=descriptorsNames.begin();
	for(unsigned int i = 0; i < nEPFEditors; i++, it++)
	{
		updateEnvelopeData(i, mpDescriptorPool->GetWritePool<CLAM::TData>("Frame",*it));
	}
}

void Annotator::updateEnvelopeData(int bpfIndex, CLAM::TData* descriptor)
{
	int nPoints = mEPFs[bpfIndex].GetBPF().Size();
	for (int i=0; i<nPoints; i++)
	{
		descriptor[i] = mEPFs[bpfIndex].GetBPF().GetValueFromIndex(i);
	}
}


void Annotator::loadDescriptorPool()
{
	mFrameDescriptorsNeedUpdate = false;
	markCurrentSongChanged(false);

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
	std::cout << "Refressing song data..." << std::endl;
	if (!mpDescriptorPool) return;
	mGlobalDescriptors->refreshData(0,mpDescriptorPool);
	mDescriptorsTable->show();
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
		reader.Do(mClick);
		reader.Stop();
	}
	const std::vector<double> & marks = mSegmentation->onsets();
	int nMarks = marks.size();
	mOnsetAuralizationAudio.SetSize(0);
	mOnsetAuralizationAudio.SetSize(mCurrentAudio.GetSize());
	mOnsetAuralizationAudio.SetSampleRate(mCurrentAudio.GetSampleRate());
	int size = mOnsetAuralizationAudio.GetSize();
	for (int i=0; i<nMarks; i++)
	{
		int samplePosition = Round(marks[i]*mCurrentAudio.GetSampleRate());
		if(samplePosition<size)
			mOnsetAuralizationAudio.SetAudioChunk(samplePosition,mClick[0]);
	} 
}

void Annotator::updateAuralizationOptions()
{
	bool playOnsets = mAuralizeSegmentOnsetsAction->isChecked();
	bool playLLDs = mAuralizeFrameLevelDescriptorsAction->isChecked();

	unsigned int LEFT_CHANNEL = 1;
	unsigned int RIGHT_CHANNEL = 2;
	mPlayer->SetAudioPtr(&mCurrentAudio);
	if (playOnsets)
		mPlayer->SetAudioPtr(&mOnsetAuralizationAudio, RIGHT_CHANNEL);
	mPlayer->SetPlayingFlags( CLAM::VM::eAudio | (playLLDs?CLAM::VM::eUseOscillator:0));
}

void Annotator::setMenuAudioItemsEnabled(bool enabled)
{
	playAction->setEnabled(enabled);
	pauseAction->setEnabled(enabled);
	stopAction->setEnabled(enabled);
	mAuralizeSegmentOnsetsAction->setEnabled(enabled);
	mAuralizeFrameLevelDescriptorsAction->setEnabled(enabled);
}

QString Annotator::constructFileError(const std::string& fileName,const CLAM::XmlStorageErr& e)
{
	return tr(
		"<p><b>XML loading Error: <pre>%1</pre></b></p>"
		"<p>Check that your file '%2'\n"
		"is well formed and folllows the specifications"
		"</p>"
		).arg(e.what()).arg(fileName.c_str());
}

void Annotator::onStopPlaying(float time)
{
	if(!mMustUpdateMarkedAudio) return;
	mMustUpdateMarkedAudio = false;
	auralizeMarks();
}

void Annotator::changeFrameLevelDescriptor(int current)
{
	unsigned index = mFrameLevelAttributeList->currentRow();
	if (index >= mEPFs.size()) return; // No valid descriptor
	double minValue = mEPFs[index].getMin();
	double maxValue = mEPFs[index].getMax();
	// TODO: Move this margin to the widget
	double span = maxValue-minValue;
	minValue -= span*0.1;
	maxValue += span*0.1;
	mBPFEditor->SetData(&mEPFs[index].GetBPF());
	bool scale_log = (fabs(minValue) > 9999.99 || fabs(maxValue) > 9999.99 || maxValue-minValue < TData(5E-2));
	mBPFEditor->SetYRange(minValue,maxValue, scale_log ? CLAM::VM::eLogScale : CLAM::VM::eLinearScale);

	mPlayer->SetData(mEPFs[index].GetBPF());
	mPlayer->SetPitchBounds(minValue, maxValue);
	mBPFEditor->show();
}

void Annotator::on_helpWhatsThisAction_triggered()
{
	QWhatsThis::enterWhatsThisMode();
}

void Annotator::on_reloadDescriptorsAction_triggered()
{
	mStatusBar << "Reloading.." << mStatusBar;
	currentSongChanged(mSongListView->currentItem(), mSongListView->currentItem());
}

void Annotator::on_playAction_triggered()
{
	mStatusBar << "Playing" << mStatusBar;
	if(!mPlayer) return;
	mPlayer->play();
}
void Annotator::on_pauseAction_triggered()
{
	mStatusBar << "Pause" << mStatusBar;
	if(!mPlayer) return;
	mPlayer->pause();
}
void Annotator::on_stopAction_triggered()
{
	mStatusBar << "Stop" << mStatusBar;
	if(!mPlayer) return;
	mPlayer->stop();
}

void Annotator::on_browseSchemaAction_triggered()
{
	mMainTabWidget->setCurrentWidget(mSchemaBrowser);
}



