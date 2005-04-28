#ifndef __BPFEDITORRENDERER__
#define __BPFEDITORRENDERER__

#include "BPF.hxx"
#include "VMColor.hxx"

namespace CLAM
{
    namespace VM
    {
	class BPFEditorRenderer
	{
	public:
	    BPFEditorRenderer();
	    ~BPFEditorRenderer();

	    void SetData(const BPF& bpf);
	    void SetDataColor(const Color& c);
	    void SetHandlersColor(const Color& c);

	    void Render();
	    void SetBounds(const TIndex& left, const TIndex& right);
	    void SetSelectedIndex(const int& index);
	    
	private:
	    BPF mData;
	    Color mDataColor, mHandlersColor;
	    enum { POINT_SIZE=3 };
	    TIndex mLeftIndex, mRightIndex;
	    int mSelectedIndex;
	    
	    void DrawData();
	    void DrawHandlers();
	};
    }
}

#endif

