/****************************************************************************
** Form interface generated from reading ui file '../src/AboutBase.ui'
**
** Created: Mon Mar 14 18:33:39 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ABOUTBASE_H
#define ABOUTBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;

class AboutBase : public QDialog
{
    Q_OBJECT

public:
    AboutBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AboutBase();

    QLabel* pixmapLabel1;

protected:
    QVBoxLayout* AboutBaseLayout;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // ABOUTBASE_H
