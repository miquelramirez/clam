#ifndef __XRULER__
#define __XRULER__

#include <qwt/qwt_scale.h>

namespace CLAM
{
	namespace VM
	{
		class XRuler : public QwtScale
		{
			Q_OBJECT

			public:
				XRuler(QWidget *parent);
				virtual ~XRuler();

				void SetLabelFormat(char f,int prec,int fieldWidth);

			signals:
				void scaleDiv(QwtScaleDiv);

			public slots:
				void SetScaleDiv(QwtScaleDiv);

			private:
				void Init();
		};
	}
}

#endif

