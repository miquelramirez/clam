#ifndef _FrameTransformation_
#define _FrameTransformation_

#include "Processing.hxx"
#include "InControl.hxx"

#include "FrameTransformationConfig.hxx"

namespace CLAM
{
	class Frame;

	class FrameTransformation : public Processing
	{
		public:
			FrameTransformation()
				: mAmount("Amount", this) //soon to be deprecated
			{
			}
			virtual bool Do(const Frame& in,Frame& out) = 0; 
			
			const ProcessingConfig& GetConfig() const { return mConfig; }

			virtual bool ConcreteStart(){ return InitControls();}
			
			/** Virtual function to override when needing to give different default values to the controls*/
			virtual bool InitControls()
			{ 
				GetInControl("Amount").DoControl(0.);
				return true;
			}

		protected:
		
			FrameTransformationConfig mConfig;
			
			InControl mAmount; //soon to be deprecated
	};
	
	//@todo: could also add ports here
	template <typename T>
	class FrameTransformationTmpl : public FrameTransformation
	{
	public:	
		virtual bool Do(const T&, T&) = 0;
	};

} // namespace CLAM

#endif

