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

#include "MonoAudioFileWriterConfigPresentation.hxx"
#include "ProcessingConfig.hxx"
#include "AudioFileHeader.hxx"
#include <qhbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qvalidator.h>
#include <qpushbutton.h>
#include <iostream> // TODO:remove
#include <string>
namespace NetworkGUI
{

MonoAudioFileWriterConfigPresentation::MonoAudioFileWriterConfigPresentation( QWidget * parent )
	: Qt_ProcessingConfigPresentation( parent , "config"  ),
	mLocation(0),
	mLayout(0),
	mSampleRate(0),
	mFormat(0)
{
}

MonoAudioFileWriterConfigPresentation::~MonoAudioFileWriterConfigPresentation()
{
}


void MonoAudioFileWriterConfigPresentation::ConfigureProcessing()
{
	CLAM::AudioFileHeader header;
	header.AddAll();
	header.UpdateData();

	mConfig.GetTargetFile().SetLocation( mLocation->text().latin1() );
	
	CLAM::TData convertedValue = 0;
	const char * readValueSampleRate = mSampleRate->text().latin1();
	std::stringstream s(readValueSampleRate);
	s >> convertedValue ;

	header.SetSampleRate( convertedValue );
	header.SetChannels( 1 ); // just a mono file

	const CLAM::Enum::tEnumValue * mapping = header.GetFormat().GetSymbolMap();
	header.SetFormat( mapping[mFormat->currentItem()].value );

	mConfig.GetTargetFile().SetHeader( header );

	SignalConfigureProcessing.Emit(mConfig);
}

void MonoAudioFileWriterConfigPresentation::SetConfig( const CLAM::ProcessingConfig & cfg )
{
	mConfig = static_cast<const CLAM::MonoAudioFileWriterConfig &>(cfg);
	mConfig.AddAll();
	mConfig.UpdateData();
	CLAM::AudioFileHeader header;
	header.AddAll();
	header.UpdateData();
	mConfig.GetTargetFile().SetHeader(header);
	CLAM_ASSERT(!mLayout, "Configurator: Configuration assigned twice");

	mLayout = mAttributeContainer;
	mLayout->setSpacing(5);
	mLayout->setMargin(5);
	mLayout->setMinimumWidth(120);

	CreateGUI();
	adjustSize();
}


void MonoAudioFileWriterConfigPresentation::CreateGUI()
{
	CreateLocation();
	CreateSampleRate();
	CreateFormat();
}

void MonoAudioFileWriterConfigPresentation::CreateLocation()
{	
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	QLabel * label = new QLabel("TargetFile", cell);
	
	mLocation = new QLineEdit(QString(mConfig.GetTargetFile().GetLocation().c_str()), cell); 
	mLocation->setMinimumWidth(300);

	QPushButton * fileBrowserLauncher = new QPushButton("...",cell);
	fileBrowserLauncher->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	QFileDialog * fd = new QFileDialog(this, "file dialog", FALSE );
	fd->setMode( QFileDialog::AnyFile );

	connect( fileBrowserLauncher, SIGNAL(clicked()), fd, SLOT(exec()) );
	connect( fd, SIGNAL(fileSelected( const QString & )), mLocation, SLOT( setText( const QString & )));
}

void MonoAudioFileWriterConfigPresentation::CreateSampleRate()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel("SampleRate", cell);
	std::stringstream val;
	val << mConfig.GetTargetFile().GetHeader().GetSampleRate() << std::ends;
	mSampleRate = new QLineEdit(QString(val.str().c_str()), cell);
	mSampleRate->setAlignment(Qt::AlignRight);
	mSampleRate->setValidator(new QDoubleValidator(mSampleRate));

}

void MonoAudioFileWriterConfigPresentation::CreateFormat()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel("Format", cell);
	mFormat = new QComboBox(false, cell); // false editable
	
	const CLAM::Enum::tEnumValue * mapping = mConfig.GetTargetFile().GetHeader().GetFormat().GetSymbolMap();
	for (unsigned i = 0; mapping[i].name; i++) 
	{
		mFormat->insertItem( mapping[i].name );
		if (mapping[i].value==mConfig.GetTargetFile().GetHeader().GetFormat().GetValue()) 
			mFormat->setCurrentItem(i);
	}

}

} // namespace NetworkGUI

