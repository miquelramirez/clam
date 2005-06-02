#ifndef _FrameTransformation_
#define _FrameTransformation_

#include "Processing.hxx"
#include "InControl.hxx"

namespace CLAM
{
	class Frame;

	class FrameTransformation : public Processing
	{
		public:
			FrameTransformation():mAmount("Amount", this){}
			virtual bool Do(const Frame& in,Frame& out) = 0; 
		protected:
			InControl mAmount;
	};

} // namespace CLAM

#endif
