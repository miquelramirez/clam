#ifndef _FrameTransformationConfig_
#define _FrameTransformationConfig_

#include "Processing.hxx"
#include "BPF.hxx"

namespace CLAM
{

	class FrameTransformationConfig : public ProcessingConfig
	{
		public:
                DYNAMIC_TYPE_USING_INTERFACE (FrameTransformationConfig, 2,ProcessingConfig);
				DYN_ATTRIBUTE(0, public, bool, PreserveOuts);
				DYN_ATTRIBUTE (1, public, BPF, BPF);
		private:
			void DefaultInit()
			{
				AddPreserveOuts();
				UpdateData();
				SetPreserveOuts(false);
			}
	};

} // namespace CLAM

#endif

