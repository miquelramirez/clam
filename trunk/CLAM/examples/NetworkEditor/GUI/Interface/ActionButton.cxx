
#include "ActionButton.hxx"

namespace NetworkGUI
{

	ActionButton::ActionButton( QWidget * parent , const char *name )
	: QPushButton(parent, name)
{
	setPalette( QPalette( QColor( 200, 200, 200 )));
	setToggleButton(TRUE);
	setText(name);
}

void ActionButton::mousePressEvent ( QMouseEvent * )
{
		Pressed.Emit( false );
}

} // namespace NetworkGUI


