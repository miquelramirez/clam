/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "AudioFileHeader.hxx"

namespace CLAM
{
	void AudioFileHeader::DefaultInit()
	{
	
	}

	void AudioFileHeader::SetValues( TData rate, TSize nChannels,
					 const char* fmtString )
	{
		SetValues( rate, nChannels, EAudioFileFormat( fmtString ) );
	}

	void AudioFileHeader::SetValues( TData rate, TSize nChannels,
					 EAudioFileFormat format )
	{
		AddAll();
		UpdateData();
		SetSampleRate( rate );
		SetChannels( nChannels );
		SetFormat( format );
		SetEndianess( EAudioFileEndianess::eDefault );
		SetLength( TTime( 0 ) );

		// Default encoding
		if ( GetFormat() == EAudioFileFormat::eWAV )
		{
			//SetEncoding( EAudioFileEncoding::ePCM_24 ); //TODO: We'd like to be able to choose between float and int pcm
			SetEncoding( EAudioFileEncoding::eFLOAT );
			return;
		}
		if ( GetFormat() == EAudioFileFormat::eAIFF )
		{
			SetEncoding( EAudioFileEncoding::ePCM_24 );
			return;
		}
		SetEncoding( EAudioFileEncoding::eDefault );
	}


}

