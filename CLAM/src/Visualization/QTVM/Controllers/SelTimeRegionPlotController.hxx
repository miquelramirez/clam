#ifndef __SELTIMEREGIONPLOTCONTROLLER__
#define __SELTIMEREGIONPLOTCONTROLLER__

#include "MediaTime.hxx"
#include "RegionMarker.hxx"
#include "SelPosPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SelTimeRegionPlotController : public SelPosPlotController
		{
			Q_OBJECT

			public:
				SelTimeRegionPlotController();
				virtual ~SelTimeRegionPlotController();

				virtual void SetSelPos(const TData& value);
				virtual void Draw();

				void SetRegionColor(Color c);
				void SetKeyShiftPressed(bool pr);

			signals:
				void selectedRegion(MediaTime);
			
			protected:
				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);

			private:
				RegionMarker _rMarker;
				bool _keyShiftPressed;
				
				TData GetBeginRegion() const;
				TData GetEndRegion() const;
				void SetBeginRegion(const TData& value);
				void SetEndRegion(const TData& value);

				MediaTime GetRegionTime() const;
		};
	}
}

#endif

