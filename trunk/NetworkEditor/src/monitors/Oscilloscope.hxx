#ifndef Oscilloscope_hxx
#define Oscilloscope_hxx


#include <CLAM/PortMonitor.hxx>
#include "FloatArrayDataSource.hxx"


//TODO move to a clam lib

class OscilloscopeMonitor :
	public CLAM::AudioPortMonitor,
	public CLAM::VM::FloatArrayDataSource
{
public:
	OscilloscopeMonitor()
		: _size(0)
		{ }
private:
	const char* GetClassName() const { return "Oscilloscope"; };
	std::string getLabel(unsigned bin) const
	{
		static std::string a;
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
	bool hasUpperBound() const { return true; }
	bool hasLowerBound() const { return true; }
	CLAM::TData upperBound() const {return 1;}
	CLAM::TData lowerBound() const {return -1;}
	bool isEnabled() const
	{
		return IsRunning();
	}
private:
	unsigned _size;
	
};

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <QtGui/QPainter>
#include <CLAM/DataTypes.hxx>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT Oscilloscope : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
	enum Dimensions {
	};
public:
	Oscilloscope(QWidget * parent=0, CLAM::VM::FloatArrayDataSource * dataSource=0)
		: QWidget(parent)
		, _dataSource(0)
		, _lineColor(Qt::black)
	{
		setDataSource(dataSource ? *dataSource : dummySource());
		startTimer(50);
	}
	void setDataSource(CLAM::VM::FloatArrayDataSource & dataSource)
	{
		_dataSource = & dataSource;
	}
	void paintEvent(QPaintEvent * event)
	{
		if ( !_dataSource) return;
		QPainter painter(this);
//		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.translate(0,height()/2);
		painter.scale(width(),-height()/2);
		painter.drawLine(0,0,1,0);

		painter.setPen(_lineColor);
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
		if ( !_dataSource) return;
		if ( !_dataSource->isEnabled()) return;
		update();
	}
	void setLineColor(const QColor & color) { _lineColor = color; }
	QColor lineColor() const { return _lineColor; }
public:
	static CLAM::VM::FloatArrayDataSource & dummySource()
	{
		static CLAM::VM::DummyFloatArrayDataSource sDummySource(100,initDummyData(100));
		return sDummySource;
	}
private:
	static CLAM::TData * initDummyData(unsigned size)
	{
		static CLAM::TData * data = new CLAM::TData[size];
		for (unsigned i=0; i<size; i++)
			data[i]=std::sin(i*M_PI*7/size);
		return data;
	}
private:
	CLAM::VM::FloatArrayDataSource * _dataSource;
	QColor _lineColor;
};



#endif//Oscilloscope_hxx

