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
#ifndef _GRID_GEN_
#define _GRID_GEN_

#include <CLAM/Processing.hxx>
#include <CLAM/Array.hxx>
#include "TimeIndex.hxx"
#include <CLAM/InControl.hxx>

namespace CLAM
{

	namespace RhythmDescription
	{

		class PulseGridGeneratorConfig : public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (PulseGridGeneratorConfig, 3,ProcessingConfig);
			DYN_ATTRIBUTE (0, public,TData, Start);
			DYN_ATTRIBUTE (1, public,TData, Gap);
			DYN_ATTRIBUTE (2, public,TData, End);
		protected:
			void DefaultInit();	
		};

		class PulseGridGenerator : public Processing
		{

		public:
			PulseGridGenerator();

			~PulseGridGenerator() {}

			const ProcessingConfig &GetConfig() const { return mConfig;}

			bool Do(void);
			bool Do(Array<TimeIndex>& out);

			const char *GetClassName() const;

		protected:

			bool ConcreteConfigure(const ProcessingConfig&);
		private:
			FloatInControl mStart;
			FloatInControl mGap;
			FloatInControl mEnd;
			PulseGridGeneratorConfig mConfig;
		};

	}

} // namespace CLAM

#endif // _GRID_GEN_

