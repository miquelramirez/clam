#ifndef ANNOTATOR_HXX
#define ANNOTATOR_HXX

#include "AnnotatorBase.h"
#include "AnnotatorDataFacade.hxx"
#include <string>

//xamat

#include "Audio.hxx"
#include "envelope_point_editor.hxx"
#include "envelope.hxx"

#include "LLDSchema.hxx"
#include "DescriptionScheme.hxx"
#include "Pool.hxx"

namespace CLAM {
		namespace VM 
		{
				class QtAudioPlot;
		};
};

class QProgressDialog;

class Annotator : public AnnotatorBase
{
	Q_OBJECT
	public:
		Annotator( const std::string &, const AnnotatorDataFacade::StringList &, AnnotatorDataFacade &, QWidget * parent = 0, const char * name = 0, WFlags f = 0);
		void initSongs(const std::string &, const AnnotatorDataFacade::StringList &, AnnotatorDataFacade &);
		void fillGlobalDescriptors( int index);
		void songsClicked( QListViewItem * item);
		void playPause();
		void chooseColor();
		void initFileMenu();
		void initEditMenu();
	public slots:
		void descriptorsTableChanged( int, int);
		void fileNew();			
		void fileOpen();
		void addSongsToProject();
		void addSongs( const AnnotatorDataFacade::StringList & );
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
		void value( const std::string & descriptor, std::string & descriptorValue ) const;
		void createListOfGenres( QStringList &, const QString & ) const;
		void createListOfTonalKeys( QStringList & list, const QString & value ) const;
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
		void drawArtists( int, bool );
		void drawTitle( int, bool );
		void drawGenre( int, bool );
		void drawDanceability( int, bool );
		void drawTonality( int, bool );
		void drawBPM( int, bool );
		void drawDynamicComplexity( int, bool );
//xamat
                void languageChange();
                void initAudioWidget();
                void initLLDescriptorsWidgets();
		void drawAudio(QListViewItem * item);
		void loadAudioFile(const char* filename);
  
                void doAnalysis();
                void drawLLDescriptors(int index);
                void initEnvelopes();
                void generateRandomEnvelopes();
		CLAM::Envelope* generateRandomEnvelope();

                void generateEnvelopesFromDescriptors();
                CLAM::Envelope* generateEnvelopeFromDescriptor(const std::string& name);

                void LoadDescriptorPool();
	private:
		bool mChanges;
		AnnotatorDataFacade & mData;
		typedef std::map< std::string, int > SongDescriptorsIndex;
		SongDescriptorsIndex mSongDescriptorsIndex;
		typedef std::vector< Song::Segments > SongSegments;
		SongSegments mSongSegments;
//xamat
		CLAM::VM::QtAudioPlot* mpAudioPlot;
                CLAM::Audio mCurrentAudio;

                std::vector<CLAM::Envelope_Point_Editor*> mFunctionEditors;
                std::vector<CLAM::Envelope*> mEnvelopes;
                
                int mCurrentIndex;
                
                std::vector<bool> mHaveLLDescriptors;
                std::vector<bool> mHaveHLDescriptors;
                
                QVBoxLayout* mpTabLayout;
                std::vector<QWidget*> mTabPages;
               
                CLAM::LLDSchema mLLDSchema;
                CLAM::DescriptionScheme mDescriptionScheme;
                CLAM::DescriptionDataPool* mpDescriptorPool;

                QProgressDialog* mpProgressDialog;
};

#endif
