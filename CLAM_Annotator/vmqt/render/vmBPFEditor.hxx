#ifndef __VMQT_BPF_EDITOR_H__
#define __VMQT_BPF_EDITOR_H__

#include "BPF.hxx"
#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class BPFEditor : public Renderer2D
		{
			Q_OBJECT
		public:
			BPFEditor();
			~BPFEditor();

			void SetData(BPF* bpf);
			void SetColors(const Color& cline, const Color& chandler);
			void SetFlags(int flags);
			int GetFlags() const;
			const BPF& GetData() const;

		signals:
			void xValueChanged(QString, unsigned, double);
			void yValueChanged(QString, unsigned, double);
			void elementAdded(QString, unsigned, double, double);
			void elementRemoved(QString, unsigned);
			
		protected:
			void Render();
			void SetHBounds(double left, double right);
			void MousePressEvent(double x, double y);
			void MouseReleaseEvent(double x, double y);
			void MouseMoveEvent(double x, double y);
			void KeyPressEvent(int key);
			void KeyReleaseEvent(int key);		   
			void LeaveEvent();

		private:
			int  mglList;
			int  mFlags;
			long mCurrentIndex;
			bool mRebuildglList;

			BPF* mBPF;
			
			Color mLineColor;
			Color mHandlerColor;

			enum { LINE_WIDTH=2, POINT_SIZE=5, TOLERANCE=5 };

			enum EditionMode
			{
				Idle,
				DraggingPoint
			};

			EditionMode mEditionMode;

			void Remove();
			long Add(double x, double y);
			void Update(double x, double y);

			long PickPoint(double x, double y);
			long GetBound(double search_value);
			std::pair<long, long> GetBounds();

			double Round(double x);
			QString GetToolTip(double x, double y);

			void ChooseCurrentPoint(long index);
			void MoveCurrentPointdx(double dx);
			void MoveCurrentPointdy(double dy);

			double GetXStep();
			double GetYStep();
		};
	}
}

#endif

