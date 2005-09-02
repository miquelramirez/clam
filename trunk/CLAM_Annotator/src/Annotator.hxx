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
	void fillGlobalDescriptors( int index);
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
	void loadDescriptors();

	void saveDescriptors();

	void deleteSongsFromProject();
	void fileSaveAs();
	void fileSave();

	void playMarks(bool);
	void playOriginalAudioAndLLD(bool);
	void refreshAudioData();
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
	void drawDescriptorsValue( int, bool );

	//xamat

	QString constructFileError(const std::string& fileName, const CLAM::XmlStorageErr& e);

	void initAudioWidget();
	void AdaptInterfaceToCurrentSchema();
	void AdaptEnvelopesToCurrentLLDSchema();
	void AdaptDescriptorsTableToCurrentHLDSchema();
	void removeLLDTabs();
	void drawAudio(const char * filename);
	void deleteAllSongsFromProject();
	void drawLLDescriptors(int index);
	void drawHLD(int songIndex, const std::string& descriptorName, 
		const CLAM_Annotator::RestrictedString& value, bool computed=true);
	void drawHLD(int songIndex, const std::string& descriptorName,const std::string& value, 
		bool computed=true);
	void drawHLD(int songIndex, const std::string& descriptorName, float value, 
		CLAM_Annotator::Range<float> renge, bool computed=true);
	void drawHLD(int songIndex, const std::string& descriptorName, int value, 
		CLAM_Annotator::Range<int> range, bool computed=true);

	int findHLDescriptorIndex(const std::string& name);
	std::string getHLDescriptorNameFromIndex(int index);
	void getHLDSchemaElementFromIndex(int index, CLAM_Annotator::HLDSchemaElement&);
	void setHLDescriptorPoolFromString(const std::string& descriptorName, 
		const std::string& descriptorType,
		const std::string& descriptorValue);

	int getIndexFromFileName(const std::string& fileName);

	void initEnvelopes();

	void generateEnvelopesFromDescriptors();
	void fillEnvelopeWithLLDValues(CLAM::BPF & bpf, const std::string& descriptorName);
	void generateDescriptorFromEnvelope(int bpfIndex, float* descriptors);
	void generateDescriptorsFromEnvelopes();

	void loadDescriptorPool();

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

	void SetSchema(const std::string & schemaFile);

private:
	CLAM::Audio mCurrentAudio; ///< The current audio piece
	CLAM::Audio mCurrentMarkedAudio; ///< Current audio with segmentation marks inserted
	std::vector<CLAM::Audio> mClick; ///< A vector of audios to produce click

	CLAM_Annotator::Project mProject;
	std::string mProjectFileName; 

	int mCurrentIndex;
	std::string mCurrentSoundFileName;
	std::string mCurrentDescriptorsPoolFileName;

	CLAM_Annotator::Schema mSchema;
	CLAM::DescriptionScheme mDescriptionScheme;
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
