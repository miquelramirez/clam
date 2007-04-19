/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __AUDIOFILEFORMATS__
#define __AUDIOFILEFORMATS__

#if USE_SNDFILE != 1
#error USE_SNDFILE was not set to 1 in your settings.cfg file, but you are including files that require this. Please fix your settings.cfg
#endif

#include <sndfile.h>
#include "Enum.hxx"
#include <CLAM/ConfigurableFile.hxx>
#include <string>
#include <list>
#include <utility>

namespace CLAM
{

	/** @ingroup AudioFileIO */
	class EAudioFileKind : public Enum
	{
	public:
		EAudioFileKind() : Enum(EnumValues(), DefaultValue()) {}
		EAudioFileKind( tValue val ) : Enum(EnumValues(), val) {}
		EAudioFileKind( std::string s ) : Enum(EnumValues(), s) {}
		virtual Component* Species() const { return new EAudioFileKind; }

		typedef enum {
			ePCM,  /* String value: "PCM". Some variant of good ol' Pulse Code Modulation */
			eOggVorbis, /* String value: "Ogg/Vorbis". Compressed with OggVorbis scheme */
			eMpeg,        /* String value: "Mpeg Audio". Compressed with MPEG-1 Layer 3 scheme */
			eUnknown /* For error handling */
		};

		static tValue     DefaultValue() { return ePCM; }
		static tEnumValue * EnumValues() 
		{
			static tEnumValue sEnumValues[] = {
				{ ePCM,         "PCM" },
				{ eOggVorbis,   "Ogg/Vorbis" },
				{ eMpeg,        "Mpeg Audio" },
				{ eUnknown,     "Unknown" },
				{ 0,            NULL }
			};
			return sEnumValues;
		}

	};

	/** @ingroup AudioFileIO */
	class EAudioFileFormat : public Enum
	{
	public:
		EAudioFileFormat() : Enum(EnumValues(), DefaultValue()) {}
		EAudioFileFormat( tValue val ) : Enum(EnumValues(), val) {}
		EAudioFileFormat( std::string s ) : Enum(EnumValues(), s) {}
		virtual Component* Species() const { return new EAudioFileFormat; }

		typedef enum
		{
			eWAV = SF_FORMAT_WAV,   /* Microsoft WAV  (little endian)*/
			eAIFF = SF_FORMAT_AIFF,  /* Apple/SGI AIFF (big endian)*/
			eAU = SF_FORMAT_AU,    /* Sun/NeXT AU (big endian)*/
			eRAW = SF_FORMAT_RAW,   /* RAW PCM */
			ePAF = SF_FORMAT_PAF,   /* Ensoniq PARIS File format */
			eSVX = SF_FORMAT_SVX,   /* Amiga IFF / SVX8 / SV16 */
			eNIST = SF_FORMAT_NIST,  /* Sphere NIST */
			eVOC = SF_FORMAT_VOC,   /* VOC */
			eIRCAM = SF_FORMAT_IRCAM, /* Berkeley / IRCAM / CARL */
			eW64 = SF_FORMAT_W64,   /* Sonic Foundry's 64 RIFF/WAV */
			eMAT4 = SF_FORMAT_MAT4,  /* MATLAB 4.2 - GNU Octave 2.0 */
			eMAT5 = SF_FORMAT_MAT5,   /* MATLAB 5.2 - GNU Octave 2.1 */
			eVorbisMk1 = 1000 + SF_FORMAT_MAT5,
			eMpegLayer1 = 1001 + SF_FORMAT_MAT5,
			eMpegLayer2 = 1002 + SF_FORMAT_MAT5,
			eMpegLayer3 = 1003 + SF_FORMAT_MAT5
		};

		static tValue     DefaultValue() { return eWAV; }
		static tEnumValue * EnumValues()
		{
			static tEnumValue sEnumValues[] = {
				{ eWAV,       "WAV" },
				{ eAIFF,      "AIFF" },
				{ eAU,        "AU"   },
				{ eRAW,       "RAW"  },
				{ ePAF,       "PAF"  },
				{ eSVX,       "SVX"  },
				{ eNIST,      "NIST" },
				{ eVOC,       "VOC"  },
				{ eIRCAM,     "IRCAM" },
				{ eW64,       "W64"  },
				{ eMAT4,      "MAT4" },
				{ eMAT5,      "MAT5" },
				{ eVorbisMk1, "VorbisMk1"},
				{ eMpegLayer1, "Mpeg Audio Layer 1"},
				{ eMpegLayer2, "Mpeg Audio Layer 2"},
				{ eMpegLayer3, "Mpeg Audio Layer 3"},
				{ 0, 0 }
			};
			return sEnumValues;
		}

	public:

		static  EAudioFileFormat FormatFromFilename( std::string filename );

	};

	/** @ingroup AudioFileIO */
	class EAudioFileEncoding : public Enum
	{
	public:
		EAudioFileEncoding() : Enum(EnumValues(), DefaultValue()) {}
		EAudioFileEncoding( tValue val ) : Enum(EnumValues(), val) {}
		EAudioFileEncoding( std::string s ) : Enum(EnumValues(), s) {}
		virtual Component* Species() const { return new EAudioFileEncoding; }

		enum
		{
			ePCM_S8 = SF_FORMAT_PCM_S8,  /* Signed 8-bit PCM */
			ePCM_16 = SF_FORMAT_PCM_16,  /* Signed 16-bit PCM */
			ePCM_24 = SF_FORMAT_PCM_24,  /* Signed 24-bit PCM */
			ePCM_32 = SF_FORMAT_PCM_32,  /* Signed 32-bit PCM */
			ePCM_U8 = SF_FORMAT_PCM_U8,  /* Unsigned 8-bit PCM */
			eFLOAT = SF_FORMAT_FLOAT,   /* IEEE-7354 32-bit floating point */
			eDOUBLE = SF_FORMAT_DOUBLE,  /* IEEE-7354 64-bit floating point */
			eU_LAW = SF_FORMAT_ULAW,   /* u-Law */
			eA_LAW = SF_FORMAT_ALAW,   /* a-Law */
			eIMA_ADPCM = SF_FORMAT_IMA_ADPCM,  /* IMA ADPCM */
			eMS_ADPCM = SF_FORMAT_MS_ADPCM,   /* Microsoft ADPCM */
			eGSM610 = SF_FORMAT_GSM610,     /* GSM 6.10 */
			eVOX_ADPCM = SF_FORMAT_VOX_ADPCM,  /* Oki ADPCM */
			eG721_32 = SF_FORMAT_G721_32,    /* 32kbs G721 ADPCM */
			eG723_24 = SF_FORMAT_G723_24,    /* 24kbs G723 ADPCM */
			eG723_40 = SF_FORMAT_G723_40,    /* 40kbs G723 ADPCM */
			eDWVW_12 = SF_FORMAT_DWVW_12,    /* 12-bit Delta Width Variable Word */
			eDWVW_16 = SF_FORMAT_DWVW_16,    /* 16-bit Delta Width Variable Word */
			eDWVW_24 = SF_FORMAT_DWVW_24,    /* 24-bit Delta Width Variable Word */
			eDWVW_N  = SF_FORMAT_DWVW_N,    /* N-bit Delta Width Variable Word */
			eDefault = 1000 + SF_FORMAT_DWVW_N,  /*Format's default encoding*/
			e5015US = 1001 + SF_FORMAT_DWVW_N,
			eCCITTJ17 = 1002 + SF_FORMAT_DWVW_N			
		};
		static tValue      DefaultValue() { return ePCM_16; }
		static tEnumValue * EnumValues()
		{
			static tEnumValue sEnumValues[] = {
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
			return sEnumValues;
		}
	};

	/** @ingroup AudioFileIO */
	class EAudioFileEndianess : public Enum
	{
	public:
		EAudioFileEndianess() : Enum(EnumValues(), DefaultValue()) {}
		EAudioFileEndianess( tValue val ) : Enum(EnumValues(), val) {}
		EAudioFileEndianess( std::string s ) : Enum(EnumValues(), s) {}
		virtual Component* Species() const { return new EAudioFileEndianess; }

		enum
		{
			eDefault = SF_ENDIAN_FILE, /* Default format endianess */
			eLittle = SF_ENDIAN_LITTLE,  /* Force little endianess (if supported by format)*/
			eBig = SF_ENDIAN_BIG,     /* Force big endianess (if supported by format)*/
			eHost = SF_ENDIAN_CPU     /* Force host CPU endianess (if supported by format)*/
		};
		static tValue      DefaultValue() { return eDefault; }
		static tEnumValue * EnumValues()
		{
			static tEnumValue sEnumValues[] = {
				{ eDefault, "Format Default" },
				{ eLittle,  "Little Endian"},
				{ eBig,     "Big Endian"},
				{ eHost,    "Host CPU Endianess"},
				{ 0, NULL }
			};
			return sEnumValues;
		}
	};

}

#endif // AudioFileFormats.hxx

