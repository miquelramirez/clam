#ifndef __YRULER__
#define __YRULER__

#include <qwt/qwt_scale.h>

namespace CLAM
{
	namespace VM
	{
		class YRuler : public QwtScale
		{
			Q_OBJECT

			public:
				YRuler(QWidget* parent=0);
				virtual ~YRuler();

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

