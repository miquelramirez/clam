/****************************************************************************
** Form interface generated from reading ui file '../src/AnnotatorBase.ui'
**
** Created: Tue Mar 15 09:18:23 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ANNOTATORBASE_H
#define ANNOTATORBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>
#include "ListViewItem.hxx"
#include "qlistview.h"
#include "TableItem.hxx"
#include "Table.hxx"
#include "qstringlist.h"
#include "ComboTableItem.hxx"
#include "qtimer.h"
#include "vector"
#include "qgroupbox.h"
#include "qlayout.h"
#include "qpoint.h"
#include "qcolordialog.h"
#include "iostream"
#include "About.hxx"
#include "ConfigurationDialog.hxx"
#include "AnalyzeWindow.hxx"
#include "Song.hxx"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class Table;
class QListView;
class QListViewItem;
class QFrame;
class QTabWidget;
class QWidget;
class QGroupBox;

class AnnotatorBase : public QMainWindow
{
    Q_OBJECT

public:
    AnnotatorBase( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~AnnotatorBase();

    QListView* mProjectOverview;
    QFrame* frame3;
    Table* mDescriptorsTable;
    QTabWidget* tabWidget2;
    QWidget* tab;
    QGroupBox* mLogicGroup;
    QMenuBar *MenuBar;
    QPopupMenu *mFileMenu;
    QPopupMenu *mEditMenu;
    QPopupMenu *SongMenu;
    QPopupMenu *configuration;
    QPopupMenu *help;
    QToolBar *Toolbar;
    QToolBar *mIntraSongToolbar;
    QAction* fileExitAction;
    QAction* helpAboutAction;
    QAction* configurationPreferencesAction;
    QAction* configurationSave_configurationAction;
    QAction* songAnalyzeAction;
    QAction* fileExportAction;
    QAction* fileExportto_MPEG7Action;
    QAction* fileNew_projectAction;
    QAction* fileSave_projectAction;
    QAction* fileSave_project_asAction;
    QAction* fileOpen_projectAction_2;
    QAction* fileAdd_to_projectAction;
    QAction* filePublishAction;
    QAction* songAnalyzeAllAction;
    QAction* songPreviousAction;
    QAction* editDelete_from_projectAction;

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void filePrint();
    virtual void fileExit();
    virtual void viewTitle();
    virtual void viewArtist();
    virtual void viewGenre();
    virtual void viewLyrics();
    virtual void viewItem();
    virtual void viewStructureItem();
    virtual void viewIntraSong();
    virtual void viewSongDescriptors();
    virtual void chooseColor();
    virtual void changeViewMenu( QWidget * q );
    virtual void viewIntraSongViewMenu();
    virtual void AnnotatorBrowserGLgroup_toggled( bool a );
    virtual void songsClicked( QListViewItem * item );
    virtual void addSongsToProject();
    virtual void deleteSongsFromProject();
    virtual void songAnalyzeAction_activated();
    virtual void analyze() = 0;
    virtual void analyzeAll() = 0;
    virtual void AnnotatorBase_usesTextLabelChanged( bool );

protected:
    AnalyzeWindow mAnalyzeWindow;
    About mAbout;
    QPopupMenu * mIntraSongViewMenu;
    bool play;
    bool mSongDescriptorsViewed;
    bool mIntraSongViewed;
    bool artistViewed;
    bool titleViewed;
    bool genreViewed;
    bool happyMusicViewed;
    bool lyricsViewed;
    bool itemViewed;
    bool structureItemViewed;
    bool mIntraSong2Viewed;
    ConfigurationDialog mConfigurationDialog;
    std::vector < Descriptors >  mSongDescriptors;

    QVBoxLayout* AnnotatorBaseLayout;
    QHBoxLayout* layout6;
    QHBoxLayout* layout5;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    virtual void init();

};

#endif // ANNOTATORBASE_H
