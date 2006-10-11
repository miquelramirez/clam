#ifndef PeakView_hxx
#define PeakView_hxx

#include <CLAM/SpectralPeakArray.hxx>

class PeakDataSource
{
public:
	virtual ~PeakDataSource() {}
	virtual const CLAM::TData * orderData()=0;
	virtual const CLAM::TData * magnitudeData()=0;
	virtual unsigned nBins() const=0;
	virtual bool isEnabled() const=0;
};

#include <CLAM/PortMonitor.hxx>
class PeakViewMonitor : public CLAM::PortMonitor<CLAM::SpectralPeakArray>, public PeakDataSource
{
	const char* GetClassName() const { return "PeakView"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const CLAM::TData * orderData()
	{
		_peaks = FreezeAndGetData();
		UnfreezeData();
		if ( !_peaks.HasFreqBuffer()) return 0;
		const CLAM::Array<CLAM::TData> & data = _peaks.GetFreqBuffer();
		_size = data.Size();
		return &data[0];
	}
	const CLAM::TData * magnitudeData()
	{
		if ( !_peaks.HasMagBuffer()) return 0;
		const CLAM::Array<CLAM::TData> & data = _peaks.GetMagBuffer();
		return &data[0];
	}
	unsigned nBins() const
	{
		return _size;
	}
	unsigned nAspects() const
	{
		return 2;
	}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
	}
private:
	unsigned _size;
	CLAM::SpectralPeakArray _peaks;
};



#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>


class PeakView : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
	Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor)
	enum Dimensions {
	};
public:
	PeakView(QWidget * parent=0)
		: QWidget(parent)
		, _dataSource(0)
		, _lineColor(Qt::black)
		, _pointColor(Qt::red)
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		if ( !_dataSource) return;
		const CLAM::TData * freq = _dataSource->orderData();
		if ( !freq) return;
		const CLAM::TData * mag = _dataSource->magnitudeData();
		if ( !mag) return;
		int size = _dataSource->nBins();
		QVector<QPointF> lines;
		QPainter painter(this);
		painter.setPen(QPen(_pointColor,4));
		for (int i=0; i<size; i++)
			// TODO: Take the scaling from the datasource
			painter.drawPoint(freq[i]*width()/11050+2, -mag[i]*height()/50.);
		painter.scale(width()/11025.,-height()/50.);
		painter.translate(10,0);
		painter.drawLines(lines);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.drawLine(10,0,-10,0);
		painter.drawLine(0,50,0,-50);
		painter.setPen(_lineColor);
		for (int i=0; i<size; i++)
			lines 
				<< QPointF(freq[i], -50)
				<< QPointF(freq[i], mag[i]);
		painter.drawLines(lines);
	}
	void setDataSource(PeakDataSource & dataSource)
	{
		_dataSource = & dataSource;
	}
	void setPointColor(const QColor & color)
	{
		_pointColor = color;
	}
	QColor pointColor() const
	{
		return _pointColor;
	}
	void setLineColor(const QColor & color)
	{
		_lineColor = color;
	}
	QColor lineColor() const
	{
		return _lineColor;
	}
	void timerEvent(QTimerEvent *event)
	{
		if ( !_dataSource) return;
		if ( !_dataSource->isEnabled()) return;
		update();
	}
private:
	PeakDataSource * _dataSource;
	QColor _lineColor;
	QColor _pointColor;
};



#endif//PeakView_hxx
