#ifndef __NETPEAKSRENDERER__
#define __NETPEAKSRENDERER__

#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
	    class NetPeaksRenderer : public Renderer
	    {
	    public:
			NetPeaksRenderer();
			~NetPeaksRenderer();

			void SetDataPtr(const TData* magBuff, const TData* freqBuff, int nElems);
			void SetPeakColor(const Color& cline, const Color& cpoint);
			void Render();
		
	    private:
			const TData *mMagBuff, *mFreqBuff;
			Color mCline, mCpoint;
			int mElems;
	    };

	}
}

#endif




