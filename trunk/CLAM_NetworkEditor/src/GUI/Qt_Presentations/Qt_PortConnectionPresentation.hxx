
#ifndef __QT_PORTPRESENTATION_HXX__
#define __QT_PORTPRESENTATION_HXX__

#include "Qt_ConnectionPresentation.hxx"

namespace NetworkGUI
{

class Qt_PortConnectionPresentation : public Qt_ConnectionPresentation
{
public:
	Qt_PortConnectionPresentation(  QWidget *parent = 0, const char *name = 0 );
	virtual ~Qt_PortConnectionPresentation();
protected:
	void paintEvent( QPaintEvent * );
	void UpdateBezierLine();
};

} // namespace NetworkGUI

#endif // __QT_PORTPRESENTATION_HXX__
