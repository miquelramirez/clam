#ifndef __SEGMENTEDITOR__
#define __SEGMENTEDITOR__

#include <qobject.h>
#include "SegmentRenderer.hxx"

namespace CLAM
{
	class Segmentation;
	
	namespace VM
	{
		class SegmentEditor : public QObject
		{
			Q_OBJECT
		public:
			SegmentEditor();
			~SegmentEditor();

			void SetSegmentation(Segmentation* s);
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);
			void Draw();

		private:
			Segmentation*   mStrategy;
			SegmentRenderer mRenderer;
			bool            mMustProcessData;

			void ProcessData();
		};
	}
}

#endif

