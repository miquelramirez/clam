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

#include "AudioFileFormats.hxx"
#include <algorithm>

namespace CLAM
{
	EAudioFileKind::EAudioFileKind()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileKind::EAudioFileKind( tValue v )
		: Enum( sEnumValues, v )
	{
	}

	EAudioFileKind::EAudioFileKind( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileKind::sEnumValues[] = {
		{ ePCM,         "PCM" },
		{ eOggVorbis,   "Ogg/Vorbis" },
		{ eMpeg,        "Mpeg Audio" },
		{ eUnknown,     "Unknown" },
		{ 0,            NULL }
	};

	Enum::tValue EAudioFileKind::sDefault = EAudioFileKind::ePCM;

	Component* EAudioFileKind::Species() const
	{
		return new EAudioFileKind( "PCM" );
	}


	EAudioFileFormat::EAudioFileFormat()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileFormat::EAudioFileFormat( tValue v )
		: Enum( sEnumValues, v )
	{
	}

	EAudioFileFormat::EAudioFileFormat( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileFormat::sEnumValues[] = {
		{ EAudioFileFormat::eWAV,       "WAV" },
		{ EAudioFileFormat::eAIFF,      "AIFF" },
		{ EAudioFileFormat::eAU,        "AU"   },
		{ EAudioFileFormat::eRAW,       "RAW"  },
		{ EAudioFileFormat::ePAF,       "PAF"  },
		{ EAudioFileFormat::eSVX,       "SVX"  },
		{ EAudioFileFormat::eNIST,      "NIST" },
		{ EAudioFileFormat::eVOC,       "VOC"  },
		{ EAudioFileFormat::eIRCAM,     "IRCAM" },
		{ EAudioFileFormat::eW64,       "W64"  },
		{ EAudioFileFormat::eMAT4,      "MAT4" },
		{ EAudioFileFormat::eMAT5,      "MAT5" },
		{ EAudioFileFormat::eVorbisMk1, "VorbisMk1"},
		{ EAudioFileFormat::eMpegLayer1, "Mpeg Audio Layer 1"},
		{ EAudioFileFormat::eMpegLayer2, "Mpeg Audio Layer 2"},
		{ EAudioFileFormat::eMpegLayer3, "Mpeg Audio Layer 3"},
		{ 0,                            NULL }
	};

	Enum::tValue EAudioFileFormat::sDefault = EAudioFileFormat::eWAV;

	Component* EAudioFileFormat::Species() const
	{
		return new EAudioFileFormat( "WAV" );
	}

	EAudioFileFormat EAudioFileFormat::FormatFromFilename( std::string filename )
	{
		std::string::iterator dotPos = std::find( filename.begin(), filename.end(), '.' );

		if ( dotPos == filename.end() )
			return EAudioFileFormat( eWAV );

		std::string specifiedFmt;
		specifiedFmt.assign( dotPos+1, filename.end() );

		if ( specifiedFmt == "wav" || specifiedFmt == "WAV" )
			return EAudioFileFormat( eWAV );
		else if ( specifiedFmt == "ogg" || specifiedFmt == "OGG" )
			return EAudioFileFormat( eVorbisMk1 );
		else if ( specifiedFmt == "aif" || specifiedFmt =="AIF" 
			  || specifiedFmt == "aiff" || specifiedFmt == "AIFF" )
			return EAudioFileFormat( eAIFF );
		else if ( specifiedFmt == "au" || specifiedFmt == "AU" 
			  || specifiedFmt == "snd" || specifiedFmt == "SND" )
			return EAudioFileFormat( eAU );
		else if ( specifiedFmt == "raw" || specifiedFmt == "RAW" )
			return EAudioFileFormat( eRAW );
		else if ( specifiedFmt == "paf" || specifiedFmt == "PAF" )
			return EAudioFileFormat( ePAF );
		else if ( specifiedFmt == "svx" || specifiedFmt == "SVX" )
			return EAudioFileFormat( eSVX );
		else if ( specifiedFmt == "nist" || specifiedFmt == "NIST" )
			return EAudioFileFormat( eNIST );
		else if ( specifiedFmt == "voc" || specifiedFmt == "VOC" )
			return EAudioFileFormat( eVOC );
		else if ( specifiedFmt == "ircam" || specifiedFmt == "IRCAM" )
			return EAudioFileFormat( eIRCAM );
		else if ( specifiedFmt == "w64" || specifiedFmt == "W64" )
			return EAudioFileFormat( eW64 );
		else if ( specifiedFmt == "mat4" || specifiedFmt == "MAT4" )
			return EAudioFileFormat( eMAT4 );
		else if ( specifiedFmt == "mat5" || specifiedFmt == "MAT5"
			  || specifiedFmt == "mat" || specifiedFmt == "MAT" )
			return EAudioFileFormat( eMAT5 );
		else if ( specifiedFmt == "mp1" || specifiedFmt == "MP1" )
			return EAudioFileFormat( eMpegLayer1 );
		else if ( specifiedFmt == "mp2" || specifiedFmt == "MP2" )
			return EAudioFileFormat( eMpegLayer2 );
		else if ( specifiedFmt == "mp3" || specifiedFmt == "MP3" )
			return EAudioFileFormat( eMpegLayer3 );
		else
			return EAudioFileFormat( eWAV );

	}
	
	const EAudioFileFormat::FormatFilterList& EAudioFileFormat::ReadableFormats( )
	{
		static FormatFilterList mReadableFormatsList;
		
		if ( mReadableFormatsList.empty() )
		{
			mReadableFormatsList.push_back( FormatFilter( "Microsoft RIFF/WAVE files", "*.wav" ) );
			mReadableFormatsList.push_back( FormatFilter( "Apple/SGI AIFF files", "*.aiff,*.aif") );
			mReadableFormatsList.push_back( FormatFilter( "Sun/Next AU files", "*.snd,*.au" ) );
			mReadableFormatsList.push_back( FormatFilter( "RAW PCM files","*.raw" ) );
			mReadableFormatsList.push_back( FormatFilter( "Ensoniq's PARIS Files","*.paf" ) );
			mReadableFormatsList.push_back( FormatFilter( "Amiga IFF files","*.svx") );
			mReadableFormatsList.push_back( FormatFilter( "Sphere NIST files","*.nist" ) );
			mReadableFormatsList.push_back( FormatFilter( "Creative's VOC files","*.voc" ) );
			mReadableFormatsList.push_back( FormatFilter( "Berkeley/IRCAM/CARL files","*.ircam" ) );
			mReadableFormatsList.push_back( FormatFilter( "Sonic Foundry's 64 RIFF/WAV files","*.w64" ) );
			mReadableFormatsList.push_back( FormatFilter( "Matlab/GNU Octave files", "*.mat4,*.mat5,*.mat" ) );
			mReadableFormatsList.push_back( FormatFilter( "Vorbis I files (Ogg/Vorbis)","*.ogg" ) );
			mReadableFormatsList.push_back( FormatFilter( "Mpeg Audio Layer 1","*.mp1,*.mpg") );
			mReadableFormatsList.push_back( FormatFilter( "Mpeg Audio Layer 2", "*.mp2,*.mpg" ) );
			mReadableFormatsList.push_back( FormatFilter( "Mpeg Audio Layer 3", "*.mp3,*.mpg" ) );
		}

		return mReadableFormatsList;
	}

	const EAudioFileFormat::FormatFilterList& EAudioFileFormat::WritableFormats( )
	{
		static FormatFilterList mWritableFormatsList;
		
		if ( mWritableFormatsList.empty() )
		{
			mWritableFormatsList.push_back( FormatFilter( "Microsoft RIFF/WAVE files", "*.wav" ) );
			mWritableFormatsList.push_back( FormatFilter( "Apple/SGI AIFF files", "*.aiff,*.aif") );
			mWritableFormatsList.push_back( FormatFilter( "Sun/Next AU files", "*.snd,*.au" ) );
			mWritableFormatsList.push_back( FormatFilter( "RAW PCM files","*.raw" ) );
			mWritableFormatsList.push_back( FormatFilter( "Ensoniq's PARIS Files","*.paf" ) );
			mWritableFormatsList.push_back( FormatFilter( "Amiga IFF files","*.svx") );
			mWritableFormatsList.push_back( FormatFilter( "Sphere NIST files","*.nist" ) );
			mWritableFormatsList.push_back( FormatFilter( "Creative's VOC files","*.voc" ) );
			mWritableFormatsList.push_back( FormatFilter( "Berkeley/IRCAM/CARL files","*.ircam" ) );
			mWritableFormatsList.push_back( FormatFilter( "Sonic Foundry's 64 RIFF/WAV files","*.w64" ) );
			mWritableFormatsList.push_back( FormatFilter( "Matlab/GNU Octave files", "*.mat4,*.mat5,*.mat" ) );
			mWritableFormatsList.push_back( FormatFilter( "Vorbis I files (Ogg/Vorbis)","*.ogg" ) );
		}
		
		return mWritableFormatsList;
	}


	EAudioFileEncoding::EAudioFileEncoding()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileEncoding::EAudioFileEncoding( tValue val )
		: Enum( sEnumValues, val )
	{
	}

	EAudioFileEncoding::EAudioFileEncoding( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileEncoding::sEnumValues[] = {
		{ ePCM_S8, "signed 8-bit" },
		{ ePCM_16, "signed 16-bit" },
		{ ePCM_24, "signed 24-bit" },
		{ ePCM_32, "signed 32-bit" },
		{ ePCM_U8, "unsigned 8-bit" },
		{ eFLOAT, "float 32-bit"},
		{ eDOUBLE, "float 64-bit"},
		{ eU_LAW, "u-law"},
		{ eA_LAW, "a-law"},
		{ eIMA_ADPCM, "IMA ADPCM" },
		{ eMS_ADPCM, "MS ADPCM "},
		{ eGSM610, "GSM 6.10"},
		{ eVOX_ADPCM, "Oki ADPCM"},
		{ eG721_32, "32k G721 ADPCM"},
		{ eG723_24, "24k G723 ADPCM"},
		{ eG723_40, "40k G723 ADPCM"},
		{ eDWVW_12, "DWVW 12-bit"},
		{ eDWVW_16, "DWVW 16-bit"},
		{ eDWVW_24, "DWVW 24-bit"},
		{ eDWVW_N,  "DWVW N-bit"},
		{ eDefault, "Format Default"},
		{ e5015US, "Emphasis 50/15 US"},
		{ eCCITTJ17, "Emphasis CCITT J.17"},
		{ 0, NULL }
	};

	Enum::tValue EAudioFileEncoding::sDefault = EAudioFileEncoding::ePCM_16;

	Component* EAudioFileEncoding::Species() const
	{
		return new EAudioFileEncoding("signed 16-bit");
	}

	EAudioFileEndianess::EAudioFileEndianess()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileEndianess::EAudioFileEndianess( tValue val )
		: Enum( sEnumValues, val )
	{
	}

	EAudioFileEndianess::EAudioFileEndianess( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileEndianess::sEnumValues[] = {
		{ eDefault, "Format Default" },
		{ eLittle,  "Little Endian"},
		{ eBig,     "Big Endian"},
		{ eHost,    "Host CPU Endianess"},
		{ 0, NULL }
	};

	Enum::tValue EAudioFileEndianess::sDefault = EAudioFileEndianess::eDefault;

	Component* EAudioFileEndianess::Species() const
	{
		return new EAudioFileEndianess("Default");
	}

}
