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
		virtual ~NetPeaksRenderer();

		void SetDataPtr(const TData* magBuff, const TData* freqBuff, int nElems);
		void SetPeakColor(Color cline, Color cpoint);
		void Render();
		
	    private:
		const TData *_magBuff, *_freqBuff;
		Color _cline, _cpoint;
		int _nElems;
	    };

	}
}

#endif




