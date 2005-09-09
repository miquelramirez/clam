#ifndef ANNOTATOR_HXX
#define ANNOTATOR_HXX

#include "AnnotatorBase.h"

#include <string>
#include <utility>

//xamat

#include "Audio.hxx"

#include "Schema.hxx"
#include "DescriptionScheme.hxx"
#include "Pool.hxx"

#include "SongFiles.hxx"
#include "Project.hxx"

#include "BPF.hxx"

class QTimer;
class AudioLoadThread;
using CLAM::TIndex;
using CLAM::TData;

namespace CLAM {
	namespace VM 
	{
		class QtAudioPlot;
		class BPFEditor;
	};
	class XmlStorageErr;
};

class Annotator : public AnnotatorBase
{
	Q_OBJECT

public:
	Annotator(const std::string & nameProject);
	virtual ~Annotator();
	void songsClicked( QListViewItem * item);
	void playPause();

	void initProject();
	void initInterface();
	void markProjectChanged(bool changed);
public slots:
	void descriptorsTableChanged( int, int);
	void descriptorsBPFChanged(int, float);
	void segmentationMarksChanged(int, unsigned);
	void fileNew();			
	void fileOpen();
	void addSongsToProject();

	void loadSchema();

	void saveDescriptors();

	void computeSongDescriptors();
	void deleteSongsFromProject();
	void fileSaveAs();
	void fileSave();

	void playMarks(bool);
	void playOriginalAudioAndLLD(bool);
	void refreshAudioData();
	void changeCurrentSegment(unsigned current);
protected:
	void closeEvent( QCloseEvent * e);
	bool event(QEvent* e);

private:
	void updateSongListWidget();
	bool saveProject();

	void makeConnections();
	void connectBPFs();
	void currentFile( std::string &);
	void changeCurrentFile();
	void markAllSongsUnchanged();
	void refreshGlobalDescriptorsTable();

	//xamat

	QString constructFileError(const std::string& fileName, const CLAM::XmlStorageErr& e);
	void initAudioWidget();
	void adaptInterfaceToCurrentSchema();
	void drawAudio(const char * filename);
	void loadDescriptorPool();
	void removeLLDTabs();

	void adaptEnvelopesToCurrentSchema();
	void refreshEnvelopes();
	void refreshEnvelope(CLAM::BPF & bpf, const std::string& descriptorName);
	void updateEnvelopesData();
	void updateEnvelopeData(int bpfIndex, float* descriptors);

	void adaptDescriptorsTableToCurrentSchema(QTable * table, const std::string & scope);
	void refreshDescriptorsTable(QTable * table, const std::string & scope, unsigned element);
	void updateDescriptorTableData(QTable * table, const std::string & scope, unsigned element, int row);
	void drawHLD(QTable * table, int row, const CLAM_Annotator::RestrictedString& value, 
		const std::list<std::string> & options);
	void drawHLD(QTable * table, int row,const CLAM::Text& value);
	void drawHLD(QTable * table, int row, float value, 
		CLAM_Annotator::Range<float> renge);
	void drawHLD(QTable * table, int row, int value, 
		CLAM_Annotator::Range<int> range);

	void adaptSegmentationsToCurrentSchema();
	void refreshSegmentation();
	void updateSegmentations();

	int descriptorIndexInTable(const std::string & scope, const std::string& name);
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

	bool mGlobalChanges;
	bool mHLDChanged;
	bool mLLDChanged;
	bool mSegmentsChanged;

	std::vector<CLAM::VM::BPFEditor*> mBPFEditors;
	QVBoxLayout* mpTabLayout;
	std::vector<QWidget*> mTabPages;
	CLAM::VM::QtAudioPlot* mpAudioPlot;
	QTimer * mAudioRefreshTimer;
	AudioLoadThread * mAudioLoaderThread;
};

#endif
