
#ifndef __QT_CONTROLPRESENTATION_HXX__
#define __QT_CONTROLPRESENTATION_HXX__

#include "Qt_ConnectionPresentation.hxx"

namespace NetworkGUI
{

class Qt_ControlConnectionPresentation : public Qt_ConnectionPresentation
{
public:
	Qt_ControlConnectionPresentation(  QWidget *parent = 0, const char *name = 0 );
	virtual ~Qt_ControlConnectionPresentation();
protected:
	void paintEvent( QPaintEvent * );
	void UpdateBezierLine();
};

} // namespace NetworkGUI

#endif // __QT_CONTROLPRESENTATION_HXX__
