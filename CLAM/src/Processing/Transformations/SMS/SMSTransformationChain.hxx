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


#include "SegmentTransformation.hxx"
#include "SMSTransformationChainConfig.hxx"
#include "ProcessingComposite.hxx"
#include "ProcessingData.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControlTmplArray.hxx"
#include "ProcessingFactory.hxx"
#include "Array.hxx"

#include "FrameTransformation.hxx"

namespace CLAM {

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
		SMSTransformationChainConfig* mpConfig;
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
			CLAM_DEBUG_ASSERT(IsRunning(),
				"SMSTransformationChain: Do(): Not in execution mode");

			bool result=true;
			int i=0;
			//We iterate through all chainees and call their Do()
			for (iterator obj=composite_begin(); obj!=composite_end(); obj++,i++)
			{
				Processing & proc = *(*obj);
				SegmentTransformation& trans = dynamic_cast<SegmentTransformation&>(proc);
				//TODO have a list instead of being a composite
				
				if((*mpOnCtrlArray)[i].GetLastValue()||i==0||i==int(composite_size())-1)
				//Note: First and last chainee's will always be active regartheless the value
				//of their On control.
				{
					CLAM_DEBUG_ASSERT(proc.IsRunning(), "child is not running" );
					result &= trans.DoWithSegments();
				}
			}
			return result;
		}

		bool Do()
		{
			if(IsLastFrame())
			{
				printf("TransChain::Do() is last frame\n");
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
				SegmentTransformation&concreteObj = dynamic_cast<SegmentTransformation&>(processing);
				
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
			SegmentTransformation& concreteObj = dynamic_cast<SegmentTransformation&>( *(*(obj)) );
			concreteObj.AttachIn(*mpChainInput);
			
			obj=composite_end();
			obj--;
			SegmentTransformation& concreteObj2 = dynamic_cast<SegmentTransformation&>( *(*(obj)) );

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
			mpConfig=new SMSTransformationChainConfig(dynamic_cast<const SMSTransformationChainConfig&>(c));
			bool result=true;
			iterator obj;
			
			/** First we remove existing chainee if any*/
			obj=composite_begin();
			while(obj!=composite_end())
			{
				Remove(*(*obj));
				obj=composite_begin();
			}
			
			SMSTransformationChainConfig::iterator cfg;
			for(cfg=mpConfig->ConfigList_begin();cfg!=mpConfig->ConfigList_end();cfg++)
			{
				AddChainee((*cfg).GetConcreteClassName());
				obj=composite_end();
				obj--;
				result&=(*obj)->Configure((*cfg).GetConcreteConfig());
			}
			CLAM_ASSERT(mpConfig->GetConfigurations().size()==composite_size(),"Number of configurations should be the same as number of children");
		
			//TODO: right now there is no way to add or remove controls than to instantiate control array again
			CLAM_ASSERT(mpConfig->GetOnArray().Size()==(int)composite_size(),"SMSTransformationChain::ConcreteConfigure: On array does not have same size as number of configurations");
			TSize nControls=composite_size();
			if(mpOnCtrlArray) delete mpOnCtrlArray;
			mpOnCtrlArray= new InControlTmplArray<SMSTransformationChain>(nControls,"OnControlArray",this,NULL);
			
			for(int i=0;i<nControls;i++)
			{
				(*mpOnCtrlArray)[i].DoControl(mpConfig->GetOnArray()[i]);
			}
			return result;
		}
		
		/** Returns first transformation in chain belonging to a given class
		 * 	or NULL if not found
		 * */
		SegmentTransformation* GetTransformation(const std::string& name)
		{
			for(iterator obj=composite_begin(); obj!=composite_end(); obj++)
			{
				if(name == (*obj)->GetClassName()) return dynamic_cast<SegmentTransformation*>((*obj));;
			}
			return NULL;
		}
		

	private:

		void AddChainee(const std::string& classname)
		{
			//TODO: Instead of connecting controls, use the publishing mechanism
			//TODO2: If all amount controls were named the same I might be able to get rid of the string comparison
			ProcessingFactory & theFactory = ProcessingFactory::GetInstance();
			
			if ( classname=="SMSFreqShift") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* freqshift = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(freqshift);
				wrapper->mAmountCtrl.PublishInControl(freqshift->GetInControls().Get("Shift Steps"));
//				ConnectControls(*wrapper,"Out Control", *freqshift, "Shift Amount");
				Insert( *wrapper );
				return;
			}
			
			if ( classname=="SMSSinusoidalGain") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* singain = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(singain);
				wrapper->mAmountCtrl.PublishInControl(singain->GetInControls().Get("Gain"));
//				ConnectControls(*wrapper,"Out Control", *singain, "Gain Amount");
				Insert( *wrapper );
				return;
			}

			if ( classname=="SMSResidualGain") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* resgain = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(resgain);
				wrapper->mAmountCtrl.PublishInControl(resgain->GetInControls().Get("Gain"));
//				ConnectControls(*wrapper,"Out Control", *resgain, "Gain Amount");
				Insert( *wrapper );
				return;
			}

			if ( classname=="SMSPitchShift") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* pitchshift = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(pitchshift);
				wrapper->mAmountCtrl.PublishInControl(pitchshift->GetInControls().Get("PitchSteps"));
//				ConnectControls(*wrapper,"Out Control", *pitchshift, "Shift Amount");
				Insert( *wrapper );
				return;
			}

			if ( classname=="SMSOddEvenHarmonicRatio") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* oddEvenHarmRatio = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(oddEvenHarmRatio);
				wrapper->mAmountCtrl.PublishInControl(oddEvenHarmRatio->GetInControls().Get("Amount"));
//				ConnectControls(*wrapper,"Out Control", *oddEvenHarmRatio, "Odd Harmonics Factor");
				Insert( *wrapper );
				return;
			}

			if ( classname=="SMSSpectralShapeShift") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* spectralShapeShift = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(spectralShapeShift);
				wrapper->mAmountCtrl.PublishInControl(spectralShapeShift->GetInControls().Get("Amount"));
//				ConnectControls(*wrapper,"Out Control", *spectralShapeShift, "Shift Amount");
				Insert( *wrapper );
				return;
			}

			if ( classname=="SMSPitchDiscretization") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* pitchDiscretization = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(pitchDiscretization);
				Insert( *wrapper );
				return;
			}

			if ( classname=="SMSGenderChange") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* genderChange = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(genderChange);
				wrapper->mAmountCtrl.PublishInControl(genderChange->GetInControls().Get("Amount"));
//				ConnectControls(*wrapper,"Out Control", *genderChange, "Gender Factor");
				Insert( *wrapper );
				return;
			}

			if ( classname=="SMSSineFilter") 
			{
				SegmentTransformation* wrapper = new SegmentTransformation;	
				Processing * proc = theFactory.Create(classname);
				FrameTransformation* sineFilter = dynamic_cast<FrameTransformation*>(proc); 
				wrapper->WrapFrameTransformation(sineFilter);
				Insert( *wrapper );
				return;
			}
			Insert( *( theFactory.Create( classname ) ) );
		}
		/** Helper method for updating frame counters both in ports and in internal data*/
		void NextFrame()
		{
			mpChainInput->mCurrentFrameIndex++;
		}
		/** Returns true if current frame pointer at input port is pointing past the last
		 *	frame in the segment
		 */
		bool IsLastFrame()
		{
			for(iterator obj=composite_begin(); obj!=composite_end(); obj++)
			{
				SegmentTransformation* transf =dynamic_cast<SegmentTransformation*>((*obj));
				if(!transf->IsLastFrame()) return false;
			}
			return true;

		}


	};


} // namespace CLAM

#endif // _SMSTransformationChain_

