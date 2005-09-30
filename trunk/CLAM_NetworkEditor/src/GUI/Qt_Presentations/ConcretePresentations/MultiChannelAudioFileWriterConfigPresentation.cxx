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

#include "MultiChannelAudioFileWriterConfigPresentation.hxx"
#include "ProcessingConfig.hxx"
#include "AudioFileHeader.hxx"

#include <limits>

#include <qlabel.h>
#include <qspinbox.h>
#include <string>
#include <qhbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qvalidator.h>
#include <qpushbutton.h>

namespace NetworkGUI
{

MultiChannelAudioFileWriterConfigPresentation::MultiChannelAudioFileWriterConfigPresentation( QWidget * parent )
	: Qt_ProcessingConfigPresentation( parent , "config"  ),
	  mLayout(0),
	  mLocation(),
	  mSampleRate(0),
	  mFormat(0),
	  mChannels(0)
{
}

MultiChannelAudioFileWriterConfigPresentation::~MultiChannelAudioFileWriterConfigPresentation()
{
}

void MultiChannelAudioFileWriterConfigPresentation::CreateSampleRate()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel("SampleRate", cell);
	std::stringstream val;
	val << mMultiConfig.GetTargetFile().GetHeader().GetSampleRate() << std::ends;
	mSampleRate = new QLineEdit(QString(val.str().c_str()), cell);
	mSampleRate->setAlignment(Qt::AlignRight);
	mSampleRate->setValidator(new QDoubleValidator(mSampleRate));

}

void MultiChannelAudioFileWriterConfigPresentation::CreateFormat()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel("Format", cell);
	mFormat = new QComboBox(false, cell); // false editable
	
	const CLAM::Enum::tEnumValue * mapping = mMultiConfig.GetTargetFile().GetHeader().GetFormat().GetSymbolMap();
	for (unsigned i = 0; mapping[i].name; i++) 
	{
		mFormat->insertItem( mapping[i].name );
		if (mapping[i].value==mMultiConfig.GetTargetFile().GetHeader().GetFormat().GetValue()) 
			mFormat->setCurrentItem(i);
	}

}

void MultiChannelAudioFileWriterConfigPresentation::SetConfig( const CLAM::ProcessingConfig& cfg )
{
	mMultiConfig = static_cast<const CLAM::MultiChannelAudioFileWriterConfig &>(cfg);
	CLAM::AudioFileHeader header;
	header.AddAll();
	header.UpdateData();
	

	if (!mLocation )
		mMultiConfig.GetTargetFile().CreateNew( "", header);
	else
		mMultiConfig.GetTargetFile().CreateNew( mLocation->text().latin1(), header);
	CLAM_ASSERT(!mLayout, "Configurator: Configuration assigned twice");

	mLayout = mAttributeContainer;
	mLayout->setSpacing(5);
	mLayout->setMargin(5);
	mLayout->setMinimumWidth(120);
	CreateGUI();
	adjustSize();
}

void MultiChannelAudioFileWriterConfigPresentation::ConfigureProcessing()
{
	CLAM::AudioFileHeader header;
	header.AddAll();
	header.UpdateData();
	
	CLAM::TData convertedValue = 0;
	const char * readValueSampleRate = mSampleRate->text().latin1();
	std::stringstream s(readValueSampleRate);
	s >> convertedValue ;
	
	header.SetSampleRate( convertedValue );
	header.SetChannels( mChannels->value() );

	const CLAM::Enum::tEnumValue * mapping = header.GetFormat().GetSymbolMap();
	header.SetFormat( mapping[mFormat->currentItem()].value );
	
	mMultiConfig.GetTargetFile().CreateNew( mLocation->text().latin1(), header );

	// finally we add all of the information to the multi config and pass it to the processing 
	SignalConfigureProcessing.Emit(mMultiConfig);
}


void MultiChannelAudioFileWriterConfigPresentation::CreateGUI()
{
	CreateLocation();
	CreateSampleRate();
	CreateChannels();
	CreateFormat();
}

void MultiChannelAudioFileWriterConfigPresentation::CreateLocation()
{	
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	//Unused variable: QLabel * label = new QLabel("TargetFile", cell);
	
	mLocation = new QLineEdit(QString(mMultiConfig.GetTargetFile().GetLocation().c_str()), cell); 
	mLocation->setMinimumWidth(300);
	mLocation->setText( "" );

	QPushButton * fileBrowserLauncher = new QPushButton("...",cell);
	fileBrowserLauncher->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	QFileDialog * fd = new QFileDialog(this, "file dialog", FALSE );
	fd->setMode( QFileDialog::AnyFile );

	connect( fileBrowserLauncher, SIGNAL(clicked()), fd, SLOT(exec()) );
	connect( fd, SIGNAL(fileSelected( const QString & )), mLocation, SLOT( setText( const QString & )));
}

void MultiChannelAudioFileWriterConfigPresentation::CreateChannels()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel(QString("Channels"), cell);
	mChannels = new QSpinBox(cell);
	mChannels->setMaxValue(std::numeric_limits<CLAM::TSize>::max());
	mChannels->setMinValue(std::numeric_limits<CLAM::TSize>::min());
	mChannels->setValue( mMultiConfig.GetTargetFile().GetHeader().GetChannels() );
}

} // namespace NetworkGUI

