#ifndef __BUFFDATARENDERER__
#define __BUFFDATARENDERER__

#include "Renderer.hxx"

namespace CLAM
{
    namespace VM
    {
	class BuffDataRenderer : public Renderer
	{
	public:
	    BuffDataRenderer();
	    virtual ~BuffDataRenderer();

	    void SetDataPtr(const TData* data, unsigned nElems, unsigned index);
	    void Render();

	private:
	    const TData* _data;
	    unsigned _nElems, _index;
	};
    }
}

#endif

