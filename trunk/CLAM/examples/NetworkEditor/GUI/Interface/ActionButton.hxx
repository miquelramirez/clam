
#ifndef __ACTION_BUTTON_HXX__
#define __ACTION_BUTTON_HXX__

// XR: Temporary class used until qt signal/slots and preprocessor macros (mocs) are supported by clam build system


#include <qpushbutton.h>
#include "Signalv1.hxx"

namespace NetworkGUI
{

class ActionButton : public QPushButton	
{
public:
	ActionButton( QWidget * parent = 0, const char *name = 0 );
protected:
	virtual void mousePressEvent ( QMouseEvent * );

public: //signals
	SigSlot::Signalv1 < bool > Pressed;
};

} // namespace NetworkGUI

#endif // __ACTION_BUTTON_HXX__

