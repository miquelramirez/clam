/****************************************************************************
** Form implementation generated from reading ui file '../src/AnnotatorBase.ui'
**
** Created: Tue Mar 15 09:18:23 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "AnnotatorBase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qheader.h>
#include <qlistview.h>
#include <qframe.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "Table.hxx"
#include "../../src/AnnotatorBase.ui.h"
/*
 *  Constructs a AnnotatorBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
AnnotatorBase::AnnotatorBase( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "AnnotatorBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMaximumSize( QSize( 32767, 400 ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );
    AnnotatorBaseLayout = new QVBoxLayout( centralWidget(), 11, 6, "AnnotatorBaseLayout"); 

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    mProjectOverview = new QListView( centralWidget(), "mProjectOverview" );
    mProjectOverview->addColumn( tr( "File" ) );
    mProjectOverview->header()->setClickEnabled( FALSE, mProjectOverview->header()->count() - 1 );
    mProjectOverview->addColumn( tr( "Calculated" ) );
    mProjectOverview->addColumn( tr( "Changed" ) );
    mProjectOverview->setMinimumSize( QSize( 0, 225 ) );
    mProjectOverview->setSelectionMode( QListView::Single );
    mProjectOverview->setRootIsDecorated( TRUE );
    layout6->addWidget( mProjectOverview );

    frame3 = new QFrame( centralWidget(), "frame3" );
    frame3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, frame3->sizePolicy().hasHeightForWidth() ) );
    frame3->setMinimumSize( QSize( 700, 225 ) );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );
    layout6->addWidget( frame3 );
    AnnotatorBaseLayout->addLayout( layout6 );

    layout5 = new QHBoxLayout( 0, 0, 6, "layout5"); 

    mDescriptorsTable = new Table( centralWidget(), "mDescriptorsTable" );
    mDescriptorsTable->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, mDescriptorsTable->sizePolicy().hasHeightForWidth() ) );
    mDescriptorsTable->setMinimumSize( QSize( 0, 250 ) );
    layout5->addWidget( mDescriptorsTable );

    tabWidget2 = new QTabWidget( centralWidget(), "tabWidget2" );
    tabWidget2->setMinimumSize( QSize( 700, 250 ) );

    tab = new QWidget( tabWidget2, "tab" );
    tabWidget2->insertTab( tab, QString("") );
    layout5->addWidget( tabWidget2 );
    AnnotatorBaseLayout->addLayout( layout5 );

    mLogicGroup = new QGroupBox( centralWidget(), "mLogicGroup" );
    mLogicGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, mLogicGroup->sizePolicy().hasHeightForWidth() ) );
    AnnotatorBaseLayout->addWidget( mLogicGroup );

    // actions
    fileExitAction = new QAction( this, "fileExitAction" );
    fileExitAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    helpAboutAction = new QAction( this, "helpAboutAction" );
    configurationPreferencesAction = new QAction( this, "configurationPreferencesAction" );
    configurationSave_configurationAction = new QAction( this, "configurationSave_configurationAction" );
    songAnalyzeAction = new QAction( this, "songAnalyzeAction" );
    songAnalyzeAction->setEnabled( FALSE );
    songAnalyzeAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "stock_autopilot.png" ) ) );
    fileExportAction = new QAction( this, "fileExportAction" );
    fileExportAction->setEnabled( FALSE );
    fileExportto_MPEG7Action = new QAction( this, "fileExportto_MPEG7Action" );
    fileExportto_MPEG7Action->setEnabled( FALSE );
    fileNew_projectAction = new QAction( this, "fileNew_projectAction" );
    fileNew_projectAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "stock_new-document-32.png" ) ) );
    fileSave_projectAction = new QAction( this, "fileSave_projectAction" );
    fileSave_projectAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "stock_save_as.png" ) ) );
    fileSave_project_asAction = new QAction( this, "fileSave_project_asAction" );
    fileSave_project_asAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    fileOpen_projectAction_2 = new QAction( this, "fileOpen_projectAction_2" );
    fileOpen_projectAction_2->setIconSet( QIconSet( QPixmap::fromMimeSource( "stock_open.png" ) ) );
    fileAdd_to_projectAction = new QAction( this, "fileAdd_to_projectAction" );
    filePublishAction = new QAction( this, "filePublishAction" );
    filePublishAction->setEnabled( FALSE );
    filePublishAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );
    songAnalyzeAllAction = new QAction( this, "songAnalyzeAllAction" );
    songAnalyzeAllAction->setEnabled( FALSE );
    songPreviousAction = new QAction( this, "songPreviousAction" );
    songPreviousAction->setEnabled( FALSE );
    editDelete_from_projectAction = new QAction( this, "editDelete_from_projectAction" );
    editDelete_from_projectAction->setEnabled( TRUE );
    editDelete_from_projectAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "" ) ) );


    // toolbars
    Toolbar = new QToolBar( QString(""), this, DockTop ); 

    songAnalyzeAction->addTo( Toolbar );
    mIntraSongToolbar = new QToolBar( QString(""), this, DockTop ); 

    mIntraSongToolbar->setCloseMode( 0 );
    fileNew_projectAction->addTo( mIntraSongToolbar );
    fileOpen_projectAction_2->addTo( mIntraSongToolbar );
    fileSave_projectAction->addTo( mIntraSongToolbar );


    // menubar
    MenuBar = new QMenuBar( this, "MenuBar" );


    mFileMenu = new QPopupMenu( this );
    fileNew_projectAction->addTo( mFileMenu );
    fileOpen_projectAction_2->addTo( mFileMenu );
    fileSave_projectAction->addTo( mFileMenu );
    fileSave_project_asAction->addTo( mFileMenu );
    mFileMenu->insertSeparator();
    filePublishAction->addTo( mFileMenu );
    mFileMenu->insertSeparator();
    fileAdd_to_projectAction->addTo( mFileMenu );
    QPopupMenu *popupMenu_9 = new QPopupMenu( this );
    mFileMenu->setAccel( tr( "" ), 
        mFileMenu->insertItem( fileExportAction->iconSet(),tr( "E&xport to" ), popupMenu_9 ) );
    fileExportto_MPEG7Action->addTo( popupMenu_9 );
    mFileMenu->insertSeparator();
    fileExitAction->addTo( mFileMenu );
    MenuBar->insertItem( QString(""), mFileMenu, 1 );

    mEditMenu = new QPopupMenu( this );
    editDelete_from_projectAction->addTo( mEditMenu );
    MenuBar->insertItem( QString(""), mEditMenu, 2 );

    SongMenu = new QPopupMenu( this );
    songAnalyzeAction->addTo( SongMenu );
    songAnalyzeAllAction->addTo( SongMenu );
    MenuBar->insertItem( QString(""), SongMenu, 3 );

    configuration = new QPopupMenu( this );
    configurationPreferencesAction->addTo( configuration );
    configuration->insertSeparator();
    configurationSave_configurationAction->addTo( configuration );
    MenuBar->insertItem( QString(""), configuration, 4 );

    help = new QPopupMenu( this );
    helpAboutAction->addTo( help );
    MenuBar->insertItem( QString(""), help, 5 );

    MenuBar->insertSeparator( 6 );

    languageChange();
    resize( QSize(836, 592).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( fileExitAction, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    connect( fileExitAction, SIGNAL( activated() ), this, SLOT( close() ) );
    connect( fileNew_projectAction, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    connect( fileOpen_projectAction_2, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    connect( fileAdd_to_projectAction, SIGNAL( activated() ), this, SLOT( addSongsToProject() ) );
    connect( editDelete_from_projectAction, SIGNAL( activated() ), this, SLOT( deleteSongsFromProject() ) );
    connect( fileSave_project_asAction, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    connect( fileSave_projectAction, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    connect( mProjectOverview, SIGNAL( clicked(QListViewItem*) ), this, SLOT( songsClicked(QListViewItem*) ) );
    connect( songAnalyzeAction, SIGNAL( activated() ), this, SLOT( analyze() ) );
    connect( songAnalyzeAllAction, SIGNAL( activated() ), this, SLOT( analyzeAll() ) );
    connect( this, SIGNAL( usesTextLabelChanged(bool) ), this, SLOT( AnnotatorBase_usesTextLabelChanged(bool) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
AnnotatorBase::~AnnotatorBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AnnotatorBase::languageChange()
{
    setCaption( tr( "Music Annotator" ) );
    mProjectOverview->header()->setLabel( 0, tr( "File" ) );
    mProjectOverview->header()->setLabel( 1, tr( "Calculated" ) );
    mProjectOverview->header()->setLabel( 2, tr( "Changed" ) );
    tabWidget2->changeTab( tab, tr( "Tab 1" ) );
    mLogicGroup->setTitle( tr( "LogicGroup" ) );
    fileExitAction->setText( tr( "Exit" ) );
    fileExitAction->setMenuText( tr( "E&xit" ) );
    fileExitAction->setAccel( QString::null );
    helpAboutAction->setText( tr( "&About" ) );
    helpAboutAction->setMenuText( tr( "&About" ) );
    configurationPreferencesAction->setText( tr( "&Preferences" ) );
    configurationPreferencesAction->setMenuText( tr( "&Preferences" ) );
    configurationSave_configurationAction->setText( tr( "&Save configuration" ) );
    configurationSave_configurationAction->setMenuText( tr( "&Save configuration" ) );
    songAnalyzeAction->setText( tr( "&AnalyzeLLDescriptors" ) );
    songAnalyzeAction->setMenuText( tr( "&AnalyzeLLDescriptors" ) );
    songAnalyzeAction->setToolTip( tr( "Analyze" ) );
    songAnalyzeAction->setAccel( tr( "F9" ) );
    fileExportAction->setText( tr( "E&xport to" ) );
    fileExportAction->setMenuText( tr( "E&xport to" ) );
    fileExportto_MPEG7Action->setText( tr( "&MPEG7" ) );
    fileExportto_MPEG7Action->setMenuText( tr( "&MPEG7" ) );
    fileNew_projectAction->setText( tr( "&New project" ) );
    fileNew_projectAction->setMenuText( tr( "&New project" ) );
    fileNew_projectAction->setToolTip( tr( "New project" ) );
    fileSave_projectAction->setText( tr( "&Save project" ) );
    fileSave_projectAction->setMenuText( tr( "&Save project" ) );
    fileSave_projectAction->setToolTip( tr( "Save project" ) );
    fileSave_project_asAction->setText( tr( "S&ave project as" ) );
    fileSave_project_asAction->setMenuText( tr( "S&ave project as" ) );
    fileSave_project_asAction->setToolTip( tr( "Save project as" ) );
    fileOpen_projectAction_2->setText( tr( "&Open project" ) );
    fileOpen_projectAction_2->setMenuText( tr( "&Open project" ) );
    fileOpen_projectAction_2->setToolTip( tr( "Open project" ) );
    fileOpen_projectAction_2->setStatusTip( tr( "Open project" ) );
    fileAdd_to_projectAction->setText( tr( "&Add to project" ) );
    fileAdd_to_projectAction->setMenuText( tr( "&Add to project" ) );
    filePublishAction->setText( tr( "&Publish" ) );
    filePublishAction->setMenuText( tr( "&Publish" ) );
    filePublishAction->setToolTip( tr( "Publish" ) );
    songAnalyzeAllAction->setText( tr( "&AnalyzeAllDescriptors" ) );
    songAnalyzeAllAction->setMenuText( tr( "&AnalyzeAllDescriptors" ) );
    songAnalyzeAllAction->setAccel( tr( "F10" ) );
    songPreviousAction->setText( tr( "&Previous" ) );
    songPreviousAction->setMenuText( tr( "&Previous" ) );
    songPreviousAction->setAccel( tr( "-" ) );
    editDelete_from_projectAction->setText( tr( "Delete (from project)" ) );
    editDelete_from_projectAction->setMenuText( tr( "Delete (from project)" ) );
    Toolbar->setLabel( tr( "Toolbar" ) );
    mIntraSongToolbar->setLabel( tr( "Intrasong toolbar" ) );
    MenuBar->findItem( 1 )->setText( tr( "&File" ) );
    MenuBar->findItem( 2 )->setText( tr( "&Edit" ) );
    MenuBar->findItem( 3 )->setText( tr( "Analyze" ) );
    MenuBar->findItem( 4 )->setText( tr( "&Configuration" ) );
    MenuBar->findItem( 5 )->setText( tr( "&Help" ) );
}

