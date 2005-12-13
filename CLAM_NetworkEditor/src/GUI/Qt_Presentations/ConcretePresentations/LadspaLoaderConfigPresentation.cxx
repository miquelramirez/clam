/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "LadspaLoaderConfigPresentation.hxx"
#include <CLAM/ProcessingConfig.hxx>
#include <qhbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qvalidator.h>
#include <qpushbutton.h>
#include <string>

namespace NetworkGUI
{

LadspaLoaderConfigPresentation::LadspaLoaderConfigPresentation( QWidget * parent )
	: Qt_ProcessingConfigPresentation( parent , "config"  ),
	mLayout(0),
	mLocation(0),
	mPluginsList(0),	
	mSampleRate(0),
	mFrameSize(0)
{
}

LadspaLoaderConfigPresentation::~LadspaLoaderConfigPresentation()
{
}


void LadspaLoaderConfigPresentation::ConfigureProcessing()
{
	mConfig.SetSharedObjectName( mLocation->text().latin1() );
	mConfig.SetIndex( mPluginsList->currentItem() );
	
	unsigned long sampleRate = 0;
	const char * readValueSampleRate = mSampleRate->text().latin1();
	std::stringstream s(readValueSampleRate);
	s >> sampleRate;
	mConfig.SetSampleRate( sampleRate );
	
	unsigned long frameSize = 0;
	const char * readValueFrameSize = mFrameSize->text().latin1();
	std::stringstream s2(readValueFrameSize);
	s2 >> frameSize ;
	mConfig.SetSize( frameSize );

	SignalConfigureProcessing.Emit(mConfig);
}

void LadspaLoaderConfigPresentation::SetConfig( const CLAM::ProcessingConfig & cfg )
{
	mConfig = static_cast<const CLAM::LadspaLoaderConfig &>(cfg);
	mConfig.AddAll();
	mConfig.UpdateData();
	CLAM_ASSERT(!mLayout, "Configurator: Configuration assigned twice");

	mLayout = mAttributeContainer;
	mLayout->setSpacing(5);
	mLayout->setMargin(5);
	mLayout->setMinimumWidth(120);

	CreateGUI();
	adjustSize();
}


void LadspaLoaderConfigPresentation::CreateGUI()
{
	CreateLocation();
	CreatePluginsList();
	CreateSampleRate();
	CreateFrameSize();
}

void LadspaLoaderConfigPresentation::CreateLocation()
{	
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	//Unused variable: QLabel * label = new QLabel("Shared Object to Load", cell);
	
	mLocation = new QLineEdit(QString(mConfig.GetSharedObjectName().c_str()), cell); 
	mLocation->setMinimumWidth(300);

	QPushButton * fileBrowserLauncher = new QPushButton("...",cell);
	fileBrowserLauncher->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	QFileDialog * fd = new QFileDialog(this, "file dialog", FALSE );
	QString types("Shared Objects (*.so)" );   
	fd->setFilters( types );
	fd->setMode( QFileDialog::ExistingFile );

	connect( fileBrowserLauncher, SIGNAL(clicked()), fd, SLOT(exec()) );
	connect( fd, SIGNAL(fileSelected( const QString & )), mLocation, SLOT( setText( const QString & )));
	connect( mLocation, SIGNAL( textChanged( const QString & )), this, SLOT( UpdatePluginsList( const QString & )) );
}

void LadspaLoaderConfigPresentation::CreatePluginsList()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel("Plugins List", cell);
	mPluginsList = new QComboBox(false, cell); // false editable
	UpdatePluginsList( QString( mConfig.GetSharedObjectName().c_str() ) );

}

void LadspaLoaderConfigPresentation::UpdatePluginsList( const QString & sharedObject )
{
	mPluginsList->clear();
	CLAM::LadspaPluginExaminer examiner( sharedObject.latin1() );
	
	CLAM::LadspaPluginExaminer::NamesList::iterator it;
	for( it=examiner.BeginDescriptors(); it!=examiner.EndDescriptors(); it++ ) 
		mPluginsList->insertItem( (*it).c_str() );
	mPluginsList->setCurrentItem( 0 );
}

void LadspaLoaderConfigPresentation::CreateSampleRate()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel("SampleRate", cell);
	std::stringstream val;
	val << mConfig.GetSampleRate() << std::ends;
	mSampleRate = new QLineEdit(QString(val.str().c_str()), cell);
	mSampleRate->setAlignment(Qt::AlignRight);
	mSampleRate->setValidator(new QDoubleValidator(mSampleRate));

}

void LadspaLoaderConfigPresentation::CreateFrameSize()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel("FrameSize", cell);
	std::stringstream val;
	val << mConfig.GetSize() << std::ends;
	mFrameSize = new QLineEdit(QString(val.str().c_str()), cell);
	mFrameSize->setAlignment(Qt::AlignRight);
	mFrameSize->setValidator(new QDoubleValidator(mFrameSize));

}

} // namespace NetworkGUI

