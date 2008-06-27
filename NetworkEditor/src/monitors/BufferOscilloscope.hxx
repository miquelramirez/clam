#ifndef BufferOscilloscope_hxx
#define BufferOscilloscope_hxx


#include "Oscilloscope.hxx"
#include "FloatArrayDataSource.hxx"


//TODO move to a clam lib


class QDESIGNER_WIDGET_EXPORT BufferOscilloscope : public Oscilloscope
{
	Q_OBJECT
public:
	BufferOscilloscope(QWidget * parent=0, CLAM::VM::FloatArrayDataSource * dataSource=0)
		: Oscilloscope(parent, dataSource)
	{
	}
};


#endif//BufferOscilloscope_hxx
