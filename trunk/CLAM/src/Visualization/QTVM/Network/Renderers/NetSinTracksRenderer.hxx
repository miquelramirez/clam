#ifndef __NETSINTRACKSRENDERER__
#define __NETSINTRACKSRENDERER__

#include "QtPalette.hxx"
#include "SinTracksDef.hxx"
#include "Renderer.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSinTracksRenderer : public Renderer
	{
	public:
	    NetSinTracksRenderer();
	    virtual ~NetSinTracksRenderer();

	    void SetData(SineTrackList& data);

	    void Render();

	private:
	    SineTrackList _trackList;
	    QtPalette _palette;

	    float ClampToRange(TData value) const;

	};
    }
}

#endif

