#include "XRuler.hxx"

namespace CLAM
{
	namespace VM
	{
		XRuler::XRuler(QWidget *parent) 
			: QwtScale(QwtScale::Top,parent,"x ruler")
		{
			Init();
		}

		XRuler::~XRuler()
		{
		}

		void XRuler::SetScaleDiv(QwtScaleDiv div)
		{
			setScaleDiv(div);
			emit scaleDiv(div);
		}

		void XRuler::Init()
		{
			QFont f("Sans",7);
			f.setBold(true);

			QwtScaleDiv div;

		    setFont(f);
			setFixedHeight(40);
			div.rebuild(0.0,10,10,10,false,10.0);
			setScaleDiv(div);
		}

		void XRuler::SetLabelFormat(char f,int prec,int fieldWidth)
		{
			setLabelFormat(f,prec,fieldWidth);
		}
	}
}

// END

