#ifndef __MPDATARENDERER__
#define __MPDATARENDERER__

#include "Array.hxx"
#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class MPDataRenderer : public Renderer
		{
			public:
				MPDataRenderer();
				virtual ~MPDataRenderer();

				void SetData(const TData* data,unsigned nElems);
				void Render();

			private:
				DataArray _data;
				
		};
	}
}

#endif


