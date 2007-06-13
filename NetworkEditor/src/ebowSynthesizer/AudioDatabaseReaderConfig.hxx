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

#ifndef _AudioDatabaseReaderConfig_
#define _AudioDatabaseReaderConfig_

#include <typeinfo> // for bad_cast definition
#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Enum.hxx"
#include "Filename.hxx"

namespace CLAM {

	/** Configuration class for AudioFileIn/Out classes
	 */
	struct AudioDatabaseReaderConfig: public ProcessingConfig {
	public:
		DYNAMIC_TYPE_USING_INTERFACE
			(AudioDatabaseReaderConfig, 4, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, Filename, Filename);
		DYN_ATTRIBUTE (1, public, TData, SampleRate);
		DYN_ATTRIBUTE (2, public, short, Channels);
		DYN_ATTRIBUTE (3, public, int, FrameSize);
	protected:
		/** Dynamic type initialization: All attributes are
		 *   instantiated.
		 */
		void DefaultInit(void);
	};

};//namespace CLAM

#endif // _AudioDatabaseReaderConfig_

