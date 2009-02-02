/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#define QTPORT

#include "Annotator.hxx"
#include "FrameDivision.hxx"
#include "AudioLoadThread.hxx"
#include "SchemaBrowser.hxx"
//#include "AggregationEditor.hxx"
#include "TaskRunner.hxx"
#include "ui_About.hxx"
#include "ProjectEditor.hxx"

#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QTextBrowser>
#include <QtGui/QCloseEvent>
#include <QtGui/QSplashScreen>
#include <QtGui/QWhatsThis>

#if QT_VERSION >= 0x040200
#include <QtGui/QDesktopServices>
#else
#include <QtCore/QProcess>
#endif

#include <algorithm>
#include <iostream>
#include <fstream>

#include <CLAM/Array.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/ContiguousSegmentation.hxx>
#include <CLAM/DiscontinuousSegmentation.hxx>
#include <CLAM/UnsizedSegmentation.hxx>
#include <CLAM/Oscillator.hxx>

#include <CLAM/CLAMVersion.hxx>
#include "MusicAnnotatorVersion.hxx"

#include <vmBPFPlot.hxx>
#include <vmAudioPlot.hxx>
#include "InstantViewPlugin.hxx"

#ifndef VERSION
#define VERSION "0.3.2-CVS"
#endif

#ifndef RESOURCES_BASE
#define RESOURCES_BASE "../resources"
#endif

#include <QtGui/QSplitter>
#include "SegmentationPane.hxx"
#include "FrameDescriptorsPane.hxx"
#include "Auralizer.hxx"

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
	std::string filename = mSongListView->currentItem()->text(0).toStdString();
	filename  = mProject.RelativeToAbsolute(filename);
	QString qfilename = QString::fromStdString(filename);
	if (!std::ifstream(filename.c_str()))
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Unable to open selected file '%1'</b></p>.").arg(qfilename));
		return;
	}
	if (!mProject.HasExtractor() || mProject.GetExtractor()=="")
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: No extractor was defined for the project.</b></p>\n"));
		return;
	}
	mStatusBar << "Launching Extractor..." << mStatusBar;
	TaskRunner * runner = new TaskRunner();
	connect(runner, SIGNAL(taskDone(bool)), this, SLOT(endExtractorRunner(bool)));
	//jun: comment out as it occupies to much CPU&MEM while execute ClamExtractorExample
	
	//addDockWidget( Qt::BottomDockWidgetArea, runner);  

	
	// Wait the window to be redrawn after the reconfiguration
	// before loading the cpu with the extractor
	qApp->processEvents();

	std::string str2=".conf";
	std::string configurationOption = mProject.File()+str2;
	std::string schemaPath = mProject.RelativeToAbsolute(mProject.GetSchema());
	bool ok = (runner->run(mProject.GetExtractor().c_str(),
		QStringList() << "-c" <<configurationOption.c_str()<<"-s"<<schemaPath.c_str(),	QDir::current().path()))&&
		(runner->run(mProject.GetExtractor().c_str(),
		QStringList() << qfilename << "-f" << mProject.PoolSuffix().c_str() << "-c" <<configurationOption.c_str(),
		//QStringList() << qfilename << "-f" << mProject.PoolSuffix().c_str() << "-c" <<configurationOption.c_str()<<"-s"<<mProject.GetSchema().c_str(),
		//QStringList() << qfilename << "-f" << mProject.PoolSuffix().c_str(),
		QDir::current().path()));
	
	if (!ok)
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
			tr("<p><b>Error: Unable to launch the extractor.</b></p>\n"
				"<p>Check that the project extractor is well configured and you have permissions to run it.</p>\n"
				"<p>The configured command was:</p>\n<tt>%1</tt>")
			.arg(mProject.GetExtractor().c_str())
			);
		delete runner;
	}
	return;

	/*loadDescriptorPool(); // TODO: This should be done thru an slot from the runner
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
}

void Annotator::endExtractorRunner(bool done)
{
	if (done)
		loadDescriptorPool();
	else
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: The extractor was terminated with an error.</b></p>"));
		return;
	}
}

Annotator::Annotator(const std::string & nameProject = "")
	: QMainWindow( 0 )
	, Ui::Annotator( )
	, mpDescriptorPool(0)
	, mDescriptorsNeedSave(false)
	, mMustUpdateMarkedAudio(false)
	, mAudioRefreshTimer(new QTimer(this))
	, mAudioLoaderThread(0)
	, mGlobalDescriptors(0)
	, mStatusBar(statusBar())
	, _auralizer(0)
	, _visibleRangeMin(0)
	, _visibleRangeMax(0)
{

	QSplashScreen * splash = new QSplashScreen( QPixmap(":/logos/images/annotator-splash1.png") );
	splash->setCursor( QCursor(Qt::WaitCursor) );
	splash->show();
	splash->showMessage(tr("Loading data ... "));

	setupUi(this);
	mGlobalDescriptors = new CLAM_Annotator::DescriptorTableController(mDescriptorsTable, mProject);
	addNewFrameDescriptorsPane();
	addNewSegmentationPane();
	mAbout = new QDialog(this);
	Ui::About aboutUi;
	aboutUi.setupUi(mAbout);
	aboutUi.versionInfo->setText(tr(
			"<p><b>Music Annotator version %1</b></p>\n"
			"<p>Based on CLAM version %2</p>")
			.arg(MusicAnnotator::GetFullVersion())
			.arg(CLAM::GetFullVersion()));
	initInterface();
	setMenuAudioItemsEnabled(false);
	loadSettings();
	mAudioRefreshTimer->setSingleShot(true);
	connect (mAudioRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshAudioData()) );
	if (nameProject!="") // Project on command line
		loadProject(nameProject);
	else if (mProjectFileName!="") // Last openend project on settings
		loadProject(mProjectFileName);
	/* // TODO: Do something when no project is open
	else
		on_newProjectAction_triggered();
	*/
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
	settings.setValue(VERSION "/LastSession/ProjectFile", mProject.File().c_str()  );
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
		QString text = QString("&%1 %2").arg(i + 1).arg(mRecentOpenedProjects[i].c_str());
		mRecentFilesActions[i]->setText(text);
		mRecentFilesActions[i]->setData(mRecentOpenedProjects[i].c_str());
		mRecentFilesActions[i]->setVisible(true);
	}
	for (unsigned i = mRecentOpenedProjects.size(); i < MaxRecentFiles; ++i)
		mRecentFilesActions[i]->setVisible(false);
}

Annotator::~Annotator()
{
	saveSettings();
	abortLoader();
	for (InstantViewPlugins::iterator it=mInstantViewPlugins.begin(); it!=mInstantViewPlugins.end(); it++)
		delete *it;
}

void Annotator::initInterface()
{
	mSongListView->setSortingEnabled(false); // Unordered

	mProjectDocumentation = new QTextBrowser;
	mMainTabWidget->insertTab(0, mProjectDocumentation, tr("Project Documentation"));
	mMainTabWidget->setCurrentIndex(0);

	mCurrentAudio.ResizeToDuration(2.0);
	// init the segmentation panes
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->setData(0, mCurrentAudio);

	// ...and the frame descriptors panes
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
		_frameDescriptorsPanes[i]->setData(0, mCurrentAudio);

	mSchemaBrowser = new SchemaBrowser;
	mMainTabWidget->addTab(mSchemaBrowser, tr("Description Schema"));

	//mAggregationEditor = new AggregationEditor;

	//mMainTabWidget->addTab(mAggregationEditor, tr("Aggregation Editor"));
	
	_auralizer = new Auralizer(this);
	_auralizer->setAudio(mCurrentAudio);

	makeConnections();
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
	songSaveDescriptorsAction->setEnabled(changed);
	updateApplicationTitle();
}

void Annotator::updateApplicationTitle()
{
	QString title(tr("Music Annotator"));
	if (mProjectNeedsSave)
		title+=tr(" [modified project]");
	if (mDescriptorsNeedSave)
		title+=tr(" [modified descriptors]");
	setWindowTitle(title);
}

void Annotator::initProject()
{
	currentSongChanged(0, mSongListView->currentItem());
	updateSongListWidget();

	QString projectDescription;
	if (mProject.HasDescription() && mProject.GetDescription()!="")
		projectDescription += mProject.GetDescription().c_str();
	else
		projectDescription += tr("<p><em>(No project documentation available)</em></p>");
	mProjectDocumentation->setHtml(projectDescription);

	if (mProject.HasConfiguration())
	{
		mProject.InitConfiguration();
	}

	try
	{
		mProject.LoadScheme(mProject.GetSchema());
	}
	catch (CLAM::XmlStorageErr & e)
	{
		QMessageBox::warning(this,tr("Error Loading Schema File"),
				constructFileError(mProject.GetSchema(),e));
		return;
	}
	adaptInterfaceToCurrentSchema();

	markCurrentSongChanged(false);
	markProjectChanged(false);
	appendRecentOpenedProject(mProject.File());
}

void Annotator::adaptInterfaceToCurrentSchema()
{
	mStatusBar << tr("Adapting Interface to Song level descriptors...") << mStatusBar;
	mGlobalDescriptors->refreshSchema("Song");
	mStatusBar << tr("Adapting Interface to Frame level descriptors...") << mStatusBar;
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
		_frameDescriptorsPanes[i]->adaptEnvelopesToCurrentSchema();
	mStatusBar << tr("Adapting Interface to Segmentations...") << mStatusBar;
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->adaptToSchema();
	mStatusBar << tr("Updating schema browser...") << mStatusBar;
	mSchemaBrowser->setSchema(mProject.GetAnnotatorSchema());
	/*
	if(mProject.GetConfiguration()=="")
		mStatusBar << tr("Aggregation editor is void, skip...") << mStatusBar;
	else
	{
		mStatusBar << tr("Updating aggregation editor...") << mStatusBar;
		mAggregationEditor->loadConfig(mProject.GetConfiguration().c_str());
		mAggregationEditor->setSchema();
	}
	*/
		
	mStatusBar << tr("Creating instant views...") << mStatusBar;
	adaptInstantViewsToSchema();
	mStatusBar << tr("User interface adapted to the new schema.") << mStatusBar;
}
void Annotator::adaptInstantViewsToSchema()
{
	for (unsigned i=0; i<mInstantViewPlugins.size(); i++)
		delete mInstantViewPlugins[i];
	mInstantViewPlugins.clear();

	std::vector<CLAM_Annotator::InstantView> & instantViews = mProject.GetViews();
	for (unsigned i=0; i<instantViews.size(); i++)
	{
		InstantViewPlugin * plugin = InstantViewPlugin::createPlugin(instantViews[i].GetType());
		if (!plugin)
		{
			QMessageBox::warning(this,
				tr("Invalid instant view"), 
				tr("The project tried to create a instant view of type '%1' "
					"which is not available in your system")
					.arg(instantViews[i].GetType().c_str())
			);
			continue;
		}
		mInstantViewPlugins.push_back(plugin);
		QWidget * view = plugin->createView(0, mProject, instantViews[i]);
		mVSplit->addWidget(view);
	}
}


void Annotator::setVisibleXRange(double min, double max)
{
	_visibleRangeMin=min;
	_visibleRangeMax=max;
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
		_frameDescriptorsPanes[i]->setVisibleXRange(min, max);
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->setVisibleXRange(min, max);
}


void Annotator::globalDescriptorsTableChanged(int row)
{
	markCurrentSongChanged(true);
}

unsigned Annotator::addNewSegmentationPane()
{
	unsigned index = _segmentationPanes.size();
	SegmentationPane * pane = new SegmentationPane(0, mProject);
	// Changing the current segmentation descriptor
	connect(pane, SIGNAL(segmentationSelectionChanged()),
		this, SLOT(auralizeSegmentation()) );
	// Apply segment descriptors changes
	connect(pane, SIGNAL(dataChanged()),
		this, SLOT(updateSegmentation()) );
	// Current position update
	connect(pane, SIGNAL(playRegionChanged(double,double)),
		this, SLOT(setCurrentTime(double,double)));
	// Making the splitters look like a table
	connect(pane, SIGNAL(splitterMoved(int,int)),
		this, SLOT(syncronizeSplits()));

	// sync zoom
	connect(pane, SIGNAL(visibleXRangeChanged(double,double)),
		this, SLOT(setVisibleXRange(double,double)));
	connect(this, SIGNAL(deferredVisibleXRangeChange(double,double)),
		this, SLOT(setVisibleXRange(double,double)));

	if (!index)
		mVSplit->addWidget(pane);
	else 
		mVSplit->insertWidget(mVSplit->indexOf(_segmentationPanes.back())+1, pane);
	_segmentationPanes.push_back(pane);
	mVSplit->refresh();
	pane->show();
	return index;
}

unsigned Annotator::addNewFrameDescriptorsPane()
{
	FrameDescriptorsPane * pane = new FrameDescriptorsPane(0, mProject, mEPFs);
	// Changing the current frame descriptor
	connect(pane, SIGNAL(frameDescriptorsSelectionChanged()),
		this, SLOT(auralizeLLDs()) );
	// Current position update
	connect(pane, SIGNAL(playRegionChanged(double,double)),
		this, SLOT(setCurrentTime(double,double)));
	// Making the splitters look like a table
	connect(pane, SIGNAL(splitterMoved(int,int)),
		this, SLOT(syncronizeSplits()));
	// Horizontal view range changed
	connect(pane, SIGNAL(visibleXRangeChanged(double,double)),
		this, SLOT(setVisibleXRange(double,double)));
	// Be aware when frame descriptors are edited
	connect(pane, SIGNAL(dataChanged()),
		this, SLOT(markCurrentSongChanged()));

	unsigned index = _frameDescriptorsPanes.size();
	mVSplit->insertWidget(index,pane);
	_frameDescriptorsPanes.push_back(pane);
	mVSplit->refresh();
	pane->show();
	return index;
}

void Annotator::makeConnections()
{
	// Action Signals
	connect(fileExitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(fileOpen_projectAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileSave_projectAction, SIGNAL(triggered()), this, SLOT(saveProject()));
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
	for (unsigned i = 0; i < MaxRecentFiles; ++i) {
		mRecentFilesActions.push_back(new QAction(this));
		mRecentFilesActions[i]->setVisible(false);
		connect(mRecentFilesActions[i], SIGNAL(triggered()), this, SLOT(fileOpenRecent()));
		mFileMenu->addAction(mRecentFilesActions[i]);
	}
	std::list<std::string> pluginIds = InstantViewPlugin::availablePlugins();
	for (std::list<std::string>::iterator pluginId = pluginIds.begin();
			pluginId != pluginIds.end(); pluginId++)
	{
		QAction * viewAction = new QAction(this);
		viewAction->setText(InstantViewPlugin::createPlugin(*pluginId)->name());
		viewAction->setData(pluginId->c_str());
		connect(viewAction, SIGNAL(triggered()), this, SLOT(addInstantView()));
		menuAddInstantView->addAction(viewAction);
	}

	// Changing the current song
	connect(mSongListView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
			this, SLOT(currentSongChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

	// Apply global descriptors changes
	connect(mGlobalDescriptors, SIGNAL(contentEdited(int) ),
			this, SLOT(globalDescriptorsTableChanged(int) ) );

	// Playhead update
	connect( _auralizer->mPlayer, SIGNAL(playingTime(double)),
			this, SLOT(setCurrentPlayingTime(double)), Qt::DirectConnection);
	connect( _auralizer->mPlayer, SIGNAL(stopTime(double,bool)),
			this, SLOT(setCurrentStopTime(double,bool)), Qt::DirectConnection);

	// Update auralization whenever player stop and they have been modified
	connect(_auralizer->mPlayer, SIGNAL(stopTime(double)),
			this, SLOT(updatePendingAuralizationsChanges()));
}

// Playhead update while playing
void Annotator::setCurrentPlayingTime(double timeMilliseconds)
{
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->updateLocator(timeMilliseconds);
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
		_frameDescriptorsPanes[i]->updateLocator(timeMilliseconds);
	for (unsigned i=0; i<mInstantViewPlugins.size(); i++)
		mInstantViewPlugins[i]->setCurrentTime(timeMilliseconds);
	if (timeMilliseconds<_visibleRangeMin or timeMilliseconds>_visibleRangeMax)
		emit deferredVisibleXRangeChange(timeMilliseconds, timeMilliseconds+(_visibleRangeMax-_visibleRangeMin));
}

// Playhead update when stoping or pausing
void Annotator::setCurrentStopTime(double timeMilliseconds, bool paused)
{
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->updateLocator(timeMilliseconds, paused);
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
		_frameDescriptorsPanes[i]->updateLocator(timeMilliseconds, paused);
	for (unsigned i=0; i<mInstantViewPlugins.size(); i++)
		mInstantViewPlugins[i]->setCurrentTime(timeMilliseconds);
}

// The user set the playhead
void Annotator::setCurrentTime(double timeMilliseconds, double endTimeMiliseconds)
{
	static bool updating = false;
	if (updating) return;
	updating=true;
	_auralizer->seekTo(timeMilliseconds,endTimeMiliseconds);
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->updateLocator(timeMilliseconds, true);
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
		_frameDescriptorsPanes[i]->updateLocator(timeMilliseconds, true);
	for (unsigned i=0; i<mInstantViewPlugins.size(); i++)
		mInstantViewPlugins[i]->setCurrentTime(timeMilliseconds);
	updating=false;
}

void Annotator::addInstantView()
{
	// This hack is from the qt example, don't ask me...
	QAction *action = qobject_cast<QAction *>(sender());
	if (!action) return;
	std::string viewType = action->data().toString().toStdString();
	InstantViewPlugin * plugin = InstantViewPlugin::createPlugin(viewType);
	mInstantViewPlugins.push_back(plugin);
	CLAM_Annotator::InstantView config;
	config.SetType(viewType);
	if (!plugin->configureDialog(mProject, config)) return;
	mProject.GetViews().push_back(config);
	QWidget * view = plugin->createView(0, mProject, config);
	mVSplit->addWidget(view);
	markProjectChanged(true);
	if (mpDescriptorPool)
		plugin->updateData(*mpDescriptorPool, mCurrentAudio.GetSampleRate());
	else
		plugin->clearData();
	// TODO: Set current time
	//plugin->setCurrentTime(timeMilliseconds);
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
		QMessageBox::warning(this,tr("Error loading project file"),
			constructFileError(projectName,e));
		return;
	}
	mProject = temporaryProject;
	// To cope with old projects
	if (!mProject.HasViews())
	{
		mProject.AddViews();
		mProject.UpdateData();
	}
	mProject.SetProjectPath(projectName);
	initProject();
}

void Annotator::syncronizeSplits()
{
	QSplitter * movedSplitter = dynamic_cast<QSplitter*>(sender());
	if (!movedSplitter) return;
	QList<int> sizes = movedSplitter->sizes();
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
		_frameDescriptorsPanes[i]->setSizes(sizes);
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->setSizes(sizes);
}

void Annotator::linkCurrentSegmentToPlayback(bool enabled)
{
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->setCurrentSegmentFollowsPlay(enabled);
}

void Annotator::updateSegmentation()
{
	markCurrentSongChanged(true);

	if(_auralizer->isPlaying())
		mMustUpdateMarkedAudio = true;
	else
		auralizeSegmentation();
	// TODO: Warn other panes about the segmentation changes
}

void Annotator::updatePendingAuralizationsChanges()
{
	mStatusBar << tr("Updating auralizeSegmentation after playback") << mStatusBar;
	if(!mMustUpdateMarkedAudio) return;
	mMustUpdateMarkedAudio = false;
	auralizeSegmentation();
}

void Annotator::updateSongListWidget()
{
	mSongListView->clear();
	std::vector< CLAM_Annotator::Song> songs = mProject.GetSongs();
	for ( std::vector<CLAM_Annotator::Song>::const_iterator it = songs.begin() ; it != songs.end() ; it++)
	{
		QTreeWidgetItem * item = new QTreeWidgetItem;
		item->setText(0, it->GetSoundFile().c_str());
		mSongListView->addTopLevelItem(item);
	}
	mSongListView->show();
	mSongListView->resizeColumnToContents(0);
}

void Annotator::closeEvent ( QCloseEvent * e ) 
{
	_auralizer->stop();
	askToSaveDescriptorsIfNeeded();

	if (! mProjectNeedsSave )
	{
		e->accept();
		return;
	}
	int result = QMessageBox::question(this, tr("Close project"), 
		tr("Do you want to save changes to the project?"), 
		QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
	if (result == QMessageBox::Cancel)
	{
		e->ignore();
		return;
	}
	if (result == QMessageBox::Yes)
	{
		saveProject();
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
	// TODO: Clear song dependent widgets
}

void Annotator::addSongsToProject()
{
	QStringList files = QFileDialog::getOpenFileNames(this,
		tr("Add files to the project"),
		QString(mProject.BaseDir().c_str()),
		tr("Songs (*.wav *.mp3 *.ogg)"));
	QStringList::Iterator it = files.begin();
	for (; it != files.end(); it++ )
	{
		mProject.AppendSong(mProject.AbsoluteToRelative(it->toStdString()));
	}
	
	updateSongListWidget();
	markProjectChanged(true);
}

void Annotator::fileOpen()
{
	QString qFileName = QFileDialog::getOpenFileName(this, 
			tr("Choose a project to work with"), QString::null, 
			tr("Annotator project file (*.pro)"));
	if(qFileName == QString::null) return;
	loadProject(qFileName.toStdString());
}

void Annotator::on_newProjectAction_triggered()
{
	QString newProjectName = QFileDialog::getSaveFileName(this,
			tr("Choose a filename for the new project"),
			0, tr("Annotator project file (*.pro)")
			);
	if (newProjectName.isNull()) return;
	ProjectEditor projectDialog(this);
	projectDialog.setProjectPath(newProjectName.toStdString());
	if (projectDialog.exec()== QDialog::Rejected) return;
	projectDialog.applyChanges(mProject);
	mProject.GetSongs().clear();
	initProject();
	markProjectChanged(true);
}

void Annotator::on_editProjectPropertiesAction_triggered()
{
	ProjectEditor projectDialog(this);
	projectDialog.setProject(mProject);
	if (projectDialog.exec() == QDialog::Rejected) return;
	projectDialog.applyChanges(mProject);
	initProject();
	markProjectChanged(true);	
}


void Annotator::saveProject()
{
	CLAM_ASSERT(mProject.File()!="", "Saving using empty file name");
	CLAM::XMLStorage::Dump(mProject,"Project",mProject.File());
	markProjectChanged(false);
	appendRecentOpenedProject(mProject.File());
}

void  Annotator::saveDescriptors()
{
	CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",mProject.RelativeToAbsolute(mCurrentDescriptorsPoolFileName));
	markCurrentSongChanged(false);
}

void  Annotator::askToSaveDescriptorsIfNeeded()
{
	if (!mDescriptorsNeedSave) return;
	if (QMessageBox::question(this,QString("Save Descriptors"),
		tr("Current song descriptors have been changed.\n"
			"Do you want to save them?"),
		tr("Save Changes"),tr("Discard Them")) != 0) return;
	saveDescriptors();
}

void Annotator::currentSongChanged(QTreeWidgetItem * current, QTreeWidgetItem *previous)
{
	_auralizer->stop();
	setCurrentTime(0,0);
	mStatusBar << tr("Saving previous song descriptors...") << mStatusBar;
	if (previous) askToSaveDescriptorsIfNeeded();
	if (!current) return;

	setCursor(Qt::WaitCursor);

	mStatusBar << tr("Loading descriptors...") << mStatusBar;
	const std::string & songFilename = current->text(0).toStdString();
	mCurrentDescriptorsPoolFileName = mProject.GetDescriptorsFileName(songFilename);
	if (mCurrentDescriptorsPoolFileName=="") return; // TODO: fill with default data
	loadDescriptorPool();
	mStatusBar << tr("Filling global descriptors...") << mStatusBar;
	refreshGlobalDescriptorsTable();
	mStatusBar << tr("Drawing audio...") << mStatusBar;
	mAudioRefreshTimer->stop();
	
	setMenuAudioItemsEnabled(false);
	const std::string absolutePath = mProject.RelativeToAbsolute(songFilename);
	if (!loaderCreate(mCurrentAudio, absolutePath))
	{
		setCursor(Qt::ArrowCursor);
		QMessageBox::critical(this, tr("Error opening audio file"), absolutePath.c_str());
		return;
	}
	_auralizer->setAudio(mCurrentAudio);
	setMenuAudioItemsEnabled(true);
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
	{
		_segmentationPanes[i]->setData(mpDescriptorPool,mCurrentAudio);
	}
	for (unsigned i=0; i<_frameDescriptorsPanes.size(); i++)
	{
		_frameDescriptorsPanes[i]->setData(mpDescriptorPool, mCurrentAudio);
		_frameDescriptorsPanes[i]->refreshEnvelopes();
	}	
	setVisibleXRange(0, mCurrentAudio.GetSize()/mCurrentAudio.GetSampleRate());
	auralizeSegmentation();
	refreshInstantViews();	
	
	mStatusBar << tr("Done") << mStatusBar;
	loaderLaunch();
	setCursor(Qt::ArrowCursor);
	mAudioRefreshTimer->start(4000);
}
void Annotator::refreshInstantViews()
{
	mStatusBar << tr("Loading instant views data...") << mStatusBar;
	for (unsigned i=0; i<mInstantViewPlugins.size(); i++)
	{
		if (mpDescriptorPool)
			mInstantViewPlugins[i]->updateData(*mpDescriptorPool, mCurrentAudio.GetSampleRate());
		else
			mInstantViewPlugins[i]->clearData();
	}
}


void Annotator::refreshAudioData()
{
	mStatusBar << tr("Refreshing audio...") << mStatusBar;
	bool finished= loaderFinished();
	if (finished)
	{
		mStatusBar << tr("Updating segment auralization, after last audio refresh...") << mStatusBar;
		mAudioRefreshTimer->stop();
		auralizeSegmentation();
	}
	for (unsigned i=0; i<_segmentationPanes.size(); i++)
		_segmentationPanes[i]->redrawAudio();

	if (!finished)
		mAudioRefreshTimer->start(2000);
}


void Annotator::loadDescriptorPool()
{
	markCurrentSongChanged(false);

	if (mpDescriptorPool) delete mpDescriptorPool;
	mpDescriptorPool = 0;

	CLAM::DescriptionDataPool * tempPool = new CLAM::DescriptionDataPool(mProject.GetDescriptionScheme());

	//Load Descriptors Pool
	CLAM_ASSERT(mCurrentDescriptorsPoolFileName!="", "Empty file name");
	std::string poolFile = mProject.RelativeToAbsolute(mCurrentDescriptorsPoolFileName);
	try
	{
		CLAM::XMLStorage::Restore(*tempPool,poolFile);
		std::ostringstream os;
		os << tr("Descriptors file could not be validated with the schema.").toStdString() <<std::endl;
		if (!mProject.ValidateDataPool(*tempPool, os))
		{
			std::cerr << os.str() << std::endl;
			QMessageBox::warning(this, tr("Error loading descriptors"),
					os.str().c_str());
			delete tempPool;
			return;
		}
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,tr("Error loading descriptors"), 
			constructFileError(poolFile,e));
		delete tempPool;
		return;
	}

	//Create Descriptors Pool
	mpDescriptorPool = tempPool;
	mProject.InitializeMissingAttributes(*mpDescriptorPool);
}

void Annotator::refreshGlobalDescriptorsTable()
{
	mStatusBar << tr("Refressing song data...") << mStatusBar;
	if (!mpDescriptorPool) return;
	mGlobalDescriptors->refreshData(0,mpDescriptorPool);
	mDescriptorsTable->show();
}


void Annotator::auralizeSegmentation()
{
	// Taking just the segmentation of the first pane
	const CLAM::Segmentation * segmentation = _segmentationPanes[0]->getSegmentation();
	_auralizer->setSegmentation(segmentation);
}

void Annotator::auralizeLLDs()
{
	// taking the LLDs of the first pane
	const CLAM::EquidistantPointsFunction & EPFs= _frameDescriptorsPanes[0]->getCurrentEPFs();
	_auralizer->setLLD(EPFs);
}

void Annotator::updateAuralizationOptions()
{
	bool playOnsets = mAuralizeSegmentOnsetsAction->isChecked();
	bool playLLDs = mAuralizeFrameLevelDescriptorsAction->isChecked();
	_auralizer->setOptions(playOnsets, playLLDs);
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
		"<p><b>Error, reading '%1:\n"
		"<b>Details:</p>"
		"<pre>%2</pre></b></p>"
		)
		.arg(fileName.c_str())
		.arg(e.what());
}

void Annotator::on_helpWhatsThisAction_triggered()
{
	QWhatsThis::enterWhatsThisMode();
}

void Annotator::on_reloadDescriptorsAction_triggered()
{
	currentSongChanged(mSongListView->currentItem(), mSongListView->currentItem());
}

void Annotator::on_playAction_triggered()
{
	_auralizer->play();
}
void Annotator::on_pauseAction_triggered()
{
	_auralizer->pause();
}
void Annotator::on_stopAction_triggered()
{
	_auralizer->stop();
}

void Annotator::on_browseSchemaAction_triggered()
{
	mMainTabWidget->setCurrentWidget(mSchemaBrowser);
}

void Annotator::on_actionOnline_Tutorial_triggered()
{
	QString helpUrl = "http://iua-share.upf.edu/wikis/clam/index.php/Manual_Annotator";
	#if QT_VERSION >= 0x040200
	QDesktopServices::openUrl(helpUrl);
	#else
	QProcess::startDetached( "x-www-browser", QStringList() << helpUrl); // TODO: Remove this 4.1 unix only version
	#endif
}

void Annotator::on_actionAddSegmentationView_triggered()
{
	unsigned index = addNewSegmentationPane();
	_segmentationPanes[index]->adaptToSchema();
	_segmentationPanes[index]->setData(mpDescriptorPool,mCurrentAudio);
}


void Annotator::on_actionAddFrameEvolutionView_triggered()
{
	unsigned index = addNewFrameDescriptorsPane();
	_frameDescriptorsPanes[index]->adaptEnvelopesToCurrentSchema();
	_frameDescriptorsPanes[index]->setData(mpDescriptorPool,mCurrentAudio);
}
