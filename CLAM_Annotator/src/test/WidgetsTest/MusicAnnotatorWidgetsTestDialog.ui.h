/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <iostream>

void MusicAnnotatorWidgetsTestDialog::init()
{
 mSegmentationInformationWidget->setMaximumLength(1000);
 mSegmentationInformationWidget->setEnabled(true);
}

void MusicAnnotatorWidgetsTestDialog::addLine()
{
	SegmentationInformationWidget::Segments segments;
	segments.push_back("A");
	segments.push_back("#");
	segments.push_back("A");
	segments.push_back("#");
	segments.push_back("A");
	mSegmentationInformationWidget->drawLine( segments );
	segments.clear();
	segments.push_back("B");
	segments.push_back("#");
	segments.push_back("B");
	mSegmentationInformationWidget->drawLine( segments );
	segments.clear();
	segments.push_back("#");
	segments.push_back("C");
	segments.push_back("#");
	segments.push_back("C");
	segments.push_back("#");
	mSegmentationInformationWidget->drawLine( segments );
	segments.clear();
	segments.push_back("D");	
	mSegmentationInformationWidget->drawLine( segments );
}
