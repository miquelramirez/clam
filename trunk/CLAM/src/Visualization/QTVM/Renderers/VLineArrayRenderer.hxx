#ifndef __VLINEARRAYRENDERER__
#define __VLINEARRAYRENDERER__

#include "Array.hxx"
#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class VLineArrayRenderer : public Renderer
		{
			public:
				VLineArrayRenderer();
				virtual ~VLineArrayRenderer();

				void SetData(const Array<unsigned>& data);
				void Render();

			private:
				Array<unsigned> _data;
				int _nElems;

		};
	}
}

#endif
