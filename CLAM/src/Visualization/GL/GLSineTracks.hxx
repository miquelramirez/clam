#ifndef __GLSINETRACKS__
#define __GLSINETRACKS__

#include "GLRenderer.hxx"
#include "SineTracksDef.hxx"

namespace CLAMVM
{
		class Fl_Palette;

		class GLSineTracks
				: public GLRenderer
		{
		public:
				GLSineTracks();
				virtual ~GLSineTracks();

				void SetPalette( Fl_Palette& pal )
				{
					mPalette = &pal;
				}

				void SetSpanLists( SineTrackSpanEnds& heads, SineTrackSpanEnds& tails )
				{
						mHeads = &heads;
						mTails = &tails;
				}

				virtual void ExecuteGLCommands();

		private:

				SineTrackSpanEnds* mHeads;
				SineTrackSpanEnds* mTails;
				Fl_Palette*        mPalette;
		};
}

#endif // GLSineTracks.hxx
