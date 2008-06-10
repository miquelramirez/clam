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
#ifndef _GLOBAL_PULSE_
#define _GLOBAL_PULSE_

#include <CLAM/Processing.hxx>
#include "IOIHistogram.hxx"
#include <CLAM/WindowGenerator.hxx>

namespace CLAM 
{

	namespace RhythmDescription
	{

		class GlobalPulseGeneratorConfig: public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (GlobalPulseGeneratorConfig, 1,ProcessingConfig);
			DYN_ATTRIBUTE (0,public, TSize, GaussianSize);
		protected:
			void DefaultInit();
		};

		class GlobalPulseGenerator: public Processing 
		{
			GlobalPulseGeneratorConfig mConfig;

			WindowGenerator mWindowGen;
			DataArray       mWindow;

			const char *GetClassName() const;
			
			bool ConcreteConfigure(const ProcessingConfig&);

		public:
			GlobalPulseGenerator();

			~GlobalPulseGenerator();

			const ProcessingConfig &GetConfig() const { return mConfig;}

			bool Do(void);
			bool Do(const Array<TData>& input, IOIHistogram& out);

			bool SetPrototypes(Array<TData>& inputs,const Audio& out);
			bool MayDisableExecution() const {return true;}

		};

	} // namespace RhythmDescription

} // namespace CLAM

#endif //_GLOBAL_PULSE_

