#ifndef __NETSPECGRAMRENDERER__
#define __NETSPECGRAMRENDERER__

#include "Array.hxx"
#include "QtPalette.hxx"
#include "Renderer.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSpecgramRenderer : public Renderer
	{
	public:
	    NetSpecgramRenderer();
	    virtual ~NetSpecgramRenderer();

	    void SetData(Array<DataArray>& data);

	    void Render();

	private:
	    Array<DataArray> _data;
	    QtPalette _palette;

	    float ClampToRange(TData value) const;

	};
    }
}

#endif

