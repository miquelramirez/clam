#ifndef __BPFEDITORRENDERER__
#define __BPFEDITORRENDERER__

#include "BPF.hxx"
#include "Renderer.hxx"

namespace CLAM
{
    namespace VM
    {
		class BPFEditorRenderer : public Renderer
		{
		public:
			BPFEditorRenderer();
			~BPFEditorRenderer();

			void SetData(const BPF& bpf);
			void SetDataColor(const Color& lines_color, const Color& handlers_color);
			
			void Render();
			void SetBounds(const TIndex& left, const TIndex& right);
			void SetSelectedIndex(const int& index);
	    
		private:
			const BPF* mData;
			Color      mDataColor;
			Color      mHandlersColor;
			TIndex     mLeftIndex; 
			TIndex     mRightIndex;
			int        mSelectedIndex;

			enum { LINE_WIDTH=2, POINT_SIZE=4 };
	    
			void DrawData();
			void DrawHandlers();
		};
    }
}

#endif

