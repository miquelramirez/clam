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
#include "RD_GridGen.hxx"

namespace CLAM
{

	namespace RhythmDescription
	{

		void PulseGridGeneratorConfig::DefaultInit()
		{
			AddStart();
			AddGap();
			AddEnd();
			/* All Attributes are added */
			UpdateData();
			SetStart(0);
			SetGap(1);
			SetEnd(10);
		}


		PulseGridGenerator::PulseGridGenerator() :
			mStart("Start",this),
			mGap("Gap",this),
			mEnd("End",this)
		{
		}

		// Configure the Processing Object according to the Config object
		bool PulseGridGenerator::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			mStart.DoControl(mConfig.GetStart());
			mGap.DoControl(mConfig.GetGap());
			mEnd.DoControl(mConfig.GetEnd());
			return true;
		}

		bool  PulseGridGenerator::Do(void) 
		{
			return false;
		}

		const char* PulseGridGenerator::GetClassName() const
		{
			return "PulseGridGenerator";
		}

		bool PulseGridGenerator::Do(Array<TimeIndex>& out)
		{
			TData start=mStart.GetLastValue();
			TData gap=mGap.GetLastValue();
			TData end=mEnd.GetLastValue();
	
			int nUnits = 1+(end-start)/gap; //cast to an integer

			CLAM_ASSERT( out.Size() == nUnits,
				     "PulseGridGenerator::Do() : output array of TimeIndex has not enough space to hold the grid" );

			out[0].SetWeight(1.0);
			out[0].SetPosition(start);


			for (int i=1;
			     i < nUnits;
			     i++)
			{
				out[i].SetWeight(1.0);//All weights equal 1
				out[i].SetPosition( (i*gap) + start);
			}
			return true;		
		}

	} // namespace RhythmDescription
	
} // namespace CLAM

