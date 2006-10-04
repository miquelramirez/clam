#ifndef Oscilloscope_hxx
#define Oscilloscope_hxx


#include <CLAM/PortMonitor.hxx>
#include "FloatArrayDataSource.hxx"

//TODO move to a clam lib

class Oscilloscope : public CLAM::AudioPortMonitor, public CLAM::VM::FloatArrayDataSource
{
	const char* GetClassName() const { return "Oscilloscope"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const double * frameData() const
	{
		const CLAM::Audio & audio = _monitor->FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = audio.GetBuffer();
		_size = data.Size();
		return &data[0];
	}
	void release()
	{
		_monitor->UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
private:
	unsigned _size;
	
};


#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>


class OscilloscopeWidget : public QWidget
{
	enum Dimensions {
	};
public:
	OscilloscopeWidget(CLAM::VM::FloatArrayDataSource * dataSource, QWidget * parent=0)
		: QWidget(parent)
		, _dataSource(dataSource)
		, _monitor(dynamic_cast<Oscilloscope*>(dataSource)) //TODO remove
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		if (not _monitor) return;
		if (not _dataSource) return;

		QPainter painter(this);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.translate(0,height()/2);
		painter.scale(width(),-height()/2);
		painter.drawLine(0,0,1,0);

		painter.setPen(Qt::black);
		const double * data = _dataSource->frameData();
		int size = _dataSource.nBins();
		QPolygonF _line;
		for (int i=0; i<size; i++)
			_line << QPointF(double(i)/size, data[i]);
		_dataSource->release();
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		if (not _dataSource) return;
		if (_monitor->GetExecState() != CLAM::Processing::Running)
			return;
		update();
	}
private:
	Oscilloscope * _monitor;
	CLAM::VM::FloatArrayDataSource * _dataSource;
};



#endif//Oscilloscope_hxx
