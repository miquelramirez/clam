#include "ClipBoardWidget.hxx"
#include "ClipBoard.hxx"

#include <qlabel.h>

ClipBoard* ClipBoardWidget::getInstance()
{
  return ClipBoard::getInstance();
}

void ClipBoardWidget::init()
{
  mLabel= new QLabel(this,"ClipBoardLabel");
  mLabel->setText("ClipBoard");
}
