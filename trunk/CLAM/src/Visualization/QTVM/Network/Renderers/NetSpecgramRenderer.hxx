#ifndef __NETSPECGRAMRENDERER__
#define __NETSPECGRAMRENDERER__

#include <vector>
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

	    
	    void SetIndexes(const TIndex& leftIndex1, const TIndex& rightIndex1,
			    const TIndex& leftIndex2, const TIndex& rightIndex2,
			    const TIndex& begin);

	    void SetData(std::vector< std::vector<Color> >& data, 
			 const TIndex& bottom, const TIndex& top);

	    void Render();

	private:
	    std::vector< std::vector<Color> > _data;
	    TIndex _bottomBound, _topBound;
	    TIndex _leftIndex1, _rightIndex1, _leftIndex2, _rightIndex2;
	    TIndex _begin;

	};
    }
}

#endif

