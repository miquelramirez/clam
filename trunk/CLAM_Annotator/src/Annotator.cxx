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


#include <algorithm>
#include <iostream>

//xamat
#include <time.h>

#include "QtAudioPlot.hxx"
#include <qtabwidget.h>
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "IndexArray.hxx"


#include "XMLStorage.hxx"

#include "BPFEditor.hxx"

using CLAM::VM::QtAudioPlot;

using CLAM::TData;
using CLAM::TIndex;



Annotator::Annotator(const std::string & nameProject = ""):AnnotatorBase( 0, "annotator", 
									  WDestructiveClose),
							   mCurrentIndex(0),mpTabLayout(0),
							   mBPFEditors(0),mLLDChanged(false),
							   mHLDChanged(false),
							   mSegmentsChanged(false)
{
	mpDescriptorPool = NULL;
	mpAudioPlot = NULL;

	QString title = "Music annotator.- ";
	std::cerr << nameProject.c_str() << std::endl;
	title += QString( nameProject.c_str() );

	setCaption( title );

	//setCaption( QString("Music annotator.- ") + QString( nameProject.c_str() ) );
  initAudioWidget();
  initInterface();
}

void Annotator::initInterface()
{
  if (mpAudioPlot) mpAudioPlot->Hide();
  mProjectOverview->setSorting(-1);
  initFileMenu();
  makeConnections();
  
}

void Annotator::initProject()
{
  if (mProject.GetSongs()!="")
  {
    try{
      CLAM::XMLStorage::Restore(mSongFiles,mProject.GetSongs());
    }
    catch (CLAM::XmlStorageErr e)
    {
      QMessageBox::warning(this,"Error Loading Project File", 
			   constructFileError(mProject.GetSongs(),e));
      return;
    }
  }
  initSongs();
  
  if (mProject.GetSchema()!="")
  {
    try{
      CLAM::XMLStorage::Restore(mSchema,mProject.GetSchema());
    }
    catch (CLAM::XmlStorageErr e)
    {
      QMessageBox::warning(this,"Error Loading Schema File", 
			   constructFileError(mProject.GetSchema(),e));
      return;
    }
    
  }
  initLLDescriptorsWidgets();
  initHLDescriptorsTable();
  languageChange();
  
  mGlobalChanges = false;
  mLLDChanged = false;
  mHLDChanged = false;
  mSegmentsChanged = false;
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


void Annotator::fileMenuAboutToShow()
{
	fileSave_projectAction->setEnabled( mGlobalChanges );
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
  
  std::vector<CLAM::VM::BPFEditor*>::iterator it;
  mBPFEditors.resize(nTabs);
  for(i=0, it=mBPFEditors.begin();it!=mBPFEditors.end();it++,i++)
    {
      QVBoxLayout* tabLayout = new QVBoxLayout( tabWidget2->page(i));
      *it = new CLAM::VM::BPFEditor(tabWidget2->page(i),0,
				    CLAM::VM::AllowVerticalEdition | CLAM::VM::HasVerticalScroll | CLAM::VM::HasPlayer);
      (*it)->SetActivePlayer(false);
      (*it)->Hide();
      tabLayout->addWidget(*it);
    }
  connectBPFs();
}

void Annotator::initHLDescriptorsTable()
{
   makeDescriptorTable();
   drawDescriptorsName();
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


void Annotator::makeConnections()
{
  connect(helpAboutAction,SIGNAL(activated()),&mAbout,SLOT(show()));
  connect(mDescriptorsTable, SIGNAL(valueChanged( int, int) ) , this, 
	  SLOT( descriptorsTableChanged(int, int) ) );
  connect(mpAudioPlot, SIGNAL(updatedMark(int, unsigned)),this,
	  SLOT(segmentationMarksChanged(int, unsigned)));
  
}

void Annotator::connectBPFs()
{
    std::vector<CLAM::VM::BPFEditor*>::iterator it;
    for(it = mBPFEditors.begin(); it != mBPFEditors.end(); it++)
    {
      connect( (*it), SIGNAL(yValueChanged(int, float)), this, 
	      SLOT(descriptorsBPFChanged(int, float)));
      connect((*it),SIGNAL(selectedXPos(double)),mpAudioPlot,SLOT(setSelectedXPos(double)));
      connect(mpAudioPlot,SIGNAL(xRulerRange(double,double)),(*it),
	      SLOT(setHBounds(double,double)));
      connect(mpAudioPlot,SIGNAL(selectedXPos(double)),(*it),SLOT(selectPointFromXCoord(double)));
      connect(mpAudioPlot,SIGNAL(switchColorsRequested()),(*it),SLOT(switchColors()));
      connect(mpAudioPlot,SIGNAL(regionTime(float,float)),(*it),SLOT(setRegionTime(float,float)));
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


void Annotator::descriptorsTableChanged(int row, int column)
{
  mHLDChanged = true;
  //We first take the HLDSchema element where we will find all necessary info
  CLAM_Annotator::HLDSchemaElement hldSchemaElement;
  getHLDSchemaElementFromIndex(row, hldSchemaElement);

  std::string descriptorName = hldSchemaElement.GetName();
  std::string descriptorType = hldSchemaElement.GetType();
  std::string descriptorValue;
  descriptorValue = std::string( mDescriptorsTable->text(row, column).ascii() );
  setHLDescriptorPoolFromString(descriptorName, descriptorType, descriptorValue);
  changeCurrentFile();
}

void Annotator::descriptorsBPFChanged(int pointIndex,float newValue)
{
  /*TODO: right now, no matter how many points have been edited all descriptors are updated. This
    is not too smart/efficient but doing it otherwise would mean having a dynamic list of slots 
    in the class.*/
  mLLDChanged = true;
} 

void Annotator::segmentationMarksChanged(int, unsigned)
{
  std::vector<unsigned int> marks;
  CLAM::IndexArray* descriptorMarks = 
    mpDescriptorPool->GetWritePool<CLAM::IndexArray>("Song","Segments");
  marks = mpAudioPlot->GetMarks();
  int nMarks = marks.size();
  descriptorMarks->Resize(nMarks);
  descriptorMarks->SetSize(nMarks);
  int i;
  for (i=0; i<nMarks; i++)
    {
      (*descriptorMarks)[i] = marks[i];
    } 
  mSegmentsChanged = true;
  auralizeMarks();


}

void Annotator::addSongs()
{
  deleteAllSongsFromProject();
  std::vector< CLAM_Annotator::Song> songs = mSongFiles.GetFileNames();
  for ( std::vector<CLAM_Annotator::Song>::const_iterator it = songs.begin() ; it != songs.end() ; it++)
    {
      ListViewItem * item = new ListViewItem( mProjectOverview->childCount(), mProjectOverview, 
					      QString( it->GetSoundFile().c_str() ), tr("Yes"), 
					      tr("No") );
    }
}

void Annotator::closeEvent ( QCloseEvent * e ) 
{
    std::vector<CLAM::VM::BPFEditor*>::iterator it = mBPFEditors.begin();
    for(; it != mBPFEditors.end(); it++)
    {
	(*it)->stopPendingTasks();
    }

  if(mLLDChanged||mHLDChanged||mSegmentsChanged)
    {
      if(QMessageBox::question(this, "Descriptors Changed", 
			       "Do you want to save the changes to current descriptors?", 
			       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
	{
	  if (mLLDChanged) generateDescriptorsFromEnvelopes();
	  saveDescriptors();
	}
    }
  if ( mGlobalChanges )
    {
      if(QMessageBox::question(this, "Close project", 
			       "Do you want to save changes to the project?", 
			       QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
	saveDescriptors();
      
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
	for ( std::vector< QListViewItem* >::iterator it = toBeDeleted.begin() ; 
	      it != toBeDeleted.end() ; it++)
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
	for ( std::vector< QListViewItem* >::iterator it = toBeDeleted.begin() ; 
	      it != toBeDeleted.end() ; it++)
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
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.pro");
  if(qFileName != QString::null)
  {
    mProjectFileName = std::string(qFileName.ascii());
    try{
      CLAM::XMLStorage::Restore(mProject,mProjectFileName);
    }
    catch (CLAM::XmlStorageErr e)
    {
      
      QMessageBox::warning(this,"Error Loading Project File", 
			   constructFileError(mProjectFileName,e));
      return;
    }
    
    initInterface();
    initProject();
  }
}

void Annotator::fileNew()
{
  mProjectFileName = "";
  mProject.SetSongs("");
  mProject.SetSchema("");
  mSongFiles.GetFileNames().resize(0);
  mSchema.GetLLDSchema().GetLLDNames().resize(0);
  mSchema.GetHLDSchema().GetHLDs().resize(0);
  initInterface();
  initProject();
  mGlobalChanges = true;
}

void Annotator::fileSave()
{
  mGlobalChanges = false;
  markAllNoChanges();
  if(mProjectFileName=="") fileSaveAs();
  else
    {
      CLAM::XMLStorage::Dump(mProject,"Project",mProjectFileName);
      std::string songFile = mProject.GetSongs();
      if(songFile == "")
	songFile = mProjectFileName+".sl";
      CLAM::XMLStorage::Dump(mSongFiles, "Songs", songFile);
    }

}

void Annotator::fileSaveAs()
{
  QString qFileName = QFileDialog::getSaveFileName(QString::null,"*.pro");
  if(qFileName != QString::null)
  {
    mProjectFileName = std::string(qFileName.ascii());
    fileSave();
  }
}
void  Annotator::loadSongList()
{
  QString qFileName;
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.sl");
  if(qFileName != QString::null)
  {
    mProject.SetSongs(std::string(qFileName.ascii()));
    try{
      CLAM::XMLStorage::Restore(mSongFiles,mProject.GetSongs());
    }
    catch (CLAM::XmlStorageErr e)
    {
      QMessageBox::warning(this,"Error Loading Songs List File", 
			   constructFileError(mProject.GetSongs(),e));
      return;
    }
    
    //TODO: Does loading the song list affect all this
    initInterface();
    initProject();
  }
}

void  Annotator::loadSchema()
{
  QString qFileName;
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.sc");
  if(qFileName != QString::null)
  {
    mProject.SetSchema(std::string(qFileName.ascii()));
    try{
      CLAM::XMLStorage::Restore(mSchema,mProject.GetSchema());
    }
    catch (CLAM::XmlStorageErr e)
    {
      QMessageBox::warning(this,"Error Loading Schema File", 
			   constructFileError(mProject.GetSchema(),e));
      return;
    }
    
    //TODO: Does loading the schema affect all this
    initInterface();
    initProject();
  }
}

void  Annotator::loadDescriptors()
{
  QString qFileName;
  qFileName = QFileDialog::getOpenFileName(QString::null,"*.pool");
  if(qFileName != QString::null)
  {
    mCurrentDescriptorsPoolFileName = (std::string(qFileName.ascii()));
    try{
      CLAM::XMLStorage::Restore(*mpDescriptorPool,mCurrentDescriptorsPoolFileName);
    }
    catch (CLAM::XmlStorageErr e)
    {
      QMessageBox::warning(this,"Error Loading Descriptors File", 
			   constructFileError(mCurrentDescriptorsPoolFileName,e));
      return;
    }
    //TODO: Does loading the descriptors affect all this
    initInterface();
    initProject();
  }

}

void  Annotator::saveSongList()
{
  QString qFileName;
  qFileName = QFileDialog::getSaveFileName(QString(mProject.GetSongs().c_str()),"*.sl");
  if(qFileName != QString::null)
  {
    mProject.SetSongs(std::string(qFileName.ascii()));
    CLAM::XMLStorage::Dump(mSongFiles,"Songs",mProject.GetSongs());
  }

}


void  Annotator::saveDescriptors()
{
  if(QMessageBox::question(this,QString("Save Descriptors"),
			      QString("Do you want to save current song's descriptors?"),
			      QString("OK"),QString("Cancel")) == 0)
    {
      QString qFileName;
      qFileName = QFileDialog::getSaveFileName(QString(mCurrentDescriptorsPoolFileName.c_str()),
					       "*.pool");
      if(qFileName != QString::null)
	{
	  mCurrentDescriptorsPoolFileName = (std::string(qFileName.ascii()));
	  CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",mCurrentDescriptorsPoolFileName);
	  mLLDChanged = false;
	  mHLDChanged = false;
	  mSegmentsChanged = false;

	}
    }

}


void Annotator::initSongs()
{
	mLogicGroup->hide();
	addSongs();	
}

void Annotator::chooseColor()
{
	QColorDialog::getColor();
}

void Annotator::songsClicked( QListViewItem * item)
{
  /* before doing anything with the new selected file we must update information for previously
     selected song */
  if(mLLDChanged||mHLDChanged||mSegmentsChanged) 
    {
      if(mLLDChanged) 
	generateDescriptorsFromEnvelopes();
      saveDescriptors();
    }

  if (item != 0)
    {
      mCurrentIndex = getIndexFromFileName(std::string(item->text(0).ascii()));
      if (mCurrentIndex >=0)
	{
	  mCurrentSoundFileName = mSongFiles.GetFileNames()[mCurrentIndex].GetSoundFile();
	  if(mSongFiles.GetFileNames()[mCurrentIndex].HasPoolFile())
	    {
	      mCurrentDescriptorsPoolFileName = 
		mSongFiles.GetFileNames()[mCurrentIndex].GetPoolFile();
	    }
	  else 
	    mCurrentDescriptorsPoolFileName = mCurrentSoundFileName + ".pool";
	  CLAM::AudioFile file;
	  file.OpenExisting(mCurrentSoundFileName);
	  mpProgressDialog = new QProgressDialog ("Loading Audio", 
						  "Cancel",file.GetHeader().GetLength(),
						  this);
	  mpProgressDialog->setProgress(0);
	  loadDescriptorPool();
	  fillGlobalDescriptors( mCurrentIndex );
	  drawAudio(item);
	  drawLLDescriptors(mCurrentIndex);
	  
	  delete mpProgressDialog;
	  mpProgressDialog = NULL;
	}
      
    }
}

void Annotator::drawAudio(QListViewItem * item=NULL)
{
        mpAudioPlot->Hide();
	if(item)        
	  loadAudioFile(item->text(0).ascii());
	std::vector<unsigned> marks;
	const CLAM::IndexArray* descriptorsMarks = 
	  mpDescriptorPool->GetReadPool<CLAM::IndexArray>("Song","Segments");
	int i;
	int nMarks = descriptorsMarks->Size();
	for(i=0;i<nMarks;i++)
	  {
	    marks.push_back((*descriptorsMarks)[i]);
	  }
	mpAudioPlot->SetMarks(marks);
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->Show();
	auralizeMarks();
}

void Annotator::drawLLDescriptors(int index)
{
    generateEnvelopesFromDescriptors();
   
    std::vector<CLAM::VM::BPFEditor*>::iterator editors_it = mBPFEditors.begin();
    for(int i=0; editors_it != mBPFEditors.end(); editors_it++, i++)
    {
	CLAM::EScale scale;
	TData min_y = GetMinY((*editors_it)->GetData());
	TData max_y = GetMaxY((*editors_it)->GetData());
	bool scale_log = (fabs(min_y) > 9999.99 || fabs(max_y) > 9999.99);
	scale = (scale_log) ? CLAM::EScale::eLog : CLAM::EScale::eLinear;
	(*editors_it)->SetXRange(0.0,double(mCurrentAudio.GetDuration())/1000.0);
	(*editors_it)->SetYRange(min_y,max_y,scale);
	(*editors_it)->Geometry(0,0,tabWidget2->page(i)->width(),tabWidget2->page(i)->height());
	(*editors_it)->Show();
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
    std::vector<CLAM::VM::BPFEditor*>::iterator ed_it = mBPFEditors.begin();
    std::list<std::string>::iterator it;
    std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();

    for(it = descriptorsNames.begin();it != descriptorsNames.end(); ed_it++, it++)
    {
	(*ed_it)->SetData( generateEnvelopeFromDescriptor((*it)) );
    }

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

void Annotator::generateDescriptorsFromEnvelopes()
{
  mLLDChanged = false;
  unsigned i=0, editors_size = mBPFEditors.size();
  std::list<std::string>::iterator it;
  std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();
  
  for(it = descriptorsNames.begin() ;i < editors_size; i++, it++)
    {
      generateDescriptorFromEnvelope(i, mpDescriptorPool->GetWritePool<float>("Frame",(*it)));
    }
}

void Annotator::generateDescriptorFromEnvelope(int bpfIndex, float* descriptor)
{
  int nPoints = mBPFEditors[bpfIndex]->GetData().Size();
  int i = 0;
  for (i=0; i<nPoints; i++)
    {
      descriptor[i] = mBPFEditors[bpfIndex]->GetData().GetValueFromIndex(i);
    }
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
  mLLDChanged = false;
  mHLDChanged = false;
  mSegmentsChanged = false;

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
 
  //Finally segmentation marks
  mDescriptionScheme.AddAttribute<CLAM::IndexArray>("Song","Segments");

  //Create Descriptors Pool
  if(mpDescriptorPool) delete mpDescriptorPool;
  mpDescriptorPool = new CLAM::DescriptionDataPool(mDescriptionScheme);

   //Load Descriptors Pool
  if(mCurrentDescriptorsPoolFileName!="")
  {
    try{
      CLAM::XMLStorage::Restore((*mpDescriptorPool),mCurrentDescriptorsPoolFileName);
    }
    catch (CLAM::XmlStorageErr e)
    {
      QMessageBox::warning(this,"Error Loading Descriptors Pool File", 
			   constructFileError(mCurrentDescriptorsPoolFileName,e));
      return;
    }
  }
    
  
}

bool Annotator::event(QEvent* e)
{
    if(mpAudioPlot)
    {
		std::cerr << typeid(e).name() << std::endl;
	QKeyEvent* keyEvent=(QKeyEvent*)(e);
	if(keyEvent)
	{
	    if(!mpAudioPlot) return false;
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
    mDescriptorsTable->setItem(i,0,new TableItem(mDescriptorsTable,TableItem::Never, QString((*it).GetName().c_str())));
  }
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, const std::string& value, 
			bool computed)
{
  QString qvalue = QString(value.c_str());
  if(!computed) qvalue = "?";
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
			     new TableItem(mDescriptorsTable,TableItem::WhenCurrent,qvalue));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, 
			const CLAM_Annotator::RestrictedString& value, bool computed)
{
  QString qvalue = QString(value.GetString().c_str());
  if(!computed) qvalue = "?";
  QStringList qrestrictionStrings;
  std::list<std::string> restrictionStrings;
  restrictionStrings = mSchema.GetHLDSchema().FindElement(descriptorName).GetRestrictionValues();
  std::list<std::string>::iterator it;
  for(it = restrictionStrings.begin();it != restrictionStrings.end(); it++)
  {
    qrestrictionStrings << QString((*it).c_str());
  }

  std::vector<QStringList> qrestrictionStringslist;
  qrestrictionStringslist.push_back( qrestrictionStrings );
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
			     new ComboTableItem(mDescriptorsTable,qrestrictionStringslist,false));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, float value, Range<float> range, bool computed)
{
  std::ostringstream s;
  s<<value;
  QString qvalue = QString(s.str().c_str());
  if(!computed) qvalue = "?";
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
			     new RangeSelectionTableItem(mDescriptorsTable,
							 TableItem::WhenCurrent,qvalue,range));
}

void Annotator::drawHLD(int songIndex, const std::string& descriptorName, int value, Range<int> range, bool computed)
{
  std::ostringstream s;
  s<<value;
  QString qvalue = QString(s.str().c_str());
  if(!computed) qvalue = "?";
  mDescriptorsTable->setItem(findHLDescriptorIndex(descriptorName),1,
			     new RangeSelectionTableItem(mDescriptorsTable,
							 TableItem::WhenCurrent,qvalue,range));

}

void Annotator::drawDescriptorsValue( int index, bool computed = true)
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
		GetReadPool<float>("Song",(*it).GetName()),(*it).GetfRange(),computed);
      }
    if ((*it).GetType() == "Int")
      {
	drawHLD(index,(*it).GetName(),*mpDescriptorPool->
		GetReadPool<int>("Song",(*it).GetName()),(*it).GetiRange(),computed);
      }

  }

}

void Annotator::setHLDescriptorPoolFromString(const std::string& descriptorName, 
					      const std::string& descriptorType,
					      const std::string& descriptorValue)
{
  QString qValue(descriptorValue.c_str());
  if (descriptorType == "String")
    {
      *(mpDescriptorPool->GetWritePool<std::string>("Song",descriptorName)) = descriptorValue;
    }
  if (descriptorType == "RestrictedString")
    {
      CLAM_Annotator::RestrictedString* rString = mpDescriptorPool->
	GetWritePool<CLAM_Annotator::RestrictedString>("Song",descriptorName);
      rString->SetString(descriptorValue);
    }
  if (descriptorType == "Float")
    {
      *(mpDescriptorPool->GetWritePool<float>("Song",descriptorName)) = qValue.toFloat();
    }
  if (descriptorType == "Int")
    {
      *(mpDescriptorPool->GetWritePool<int>("Song",descriptorName)) = qValue.toInt();
    }
}

void Annotator::fillGlobalDescriptors( int index)
{
  mDescriptorsTable->show();
  drawDescriptorsValue(index);
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

std::string Annotator::getHLDescriptorNameFromIndex(int index)
{
  CLAM_Annotator::HLDSchemaElement hldSchemaElement;
  getHLDSchemaElementFromIndex(index, hldSchemaElement);
  return hldSchemaElement.GetName();
}

void Annotator::getHLDSchemaElementFromIndex(int index, CLAM_Annotator::HLDSchemaElement& element)
{
  /* TODO: This is not very efficient, wouldn't it be better to have HLDs as a vector instead of
     a list? */
  std::list<CLAM_Annotator::HLDSchemaElement> hlds = mSchema.GetHLDSchema().GetHLDs();
  std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
  int i=0;
  for(it = hlds.begin(), i = 0 ; it != hlds.end(); it++, i++)
  {
    if (i==index) {
      element = (*it);
      return;
    }
  }
  CLAM_ASSERT(false, "Index does not exist for HLDSchema");;

}

double Annotator::GetMinY(const CLAM::BPF& bpf)
{
    double min_value=1E9;
    double max_value=-1E9;
    for(TIndex i=0; i < bpf.Size(); i++)
    {
	double current = double(bpf.GetValueFromIndex(i));
	if(current > max_value)
	{
	    max_value = current;
	}
	else if(current < min_value)
	{
	    min_value = current;
	}
    }
    double span = max_value-min_value;
    min_value -= span*0.1;
    return min_value;
}

double Annotator::GetMaxY(const CLAM::BPF& bpf)
{
    double min_value=1E9;
    double max_value=-1E9;
    for(TIndex i=0; i < bpf.Size(); i++)
    {
	double current = double(bpf.GetValueFromIndex(i));
	if(current > max_value)
	{
	    max_value = current;
	}
	else if(current < min_value)
	{
	    min_value = current;
	}
    }
    double span = max_value-min_value;
    max_value += span*0.1;
    return max_value;
}


int Annotator::getIndexFromFileName(const std::string& fileName)
{
  //TODO: have to optimize these tasks maybe by using a map or at least std::find
  std::vector<CLAM_Annotator::Song>::iterator it;
  int i=0;
  std::vector<CLAM_Annotator::Song> fileNames = mSongFiles.GetFileNames();
  for (it = fileNames.begin(), i=0 ; it != fileNames.end(); it++, i++)
    {
      if((*it).GetSoundFile() == fileName) return i;
    }
  return -1;
}

void Annotator::auralizeMarks()
{
  if(mClick.size()==0)
  {
    mClick.resize(2);
    CLAM::AudioFile file;
    file.OpenExisting("click.mp3");
    int nChannels = file.GetHeader().GetChannels();
    CLAM::MultiChannelAudioFileReaderConfig cfg;
    cfg.SetSourceFile( file );
    CLAM::MultiChannelAudioFileReader reader(cfg);
    mClick[0].SetSize(5000);
    mClick[1].SetSize(5000);
    reader.Start();
    int beginSample=0;
    reader.Do(mClick);
    reader.Stop();
  }
  std::vector<unsigned int> marks;
  marks = mpAudioPlot->GetMarks();
  int nMarks = marks.size();
  int i;
  mCurrentMarkedAudio = mCurrentAudio;
  int size = mCurrentMarkedAudio.GetSize();
  for (i=0; i<nMarks; i++)
  {
    if(marks[i]<size)
      mCurrentMarkedAudio.SetAudioChunk((int)marks[i],mClick[0]);
  } 

}

void Annotator::playMarks(bool play)
{
  if(play)
    mpAudioPlot->SetData(mCurrentMarkedAudio);
  else
    mpAudioPlot->SetData(mCurrentAudio);
}

QString Annotator::constructFileError(const std::string& fileName,const CLAM::XmlStorageErr& e)
{
  std::string errorMessage = "XML Error:";
  errorMessage += e.what();
  errorMessage += "\n";
  errorMessage += "Check that your file ";
  errorMessage += mProjectFileName;
  errorMessage += "\n";
  errorMessage += "is well formed and folllows the specifications";
  return QString(errorMessage.c_str());
}
