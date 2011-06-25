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

#ifndef __BINARYCONTROLOP__
#define __BINARYCONTROLOP__

#include "Processing.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include <iosfwd>

namespace CLAM {


	class BinaryControlOpConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (BinaryControlOpConfig, 1,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);

	protected:

		void DefaultInit()
		{
			AddName();
			UpdateData();
		}
	};

	template < typename BinOp >
	class BinaryControlOp:public Processing
	{
	private:
		TControlData mOutValue;
		TControlData mFirstParmLastValue;
		TControlData mSecondParmLastValue;
		FloatOutControl mOutput;
		BinaryControlOpConfig mConfig;
		BinOp mOperation;

		inline const char *GetClassName() const;

		void HandleFirst( TControlData incoming_parm )
		{
			mFirstParmLastValue = incoming_parm;
			mOutValue = DoOperation();
			Do();
		}

		void HandleSecond( TControlData incoming_parm )
		{
			mSecondParmLastValue = incoming_parm;
			mOutValue = DoOperation();
			Do();
		}

	public:
		typedef BinaryControlOpConfig Config;
		FloatInControl mFirst;
		FloatInControl mSecond;
	
		BinaryControlOp(const BinaryControlOpConfig& cfg = Config())
			: mOutValue(0)
			, mFirstParmLastValue( BinOp::IdentityElement )
			, mSecondParmLastValue(BinOp::IdentityElement)
  			, mOutput( "output", this )
			, mFirst( "first_parm", this, &BinaryControlOp::HandleFirst )
			, mSecond( "second_parm", this, &BinaryControlOp::HandleSecond )
		{
			Configure(cfg);
		}

		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig(mConfig, c);
			return true;
		}

		const ProcessingConfig &GetConfig() const { return mConfig;};

		TControlData DoOperation()
		{
			return mOperation( mFirstParmLastValue, mSecondParmLastValue );
		}

		bool Do() 
		{
			mOutput.SendControl( mOutValue );	
			return true ;
		}
	};

	template < typename BinOp >
	inline const char *BinaryControlOp<BinOp>::GetClassName() const { return "BinaryControlOperation"; }

}

#endif // BinaryControlOp.hxx

