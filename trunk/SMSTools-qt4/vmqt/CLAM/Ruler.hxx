#ifndef __RULER__
#define __RULER__

#include <qwidget.h>
#include <CLAM/GlobalEnums.hxx>
#include <CLAM/VMColor.hxx>

namespace CLAM
{
    namespace VM
    {
		enum Position { Left=0, Right, Bottom, Top };

		class Ruler : public QWidget
		{
			Q_OBJECT

		public:
			Ruler(QWidget* parent, const  Position& pos, const EScale& scale=EScale::eLinear);
			~Ruler();

			void SetScale(const EScale& scale);
			const EScale& GetScale() const;
	    
			void SetBackgroundColor(const Color& c);
			void SetForegroundColor(const Color& c);

			QFont& Font();

        signals:
			void valueChanged(double, double);
	    
	    public slots:
			void updateRange(double, double);

		protected:
			void paintEvent(QPaintEvent* pe);

		private:
			Position mPosition;
			EScale   mScale;
			double   mMin; 
			double   mMax; 
			double   mSpan;
			int      mTicks;
			QFont    mFont;
			Color    mForegroundColor;
			int      mMaxLabelWidth; 
			int      mLabelHeight;

			enum { MINTICKS=3, MAXTICKS=17 };

			void DrawLeft();
			void DrawRight();
			void DrawBottom();
			void DrawTop();

			int GetnTicks();
			int TestnTicksForSpan();
			QString GetLabel(int tick);
			void ComputeMaxLabelWidth();
		};
    }
}

#endif

