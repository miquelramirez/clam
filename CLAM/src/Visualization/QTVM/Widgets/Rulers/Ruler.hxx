#ifndef __RULER__
#define __RULER__

#include <qwidget.h>
#include "VMColor.hxx"

namespace CLAM
{
    namespace VM
    {
	enum Position { Left=0, Right, Bottom, Top };

	class Ruler : public QWidget
	{
	    Q_OBJECT

	public:
	    Ruler(QWidget* parent, const  Position& pos);
	    ~Ruler();

	    void SetBackgroundColor(const Color& c);
	    void SetForegroundColor(const Color& c);

        signals:
	    void valueChanged(double, double);
	    
	public slots:
	    void updateRange(double, double);

	protected:
	    void paintEvent(QPaintEvent* pe);

	private:
	    Position mPosition;
	    double mMin, mMax, mSpan;
	    int mTicks;

	    QFont mFont;
	    Color mForegroundColor;

	    int mMaxLabelWidth, mLabelHeight;

	    enum { MINTICKS=3, MAXTICKS=17 };

	    void DrawLeft();
	    void DrawRight();
	    void DrawBottom();
	    void DrawTop();

	    int GetnTicks();
	    int TestnTicksForSpan();

	    QString GetLabel(int tick);
	};
    }
}

#endif

