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

#ifndef __FILTERKNOB__
#define __FILTERKNOB__

#include "FLKnob.hxx"
#include "CBL.hxx"
#include "GeometryKit.hxx"
#include "DataTypes.hxx"

using CLAM::TData;

namespace CLAMGUI
{

	typedef CBL::Functor0R< TData >       InputRcvdSig; // Notifies value introduced by user
	typedef CBL::Functor0                 ValueChangedCb;
	typedef CBL::Functor1< TData >        ConstrainSignal;

	class FilterKnob
		: public Fl_Knob
	{

	public:

		FilterKnob( const Geometry& g, const char* label )
			: Fl_Knob( g.GetX(), g.GetY(), g.GetW(), g.GetH(), label )
		{
			mPublisher = makeMemberFunctor0R( TData, *this, FilterKnob, GetValue );
		}

		InputRcvdSig& GetReceptionSignal() 
		{
			return mPublisher;
		}

		TData GetValue()
		{
			return TData( Fl_Knob::value() );
		}


		virtual void SetInitialValue( TData initial );

		virtual void SetValueRange( TData min, TData max );

		virtual void InitKnob() = 0;

		void ConnectKnob( const ValueChangedCb& s )
		{
			mInputSignal = s;
		}

		virtual void ConstrainKnob( Fl_Knob* )
		{
			mIsConstraining = false;
		}

		inline void Do()
		{
			redraw();
		}

	protected:
		
		static void SignalSender( Fl_Widget* knob );

	ValueChangedCb      mInputSignal;
	InputRcvdSig        mPublisher;
	bool                mIsConstraining;
	ConstrainSignal     mConstraintSignal;


	};

}

#endif // FilterKnob.hxx
