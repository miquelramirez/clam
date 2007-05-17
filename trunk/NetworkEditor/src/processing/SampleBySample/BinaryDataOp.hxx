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

#ifndef _BINARY_DATA_OP_H_
#define _BINARY_DATA_OP_H_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Audio.hxx"
#include <typeinfo> // std::bad_cast
#include "SampleDefines.hxx"

#include <iostream>

namespace CLAM
{
	
	class BinaryDataOpConfig: public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (BinaryDataOpConfig, 0, ProcessingConfig);
	};
	
	template < typename BinOp >
	class BinaryDataOp
		: public CLAM::Processing 
	{
		BinaryDataOpConfig mConfig;
		BinOp	     mOperation;
		
		/** Config change method
		 * @pre argument should be an SpecAdderConfig object.
		 */
		bool ConcreteConfigure(const CLAM::ProcessingConfig& c)
		{
			try {
				mConfig = dynamic_cast<const BinaryDataOpConfig&>(c);	    
			}
			catch (std::bad_cast)
			{
				CLAM_ASSERT(false,"Config should be a BynariaDataOpConfig");
			}
			return true;
			
		}
		
		InDataPort mFirstInput;
		InDataPort mSecondInput;
		OutDataPort mOutput;
		

	public:

		BinaryDataOp()
			:mFirstInput("First Data Input",this),
			 mSecondInput("Second Data Input",this),
			 mOutput("Data Output",this)
		{
			Configure( BinaryDataOpConfig() );
		}
		
		BinaryDataOp(const BinaryDataOpConfig &c)
			:mFirstInput("First Data Input",this),
			 mSecondInput("Second Data Input",this),
			 mOutput("Data Output",this)
									                 
		{
				Configure( c );
		}

		~BinaryDataOp()
		{
		}

		const CLAM::ProcessingConfig &GetConfig() const { return mConfig;}

		virtual const char *GetClassName() const {return "BinaryDataOperation";}
		
		bool Do(void)
		{
			bool res = Do(mFirstInput.GetData(),
				      mSecondInput.GetData(),
				      mOutput.GetData());
			mFirstInput.Consume();
			mSecondInput.Consume();
			mOutput.Produce();
			return res;
		}

		bool Do(const CLAM::TData& in1, const CLAM::TData& in2, CLAM::TData& out)
		{


			int i;

			out = mOperation( in1 , in2 );

			return true;
		}

		bool MayDisableExecution() const {return true;}

	private:
	};
	
}

#endif //	_BINARY_Data_OP_H_
