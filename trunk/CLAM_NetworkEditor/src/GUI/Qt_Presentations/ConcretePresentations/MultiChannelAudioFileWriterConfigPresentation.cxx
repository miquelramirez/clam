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
	: MonoAudioFileWriterConfigPresentation( parent ),
	mChannels(0)
{
}

MultiChannelAudioFileWriterConfigPresentation::~MultiChannelAudioFileWriterConfigPresentation()
{
}


void MultiChannelAudioFileWriterConfigPresentation::ConfigureProcessing()
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
	header.SetChannels( mChannels->value() );

	const CLAM::Enum::tEnumValue * mapping = header.GetFormat().GetSymbolMap();
	header.SetFormat( mapping[mFormat->currentItem()].value );

	mConfig.GetTargetFile().SetHeader( header );


	// finally we add all of the information to the multi config and pass it to the processing 
	mMultiConfig.AddAll();
	mMultiConfig.UpdateData();
	mMultiConfig.SetTargetFile( mConfig.GetTargetFile() );
	SignalConfigureProcessing.Emit(mMultiConfig);
}


void MultiChannelAudioFileWriterConfigPresentation::CreateGUI()
{
	CreateLocation();
	CreateSampleRate();
	CreateChannels();
	CreateFormat();
}


void MultiChannelAudioFileWriterConfigPresentation::CreateChannels()
{
	QHBox * cell = new QHBox(mLayout);
	cell->setSpacing(5);
	new QLabel(QString("Channels"), cell);
	mChannels = new QSpinBox(cell);
	mChannels->setMaxValue(std::numeric_limits<CLAM::TSize>::max());
	mChannels->setMinValue(std::numeric_limits<CLAM::TSize>::min());
	mChannels->setValue( mConfig.GetTargetFile().GetHeader().GetChannels() );
}

} // namespace NetworkGUI

