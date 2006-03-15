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
	};
	class Segmentation;
	class XmlStorageErr;
};
namespace CLAM_Annotator {
	class FrameDivision;
}
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
			mStatusBar->showMessage(QString(os->str().c_str()), 2000);
			delete os;
			os = new std::ostringstream;
			return *this;
		}
};


class Annotator : public QMainWindow, public Ui::Annotator
{
	/*  
	 *	type to store bpf info from loaded LLDs.
	 *  first:  yrange
	 *  second: BPF
	 */
	typedef std::pair<std::pair<double,double>,CLAM::BPF> BPFInfo;

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
	void segmentDescriptorsTableChanged(int row);
	void frameDescriptorsChanged(unsigned, double);
	void segmentationMarksChanged(unsigned, double);
	void removeSegment(unsigned);
	void insertSegment(unsigned);
	void refreshSegmentation();

	void fileNew();
	void fileOpen();
	void fileSave();
	void fileSaveAs();
	void fileOpenRecent();

	void loadSchema();
	void addSongsToProject();
	void deleteSongsFromProject();
	void saveDescriptors();
	void computeSongDescriptors();

	void refreshAudioData();
	void changeCurrentSegment();
	void changeFrameLevelDescriptor(int current);
	void updateAuralizationOptions();
	void linkCurrentSegmentToPlayback(bool enabled);
protected:
	void closeEvent( QCloseEvent * e);

private slots:
	void onStopPlaying(float);

	void startPlaying();
	void pausePlaying();
	void stopPlaying();
	void on_helpWhats_thisAction_triggered();
	void syncronizeSplits();

private:
	void updateApplicationTitle();
	void updateSongListWidget();
	void makeConnections();
	void currentFile( std::string &);
	void markProjectChanged(bool changed);
	void markCurrentSongChanged(bool changed=true);
	void refreshGlobalDescriptorsTable();
	void appendRecentOpenedProject(const std::string & projectFilename);

	std::string projectToAbsolutePath(const std::string & file);
	std::string absoluteToProjectPath(const std::string & file);
	QString constructFileError(const std::string& fileName, const CLAM::XmlStorageErr& e);
	void initInterface();
	void adaptInterfaceToCurrentSchema();
	void loadDescriptorPool();

	void adaptEnvelopesToCurrentSchema();
	void refreshEnvelopes();
	void refreshEnvelope(CLAM::BPF & bpf, const std::string& scope, const std::string& descriptorName, const CLAM_Annotator::FrameDivision & frameDivision);
	void updateEnvelopesData();
	void updateEnvelopeData(int bpfIndex, CLAM::TData * descriptors);

	void adaptSegmentationsToCurrentSchema();
	void updateSegmentations();

	int songIndexInTable(const std::string& fileName);
	std::pair<double, double> GetMinMaxY(const CLAM::BPF& bpf);

	void auralizeMarks();
	void setMenuAudioItemsEnabled(bool);

	// Functions to control de audio loader
	bool loaderCreate(CLAM::Audio & audio, const std::string & filename);
		///< Creates a loader for the audio after clearing any existing one.
	void loaderLaunch(); ///< Starts the execution of the created loader
	bool loaderFinished(); ///< Clears the loader if it is finished returns if it was or there wasn't any.
	void abortLoader(); ///< Clears the loader 

	bool isPlaying();
	void resetTabOrder();
	void updateRecentFilesMenu();

private:
	CLAM::Audio mCurrentAudio; ///< The current audio piece
	CLAM::Audio mOnsetAuralizationAudio; ///< Current audio with segmentation marks inserted
	std::vector<CLAM::Audio> mClick; ///< A vector of audios to produce click

	CLAM_Annotator::Project mProject;
	std::string mProjectFileName; 

	std::string mCurrentDescriptorsPoolFileName;

	CLAM::DescriptionDataPool* mpDescriptorPool;

	bool mProjectNeedsSave;
	bool mFrameDescriptorsNeedUpdate;
	bool mDescriptorsNeedSave;
	bool mMustUpdateMarkedAudio;

	CLAM::VM::AudioPlot* mSegmentEditor;
	QTimer * mAudioRefreshTimer;
	SchemaBrowser * mSchemaBrowser;
	AudioLoadThread * mAudioLoaderThread;
	CLAM_Annotator::DescriptorTableController * mGlobalDescriptors;
	CLAM_Annotator::DescriptorTableController * mSegmentDescriptors;

	std::vector<BPFInfo> mBPFs; // cached LLDs

	CLAM::VM::BPFPlot *  mBPFEditor;
	CLAM::Segmentation * mSegmentation;

    QTextBrowser* mProjectDocumentation;
	CLAM::VM::BPFPlayer* mPlayer;
	std::deque<std::string> mRecentOpenedProjects;
	StatusBarDumper mStatusBar;
	QDialog * mAbout;
	std::vector<QAction *> mRecentFilesActions;
	QAction * mRecentFilesMenuSeparator;
	enum {MaxRecentFiles=4};
};

#endif
