#ifndef ANNOTATOR_HXX
#define ANNOTATOR_HXX

#include "AnnotatorBase.h"
#include "DescriptorTableController.hxx"

#include <string>
#include <utility>

//xamat

#include "Audio.hxx"
#include "Pool.hxx"
#include "Project.hxx"
#include "BPF.hxx"

class QTimer;
class AudioLoadThread;
class SchemaBrowser;
using CLAM::TIndex;
using CLAM::TData;

namespace CLAM {
	namespace VM 
	{
		class QtAudioPlot;
		class BPFEditor;
		class QtSingleBPFPlayerExt;
	};
	class Segmentation;
	class XmlStorageErr;
};

class Annotator : public AnnotatorBase
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
	void songsClicked( QListViewItem * item);
	void playPause();

	void initProject();
	void initInterface();
	void markProjectChanged(bool changed);
	void loadSettings();
	void saveSettings();
public slots:
	void globalDescriptorsTableChanged( int, int);
	void segmentDescriptorsTableChanged( int, int);
	void frameDescriptorsChanged(int, float);
	void segmentationMarksChanged(unsigned, double);
	void removeSegment(unsigned);
	void insertSegment(unsigned);

	void fileNew();
	void fileOpen();
	void fileSave();
	void fileSaveAs();

	void loadSchema();
	void addSongsToProject();
	void deleteSongsFromProject();
	void saveDescriptors();
	void computeSongDescriptors();

	void playMarks(bool);
	void playOriginalAudioAndLLD(bool);
	void refreshAudioData();
	void changeCurrentSegment(unsigned current);
	void linkCurrentSegmentToPlayback(bool enabled);
protected:
	void closeEvent( QCloseEvent * e);

private slots:
	void onStopPlaying(float);
	void onSelectPageLLD(QWidget*);

	void startPlaying();
	void pausePlaying();
	void stopPlaying();

private:
	void updateSongListWidget();
	void makeConnections();
	void connectBPFs();
	void currentFile( std::string &);
	void markCurrentSongChanged();
	void markAllSongsUnchanged();
	void refreshGlobalDescriptorsTable();

	std::string projectToAbsolutePath(const std::string & file);
	QString constructFileError(const std::string& fileName, const CLAM::XmlStorageErr& e);
	void initAudioWidget();
	void adaptInterfaceToCurrentSchema();
	void drawAudio(const char * filename);
	void loadDescriptorPool();

	void adaptEnvelopesToCurrentSchema();
	void refreshEnvelopes();
	void refreshEnvelope(CLAM::BPF & bpf, const std::string& descriptorName);
	void updateEnvelopesData();
	void updateEnvelopeData(int bpfIndex, float* descriptors);

	void adaptSegmentationsToCurrentSchema();
	void refreshSegmentation();
	void updateSegmentations();

	int songIndexInTable(const std::string& fileName);
	std::pair<double, double> GetMinMaxY(const CLAM::BPF& bpf);

	void auralizeMarks();
	void setMenuAudioItemsEnabled(bool);
	void hideBPFEditors();
	
	// Functions to control de audio loader
	void loaderCreate(CLAM::Audio & audio, const char * filename);
		///< Creates a loader for the audio after clearing any existing one.
	void loaderLaunch(); ///< Starts the execution of the created loader
	bool loaderFinished(); ///< Clears the loader if it is finished returns if it was or there wasn't any.
	void abortLoader(); ///< Clears the loader 

	bool isPlaying();
	void removeFromCurrentLayout();

private:
	CLAM::Audio mCurrentAudio; ///< The current audio piece
	CLAM::Audio mCurrentMarkedAudio; ///< Current audio with segmentation marks inserted
	std::vector<CLAM::Audio> mClick; ///< A vector of audios to produce click

	CLAM_Annotator::Project mProject;
	std::string mProjectFileName; 

	int mCurrentIndex;
	std::string mCurrentSoundFileName;
	std::string mCurrentDescriptorsPoolFileName;

	CLAM::DescriptionDataPool* mpDescriptorPool;

	bool mProjectNeedsSave;
	bool mFrameDescriptorsNeedUpdate;
	bool mDescriptorsNeedSave;
	bool mMustUpdateMarkedAudio;

	QVBoxLayout* mpTabLayout;
	std::vector<QWidget*> mTabPages;
	CLAM::VM::QtAudioPlot* mpAudioPlot;
	QTimer * mAudioRefreshTimer;
	SchemaBrowser * mSchemaBrowser;
	AudioLoadThread * mAudioLoaderThread;
	CLAM_Annotator::DescriptorTableController mGlobalDescriptors;
	CLAM_Annotator::DescriptorTableController mSegmentDescriptors;

	std::vector<BPFInfo> mBPFs; // cached LLDs         
	CLAM::VM::BPFEditor* mBPFEditor;
	int                  mCurrentBPFIndex;
	CLAM::Segmentation * mSegmentation;

	CLAM::VM::QtSingleBPFPlayerExt* mPlayer;
};

#endif
