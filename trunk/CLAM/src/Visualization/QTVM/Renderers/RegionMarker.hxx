#ifndef __REGIONMARKER__
#define __REGIONMARKER__

#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class RegionMarker : public Renderer
		{
			public:
				RegionMarker();
				virtual ~RegionMarker();

				virtual void Render();

				void SetBegin(const TData& value);
				void SetEnd(const TData& value);
				TData GetBegin() const;
				TData GetEnd() const;

			private:
				TData _data[2];

				int GetDrawMode(int index);
				void DrawRegion(int mode);
		};
	}
}

#endif
