#include "Qt_Help_View.hxx"
//Added by qt3to4:
#include <QKeyEvent>

namespace QtSMS
{
	Qt_Help_View::Qt_Help_View(QWidget* parent)
		: Q3TextEdit(parent)
	{
		setFocusPolicy(Qt::StrongFocus);
		setHScrollBarMode(Q3ScrollView::Auto);
		setVScrollBarMode(Q3ScrollView::Auto);
		setReadOnly(true);
	}

	Qt_Help_View::~Qt_Help_View()
	{
	}

	void Qt_Help_View::SetText(const QString& text)
	{
		setText(text);
	}

	void Qt_Help_View::keyPressEvent(QKeyEvent* e)
	{
		switch(e->key())
		{
			case Qt::Key_Plus:
				if(font().pointSize() < 20)
				{
					zoomIn(font().pointSize()+1);
				}
				break;
			case Qt::Key_Minus:
				if(font().pointSize() > 8)
				{
					zoomIn(font().pointSize()-1);
				}
				break;
			default:
				break;
		}
	}	
	
}

// END

