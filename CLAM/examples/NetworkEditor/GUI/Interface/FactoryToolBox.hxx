
#ifndef __FactoryToolBox_hxx__
#define __FactoryToolBox_hxx__

#include <qframe.h> 
#include <vector>
#include <string>
#include "Signalv2.hxx"

namespace CLAM
{
	class Processing;
}

namespace NetworkGUI
{

class ProcessingLabel;

class FactoryToolBox : public QFrame
{
public:
	FactoryToolBox(  QWidget *parent = 0, const char *name = 0 );
	virtual ~FactoryToolBox();
	void AddProcessingLabel( const std::string & );
	void IncrementNumProcessings();
	int GetNumProcessings();
	

protected:
	void paintEvent( QPaintEvent * );
	void        mousePressEvent( QMouseEvent *);
	void        mouseReleaseEvent( QMouseEvent *);
	void        mouseMoveEvent( QMouseEvent *);

	bool        mDown;
	bool        mMinimized;
	QPoint     mClickPos;
	QSize      mNormalSize;
	unsigned int mNumProcessings;

	std::vector< ProcessingLabel* > mProcessingLabels;

public: //signals
	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > AddNewProcessing;

	
};

} // namespace NetworkGUI

#endif // __FactoryToolBox_hxx__
