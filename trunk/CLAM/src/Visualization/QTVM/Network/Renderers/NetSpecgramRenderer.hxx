#ifndef __NETSPECGRAMRENDERER__
#define __NETSPECGRAMRENDERER__

#include <vector>
#include "Spectrum.hxx"
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

	    void SetData(std::vector<Spectrum>& data);

	    void Render();

	private:
	    std::vector<Spectrum> _data;
	    QtPalette _palette;

	    float ClampToRange(TData value) const;

	};
    }
}

#endif

