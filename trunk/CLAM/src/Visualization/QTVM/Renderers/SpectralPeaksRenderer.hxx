#ifndef __SPECTRALPEAKSRENDERER__
#define __SPECTRALPEAKSRENDERER__

#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectralPeaksRenderer : public Renderer
		{
			public:
				SpectralPeaksRenderer();
				virtual ~SpectralPeaksRenderer();

				void SetDataPtr(const TData* data,int nElems);
				void SetPeakColor(Color cline,Color cpoint);
				void Render();

			private:
				const TData* _data;
				int _nElems;

				Color _cLine;
				Color _cPoint;
		};
	}
}

#endif


