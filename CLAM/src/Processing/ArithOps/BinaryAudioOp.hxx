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

#ifndef _BINARY_AUDIO_OP_H_
#define _BINARY_AUDIO_OP_H_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Audio.hxx"
#include <typeinfo> // std::bad_cast
#include "ErrProcessingObj.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

#include <iostream>

namespace CLAM 
{
	
	class BinaryAudioOpConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (BinaryAudioOpConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
	protected:
		void DefaultInit()
		{
			/* the dynamic type takes care if we add an existing attr .. */
			
			AddName();
			
			
			/* All Attributes are added */
			UpdateData();
			
		}
		
	};
	
	template < typename BinOp >
	class BinaryAudioOp
		: public Processing 
	{
		BinaryAudioOpConfig mConfig;
		BinOp	     mOperation;
		
		const char *GetClassName() const {return "BinaryAudioOperation";}

		/** Config change method
		 * @pre argument should be an SpecAdderConfig object.
		 */
		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			try {
				mConfig = dynamic_cast<const BinaryAudioOpConfig&>(c);	    
			}
			catch (std::bad_cast)
			{
				CLAM_ASSERT(false,"Config should be a BynariaAudioOpConfig")
			}
			return true;
			
		}
		
		InPortTmpl<Audio> mFirstInput;
		InPortTmpl<Audio> mSecondInput;
		OutPortTmpl<Audio> mOutput;
		

	public:

		BinaryAudioOp()
			:mFirstInput("First Audio Input",this,1)
			 ,mSecondInput("Second Audio Input",this,1)
			 ,mOutput("Audio Output",this,1)
		{
			Configure( BinaryAudioOpConfig() );
		}
		
		BinaryAudioOp(const BinaryAudioOpConfig &c)
			:mFirstInput("First Audio Input",this,1)
			 ,mSecondInput("Second Audio Input",this,1)
			 ,mOutput("Audio Output",this,1)
									                 
		{
				Configure( c );
		}

		~BinaryAudioOp()
		{
		}

		const ProcessingConfig &GetConfig() const { return mConfig;}

		void Check(const Audio& in1, const Audio& in2, const Audio& out)
		{
			CLAM_ASSERT(in1.GetSize() <= in2.GetSize(),
				"BinaryAudioOperation::Do(): Incompatible Input Audio Data Sizes");
			CLAM_ASSERT(in1.GetSize() <= out.GetSize(),
				"BinaryAudioOperation::Do(): Incompatible Output Audio Data Size");
			
		}

		bool Do(void)
		{
			bool res = Do(mFirstInput.GetData(),
				      mSecondInput.GetData(),
				      mOutput.GetData());
			mFirstInput.LeaveData();
			mSecondInput.LeaveData();
			mOutput.LeaveData();
			return res;
		}

		bool Do(const Audio& in1, const Audio& in2, Audio& out)
		{


			int size = in1.GetSize();
			int i;

			Check(in1,in2,out);

			TData* inb1 = in1.GetBuffer().GetPtr();
			TData* inb2 = in2.GetBuffer().GetPtr();
			TData* outb = out.GetBuffer().GetPtr();

			for (i=0;i<size;i++) 
			{
				*outb++ = mOperation( *inb1++ , *inb2++ );
			}

			return true;
		}

		// Port interfaces.

		bool SetPrototypes(const Audio& in1, const Audio& in2, const Audio& out)
		{
			return false;
		}

		bool SetPrototypes()
		{
			return false;
		}

		bool UnsetPrototypes()
		{
			return true;
		}

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:
	};
	
}

#endif //	_BINARY_AUDIO_OP_H_
