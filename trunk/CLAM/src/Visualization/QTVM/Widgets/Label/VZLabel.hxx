#ifndef __VZLABEL__
#define __VZLABEL__

#include <qframe.h>

namespace CLAM
{
	namespace VM
	{

		class VZLabel : public QFrame
		{
			public:
				VZLabel(QWidget* parent);
				virtual ~VZLabel();

				void setText(const QString& text);

			protected:
				virtual void drawContents(QPainter* p);

			private:
				QFont _f;
				QString _text;
				int _align;

				void InitLabel();
		};
	}
}
#endif



