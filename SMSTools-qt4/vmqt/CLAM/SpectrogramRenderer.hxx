#ifndef __SPECTROGRAMRENDERER__
#define __SPECTROGRAMRENDERER__

#include <vector>
#include <CLAM/Renderer.hxx>

namespace CLAM
{
    namespace VM
    {
		enum SonogramCM { ColorSonogram=0, BlackWhiteSonogram };

		class SpectrogramRenderer : public Renderer
		{
		public:
			SpectrogramRenderer();
			~SpectrogramRenderer();

	    
			void SetIndexes(const TIndex& leftIndex, const TIndex& rightIndex,
							const TIndex& bottomIndex, const TIndex& topIndex);

			void SetColorData(std::vector< std::vector<Color> >& cdata);
			void SetBWData(std::vector< std::vector<Color> >& bwdata);

			void Render();

			void SetRenderingMode(CLAM::VM::SonogramCM colorMap);

		private:
			std::vector< std::vector<Color> > mColorData;
			std::vector< std::vector<Color> > mBWData;
			TIndex mLeftIndex, mRightIndex, mBottomIndex, mTopIndex;
			CLAM::VM::SonogramCM mColorMap;;

			void DrawColorData();
			void DrawBWData();
		};
    }
}

#endif

