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
			FrameTransformation():mAmount("Amount", this){}
			virtual bool Do(const Frame& in,Frame& out) = 0; 
			
			const ProcessingConfig& GetConfig() const { return mConfig; }

			virtual bool ConcreteConfigure(const ProcessingConfig& c) { return true; }

		protected:
		
			FrameTransformationConfig mConfig;
			
			InControl mAmount;
	};

} // namespace CLAM

#endif
