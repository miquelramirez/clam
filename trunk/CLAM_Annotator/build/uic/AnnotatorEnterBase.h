/****************************************************************************
** Form interface generated from reading ui file '../src/AnnotatorEnterBase.ui'
**
** Created: Mon Mar 14 18:33:40 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ANNOTATORENTERBASE_H
#define ANNOTATORENTERBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QIconView;
class QIconViewItem;

class AnnotatorEnterBase : public QDialog
{
    Q_OBJECT

public:
    AnnotatorEnterBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AnnotatorEnterBase();

    QIconView* mChooseIconView;

public slots:
    virtual void clicked( QIconViewItem * item );
    virtual void handCursor();
    virtual void arrowCursor();

protected:
    QVBoxLayout* AnnotatorEnterBaseLayout;

protected slots:
    virtual void languageChange();

};

#endif // ANNOTATORENTERBASE_H
