#include "Annotator.hxx"
#include "RangeSelectionTableItem.hxx"
#include <qaction.h>
#include <qsplitter.h>
#include <qtextbrowser.h>
#include <qtoolbutton.h>
#include <qmessagebox.h>
#include <algorithm>
#include <qpopupmenu.h>
#include <qprogressdialog.h>
#include <qapplication.h>
#include <qeventloop.h>
#include <qfiledialog.h>


#include "AnalyzeWindow.hxx"

#include <algorithm>
#include <iostream>

//xamat
#include <time.h>

#include "QtAudioPlot.hxx"
#include <qtabwidget.h>
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "DataFacade.hxx"
#include "TXTSongParser.hxx"


#include "XMLStorage.hxx"

#include "BPFEditor.hxx"

using CLAM::VM::QtAudioPlot;
using CLAM::VM::BPFEditor;

using CLAM::TData;
using CLAM::TIndex;

Annotator::Annotator( const std::string & nameProject, const AnnotatorDataFacade::StringList & files , AnnotatorDataFacade & data, QWidget * parent, const char * name, WFlags f) : AnnotatorBase( parent, name, f),mCurrentIndex(0),mpTabLayout(0),mBPFs(0),mBPFEditors(0)
{
  //I should try to get rid of this constructor and pass things to the new one(see below)	
  setCaption( QString("Music annotator.- ") + QString( nameProject.c_str() ) );
  mpDescriptorPool = NULL;
  mpAudioPlot = NULL;
  initInterface();
  initAudioWidget();
}

Annotator::Annotator():AnnotatorBase( 0, "annotator", WDestructiveClose),mCurrentIndex(0),mpTabLayout(0),mBPFs(0),mBPFEditors(0)
{
  mpDescriptorPool = NULL;
  mpAudioPlot = NULL;
  initInterface();
  initAudioWidget();
}

void Annotator::initInterface()
{
  //  if (mpAudioPlot) delete mpAudioPlot;
  if (mpAudioPlot) mpAudioPlot->Hide();
  mProjectOverview->setSorting(-1);
  initView();
  initFileMenu();
  initEditMenu();
  makeDescriptorTable();
  makeConnections();
  
}

void Annotator::initProject()
{
  if (mProject.GetSongs()!="")
  {
    CLAM::XMLStorage::Restore(mSongFiles,mProject.GetSongs());
  }
  initSongs(name(), mSongFiles.GetFileNames());
  
  if (mProject.GetSchema()!="")
  {
    CLAM::XMLStorage::Restore(mSchema,mProject.GetSchema());
  }
  initLLDescriptorsWidgets();
  languageChange();
  loadDescriptorPool();
  
  mChanges = false;
}

void Annotator::initDataFacade()
{
   DataFacade data;
  TXTSongParser songParser( "DataTest/", data );
  mpData = new AnnotatorDataFacade( data );
  
  
}

bool Annotator::somethingIsSelected() const
{
	QListViewItemIterator it( mProjectOverview );
	for ( ; it.current() && !it.current()->isSelected() ; it++ );
	if ( it.current() )
	{
		return true;
	}
	return false;
}

void Annotator::initFileMenu()
{
	connect( mFileMenu, SIGNAL( aboutToShow() ), this, SLOT( fileMenuAboutToShow() ) );
}

void Annotator::initEditMenu()
{
	connect( mEditMenu, SIGNAL( aboutToShow() ), this, SLOT( editMenuAboutToShow() ) );
}

void Annotator::editMenuAboutToShow()
{
	editDelete_from_projectAction->setEnabled( somethingIsSelected() );
}

void Annotator::fileMenuAboutToShow()
{
	fileSave_projectAction->setEnabled( mChanges );
}

void Annotator::initAudioWidget()
{
        if(mpAudioPlot) delete mpAudioPlot;
	mpAudioPlot = new QtAudioPlot(frame3);
	mpTabLayout = new QVBoxLayout(frame3);
	mpTabLayout->addWidget(mpAudioPlot);
	mpAudioPlot->Label("Audio");
	mCurrentAudio.SetSize(20000);
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->SwitchDisplayColors(true);
	mpAudioPlot->SetToggleColorOn(true);
	mpAudioPlot->switchColors();
	mpAudioPlot->setFocus();
	mpAudioPlot->Hide();
}

void Annotator::initLLDescriptorsWidgets()
{
  
  removeLLDTabs();
  

  int nTabs = mSchema.GetLLDSchema().GetLLDNames().size();
        
  mTabPages.resize(nTabs);
  std::vector<QWidget*>::iterator it0;
  std::string baseTabString("TabPage");
  std::ostringstream tabString;
  int i = 0;
  for(it0 = mTabPages.begin(); it0 != mTabPages.end(); it0++,i++)
    {
      tabString.flush();
      tabString << baseTabString;
      if(it0 !=  mTabPages.begin())
	{
	  tabString << "_" << i;
	  (*it0) = new QWidget( tabWidget2, tabString.str().c_str());
	  tabWidget2->insertTab( (*it0), QString("") );
	}
      else
	{
	  (*it0) = tabWidget2->page(0);

	}
    }
  mBPFs.resize(nTabs);
  i=0;
  std::vector<CLAM::VM::BPFEditor*>::iterator it;
  mBPFEditors.resize(nTabs);
  for(it=mBPFEditors.begin();it!=mBPFEditors.end();it++,i++)
    {
      QVBoxLayout* tabLayout = new QVBoxLayout( tabWidget2->page(i));
      *it = new CLAM::VM::BPFEditor(tabWidget2->page(i));
      (*it)->Hide();
      tabLayout->addWidget(*it);
    }
}

void Annotator::removeLLDTabs()
{
  int nPages = mTabPages.size();
  mTabPages.resize(0);
  while(nPages>1)
  {
    tabWidget2->removePage(tabWidget2->page(nPages-1));
    nPages--;
  }
  tabWidget2->changeTab(tabWidget2->page(0), tr("") );
}

void Annotator::initView()
{	
	mIntraSongViewed=mIntraSong2Viewed=false;
	mSongDescriptorsViewed=true;
	artistViewed=titleViewed=genreViewed=lyricsViewed=itemViewed=structureItemViewed=true;
}

void Annotator::makeDescriptorTable()
{
	mDescriptorsTable->setLeftMargin(0);
	mDescriptorsTable->setLeftMargin(0);
	mDescriptorsTable->setNumRows(0);
	mDescriptorsTable->setNumCols(2);
	mDescriptorsTable->setRowMovingEnabled(false);
	mDescriptorsTable->setColumnMovingEnabled(false);
	mDescriptorsTable->setReadOnly(false);
	mDescriptorsTable->setSelectionMode(QTable::NoSelection);
	mDescriptorsTable->horizontalHeader()->setLabel( 0, tr( "Descriptor" ) );
	mDescriptorsTable->horizontalHeader()->setLabel( 1, tr( "Value" ) );
}

void Annotator::analyze()
{
  mHaveLLDescriptors[mCurrentIndex]=true;

  mHaveHLDescriptors[mCurrentIndex]=false;
  
  doAnalysis();
 
}

void Annotator::doAnalysis()
{
  drawLLDescriptors(mCurrentIndex);
  drawAudio(NULL);
  fillGlobalDescriptors(mCurrentIndex);
  mChanges=true;
  changeCurrentFile();
}

void Annotator::analyzeAll()
{
  mHaveLLDescriptors[mCurrentIndex]=true;

  mHaveHLDescriptors[mCurrentIndex]=true;
  
  doAnalysis();
}

void Annotator::showAnalyzeWindow()
{
  connect(&mAnalyzeWindow, SIGNAL( analyze() ), this, SLOT( analyze() ) );
  connect(&mAnalyzeWindow, SIGNAL( analyzeAll() ), this, SLOT( analyzeAll() ) );
  mAnalyzeWindow.show();
}

void Annotator::makeConnections()
{
  //connect(songAnalyzeAction,SIGNAL(activated()),this,SLOT(showAnalyzeWindow()));
	connect(helpAboutAction,SIGNAL(activated()),&mAbout,SLOT(show()));
	connect(configurationPreferencesAction,SIGNAL(activated()),&mConfigurationDialog,SLOT(show()));
	connect(mDescriptorsTable, SIGNAL(valueChanged( int, int) ) , this, SLOT( descriptorsTableChanged(int, int) ) );	
}

void Annotator::descriptor( std::string & descriptor, int row)
{
	if ( row == 0 )
		descriptor = "artists";
	else if ( row == 1 )
		descriptor = "name";
	else if ( row == 2 )
		descriptor = "genre";
	else if ( row == 3 )
		descriptor = "Danceability";
	else if ( row == 4 )
		descriptor = "Tonal Descriptor: Key Note";
	else if ( row == 5)
		descriptor = "Tonal Descriptor: Mode";
	else if ( row == 6 )
		descriptor = "Dynamic Complexity";
	else if ( row == 7 )
		descriptor = "BPM";
	else
		std::cout<<"a descriptor has been changed that it is not known what to do"<<std::endl;
}

void Annotator::currentFile( std::string & nameOfTheFile )
{
	QListViewItemIterator it( mProjectOverview );
	for ( ; it.current() && !it.current()->isSelected() ; it++ );
	if ( it.current() )
	{		
		nameOfTheFile = std::string( it.current()->text(0).ascii() );	
	}
}

void Annotator::changeCurrentFile()
{
	QListViewItemIterator it( mProjectOverview );
	for ( ; it.current() && !it.current()->isSelected() ; it++ );
	if ( it.current() )
	{		
		it.current()->setText(2, "Yes");	
	}

}

/*
void Annotator::value( const std::string & descriptor, std::string & descriptorValue ) const
{
	if ( descriptor == "Danceability" )
	{
		int previousDescValue = QString( descriptorValue.c_str() ).toInt();
		double descValue = mpData->scaleDanceability( previousDescValue );
		descriptorValue = std::string( QString().setNum( descValue ).ascii() );
	}
	else if ( descriptor == "Dynamic Complexity" )
	{	
		int previousDescValue = QString( descriptorValue.c_str() ).toInt();
		double descValue = mpData->scaleDynamicComplexity( previousDescValue );
		descriptorValue = std::string( QString().setNum( descValue ).ascii() );
	}
}
*/

void Annotator::descriptorsTableChanged(int row, int column)
{
  //TODO: how to deal with editiding functions?
  /*	mChanges = true;
	std::string nameOfDescriptor;
	descriptor( nameOfDescriptor, row );
	std::string nameOfTheFile;
	currentFile( nameOfTheFile );
	changeCurrentFile();
	std::string valueOfDescriptor;
	valueOfDescriptor = std::string( mDescriptorsTable->text(row, column).ascii() );
	value( nameOfDescriptor, valueOfDescriptor);
	mpData->setDescriptor( nameOfTheFile, nameOfDescriptor, valueOfDescriptor );
	int index = mSongDescriptorsIndex[ nameOfTheFile ];
	mSongDescriptors[ index ][ nameOfDescriptor ] = valueOfDescriptor;
  */	
}

void Annotator::addSongs( const AnnotatorDataFacade::StringList & list)
{
  deleteAllSongsFromProject();
        std::vector< std::string > songs;
	QListViewItemIterator it( mProjectOverview );
	while ( it.current() )
	{
		songs.push_back( std::string( it.current()->text(0).ascii() ) );
		++it;
	}

	
	Descriptors tmp;
	for ( AnnotatorDataFacade::StringList::const_iterator it = list.begin() ; it != list.end() ; it++)
	{
		if ( std::find( songs.begin(), songs.end(), *it ) == songs.end() )
		{
			ListViewItem * item = new ListViewItem( mProjectOverview->childCount(), mProjectOverview, QString( it->c_str() ), tr("Yes"), tr("No") );
			songs.push_back(*it);
			tmp.clear();
			mpData->getDescriptorsFromFile(*it, tmp);
			mSongDescriptorsIndex[*it]= mSongDescriptors.size();
			Song::Segments segments;
			mpData->getSegmentInformationFromFile( segments, *it );
			mSongSegments.push_back( segments );
			mSongDescriptors.push_back(tmp);
			//by default songs have HL descriptors but not LL
			mHaveHLDescriptors.push_back(true);
			mHaveLLDescriptors.push_back(false);
		}
	}
}

void Annotator::closeEvent ( QCloseEvent * e ) 
{
	if ( mChanges )
	{
		QMessageBox::question(this, "Close project", "Do you want to save the changes?", QMessageBox::Yes, QMessageBox::No );
	}
	e->accept();
}

void Annotator::markAllNoChanges()
{
	QListViewItemIterator it (mProjectOverview);
	for ( ; it.current() ; ++it )
	{
		it.current()->setText(2, "No");
	}
}

void Annotator::deleteAllSongsFromProject()
{
  	std::vector< QListViewItem * > toBeDeleted;
	QListViewItemIterator it( mProjectOverview );
	while ( it.current() )
	{
		toBeDeleted.push_back(*it);
		++it;
	}
	for ( std::vector< QListViewItem* >::iterator it = toBeDeleted.begin() ; it != toBeDeleted.end() ; it++)
		delete *it;
}

void Annotator::deleteSongsFromProject()
{
	std::vector< QListViewItem * > toBeDeleted;
	QListViewItemIterator it( mProjectOverview );
	while ( it.current() )
	{
		if ( it.current()->isSelected() )
			toBeDeleted.push_back(*it);
		++it;
	}
	for ( std::vector< QListViewItem* >::iterator it = toBeDeleted.begin() ; it != toBeDeleted.end() ; it++)
		delete *it;
}

void Annotator::addSongsToProject()
{
  /*	AddSongsToProjectDialog * dialog = new AddSongsToProjectDialog( *mpData, this, "add songs to project", WDestructiveClose);
	dialog->show();
  */	
}

void Annotator::fileOpen()
{
  QString qFileName;
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProjectFileName = std::string(qFileName.ascii());
    CLAM::XMLStorage::Restore(mProject,mProjectFileName);
    //loadDescriptorPool();
    initDataFacade();
    initInterface();
    initProject();
  }
}

void Annotator::fileNew()
{
  mProjectFileName = "";
  mProject.SetSongs("");
  mProject.SetDescriptorPool("");
  mProject.SetSchema("");
  mSongFiles.GetFileNames().resize(0);
  mSchema.GetLLDSchema().GetLLDNames().resize(0);
  mSchema.GetHLDSchema().GetHLDs().resize(0);
  initInterface();
  initProject();
  mChanges = true;
}

void Annotator::fileSave()
{
	mChanges = false;
	markAllNoChanges();
	if(mProjectFileName=="") fileSaveAs();
	else
	{
	  //Before adding the suffix I should better remove the .xml maybe by doing:
	  //songFile = mProjectFileName;
	  //songFile.remove(songFile.find_last_of(".xml"),4);
	  CLAM::XMLStorage::Dump(mProject,"Project",mProjectFileName);
	  std::string songFile = mProject.GetSongs();
	  if(songFile == "")
	    songFile = mProjectFileName+".songs.xml";
	  CLAM::XMLStorage::Dump(mSongFiles, "Songs", songFile);
	  std::string descriptorPoolFile = mProject.GetDescriptorPool();
	  if(descriptorPoolFile == "")
	    descriptorPoolFile = mProjectFileName+".pool.xml";
	  CLAM::XMLStorage::Dump(*mpDescriptorPool, "DescriptorPool",descriptorPoolFile);
	  std::string schemaFile = mProject.GetSchema();
	  if(schemaFile == "")
	    schemaFile = mProjectFileName+".schema.xml";
	  CLAM::XMLStorage::Dump(mSchema, "Schema",schemaFile);
	}

}

void Annotator::fileSaveAs()
{
  QString qFileName = QFileDialog::getSaveFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProjectFileName = std::string(qFileName.ascii());
    fileSave();
  }
}
void  Annotator::loadSongList()
{
  QString qFileName;
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProject.SetSongs(std::string(qFileName.ascii()));
    CLAM::XMLStorage::Restore(mSongFiles,mProject.GetSongs());
    //TODO: Does loading the song list affect all this
    initDataFacade();
    initInterface();
    initProject();
  }
}

void  Annotator::loadSchema()
{
  QString qFileName;
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProject.SetSchema(std::string(qFileName.ascii()));
    CLAM::XMLStorage::Restore(mSchema,mProject.GetSchema());
    //TODO: Does loading the schema affect all this
    initDataFacade();
    initInterface();
    initProject();
  }
}

void  Annotator::loadDescriptors()
{
  QString qFileName;
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProject.SetDescriptorPool(std::string(qFileName.ascii()));
    CLAM::XMLStorage::Restore(*mpDescriptorPool,mProject.GetDescriptorPool());
    //TODO: Does loading the descriptors affect all this
    initDataFacade();
    initInterface();
    initProject();
  }

}

void  Annotator::saveSongList()
{
  QString qFileName;
  qFileName = QFileDialog::getSaveFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProject.SetSongs(std::string(qFileName.ascii()));
    CLAM::XMLStorage::Dump(mSongFiles,"Songs",mProject.GetSongs());
  }

}

void  Annotator::saveSchema()
{
  QString qFileName;
  qFileName = QFileDialog::getSaveFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProject.SetSchema(std::string(qFileName.ascii()));
    CLAM::XMLStorage::Dump(mSchema,"Schema",mProject.GetSchema());
  }

}

void  Annotator::saveDescriptors()
{
  QString qFileName;
  qFileName = QFileDialog::getSaveFileName(QString::null,"*.xml");
  if(qFileName != QString::null)
  {
    mProject.SetDescriptorPool(std::string(qFileName.ascii()));
    CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",mProject.GetDescriptorPool());
  }

}

void  Annotator::saveAll()
{
  if(QMessageBox::information(this,QString("Saving Song List"),
			      QString("Please choose Song List filename"),
			      QString("OK"),QString("Cancel")) == 0)
    saveSongList();
  
  if(QMessageBox::information(this,QString("Saving Schema"),
			      QString("Please choose now Schema filename"),
			      QString("OK"),QString("Cancel")) == 0)
    saveSchema();

  if(QMessageBox::information(this,QString("Saving Descriptors"),
			      QString("Please finally choose Descriptors filename"),
			      QString("OK"),QString("Cancel")) == 0)
    saveSongList();
 

}


void Annotator::initSongs( const std::string & nameProject, const std::vector<std::string> & files)
{
	mLogicGroup->hide();
	addSongs( files );	
}

void Annotator::chooseColor()
{
	QColorDialog::getColor();
}

void Annotator::songsClicked( QListViewItem * item)
{
         if (item != 0)
	{
	        CLAM::AudioFile file;
	      	file.OpenExisting(item->text(0).ascii());
	        mpProgressDialog = new QProgressDialog ("Loading Audio", 
							"Cancel",file.GetHeader().GetLength(),
							this);
		mpProgressDialog->setProgress(0);
		mCurrentIndex = mSongDescriptorsIndex[std::string(item->text(0).ascii()) ];
		fillGlobalDescriptors( mCurrentIndex );
		drawAudio(item);
		drawLLDescriptors(mCurrentIndex);
		songAnalyzeAction->setEnabled( TRUE );
		songAnalyzeAllAction->setEnabled (TRUE );
		
		delete mpProgressDialog;
		mpProgressDialog = NULL;

	}
}

void Annotator::drawAudio(QListViewItem * item=NULL)
{
        mpAudioPlot->Hide();
	if(item)        
	  loadAudioFile(item->text(0).ascii());
	std::vector<unsigned> marks;
	if(mHaveLLDescriptors[mCurrentIndex])
	  {  
	    int i;
	    int randomIncr;
	    for(i=0;i<mCurrentAudio.GetSize();i+=500000)
	    {
	      randomIncr = (float (rand())/float(RAND_MAX))*500000-250000;
	      marks.push_back(i+randomIncr);
	    }
	  }
	mpAudioPlot->SetMarks(marks);
	mpAudioPlot->SetData(mCurrentAudio);
	//mpAudioPlot->SetForegroundColor(CLAM::VM::VMColor::Blue());
	mpAudioPlot->Show();
}

void Annotator::drawLLDescriptors(int index)
{
    generateEnvelopesFromDescriptors();

    std::vector<CLAM::BPF>::iterator bpf_it = mBPFs.begin();
    std::vector<CLAM::VM::BPFEditor*>::iterator editors_it = mBPFEditors.begin();
    for(int i=0;bpf_it != mBPFs.end(); i++, bpf_it++, editors_it++)
    {
	if(mHaveLLDescriptors[index])
	{
	    (*editors_it)->Show();
	    (*editors_it)->Geometry(0,0,tabWidget2->page(i)->width(),tabWidget2->page(i)->height());
	    (*editors_it)->SetData((*bpf_it));
	    (*editors_it)->SetXRange(0.0,double(mCurrentAudio.GetDuration())/1000.0);
	    (*editors_it)->SetYRange(GetMinY((*bpf_it)),GetMaxY((*bpf_it)));
	}
	else
	{
	    (*editors_it)->Hide();
	}
  }

}

void Annotator::loadAudioFile(const char* filename)
{
	const CLAM::TSize readSize = 1024;
	CLAM::AudioFile file;
	file.OpenExisting(filename);
	int nChannels = file.GetHeader().GetChannels();
	std::vector<CLAM::Audio> audioFrameVector(nChannels);
	int i;
	for (i=0;i<nChannels;i++)
			audioFrameVector[i].SetSize(readSize);
	CLAM::MultiChannelAudioFileReaderConfig cfg;
	cfg.SetSourceFile( file );
	CLAM::MultiChannelAudioFileReader reader(cfg);
	reader.Start();
	int beginSample=0;
	mCurrentAudio.SetSize(0);
	//	mpProgressDialog = new QProgressDialog("Loading Audio File", 
	//					      "Cancel",file.GetHeader().GetSamples());
//mpProgressDialog->setProgress( 0 );
	float samplingRate = mCurrentAudio.GetSampleRate();
	while(reader.Do(audioFrameVector))
	{
		mCurrentAudio.SetSize(mCurrentAudio.GetSize()+audioFrameVector[0].GetSize());
		mCurrentAudio.SetAudioChunk(beginSample,audioFrameVector[0]);
		beginSample+=readSize;
		qApp->eventLoop()->processEvents( QEventLoop::AllEvents );
		mpProgressDialog->setProgress( beginSample/samplingRate*1000.0 );
		if (mpProgressDialog->wasCanceled()) break;
	}
	reader.Stop();
 
}

void Annotator::generateEnvelopesFromDescriptors()
{
    unsigned i=0, editors_size = mBPFEditors.size();
    std::list<std::string>::iterator it;
    std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();

    for(it = descriptorsNames.begin();i < editors_size; i++, it++)
    {
	mBPFs[i]=generateEnvelopeFromDescriptor((*it));
    }
}
  
void Annotator::generateRandomEnvelopes()
{
    srand(time(NULL));
 
    for(unsigned i=0; i < mBPFEditors.size(); i++)
    {
	mBPFs[i]=generateRandomEnvelope();
    }
}

CLAM::BPF Annotator::generateRandomEnvelope()
{
    CLAM::BPF bpf;
    int audioSize=mCurrentAudio.GetSize();
    TData sr = mCurrentAudio.GetSampleRate();
    int i;
   
    int randomInt=(float (rand())/float(RAND_MAX))*100;
    int randomIncr;
    for(i=0;i<audioSize;i+=30000)
    {
	randomIncr = (float (rand())/float(RAND_MAX))*20-10;
	randomInt += randomIncr;
	if(randomInt>100) randomInt = 80;
	if(randomInt<0) randomInt=20;
     
	bpf.Insert(TData(i)/sr,TData(randomInt));
    }
    return bpf;
}


CLAM::BPF Annotator::generateEnvelopeFromDescriptor(const std::string& name)
{
    const CLAM::TData* values = mpDescriptorPool->GetReadPool<CLAM::TData>("Frame",name);
  
    int audioSize=mCurrentAudio.GetSize();
    TData sr = mCurrentAudio.GetSampleRate();
    int i,x;
  
    int nFrames = mpDescriptorPool->GetNumberOfContexts("Frame");
    int frameSize = audioSize/nFrames;

    CLAM::BPF bpf;

    for(i=0, x=0; i<nFrames ; x+=frameSize, i++)
    {
	bpf.Insert(TData(x)/sr,TData(values[i]));
    }
  
    return bpf;
}

void Annotator::languageChange()
{
    AnnotatorBase::languageChange();
    std::vector<QWidget*>::iterator it;
    std::list<std::string>::iterator it2;
    
    std::list<std::string>& names = mSchema.GetLLDSchema().GetLLDNames();
    for(it2 = names.begin() ,it = mTabPages.begin(); 
	it2 != names.end(); it++,it2++)
    {
      tabWidget2->changeTab( (*it), tr((*it2).c_str() ) );
      
    }
}

void Annotator::loadDescriptorPool()
{
 

  //Create Descriptors Pool Scheme and add attributes following loaded schema
  
  //First HLD's
  std::list<CLAM_Annotator::HLDSchemaElement>& hlds = mSchema.GetHLDSchema().GetHLDs();
  std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
  mDescriptionScheme = CLAM::DescriptionScheme();//we need to initialize everything
  for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
  {
    if((*it2).GetType()=="Float")
    {
      mDescriptionScheme.AddAttribute <float>("Song",(*it2).GetName());
    }
    else if((*it2).GetType()=="Int")
    {
      mDescriptionScheme.AddAttribute <int>("Song",(*it2).GetName());
    }
    else if((*it2).GetType()=="RestrictedString")
    {
      mDescriptionScheme.AddAttribute <CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
    }
    else
    {
      mDescriptionScheme.AddAttribute <std::string>("Song",(*it2).GetName());
    }
  }
  //And now LLD's  
  std::list<std::string>::iterator it;
  std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();
  for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
  {
    mDescriptionScheme.AddAttribute <CLAM::TData>("Frame", (*it));
  }
 
  //Create Descriptors Pool
  if(mpDescriptorPool) delete mpDescriptorPool;
  mpDescriptorPool = new CLAM::DescriptionDataPool(mDescriptionScheme);

   //Load Descriptors Pool
  if(mProject.GetDescriptorPool()!="")
    CLAM::XMLStorage::Restore((*mpDescriptorPool),mProject.GetDescriptorPool());
  
}

bool Annotator::event(QEvent* e)
{
    if(mpAudioPlot)
    {
	QKeyEvent* keyEvent=dynamic_cast<QKeyEvent*>(e);
	if(keyEvent)
	{
	    if(!mpAudioPlot->hasFocus())
	    {
		switch(keyEvent->key())
		{
		    case Qt::Key_Shift:
		    case Qt::Key_Insert:
		    case Qt::Key_Delete:
			mpAudioPlot->setFocus();
			break;
		    default:
			break;
		}
	    }
	}
    }
    return QWidget::event(e);
}


/*Functions to make HLDs edition table */
void Annotator::drawDescriptorsName()
{
  std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
  mDescriptorsTable->setNumRows(hlds.size());
  std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
  int i;
  for(it = hlds.begin(), i = 0 ; it != hlds.end(); it++, i++)
  {
    mDescriptorsTable->setItem(i,0,new TableItem(mDescriptorsTable,TableItem::Never, (*it).GetName()));
  }
}

//TODO: mSongDescriptors has to be replaced by a list of Pools
//TODO: Now I am operating as if I had only one song, have to have a map of pools??
void Annotator::drawHLD(int songIndex, const std::string& descriptorName, const std::string& value, 
			bool computed = true)
{
  QString qvalue = QString(value.c_str());
  if(!computed) qvalue = "?";
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
			     new TableItem(mDescriptorsTable,TableItem::WhenCurrent,qvalue));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, 
			const CLAM_Annotator::RestrictedString& value, bool computed = true)
{
  QString qvalue = QString(value.GetString().c_str());
  if(!computed) qvalue = "?";
  QStringList qrestrictionStrings;
  std::list<std::string> restrictionStrings;
  restrictionStrings = mSchema.GetHLDSchema().FindElement(descriptorName).GetRestrictionValues();
  std::list<std::string>::iterator it;
  for(it = restrictionStrings.begin();it != restrictionStrings.end(); it++)
  {
    qrestrictionStrings << (*it);
  }

  std::vector<QStringList> qrestrictionStringslist;
  qrestrictionStringslist.push_back( qrestrictionStrings );
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,new ComboTableItem(mDescriptorsTable,qrestrictionStringslist,false));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, float value, bool computed = true)
{
  /* TODO:RangeSelectionTableItem is harcoded between 0 and 10 and it only seems to work with ints */

  std::ostringstream s;
  s<<value;
  QString qvalue = QString(s.str().c_str());
  if(!computed) qvalue = "?";
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
			     new RangeSelectionTableItem(mDescriptorsTable,TableItem::WhenCurrent,qvalue));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, int value, bool computed = true)
{
  /* TODO:RangeSelectionTableItem is harcoded between 0 and 10 and it only seems to work with ints */

  std::ostringstream s;
  s<<value;
  QString qvalue = QString(s.str().c_str());
  if(!computed) qvalue = "?";
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
			     new RangeSelectionTableItem(mDescriptorsTable,TableItem::WhenCurrent,qvalue));

}

void Annotator::drawDescriptorsValue( int index, bool computed)
{
  std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
  std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
  for(it = hlds.begin() ; it != hlds.end(); it++)
  {
    if ((*it).GetType() == "String")
      {
	drawHLD(index,(*it).GetName(),*mpDescriptorPool->
		GetReadPool<std::string>("Song",(*it).GetName()),computed);
      }
    if ((*it).GetType() == "RestrictedString")
      {
	drawHLD(index,(*it).GetName(),*mpDescriptorPool->
		GetReadPool<CLAM_Annotator::RestrictedString>("Song",(*it).GetName()),computed);
      }
    if ((*it).GetType() == "Float")
      {
	drawHLD(index,(*it).GetName(),*mpDescriptorPool->
		GetReadPool<float>("Song",(*it).GetName()),computed);
      }
    if ((*it).GetType() == "Int")
      {
	drawHLD(index,(*it).GetName(),*mpDescriptorPool->
		GetReadPool<int>("Song",(*it).GetName()),computed);
      }

  }

}

void Annotator::fillGlobalDescriptors( int index)
{
  mDescriptorsTable->show();
  drawDescriptorsName();
  bool computed = mHaveHLDescriptors[index];
  drawDescriptorsValue( index, computed );
} 

int Annotator::findHLDescriptorIndex(const std::string& name)
{
  //TODO: should find a more efficient search algorithm

  int i;
  std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
  std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
  for(it = hlds.begin(), i = 0 ; it != hlds.end(); it++, i++)
  {
    if ((*it).GetName() == name) return i;
  }
  return -1;
}

double Annotator::GetMinY(const CLAM::BPF& bpf)
{
    double value=1E9;
    for(TIndex i=0; i < bpf.Size(); i++)
    {
	double current = double(bpf.GetValueFromIndex(i));
	if(current < value)
	{
	    value = current;
	}
    }
    value -= fabs(value)*0.1;
    return value;
}

double Annotator::GetMaxY(const CLAM::BPF& bpf)
{
    double value=-1E9;
    for(TIndex i=0; i < bpf.Size(); i++)
    {
	double current = double(bpf.GetValueFromIndex(i));
	if(current > value)
	{
	    value = current;
	}
    }
    value += fabs(value)*0.1;
    return value;
}


