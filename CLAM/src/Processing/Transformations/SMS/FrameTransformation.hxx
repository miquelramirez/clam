#ifndef _FrameTransformation_
#define _FrameTransformation_

#include "Processing.hxx"

namespace CLAM
{
	class Frame;

	class FrameTransformation : public Processing
	{
		public:
			virtual bool Do(const Frame& in,Frame& out) = 0; 
	};

} // namespace CLAM

#endif
