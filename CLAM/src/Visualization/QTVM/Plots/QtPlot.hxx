#ifndef __QTPLOT__
#define __QTPLOT__

#include <string>
#include <list>
#include <qwidget.h>
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtPlot : public QWidget
		{	
			public:
				QtPlot(QWidget* parent = 0);
				virtual ~QtPlot();
			
				virtual void Label(const std::string& label)=0;
				virtual void Geometry(int x,int y,int w,int h)=0;
				virtual void Show()=0;
				virtual void Hide()=0;

				virtual void SetMarks(std::list<unsigned>& marks)=0;
				virtual void SetMarksColor(Color c)=0;

			protected:
				virtual void closeEvent(QCloseEvent *);
		};
	}
}

#endif
