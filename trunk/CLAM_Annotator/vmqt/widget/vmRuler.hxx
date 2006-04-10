#ifndef __VMQT_RULER_H__
#define __VMQT_RULER_H__

#include <vector>
#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class Ruler : public QGLWidget
		{
			Q_OBJECT
		public:
			Ruler(QWidget* parent, ERulerPos pos, ERulerScale scale=CLAM::VM::eLinearScale);
			~Ruler();

			void SetRange(double min, double max);
			void SetStep(double step);
			void SetScale(ERulerScale scale);
			ERulerScale GetScale() const;
	    
			void SetBackgroundColor(const Color& c);
			void SetForegroundColor(const Color& c);
	
			void ShowFloats();
			void ShowIntegers();

			const QFont& GetFont() const;

		signals:
			void valueChanged(double, double);
	    
		public slots:
			void updateRange(double, double);

		protected:
			void paintGL(); 
			void resizeEvent(QResizeEvent* e);

		private:
			int    mLabelHeight;
			bool   mDoResize;
			bool   mShowFloats;
			double mStep;
			QFont  mFont;
	
			ERulerPos   mPosition;
			ERulerScale mScale;
			GLViewport  mViewport;
			Range       mTotalRange;
			Range       mCurrentRange;
			Color       mBgColor;
			Color       mFgColor;

			std::vector<double> mValuesToDraw;

			enum { MINTICKS=3 };

			void Rebuild();

			void Draw();
			void DrawLeft();
			void DrawRight();
			void DrawBottom();
			void DrawTop();
			void DrawLabels();

			int GetTicks();
			int TestTicksForSpan();
			int GetMaxLabelWidth();
			bool IsVisible(double value);
			double Round(double x);
			double Ceil(double x);
			QString GetLabel(double value);
			std::pair<double,double> GetLabelCoords(double value);
		};
    }
}

#endif

