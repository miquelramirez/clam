#ifndef __NETSINTRACKSRENDERER__
#define __NETSINTRACKSRENDERER__

#include <vector>
#include "SinTrackNode.hxx"
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

	    void SetData(std::vector< std::vector<SinTrackNode> >& data);

	    void Render();

	private:
	     std::vector< std::vector<SinTrackNode> > _tracks;

	};
    }
}

#endif

