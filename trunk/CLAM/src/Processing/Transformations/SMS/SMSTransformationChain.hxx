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
			if(IsLastFrame())
			{
				printf("TransCHain::Do() is last frame\n");
				return false;
			}
			NextFrame();
			return ProcessingChain<Segment>::Do();
		}
		bool Previously_ProcessingChainConcreteStart()
		{
			
			// init temp segments array
			for(int i=0;i<mpTmpDataArray.Size();i++)
				if(mpTmpDataArray[i])
				{
					delete mpTmpDataArray[i];
					mpTmpDataArray[i]=NULL;
				}
			mpTmpDataArray.SetSize(0);
			
			Segment* pCurrentData;
			pCurrentData=new Segment(*mpChainInput);
			mpTmpDataArray.AddElem(pCurrentData);
			iterator obj;
			
			// set up connections:     inSegment<--trans-->outSegment
			
			for(obj=composite_begin();obj!=composite_end();obj++)
			{
				//connecting ports for non-supervised mode
				Processing & processing = *(*(obj));
				SMSTransformation &concreteObj = dynamic_cast<SMSTransformation&>(processing);
				
				concreteObj.AttachIn(*pCurrentData);
				if(!(*obj)->CanProcessInplace())
				{
					printf("Can't process inplace: setting a temporal segment\n");
					pCurrentData=new Segment(*mpChainInput);
					mpTmpDataArray.AddElem(pCurrentData);
				}
				concreteObj.AttachOut(*pCurrentData);
			}
			// now we have to restore the inSegment of the first child
			obj=composite_begin();
			SMSTransformation& concreteObj = dynamic_cast<SMSTransformation&>( *(*(obj)) );
			concreteObj.AttachIn(*mpChainInput);
			
			obj=composite_end();
			obj--;
			SMSTransformation& concreteObj2 = dynamic_cast<SMSTransformation&>( *(*(obj)) );

			concreteObj2.AttachOut( *mpChainOutput);
			return ProcessingComposite::ConcreteStart();

		}
		
		bool ConcreteStart()
		{
//			bool ret= ProcessingChain<Segment>::ConcreteStart();
			bool ret= Previously_ProcessingChainConcreteStart();
			
			int i;
			for(i=0;i<mpTmpDataArray.Size();i++)
				mpTmpDataArray[i]->mCurrentFrameIndex=0;
			mpChainInput->mCurrentFrameIndex=0;
			return ret;
		}

	protected:
		/** Helper method for updating frame counters both in ports and in internal data*/
		void NextFrame()
		{
			mpChainInput->mCurrentFrameIndex++;
			int i = mpChainInput->mCurrentFrameIndex++;
			printf("TransChain::NextFrame input frame index=%d at=%x\t", i, int(&(mpChainInput->GetFramesArray()[i]) ));
		}
		/** Returns true if current frame pointer at input port is pointing past the last
		 *	frame in the segment
		 */
		bool IsLastFrame()
		{
			iterator obj;
			int i=0;
			for(obj=composite_begin(); obj!=composite_end(); i++, obj++)
			{
				SMSTransformation* transf=static_cast<SMSTransformation*>((*obj));
				if(!transf->IsLastFrame()) return false;
			}
			return true;

		}


	};

};//namespace CLAM

#endif // _SMSTransformationChain_
