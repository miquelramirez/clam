#ifndef Vumeter_hxx
#define Vumeter_hxx

#include "Oscilloscope.hxx"
class VumeterMonitor : public OscilloscopeMonitor
{
	const char* GetClassName() const { return "Vumeter"; };
};

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <CLAM/DataTypes.hxx>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT Vumeter : public QWidget
{
	Q_OBJECT
	enum Dimensions {
		margin=4,
		ledHeight=10
	};
public:
	Vumeter(QWidget * parent=0)
		: QWidget(parent)
		, _dataSource( 0)
		, _energy(0)
	{
		if (!_dataSource) _dataSource = new OscilloscopeDummySource;
		startTimer(50);
	}
	Vumeter(CLAM::VM::FloatArrayDataSource * dataSource, QWidget * parent=0)
		: QWidget(parent)
		, _dataSource( dataSource)
		, _energy(0)
	{
		if (!_dataSource) _dataSource = new OscilloscopeDummySource;
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		unsigned nLeds=height()/ledHeight;
		double ledStep = 1.0/nLeds;
		unsigned actualLedHeight = height()/nLeds;
		
		double logEnergy = energy()*10;
		QPainter painter(this);
		painter.setPen(Qt::black);
		painter.setBrush(Qt::red);
		for (unsigned i=0; i<nLeds; i++)
		{
			QColor color = Qt::green;
			if (i>3*nLeds/8) color=Qt::yellow;
			if (i>5*nLeds/8) color=QColor(0xff,0x77,0x00);
			if (i>=7*nLeds/8) color=Qt::red;
			if (logEnergy<=ledStep*i)
				painter.setBrush(color.dark());
			else
				painter.setBrush(color);
			painter.drawRect(margin,height()-actualLedHeight*i-margin,width()-2*margin,-actualLedHeight+margin);
		}

	}
	QSize minimumSizeHint() const
	{
		return QSize(2*margin+10,2*margin+2*ledHeight);
	}
	void setDataSource(CLAM::VM::FloatArrayDataSource & source)
	{
		_dataSource = &source;
	}
	double energy()
	{
		_energy*=0.5;
		if ( !_dataSource) return _energy;
		const CLAM::TData * data = _dataSource->frameData();
		unsigned size = _dataSource->nBins();
		if ( !size)
		{
			_dataSource->release();
			_energy = 0;
			return _energy;
		}
		for (unsigned i=0; i<size; i++)
		{
			const CLAM::TData & bin = data[i];
			_energy+=bin*bin*0.5;
		}
		_energy /= size;
		_dataSource->release();

		return _energy;
	}
	void timerEvent(QTimerEvent *event)
	{
		if ( !_dataSource) return;
		if ( !_dataSource->isEnabled()) return;
		update();
	}
private:
	CLAM::VM::FloatArrayDataSource * _dataSource;
	double _energy;
};



#endif//Vumeter_hxx
