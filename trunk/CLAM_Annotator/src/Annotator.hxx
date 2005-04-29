#ifndef ANNOTATOR_HXX
#define ANNOTATOR_HXX

#include "AnnotatorBase.h"

#include <string>

//xamat

#include "Audio.hxx"

#include "Schema.hxx"
#include "DescriptionScheme.hxx"
#include "Pool.hxx"

#include "SongFiles.hxx"
#include "Project.hxx"

#include "BPF.hxx"

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

class QProgressDialog;

class Annotator : public AnnotatorBase
{
  Q_OBJECT
  

public:
  Annotator(const std::string & nameProject);
  virtual ~Annotator() {}
  void initSongs();
  void fillGlobalDescriptors( int index);
  void songsClicked( QListViewItem * item);
  void playPause();
  void chooseColor();
  void initFileMenu();
 
  void initProject();
  void initInterface();
public slots:
  void descriptorsTableChanged( int, int);
  void descriptorsBPFChanged(int, float);
  void segmentationMarksChanged(int, unsigned);
  void fileNew();			
  void fileOpen();
  void addSongsToProject();
  void addSongs();
  
  void loadSongList();
  void loadSchema();
  void loadDescriptors();

  void saveSongList();
  void saveDescriptors();

  void deleteSongsFromProject();
  void fileSaveAs();
  void fileSave();
  void fileMenuAboutToShow();

  void playMarks(bool);
  void playOriginalAudioAndLLD(bool);
protected:
  void closeEvent( QCloseEvent * e);
  bool event(QEvent* e);

private:

  bool somethingIsSelected() const;
  void initSpliters();
  void initPlay();
  void makeDescriptorTable();
  void makeAnnotatorBrowserGLMenu();
  void makeConnections();
  void connectBPFs();
  void currentFile( std::string &);
  void changeCurrentFile();
  void markAllNoChanges();
  void drawDescriptorsName();
  void drawDescriptorsValue( int, bool );

//xamat
  
  QString constructFileError(const std::string& fileName, const CLAM::XmlStorageErr& e);

  void languageChange();
  void initAudioWidget();
  void initLLDescriptorsWidgets();
  void initHLDescriptorsTable();
  void removeLLDTabs();
  void drawAudio(QListViewItem * item);
  void loadAudioFile(const char* filename);
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
  CLAM::BPF generateEnvelopeFromDescriptor(const std::string& name);
  void generateDescriptorFromEnvelope(int bpfIndex, float* descriptors);
  void generateDescriptorsFromEnvelopes();

  void loadDescriptorPool();

  double GetMinY(const CLAM::BPF& bpf);
  double GetMaxY(const CLAM::BPF& bpf);

  void auralizeMarks();

private:
  CLAM::VM::QtAudioPlot* mpAudioPlot;
  CLAM::Audio mCurrentAudio;
  CLAM::Audio mCurrentMarkedAudio;
  std::vector<CLAM::Audio> mClick;

  CLAM_Annotator::Project mProject;
  CLAM_Annotator::SongFiles mSongFiles;
  
  std::string mProjectFileName;
  std::string mCurrentSoundFileName;
  std::string mCurrentDescriptorsPoolFileName;

  bool mGlobalChanges;
  bool mHLDChanged;
  bool mLLDChanged;
  bool mSegmentsChanged;
  bool mHasAudio;
  std::vector<CLAM::VM::BPFEditor*> mBPFEditors;
                
  int mCurrentIndex;     
                
  QVBoxLayout* mpTabLayout;
  std::vector<QWidget*> mTabPages;
               
  CLAM_Annotator::Schema mSchema;
  CLAM::DescriptionScheme mDescriptionScheme;
  CLAM::DescriptionDataPool* mpDescriptorPool;

  QProgressDialog* mpProgressDialog;

               
};

#endif
