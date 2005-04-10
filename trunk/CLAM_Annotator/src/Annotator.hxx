#ifndef ANNOTATOR_HXX
#define ANNOTATOR_HXX

#include "AnnotatorBase.h"
#include "AnnotatorDataFacade.hxx"
#include <string>

//xamat

#include "Audio.hxx"

#include "Schema.hxx"
#include "DescriptionScheme.hxx"
#include "Pool.hxx"

#include "SongFiles.hxx"
#include "Project.hxx"

#include "BPF.hxx"

namespace CLAM {
    namespace VM 
    {
	class QtAudioPlot;
	class BPFEditor;
    };
};

class QProgressDialog;

class Annotator : public AnnotatorBase
{
  Q_OBJECT
  

public:
  Annotator( const std::string &, const AnnotatorDataFacade::StringList &, AnnotatorDataFacade &, QWidget * parent = 0, const char * name = 0, WFlags f = 0);
  Annotator();
  void initSongs(const std::string &, const std::vector<std::string>&);
  void fillGlobalDescriptors( int index);
  void songsClicked( QListViewItem * item);
  void playPause();
  void chooseColor();
  void initFileMenu();
  void initEditMenu();

  void initProject();
  void initInterface();
public slots:
  void descriptorsTableChanged( int, int);
  void fileNew();			
  void fileOpen();
  void addSongsToProject();
  void addSongs( const AnnotatorDataFacade::StringList & );
  
  void loadSongList();
  void loadSchema();
  void loadDescriptors();

  void saveSongList();
  void saveSchema();
  void saveDescriptors();
  void saveAll();

  void deleteSongsFromProject();
  void fileSaveAs();
  void fileSave();
  void fileMenuAboutToShow();
  void editMenuAboutToShow();
  void showAnalyzeWindow();
  void analyze();
  void analyzeAll();
protected:
  void closeEvent( QCloseEvent * e);
  bool event(QEvent* e);

private:
  //void value( const std::string & descriptor, std::string & descriptorValue ) const;
  bool somethingIsSelected() const;
  void initSpliters();
  void initView();
  void initPlay();
  void makeDescriptorTable();
  void makeAnnotatorBrowserGLMenu();
  void makeConnections();
  void descriptor( std::string &, int);
  void currentFile( std::string &);
  void changeCurrentFile();
  void markAllNoChanges();
  void drawDescriptorsName();
  void drawDescriptorsValue( int, bool );

//xamat
  void initDataFacade();
  void languageChange();
  void initAudioWidget();
  void initLLDescriptorsWidgets();
  void removeLLDTabs();
  void drawAudio(QListViewItem * item);
  void loadAudioFile(const char* filename);
  void deleteAllSongsFromProject();
  
  void doAnalysis();
  void drawLLDescriptors(int index);
  void drawHLD(int songIndex, const std::string& descriptorName, 
			  const CLAM_Annotator::RestrictedString& value, bool computed);
  void drawHLD(int songIndex, const std::string& descriptorName,const std::string& value, 
	       bool computed );
  void drawHLD(int songIndex, const std::string& descriptorName, float value, bool computed);
  void drawHLD(int songIndex, const std::string& descriptorName, int value, bool computed);
 
  int findHLDescriptorIndex(const std::string& name);

  void initEnvelopes();
  void generateRandomEnvelopes();
  CLAM::BPF generateRandomEnvelope();

  void generateEnvelopesFromDescriptors();
  CLAM::BPF generateEnvelopeFromDescriptor(const std::string& name);

  void loadDescriptorPool();

  double GetMinY(const CLAM::BPF& bpf);
  double GetMaxY(const CLAM::BPF& bpf);

private:
  CLAM::VM::QtAudioPlot* mpAudioPlot;
  CLAM::Audio mCurrentAudio;

  CLAM_Annotator::Project mProject;
  CLAM_Annotator::SongFiles mSongFiles;
  std::vector<Song::Segments> mSongSegments;
  std::string mProjectFileName;
  std::map<std::string,int> mSongDescriptorsIndex;

  bool mChanges;
  AnnotatorDataFacade* mpData;
  std::vector<CLAM::BPF> mBPFs;
  std::vector<CLAM::VM::BPFEditor*> mBPFEditors;

                
  int mCurrentIndex;
                
  std::vector<bool> mHaveLLDescriptors;
  std::vector<bool> mHaveHLDescriptors;
                
  QVBoxLayout* mpTabLayout;
  std::vector<QWidget*> mTabPages;
               
  CLAM_Annotator::Schema mSchema;
  CLAM::DescriptionScheme mDescriptionScheme;
  CLAM::DescriptionDataPool* mpDescriptorPool;

  QProgressDialog* mpProgressDialog;

               
};

#endif
