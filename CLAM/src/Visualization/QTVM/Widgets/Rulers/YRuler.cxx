#include <qwt/qwt_scale.h>
#include "YRuler.hxx"

namespace CLAM
{
	namespace VM
	{
		YRuler::YRuler(QWidget* parent) 
			: QwtScale(QwtScale::Left,parent,"y ruler")
		{
			Init();
		}

		YRuler::~YRuler()
		{
		}

		void YRuler::SetScaleDiv(QwtScaleDiv div)
		{
			setScaleDiv(div);
			emit scaleDiv(div);
		}

		void YRuler::Init()
		{
			QFont f("Sans",7);
			f.setBold(true);

			QwtScaleDiv div;

			setMinimumWidth(50);
			setFont(f);
			div.rebuild(-1.0,1.0,1,0,false,1.0);
			setScaleDiv(div);
		}

		void YRuler::SetLabelFormat(char f,int prec,int fieldWidth)
		{
			setLabelFormat(f,prec,fieldWidth);
		}
	}
}

// END

