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
#include "ProcessingComposite.hxx"
#include "ProcessingData.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControlTmplArray.hxx"
#include "ProcessingFactory.hxx"
#include "Array.hxx"

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

		
	/** Using Segment as the type for *	input and output ports. 
	 * It implements the supervised Do and a helper method for
	 *	updating the frame counter in the Segment.
	 */
	class SMSTransformationChain: public ProcessingComposite
	{
		
		InControlTmplArray<SMSTransformationChain> *mpOnCtrlArray;

		/** Temporal ProcessingData used as an internal node for intermediate Processing */
		Array<Segment*> mpTmpDataArray;
		/** Internal configuration. A pointer is used because polymorphism may be used on it */
		ProcessingChainConfig* mpConfig;
		Segment* mpChainInput;
		Segment* mpChainOutput;


	public:

		void AttachIn(Segment& data) { mpChainInput = &data; }
		void AttachOut(Segment& data) { mpChainOutput = &data; }
		
		SMSTransformationChain() : 
			mpChainInput(0),
			mpChainOutput(0)
			{
				mpConfig=NULL;
				mpOnCtrlArray=NULL;
			}
		/** Destructor. Deletes temporal ProcessingData and Configuration and any previously
		 *	allocated Processing chainee. 
		 */
		virtual ~SMSTransformationChain(){
		
			for(int i=0;i<mpTmpDataArray.Size();i++)
				if(mpTmpDataArray[i]) delete mpTmpDataArray[i];
			if (mpConfig) delete mpConfig;
			
			for(iterator obj=composite_begin();obj!=composite_end();obj++)
			{
				delete (*obj);
			}
		}
		
		virtual bool DoChildren()
		{
			CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "ProcessingChain: Do(): Not in execution mode");

			if (GetExecState() == Disabled)	return true;
			
			bool result=true;
			int i=0;
			//We iterate through all chainees and call their Do()
			for (iterator obj=composite_begin(); obj!=composite_end(); obj++,i++)
			{
				Processing & proc = *(*obj);
				if((*mpOnCtrlArray)[i].GetLastValue()||i==0||i==int(composite_size())-1)
				//Note: First and last chainee's will always be active regartheless the value
				//of their On control.
				{
					CLAM_DEBUG_ASSERT(proc.GetExecState()==Running, "child is not running" );
					result &= proc.Do();
				}
			}
			return result;
		}
		bool Do()
		{
			if(IsLastFrame())
			{
				printf("TransCHain::Do() is last frame\n");
				return false;
			}
			NextFrame();
			return DoChildren();
		}

		bool ConcreteStart()
		{
			void InitSegmentArray(); 
			
			Segment* pCurrentData;
			pCurrentData=new Segment(*mpChainInput);
			mpTmpDataArray.AddElem(pCurrentData);
			
			// set up connections:     inSegment<--trans-->outSegment
			
			iterator obj;
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

			InitAllFrameIndex();
			
			return ProcessingComposite::ConcreteStart();

		}
		
		void InitSegmentArray() 
		{
			for(int i=0;i<mpTmpDataArray.Size();i++)
				if(mpTmpDataArray[i])
					delete mpTmpDataArray[i];
			mpTmpDataArray.SetSize(0);
		}

		void InitAllFrameIndex()
		{
			for(int i=0;i<mpTmpDataArray.Size();i++)
				mpTmpDataArray[i]->mCurrentFrameIndex=0;
			mpChainInput->mCurrentFrameIndex=0;
		}

		/** Accessor to On/off control. Turns on the specified control in the array.*/
		void TurnOn(TIndex index)
		{
			(*mpOnCtrlArray)[index].DoControl(1);
		}

		/** Accessor to On/off control. Turns on the specified control in the array.*/
		void TurnOff(TIndex index)
		{
			(*mpOnCtrlArray)[index].DoControl(0);
		}

		const ProcessingConfig& GetConfig() const
		{
			return *mpConfig;
		}

		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			mpConfig=new ProcessingChainConfig(dynamic_cast<const ProcessingChainConfig&>(c));
			bool result=true;
			iterator obj;
			
			/** First we remove existing chainee if any*/
			obj=composite_begin();
			while(obj!=composite_end())
			{
				Remove(*(*obj));
				obj=composite_begin();
			}
			
			ProcessingChainConfig::iterator cfg;
			for(cfg=mpConfig->ConfigList_begin();cfg!=mpConfig->ConfigList_end();cfg++)
			{
				AddChainee((*cfg).GetConcreteClassName());
				obj=composite_end();
				obj--;
				result&=(*obj)->Configure((*cfg).GetConcreteConfig());
			}
			CLAM_ASSERT(mpConfig->GetConfigurations().size()==composite_size(),"Number of configurations should be the same as number of children");
		
			//TODO: right now there is no way to add or remove controls than to instantiate control array again
			CLAM_ASSERT(mpConfig->GetOnArray().Size()==(int)composite_size(),"ProcessingChain::ConcreteConfigure: On array does not have same size as number of configurations");
			TSize nControls=composite_size();
			if(mpOnCtrlArray) delete mpOnCtrlArray;
			mpOnCtrlArray= new InControlTmplArray<SMSTransformationChain>(nControls,"OnControlArray",this,NULL);
			
			for(int i=0;i<nControls;i++)
			{
				(*mpOnCtrlArray)[i].DoControl(mpConfig->GetOnArray()[i]);
			}
			return result;
		}

	private:

		/** Adds a Processing chainee at the end of the chain. It instantiates a concrete Processing
		 *	class using input string as a switch.
		 */
		bool AddChainee(const std::string& type)
		{
			Insert( *( ProcessingFactory::GetInstance().Create( type ) ) );
						
			return true;
			
		}
		/** Helper method for updating frame counters both in ports and in internal data*/
		void NextFrame()
		{
			mpChainInput->mCurrentFrameIndex++;
			mpChainInput->mCurrentFrameIndex++;
		}
		/** Returns true if current frame pointer at input port is pointing past the last
		 *	frame in the segment
		 */
		bool IsLastFrame()
		{
			for(iterator obj=composite_begin(); obj!=composite_end(); obj++)
			{
				SMSTransformation* transf=static_cast<SMSTransformation*>((*obj));
				if(!transf->IsLastFrame()) return false;
			}
			return true;

		}


	};


} // namespace CLAM

#endif // _SMSTransformationChain_
