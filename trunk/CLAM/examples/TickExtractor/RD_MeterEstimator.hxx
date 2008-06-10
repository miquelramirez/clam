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

#ifndef _RD_MeterEstimator_
#define _RD_MeterEstimator_

#include <CLAM/ProcessingComposite.hxx>
#include "Pulse.hxx"
#include <CLAM/Segment.hxx>
#include <CLAM/SegmentDescriptors.hxx>
#include "RD_AutoCorrelationTD.hxx"
#include <CLAM/DescriptorComputation.hxx>
#include <fstream>

namespace CLAM
{

	class Audio;

	namespace RhythmDescription
	{
		class Meter;

		class MeterEstimatorConfig : public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (MeterEstimatorConfig, 5 ,ProcessingConfig);
			DYN_ATTRIBUTE ( 0, public, TData, TempoLimInf);
			DYN_ATTRIBUTE ( 1, public, TData, TempoLimSup);
			DYN_ATTRIBUTE ( 2, public, int,   ACFUpperLimit);
			DYN_ATTRIBUTE ( 3, public, bool , AutomaticIntegTime);
			DYN_ATTRIBUTE ( 4, public, int,   ACFIntegrationTime);
		protected:
			void DefaultInit();
		};

		class MeterEstimator : public ProcessingComposite
		{
		public:
			MeterEstimator();
			~MeterEstimator();

			const ProcessingConfig &GetConfig() const;
			const char *GetClassName() const;
			
			bool Do(void);
			bool Do(Audio& audioIn, const Pulse& beats, Meter& dataOut);

			std::ostream& Log();

		protected:
			bool ConcreteConfigure(const ProcessingConfig&);
			void CheckLogInitialization();

			
		private:
			MeterEstimatorConfig mConfig;

			//Internal POs
			AutoCorrelationTD mACF;

			DescriptorComputation mAudioDescGen;

			//PDs
			Segment mSegment;
			SegmentDescriptors mSegmentD;
	
			static std::ofstream  smLogFile;
			static bool           smLogInit;
			
			// Internal convenience methods
			void AttachChildren();
			bool ConfigureChildren();
			void ConfigureData();

			

		};

	} // namespace RhythmDescription

} // namespace CLAM

#endif // _MeterEstimator_

