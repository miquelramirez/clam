#ifndef __SINGLELABEL__
#define __SINGLELABEL__

#include <qlabel.h>

namespace CLAM
{
	namespace VM
	{

		class SingleLabel : public QLabel
		{
			public:
				SingleLabel(QWidget* parent,const QString& units="",const QString& tooltip="");
				virtual ~SingleLabel();

				void SetUnits(const QString& text);
				void SetToolTip(const QString& text);
				void Update(float value);

			private:
				QString _units;

				void InitLabel();
		};
	}
}
#endif



