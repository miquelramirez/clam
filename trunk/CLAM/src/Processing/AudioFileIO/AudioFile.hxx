/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _AudioFile_
#define _AudioFile_

#include <typeinfo> // for bad_cast definition
#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Enum.hxx"

namespace CLAM {

	class EAudioFileType : public Enum
	{
	public:

		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EAudioFileType() : Enum(sEnumValues, sDefault) {}
		EAudioFileType(tValue v) : Enum(sEnumValues, v) {};
		EAudioFileType(std::string s) : Enum(sEnumValues, s) {};

		typedef enum {
			eRaw,
			eWave,
			eAIFF,
			eAuto,
			eUnknown
		}; 
		
		virtual Component* Species() const
		{
			// TODO: This is a xapusa, see also FDFilterGen.hxx
			return (Component*) new EAudioFileType(eRaw);
		};
		
	};

	/** Configuration class for AudioFileIn/Out classes
	 */
	struct AudioFileConfig: public ProcessingConfig {
	public:
		DYNAMIC_TYPE_USING_INTERFACE
			(AudioFileConfig, 8, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Filename);
		DYN_ATTRIBUTE (2, public, EAudioFileType, Filetype);
		DYN_ATTRIBUTE (3, public, TData, SampleRate);
		DYN_ATTRIBUTE (4, public, short, Channels);
		DYN_ATTRIBUTE (5, public, bool, KeepFrameSizes);
		DYN_ATTRIBUTE (6, public, int, FrameSize);
		DYN_ATTRIBUTE (7, public, int, StartFrame);
	protected:
		/** Dynamic type initialization: All attributes are
		 *   instantiated.
		 */
		void DefaultInit(void);
	};

};//namespace CLAM

#endif // _AudioFile_
