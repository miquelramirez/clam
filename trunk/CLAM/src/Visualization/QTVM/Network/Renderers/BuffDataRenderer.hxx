#ifndef __BUFFDATARENDERER__
#define __BUFFDATARENDERER__

#include "Array.hxx"
#include "Renderer.hxx"

namespace CLAM
{
    namespace VM
    {
		class BuffDataRenderer : public Renderer
		{
		public:
			BuffDataRenderer();
			~BuffDataRenderer();

			void SetData(const DataArray& data);
			void SetIndexes(const TIndex& left1, const TIndex& right1, 
							const TIndex& left2, const TIndex& right2, 
							const TIndex& begin);
			void Render();

			void Init();

		private:
			DataArray mData;
			TIndex mLeftIndex1, mRightIndex1, mLeftIndex2, mRightIndex2;
			TIndex mBegin;
		};
    }
}

#endif

