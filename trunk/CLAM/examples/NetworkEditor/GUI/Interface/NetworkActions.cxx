
#include "NetworkActions.hxx"
#include <iostream>

namespace NetworkGUI
{

NetworkButton::NetworkButton(  bool condition, QWidget *parent , const char *name )
	: QPushButton(parent, name),
	  mCondition(condition)
{
	setPalette( QPalette( QColor( 200, 200, 200 )));
	setToggleButton(TRUE);
	setText(name);
}

void NetworkButton::mousePressEvent ( QMouseEvent * )
{
	if (isOn())
	{	
		setPalette( QPalette( QColor( 200, 200, 200 )));
		setText("Ready");
		Pressed.Emit( false );
	}
	else
	{
		setPalette( QPalette( QColor( 250, 250, 250 )));
		setText("Running");
		Pressed.Emit( true );
	}
	toggle();
}




NetworkActions::NetworkActions(  QWidget *parent , const char *name )
	: QFrame( parent, name ),
	  mActions( 1, QGroupBox::Horizontal, "Network State", this),
	  mState(true, &mActions, "Ready")
{	
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 1 );
	setPalette( QPalette( QColor( 100, 100, 100 )));
	mActions.resize(110, 50);
	resize( 110, 50 );

	ButtonPressed.Wrap(this, &NetworkActions::OnButtonPressed);
	mState.Pressed.Connect( ButtonPressed );	
}

NetworkActions::~NetworkActions()
{
}

void NetworkActions::OnButtonPressed( bool condition )
{
	ChangeNetworkState.Emit( condition );
}	

void NetworkActions::mousePressEvent( QMouseEvent * m )
{
	if (m->button() == RightButton )
	{
		mDown = true;
		mClickPos = m->pos();
		grabMouse();
	}
}

void NetworkActions::mouseReleaseEvent( QMouseEvent *)
{	
	releaseMouse();
	mDown = false;
}

void NetworkActions::mouseMoveEvent( QMouseEvent * m )
{	
	if(mDown)
	{
		QPoint difference(QWidget::mapFromGlobal(m->globalPos()));
		difference = mapToParent(difference) - mClickPos;
		move(difference);
		QWidget * parent = parentWidget();
		parent->repaint();
	}
}

} // namespace NetworkGUI
