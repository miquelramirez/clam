#ifndef Oscilloscope_hxx
#define Oscilloscope_hxx


#include <CLAM/PortMonitor.hxx>
#include "FloatArrayDataSource.hxx"

//TODO move to a clam lib

class OscilloscopeMonitor : public CLAM::AudioPortMonitor, public CLAM::VM::FloatArrayDataSource
{
public:
	OscilloscopeMonitor()
		: _size(0)
		{ }
private:
	const char* GetClassName() const { return "Oscilloscope"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const CLAM::TData * frameData()
	{
		const CLAM::Audio & audio = FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = audio.GetBuffer();
		_size = data.Size();
		return &data[0];
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
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


class Oscilloscope : public QWidget
{
	enum Dimensions {
	};
public:
	Oscilloscope(CLAM::VM::FloatArrayDataSource * dataSource, QWidget * parent=0)
		: QWidget(parent)
		, _dataSource(dataSource)
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		if (not _dataSource) return;

		QPainter painter(this);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.translate(0,height()/2);
		painter.scale(width(),-height()/2);
		painter.drawLine(0,0,1,0);

		painter.setPen(Qt::black);
		const CLAM::TData * data = _dataSource->frameData();
		int size = _dataSource->nBins();
		QPolygonF _line;
		for (int i=0; i<size; i++)
			_line << QPointF(float(i)/size, data[i]);
		_dataSource->release();
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		if (not _dataSource) return;
		if (not _dataSource->isEnabled()) return;
		update();
	}
private:
	CLAM::VM::FloatArrayDataSource * _dataSource;
};



#endif//Oscilloscope_hxx
