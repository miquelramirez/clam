#ifndef __SINTRACKSRENDERER__
#define __SINTRACKSRENDERER__

#include "QtPalette.hxx"
#include "SinTracksDef.hxx"
#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksRenderer : public Renderer
		{
			public:
				SinTracksRenderer();
				virtual ~SinTracksRenderer();

				void SetPalette(QtPalette& palette);

				void SetSpanLists( SineTrackSpanEnds& heads, 
									SineTrackSpanEnds& tails,
									const TData& left,
									const TData& right );
				void Render();

			private:
				SineTrackSpanEnds* _heads;
				SineTrackSpanEnds* _tails;
				QtPalette* _palette;
				TData _left,_right;
		};
	}
}

#endif

