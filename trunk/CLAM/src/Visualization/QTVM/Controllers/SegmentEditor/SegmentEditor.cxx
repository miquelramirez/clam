#include "Segmentation.hxx"
#include "SegmentEditor.hxx"

namespace CLAM
{
	namespace VM
	{
		SegmentEditor::SegmentEditor()
			: mStrategy(0)
			, mMustProcessData(false)
		{
		}

		SegmentEditor::~SegmentEditor()
		{
			if(mStrategy) delete mStrategy;
		}

		void SegmentEditor::SetSegmentation(Segmentation* s)
		{
			mStrategy = s;
			mMustProcessData = true;
		}

		void SegmentEditor::SetHBounds(double left, double right)
		{
			mRenderer.SetHBounds(left,right);
		}

		void SegmentEditor::SetVBounds(double bottom, double top)
		{
			mRenderer.SetVBounds(bottom,top);
		}

		void SegmentEditor::Draw()
		{
			if(!mStrategy) return;
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
		}

		void SegmentEditor::ProcessData()
		{
			mRenderer.SetData(*mStrategy);
			mMustProcessData = false;
		}
	}
}

// END
