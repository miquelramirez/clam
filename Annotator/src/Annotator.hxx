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

#ifndef ANNOTATOR_HXX
#define ANNOTATOR_HXX

#include "ui_Annotator.hxx"
#include "DescriptorTableController.hxx"

#include <string>
#include <utility>
#include <deque>
#include <QtCore/Qt>
#include <QtGui/QStatusBar>

//xamat

#include <CLAM/Audio.hxx>
#include <CLAM/Pool.hxx>
#include <CLAM/BPF.hxx>
#include "Project.hxx"
#include "EquidistantPointsFunction.hxx"

class QTimer;
class QTabBar;
class QTextBrowser;
class QCloseEvent;
class AudioLoadThread;
class SchemaBrowser;
using CLAM::TIndex;
using CLAM::TData;

namespace CLAM {
	namespace VM 
	{
		class AudioPlot;
		class BPFPlot;
		class BPFPlayer;
		class InstantView;
	};
	class Segmentation;
	class XmlStorageErr;
};
namespace CLAM_Annotator {
	class FrameDivision;
}
class InstantViewPlugin;
class SegmentationPane;
class FrameDescriptorsPane;
class Auralizer;

class StatusBarDumper 
{
		QStatusBar * mStatusBar;
		std::ostringstream * os;
	public:
		StatusBarDumper(QStatusBar * statusBar)
			: mStatusBar(statusBar)
		{
			os = new std::ostringstream;
		}
		~StatusBarDumper() {}
		StatusBarDumper & operator << (const QString & object)
		{
			(*os) << object.toLocal8Bit().constData();
			std::cout << object.toStdString();
			return *this;
		}
		template <typename ObjectType>
		StatusBarDumper & operator << (const ObjectType & object)
		{
			(*os) << object;
			std::cout << object;
			return *this;
		}
		StatusBarDumper & operator << (const StatusBarDumper & statusDumper)
		{
			(*os) << std::flush;
			std::cout << std::endl;
			mStatusBar->showMessage(QString::fromLocal8Bit(os->str().c_str()), 2000);
			delete os;
			os = new std::ostringstream;
			return *this;
		}
};


class Annotator : public QMainWindow, public Ui::Annotator
{
	Q_OBJECT

public:
	Annotator(const std::string & nameProject);
	virtual ~Annotator();
	void playPause();

	void initProject();
	void loadSettings();
	void saveSettings();
public slots:
	void currentSongChanged(QTreeWidgetItem*current, QTreeWidgetItem*previous);
	void globalDescriptorsTableChanged(int row);
	//void frameDescriptorsChanged(unsigned, double);
	void auralizeSegmentation();
	void updateSegmentation();

	void auralizeLLDs();

	void fileOpen();
	void fileOpenRecent();
	void addInstantView();

	void addSongsToProject();
	void deleteSongsFromProject();
	void saveDescriptors();
	void askToSaveDescriptorsIfNeeded();
	void computeSongDescriptors();
	//void endExtractorRunner(bool done);
	void refreshAudioData();
	void updateAuralizationOptions();
	void linkCurrentSegmentToPlayback(bool enabled);
	void setCurrentTime(double timeMilliseconds, double endTimeMiliseconds);
	void setVisibleXRange(double min, double max);

protected:
	void closeEvent( QCloseEvent * e);
private slots:
//	void setCurrentTime(double timeMilliseconds, double endTimeMiliseconds);
	void setCurrentPlayingTime(double timeMilliseconds);
	void setCurrentStopTime(double timeMilliseconds, bool paused);
	void updatePendingAuralizationsChanges();
	void saveProject();
	void syncronizeSplits();
	void markCurrentSongChanged(bool changed=true);

	void on_helpWhatsThisAction_triggered();
	void on_reloadDescriptorsAction_triggered();
	void on_playAction_triggered();
	void on_stopAction_triggered();
	void on_pauseAction_triggered();
	void on_newProjectAction_triggered();
	void on_editProjectPropertiesAction_triggered();
	void on_browseSchemaAction_triggered();
	void on_actionOnline_Tutorial_triggered();
	void on_actionAddSegmentationView_triggered();
	void on_actionAddFrameEvolutionView_triggered();
private:
	void loadProject(const std::string & projectFileName);
	void updateApplicationTitle();
	void updateSongListWidget();
	void makeConnections();
	void currentFile( std::string &);
	void markProjectChanged(bool changed);
	void refreshGlobalDescriptorsTable();
	void appendRecentOpenedProject(const std::string & projectFilename);

	QString constructFileError(const std::string& fileName, const CLAM::XmlStorageErr& e);
	void initInterface();
	void adaptInterfaceToCurrentSchema();
	void loadDescriptorPool();
	void adaptInstantViewsToSchema();

	void refreshInstantViews();

	void setMenuAudioItemsEnabled(bool);

	// Functions to control de audio loader
	bool loaderCreate(CLAM::Audio & audio, const std::string & filename);
		///< Creates a loader for the audio after clearing any existing one.
	void loaderLaunch(); ///< Starts the execution of the created loader
	bool loaderFinished(); ///< Clears the loader if it is finished returns if it was or there wasn't any.
	void abortLoader(); ///< Clears the loader 

	void resetTabOrder();
	void updateRecentFilesMenu();
	unsigned addNewSegmentationPane();
	unsigned addNewFrameDescriptorsPane();
	
private:
	CLAM::Audio mCurrentAudio; ///< The current audio piece
	CLAM_Annotator::Project mProject;
	std::string mProjectFileName; 
	std::string mCurrentDescriptorsPoolFileName;
	CLAM::DescriptionDataPool* mpDescriptorPool;
	bool mProjectNeedsSave;
	bool mDescriptorsNeedSave;
	bool mMustUpdateMarkedAudio;

	QTimer * mAudioRefreshTimer;
	SchemaBrowser * mSchemaBrowser;
	AudioLoadThread * mAudioLoaderThread;
	CLAM_Annotator::DescriptorTableController * mGlobalDescriptors;

	std::vector<CLAM::EquidistantPointsFunction> mEPFs; // Cached LLD's

  	QTextBrowser* mProjectDocumentation;
	
	typedef std::vector<InstantViewPlugin *> InstantViewPlugins;
	InstantViewPlugins mInstantViewPlugins;

	std::deque<std::string> mRecentOpenedProjects;
	StatusBarDumper mStatusBar;
	QDialog * mAbout;
	std::vector<QAction *> mRecentFilesActions;
	QAction * mRecentFilesMenuSeparator;
	enum {MaxRecentFiles=4};
	std::vector<SegmentationPane *> _segmentationPanes;
	std::vector<FrameDescriptorsPane *> _frameDescriptorsPanes;
	Auralizer * _auralizer;
	TData _visibleRangeMin;
	TData _visibleRangeMax;
signals:
	void deferredVisibleXRangeChange(double min, double max);
};

#endif

