#include "QtWrapper.hxx"
#include <qobject.h>
#include <qapplication.h>

namespace CLAMGUI
{

class QtAppSignaler : public QObject
{
	Q_OBJECT

	friend class QtWrapper;

	bool is_finalizing;

public:

	QtAppSignaler(QApplication* pApp)
		: QObject(), is_finalizing( false )
	{
		connect( pApp, SIGNAL(lastWindowClosed()), this, SLOT(LastWindowClosed()) );
	}

public slots:

	void LastWindowClosed()
	{
		is_finalizing = true;
	}
};

}