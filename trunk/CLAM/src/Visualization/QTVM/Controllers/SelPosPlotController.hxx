#ifndef __SELPOSPLOTCONTROLLER__
#define _SELPOSPLOTCONTROLLER__

#include "VMColor.hxx"
#include "Dial.hxx"
#include "SegmentationMarksPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SelPosPlotController : public SegmentationMarksPlotController
		{
			Q_OBJECT
			
			public:
				SelPosPlotController();
				virtual ~SelPosPlotController();

				virtual void SetSelPos(const TData& value);
				virtual void Draw();

				void SetDialColor(Color c);
				
			protected:
				TData _sampleRate;
				TData _duration;

				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);
				void UpdateDial(const TData& value);
				TData GetDialPos() const;
			
			private:
				Dial _dial;

		};
	}
}

#endif
