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

#ifndef _Normalization_
#define _Normalization_

#include "Processing.hxx"
#include "Audio.hxx"


namespace CLAM
{

	class NormalizationConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (NormalizationConfig, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, int, Type);
		DYN_ATTRIBUTE (2, public, TSize, FrameSize);

		void DefaultInit(void);
	};
	
	class Normalization: public Processing
	{
	private:
		NormalizationConfig mConfig;

		int mType;
		TSize mFrameSize;
		
		bool ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast);

	public:

		Normalization();
		Normalization(NormalizationConfig& c);
		~Normalization();

	

		bool Do(void);

		bool Do(Audio &in) throw(ErrProcessingObj);
		void sort(DataArray& list, int size);
		void swap(TData& a, TData& b);
		
		const ProcessingConfig &GetConfig() const { return mConfig;}

	};
	
}

#endif


