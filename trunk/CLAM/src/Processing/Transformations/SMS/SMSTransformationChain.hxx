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

#ifndef _SMSTransformationChain_
#define _SMSTransformationChain_


#include "ProcessingChain.hxx"
#include "SMSTransformation.hxx"


namespace CLAM {


	/** SMSTransformationChain configuration. Right now it does not add any 
	 *	behaviour to its base class but will probably in the future.
	 *	@see ProcessingChainConfig
	 */
	class SMSTransformationChainConfig: public ProcessingChainConfig
	{
	public:
		virtual ~SMSTransformationChainConfig(){}
		bool AddSMSConfiguration(const SMSTransformationConfig& cfg,std::string type)
		{
			AddConfiguration(cfg,type);
			return true;
		}
		
	};

	/** ProcessingChain particularized to the needs of SMSTransformations. It is an
	 *	instance of the template class ProcessingChain, using Segment as the type for
	 *	input and output ports. It implements the supervised Do and a helper method for
	 *	updating the frame counter in the Segment.
	 *	@see ProcessingChain<T>
	 */
	class SMSTransformationChain:public ProcessingChain<Segment>
	{
	public:
		
		/** Default constructor. */
		SMSTransformationChain(){}
		/** Virtual Destructor. */
		virtual ~SMSTransformationChain(){}
		/** Supervised Do method. Does the same as the ProcessingChain Do method but adds a call
		 *	to the counter updating method.
		 *	@see ProcessingChain<T>::Do()
		*/
		bool Do()
		{
			if(IsLastFrame()) return false;
			bool result=ProcessingChain<Segment>::Do();
			NextFrame();
			return result;
		}

		bool ConcreteStart()
		{
			bool ret= ProcessingChain<Segment>::ConcreteStart();
			mpTmpData->mCurrentFrameIndex=0;
			mChainInput.GetData().mCurrentFrameIndex=0;
			return ret;
		}

	protected:
		/** Helper method for updating frame counters both in ports and in internal data*/
		void NextFrame()
		{
			mpTmpData->mCurrentFrameIndex++;
			mChainInput.GetData().mCurrentFrameIndex++;
			if(!mChainInput.IsConnectedTo(mChainOutput))
				mChainOutput.GetData().mCurrentFrameIndex++;
		}
		/** Returns true if current frame pointer at input port is pointing past the last
		 *	frame in the segment
		 */
		bool IsLastFrame()
		{
			return mChainInput.GetData().mCurrentFrameIndex>=mChainInput.GetData().GetnFrames();
		}


	};

};//namespace CLAM

#endif // _SMSTransformationChain_
