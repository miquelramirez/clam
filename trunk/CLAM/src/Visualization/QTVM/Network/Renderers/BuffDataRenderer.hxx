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

	    void SetDataPtr(const TData* data);
	    void SetIndexes(const TIndex& left1, const TIndex& right1, 
			    const TIndex& left2, const TIndex& right2, 
			    const TIndex& begin);
	    void Render();

	private:
	    const TData* _data;
	    TIndex _leftIndex1, _rightIndex1, _leftIndex2, _rightIndex2;
	    TIndex _begin;
	};
    }
}

#endif

