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

#include "AnalyzeWindow.hxx"
#include "OpenProjectDialog.hxx"
#include "NewProjectDialog.hxx"
#include "AddSongsToProjectDialog.hxx"
#include "SaveProjectAsDialog.hxx"


#include <algorithm>

//xamat
#include <time.h>

#include "QtAudioPlot.hxx"
#include <qtabwidget.h>
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "envelope_point_editor.hxx"
#include "DataFacade.hxx"
#include "TXTSongParser.hxx"


#include "LLDSchema.hxx"
#include "XMLStorage.hxx"

using CLAM::VM::QtAudioPlot;

Annotator::Annotator( const std::string & nameProject, const AnnotatorDataFacade::StringList & files , AnnotatorDataFacade & data, QWidget * parent, const char * name, WFlags f) : AnnotatorBase( parent, name, f),mEnvelopes(0),mFunctionEditors(0),mCurrentIndex(0),mpTabLayout(0)
{
  //I should try to get rid of this constructor and pass things to the new one(see below)	
  setCaption( QString("Music annotator.- ") + QString( nameProject.c_str() ) );
  initProject();
	
}

Annotator::Annotator():AnnotatorBase( 0, "annotator", WDestructiveClose),mEnvelopes(0),mFunctionEditors(0),mCurrentIndex(0),mpTabLayout(0)
{
  initDataFacade();
}

void Annotator::initProject()
{
  mpAudioPlot = NULL;
  mProjectOverview->setSorting(-1);
  initView();
  initFileMenu();
  initEditMenu();
  makeDescriptorTable();
  makeConnections();
  initDataFacade();
  initSongs(name(), mSongFiles.GetFileNames());
  initLLDescriptorsWidgets();
  initAudioWidget();
  initEnvelopes();
  languageChange();
  mChanges = false;
}

void Annotator::initDataFacade()
{
  DataFacade data;
  TXTSongParser songParser( "DataTest/", data );
  mpData = new AnnotatorDataFacade( data );

  //this should go elsewhere
  CLAM::XMLStorage::Restore(mSongFiles,"Songs.xml");
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

void Annotator::createListOfGenres( QStringList & list, const QString & value) const
{
	AnnotatorDataFacade::StringList genres;
	mpData->getGenres( genres );
	AnnotatorDataFacade::StringList::iterator itValue = std::find( genres.begin(), genres.end(), value);
	AnnotatorDataFacade::StringList::iterator itEnd = genres.end();
	AnnotatorDataFacade::StringList::iterator itBegin = genres.begin();
	if ( itValue == itEnd )
	{
		list<<"?";
	}
	for ( ; itValue != itEnd ; itValue++ )
	{
		list<<*itValue;
	}
	itValue = std::find( genres.begin(), genres.end(), value);
	for ( ; itBegin != itValue ; itBegin++ )
	{
		list<<*itBegin;
	}
}

void Annotator::createListOfTonalKeys( QStringList & list, const QString & value ) const
{
	static const std::string keys[13] = {"?","A","A#","B","C","C#","D","D#","E","F","F#","G","G#"};
	int i;
	for ( i = 0; i < 13 && keys[i] != std::string( value.ascii() ) ; i++)
	{
	}
	for ( int j = i ; j<13 ; j++ )
	{
		list<<QString( keys[j].c_str() );
	}
	for ( int j = 0 ; j<i ; j++)
	{
		list<<QString( keys[j].c_str() );
	}
}



void Annotator::initAudioWidget()
{
        mpAudioPlot = NULL;
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
	mpAudioPlot->Show();
}

void Annotator::initLLDescriptorsWidgets()
{
  

  LoadDescriptorPool();

  int nTabs = mLLDSchema.GetLLDNames().size();
        
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
	  else (*it0)=tabWidget2->page(0);
	}

	std::vector<CLAM::Envelope_Point_Editor*>::iterator it;
	std::vector<CLAM::Envelope*>::iterator it2;
	i=0;
	mFunctionEditors.resize(nTabs);
	mEnvelopes.resize(nTabs);
	for(it=mFunctionEditors.begin();it!=mFunctionEditors.end();it++,i++)
	{
	  *it = new CLAM::Envelope_Point_Editor(tabWidget2->page(i));
	  (*it)->setTimeFactor((float)1.0/44100.0);//TODO: should be sampling rate
	  QVBoxLayout* tabLayout = new QVBoxLayout( tabWidget2->page(i));
	  tabLayout->addWidget(*it);
	}

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

void Annotator::descriptorsTableChanged(int row, int column)
{
	mChanges = true;
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
}

void Annotator::addSongs( const AnnotatorDataFacade::StringList & list)
{
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

void Annotator::fileSaveAs()
{
	SaveProjectAsDialog * dialog = new SaveProjectAsDialog(this, "Save as dialog", WDestructiveClose);
	connect(dialog, SIGNAL(nameToBeSaveEmited(const QString &) ), this, SLOT(setCaption(const QString & ) ) );
	dialog->show();
	fileSave();
}

void Annotator::markAllNoChanges()
{
	QListViewItemIterator it (mProjectOverview);
	for ( ; it.current() ; ++it )
	{
		it.current()->setText(2, "No");
	}
}

void Annotator::fileSave()
{
	mChanges = false;
	markAllNoChanges();
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
	AddSongsToProjectDialog * dialog = new AddSongsToProjectDialog( *mpData, this, "add songs to project", WDestructiveClose);
	dialog->show();
}

void Annotator::fileOpen()
{
	OpenProjectDialog * dialog = new OpenProjectDialog(this, *mpData, 0, "open project dialog", WDestructiveClose );
	dialog->show();
}

void Annotator::fileNew()
{
	NewProjectDialog * dialog = new NewProjectDialog( *mpData, 0, "new dialog", WDestructiveClose );
	dialog->show();
}

void Annotator::initSongs( const std::string & nameProject, const std::vector<std::string> & files)
{
	mLogicGroup->hide();
	addSongs( files );	
}

void Annotator::drawDescriptorsName()
{
	mDescriptorsTable->setNumRows(8);
	mDescriptorsTable->setItem(0,0,new TableItem(mDescriptorsTable,TableItem::Never,"Artists"));    
	mDescriptorsTable->setItem(1,0,new TableItem(mDescriptorsTable,TableItem::Never,"Title"));
	mDescriptorsTable->setItem(2,0,new TableItem(mDescriptorsTable,TableItem::Never,"Genre"));
	mDescriptorsTable->setItem(3,0,new TableItem(mDescriptorsTable,TableItem::Never,"Danceability"));
	mDescriptorsTable->setItem(4,0,new TableItem(mDescriptorsTable,TableItem::Never,"Tonality key") );
	mDescriptorsTable->setItem(5,0,new TableItem(mDescriptorsTable,TableItem::Never,"Tonality mode") );
	mDescriptorsTable->setItem(6,0,new TableItem(mDescriptorsTable,TableItem::Never,"Dynamic complexity") );
	mDescriptorsTable->setItem(7,0,new TableItem(mDescriptorsTable,TableItem::Never,"Beats per minute") );
}

void Annotator::drawArtists( int index, bool computed = true )
{
	QString value = mSongDescriptors[index]["Artist"];
	if(!computed) value = "?";
	mDescriptorsTable->setItem(0,1,new TableItem(mDescriptorsTable,TableItem::WhenCurrent,value));
}

void Annotator::drawTitle( int index, bool computed = true )
{
	QString value = QString( mSongDescriptors[index]["Title"].c_str() );
	if(!computed) value = "?";
	mDescriptorsTable->setItem(1,1,new TableItem(mDescriptorsTable,TableItem::WhenCurrent,value));    

}

void Annotator::drawGenre( int index, bool computed = true )
{
	QString value = mpData->genreWithoutSinonims( mSongDescriptors[index]["genre"] );
	if(!computed) value = "?";
	QStringList listOfGenres;
	createListOfGenres( listOfGenres, value);
	std::vector<QStringList> listOfGenreslist;
	listOfGenreslist.push_back( listOfGenres );
	mDescriptorsTable->setItem(2,1,new ComboTableItem(mDescriptorsTable,listOfGenreslist,false)); 
	
}

void Annotator::drawDynamicComplexity( int index, bool computed = true )
{
	QString value = mSongDescriptors[index]["Dynamic Complexity"];
	if(!computed) value = "?";
	double valueInDouble = value.toDouble();
	valueInDouble = mpData->normalizeDynamicComplexity( valueInDouble ) *10.0;
	int valueInInt = int(valueInDouble);
	value.setNum( valueInInt );
	mDescriptorsTable->setItem(6,1,new RangeSelectionTableItem(mDescriptorsTable,TableItem::WhenCurrent, value ) );
	
}

void Annotator::drawDanceability( int index, bool computed = true )
{
	QString value = mSongDescriptors[index]["Danceability"];
	if(!computed) value = "?";
	double valueInDouble = value.toDouble() ;
	valueInDouble = mpData->normalizeDanceability( valueInDouble )*10.0;
	int valueInInt = int(valueInDouble);
	value.setNum( valueInInt );
	mDescriptorsTable->setItem(3,1,new RangeSelectionTableItem(mDescriptorsTable,TableItem::WhenCurrent, value ) );
	
}

void Annotator::drawBPM( int index, bool computed = true )
{
	QString value = mSongDescriptors[index]["BPM"];
	if(!computed) value = "?";
	mDescriptorsTable->setItem(7,1, new TableItem(mDescriptorsTable,TableItem::WhenCurrent,value) );
}

void Annotator::drawTonality( int index, bool computed = true )
{
	std::vector<QStringList> listOfKeysList;
	QStringList listOfKeys;
	QString key = QString( mSongDescriptors[index]["Tonal Descriptor: Key Note"].c_str() );
	if(!computed) key = "?";
	createListOfTonalKeys( listOfKeys, key );
	listOfKeysList.push_back( listOfKeys );
	mDescriptorsTable->setItem(4,1,new ComboTableItem( mDescriptorsTable, listOfKeysList, false ) );
	
	std::vector<QStringList> listOfModesList;
	QStringList listOfModes;
	QString value = mSongDescriptors[index]["Tonal Descriptor: Mode"];
	if(!computed) value = "?";
	if ( value == "Major" )
		listOfModes<<"Major"<<"Minor";
	else if ( value == "Minor" )
		listOfModes<<"Minor"<<"Major";
	else
		listOfModes<<"?"<<"Major"<<"Minor";
	listOfModesList.push_back( listOfModes );
	mDescriptorsTable->setItem(5,1,new ComboTableItem( mDescriptorsTable, listOfModesList, false ) );
}

void Annotator::drawDescriptorsValue( int index, bool computed)
{
	drawArtists( index );
	drawTitle( index );
	drawGenre( index, computed );
	drawDanceability( index, computed );
	drawTonality( index, computed );
	drawDynamicComplexity( index, computed );
	drawBPM( index, computed );
}

void Annotator::fillGlobalDescriptors( int index)
{
   mDescriptorsTable->show();
  drawDescriptorsName();
  bool computed = mHaveHLDescriptors[index];
  drawDescriptorsValue( index, computed );
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
  std::vector<CLAM::Envelope_Point_Editor*>::iterator it;
  std::vector<CLAM::Envelope*>::iterator it2;
 
  int i;

  //generateRandomEnvelopes();
  generateEnvelopesFromDescriptors();

  for(it=mFunctionEditors.begin(),it2=mEnvelopes.begin();it!=mFunctionEditors.end();it++,it2++)
    {
     if(mHaveLLDescriptors[index])
	{
	  (*it)->set_envelope(*it2);
	  (*it)->show();
	}
      else (*it)->hide();
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

void Annotator::initEnvelopes()
{
  std::vector<CLAM::Envelope*>::iterator it;
  for(it=mEnvelopes.begin();it!=mEnvelopes.end();it++)
  {
    (*it)=NULL;
  }
}

void Annotator::generateEnvelopesFromDescriptors()
{
  std::vector<CLAM::Envelope*>::iterator it;
  std::list<std::string>::iterator it2;
  std::list<std::string>& descriptorsNames = mLLDSchema.GetLLDNames();

  for(it=mEnvelopes.begin(), it2 = descriptorsNames.begin();it!=mEnvelopes.end();it++, it2++)
  {
      if(*it)
      {
	delete (*it);
	(*it)=NULL;
      }
      (*it)=generateEnvelopeFromDescriptor((*it2));
  }
}
  
void Annotator::generateRandomEnvelopes()
{
  std::vector<CLAM::Envelope*>::iterator it;
  
  srand(time(NULL));
 
  for(it=mEnvelopes.begin();it!=mEnvelopes.end();it++)
  {
      if(*it)
      {
	delete (*it);
	(*it)=NULL;
      }
      (*it)=generateRandomEnvelope();
  }
}

CLAM::Envelope* Annotator::generateRandomEnvelope()
{
  CLAM::Envelope* tmpEnvelope = new CLAM::Envelope();
  int audioSize=mCurrentAudio.GetSize();
  int i;
  tmpEnvelope->set_maxY_value(100);
  tmpEnvelope->set_minY_value(0);
  tmpEnvelope->set_maxX_value(audioSize);
  tmpEnvelope->set_minX_value(0);
  tmpEnvelope->set_max_nodes(-1);
   
  int randomInt=(float (rand())/float(RAND_MAX))*100;
  int randomIncr;
  for(i=0;i<audioSize;i+=30000)
  {
     randomIncr = (float (rand())/float(RAND_MAX))*20-10;
     randomInt += randomIncr;
     if(randomInt>100) randomInt = 80;
     if(randomInt<0) randomInt=20;
     
     tmpEnvelope->add_node_at_offset(i,randomInt);
  }
  return tmpEnvelope;
}


CLAM::Envelope* Annotator::generateEnvelopeFromDescriptor(const std::string& name)
{
  const CLAM::TData* values = mpDescriptorPool->GetReadPool<CLAM::TData>("Frame",name);
  CLAM::Envelope* tmpEnvelope = new CLAM::Envelope();

  int audioSize=mCurrentAudio.GetSize();
  int i,x;
  
  int nFrames = mpDescriptorPool->GetNumberOfContexts("Frame");
  int frameSize = audioSize/nFrames;

  tmpEnvelope->set_maxY_value(100);
  tmpEnvelope->set_minY_value(0);
  tmpEnvelope->set_maxX_value(audioSize);
  tmpEnvelope->set_minX_value(0);
  tmpEnvelope->set_max_nodes(-1);
   
  for(i=0, x=0; i<nFrames ; x+=frameSize, i++)
  {
    int value;
    //we must make sure that the limits of the envelope are not surpassed
    //TODO: we should have dynamic limits!
    if(values[i]>100) value = 100;
    else if(values[i]<0) value = 0;
    else value = values[i];

    tmpEnvelope->add_node_at_offset(x,value);

  }
  return tmpEnvelope;
}

void Annotator::languageChange()
{
    AnnotatorBase::languageChange();
    std::vector<QWidget*>::iterator it;
    std::list<std::string>::iterator it2;
    
    std::list<std::string>& names = mLLDSchema.GetLLDNames();
    for(it2 = names.begin() ,it = mTabPages.begin(); 
	it2 != names.end(); it++,it2++)
    {
      tabWidget2->changeTab( (*it), tr((*it2).c_str() ) );
      
    }
}

void Annotator::LoadDescriptorPool()
{
  //TODO: The user should select this schema
  CLAM::XMLStorage::Restore(mLLDSchema,"LLDSchema.xml");
  //Create Descriptors Pool Scheme and add attributes following loaded LLD schema
  
  std::list<std::string>::iterator it;
  std::list<std::string>& descriptorsNames = mLLDSchema.GetLLDNames();
  for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
  {
    mDescriptionScheme.AddAttribute <CLAM::TData>("Frame", (*it));
  }
 
  //Create Descriptors Pool
  mpDescriptorPool = new CLAM::DescriptionDataPool(mDescriptionScheme);

  //Load Descriptors Pool
  CLAM::XMLStorage::Restore((*mpDescriptorPool),"DescriptorsPool.xml");
  
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

