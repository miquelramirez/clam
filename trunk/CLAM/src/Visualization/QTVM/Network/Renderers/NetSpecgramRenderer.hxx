#ifndef __NETSPECGRAMRENDERER__
#define __NETSPECGRAMRENDERER__

#include <vector>
#include "Renderer.hxx"

namespace CLAM
{
    namespace VM
    {
		class NetSpecgramRenderer : public Renderer
		{
		public:
			NetSpecgramRenderer();
			~NetSpecgramRenderer();

	    
			void SetIndexes(const TIndex& leftIndex1, const TIndex& rightIndex1,
							const TIndex& leftIndex2, const TIndex& rightIndex2,
							const TIndex& begin);

			void SetData(std::vector< std::vector<Color> >& data, 
						 const TIndex& bottom, 
						 const TIndex& top);

			void Render();

		private:
			std::vector< std::vector<Color> > mData;
			TIndex mBottomBound, mTopBound;
			TIndex mLeftIndex1, mRightIndex1, mLeftIndex2, mRightIndex2;
			TIndex mBegin;

		};
    }
}

#endif

