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


#ifndef _SegmentSMSHarmonizer_
#define _SegmentSMSHarmonizer_

#include "SMSPitchShift.hxx"
#include "SpectrumAdder2.hxx"
#include "FrameTransformation.hxx"


// TODO: this transformation needs to be ported to inherit from FrameTransformation instead of SegmentTransformation
// 		 also, a solution has to be figured out to make the transformation controllable via ports

namespace CLAM{


	class SegmentSMSHarmonizer: public FrameTransformation
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SegmentSMSHarmonizer";}
		
		FloatInControl mIndexCtl;//says what the amount sent as control is modifying
		FloatInControl mTransCtl;
		/** xamat:adding residual does not improve results much and adds a lot of overhead, there should
		 *	probably be a configuration parameter to control whether we want to add residual or not, but that
		 *	would mean changing the kind of configuration. For the time being the output residual is the input.*/
		FloatInControl mIgnoreResidualCtl;
		FloatInControl mUpdateBPFCtl;///< "boolean" control used to say that we want to update BPF
	public:
			
		void UpdateBPF(TControlData value)
		{
			CLAM::BPF& bpf= mConfig.GetBPF();
			//this should never happen, it should be initialized at configuration time
			if(bpf.Size()==0)
			{
				InitBPF();
			}
			
			bpf.SetValue((int)mIndexCtl.GetLastValue(), mTransCtl.GetLastValue());
		}
		void IgnoreResidual(TControlData value)
		{
			mPitchShift.mIgnoreResidual.DoControl(value);
		}
	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SegmentSMSHarmonizer()
			: mIndexCtl("Index", this)
			, mTransCtl("Transposition",this)
			, mIgnoreResidualCtl("IgnoreResidual",this, &SegmentSMSHarmonizer::IgnoreResidual)
			, mUpdateBPFCtl("UpdateBPF", this, &SegmentSMSHarmonizer::UpdateBPF)
		{
			Configure(FrameTransformationConfig());
			mTmpFrame.AddAll();
			mTmpFrame.UpdateData();
			mTmpFund.AddElem();
		}
		
		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			InitBPF();
			mPitchShift.Configure(FrameTransformationConfig());
			//By default we ignore residual!!
			mIgnoreResidualCtl.DoControl(1.);
			return true;
		}

		/** Destructor of the class*/
 		~SegmentSMSHarmonizer()
		{}
		
		bool Do()
		{
			CLAM_ASSERT(false, "Do with ports not implemented");
			return false;
		}
		
		bool Do(const Frame& in, Frame& out);
	private:
		SMSPitchShift mPitchShift;
		SpectrumAdder2 mSpectrumAdder;
		void AddFrame(const Frame& in1, const Frame& in2, Frame& out);
		void Gain(Frame& inputFrame, TData gain);
		
		Fundamental mTmpFund;
		Frame mTmpFrame;
		
		void InitBPF()
		{
			if (!mConfig.HasBPF())
			{
				mConfig.AddBPF();
				mConfig.UpdateData();
			}
			if(mConfig.GetBPF().Size()==0)//else we asume that the user has initialized it before
			{
				BPF& bpf=mConfig.GetBPF();
				bpf.Resize(10);
				bpf.SetSize(10);
				int i;
				//we add ten voices with gain going from -30 to +30 but no transposition (note that X controls gain and Y transposition)
				for (i=0; i< 10; i++)
				{
					bpf.SetValue(i,1);
					bpf.SetXValue(i,(i-5)*6);
				}
			}
		}
		
	
	};		
};//namespace CLAM

#endif // _SegmentSMSHarmonizer_

