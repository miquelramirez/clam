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

#ifndef __FILTERCONTROLKNOBS__
#define __FILTERCONTROLKNOBS__

#include "FilterKnob.hxx"
#include "GeometryKit.hxx"
#include "DataTypes.hxx"

using CLAM::TData;

namespace CLAMGUI
{

	class GainKnob
		: public FilterKnob
	{
	public:
		
		GainKnob( const Geometry& g, const char* label = "Gain" )
			: FilterKnob( g, label )
		{
			InitKnob();
		}

		void InitKnob();

	};


class SlopeKnob
		: public FilterKnob
	{
	public:
		
		SlopeKnob( const Geometry& g, const char* label = "Knob" )
			: FilterKnob( g, label )
		{
			InitKnob();
		}

		void InitKnob();
	
	};

class LowcutoffKnob
		: public FilterKnob
	{
		TData           mSampleRate;
	public:
		
		LowcutoffKnob( TData maxfreq, const Geometry& g, const char* label = "Knob" )
			: FilterKnob( g, label )
		{
			SetSamplingRate( maxfreq );
			InitKnob();
		}

		inline void SetSamplingRate( TData value )
		{
			mSampleRate = value;
		}

		void InitKnob();

		void SatisfyConstriction( TData new_value );

		void ConstrainKnob( Fl_Knob* k );
 

	};


class HighcutoffKnob
		: public FilterKnob
	{
		TData            mSampleRate;
	public:
		
		HighcutoffKnob( TData maxfreq, const Geometry& g, const char* label = "Higher Cutoff Knob" )
			: FilterKnob( g, label )
		{
			SetSamplingRate( maxfreq );
			InitKnob();
		}

		inline void SetSamplingRate( TData value )
		{
			mSampleRate = value;
		}

		void InitKnob();
 
		void SatisfyConstriction( TData new_value );

		void ConstrainKnob( Fl_Knob* k );

	};


}

#endif // FilterControlKnobs.hxx
