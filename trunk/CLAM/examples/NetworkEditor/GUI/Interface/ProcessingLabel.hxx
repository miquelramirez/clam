
#ifndef __ProcessingLabel_hxx__
#define __ProcessingLabel_hxx__

#include <qwidget.h>
#include <qlabel.h>
#include <string>

namespace NetworkGUI
{

class ProcessingLabel : public QLabel
{
public:
	ProcessingLabel(  const std::string & name, QWidget *parent );
	virtual ~ProcessingLabel();
	void mouseDoubleClickEvent( QMouseEvent *);
	void mouseReleaseEvent( QMouseEvent *);
protected:
	std::string mName;
};

}  // namespace NetworkGUI

#endif // __ProcessingLabel_hxx__
