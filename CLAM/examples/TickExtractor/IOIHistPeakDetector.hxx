/*
 * Author: fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
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

#ifndef _IOI_HIST_PEAK_DETECTOR_
#define _IOI_HIST_PEAK_DETECTOR_

#include "Processing.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "IOIHistogram.hxx"

namespace CLAM 
{
	class Audio;
	
	namespace RhythmDescription
	{

		class IOIHistPeakDetectorConfig: public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (IOIHistPeakDetectorConfig, 3, ProcessingConfig);
			DYN_ATTRIBUTE (0,public, TData, Threshold); //from 0 to 1
			DYN_ATTRIBUTE (1,public, bool,  NormalizeWeights);
			DYN_ATTRIBUTE (2,public, TData, SampleRate );

			void Check();

		protected:
			void DefaultInit();
		};


		class IOIHistPeakDetector : public Processing 
		{
			IOIHistPeakDetectorConfig mConfig;
			const char *GetClassName() const;
			bool ConcreteConfigure(const ProcessingConfig&);
		public:
			IOIHistPeakDetector();

			const ProcessingConfig &GetConfig() const { return mConfig;}

			bool Do(void);
			bool Do(IOIHistogram& input, Array<TimeIndex>& out);
		};

	} // namespace RhythmDescription

} // namespace CLAM

#endif // _IOIHISTPEAKDETECTOR__

