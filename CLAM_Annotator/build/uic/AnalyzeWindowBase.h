/****************************************************************************
** Form interface generated from reading ui file '../src/AnalyzeWindowBase.ui'
**
** Created: Mon Mar 14 19:30:16 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ANALYZEWINDOWBASE_H
#define ANALYZEWINDOWBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include "qtable.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QTable;
class QPushButton;

class AnalyzeWindowBase : public QDialog
{
    Q_OBJECT

public:
    AnalyzeWindowBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AnalyzeWindowBase();

    QGroupBox* groupBox1;
    QTable* table1;
    QPushButton* mAnalyzeButton;
    QPushButton* mAnalyzeAllButton;
    QPushButton* pushButton2;

    void connect( const QObject * sender, const char * signal, const QObject * receiver, const char * member );
    virtual void newFunction();

public slots:
    virtual void emit_analyze();
    virtual void emit_analyzeAll();
    virtual void close();

signals:
    void analyze();
    void analyzeAll();

protected:
    QVBoxLayout* AnalyzeWindowBaseLayout;
    QHBoxLayout* layout18;
    QHBoxLayout* layout17;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // ANALYZEWINDOWBASE_H
