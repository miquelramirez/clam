#ifndef PeakView_hxx
#define PeakView_hxx

#include <CLAM/SpectralPeakArray.hxx>

class PeakDataSource
{
public:
	virtual ~PeakDataSource() {}
	virtual const CLAM::TData * positionData()=0;
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
	const CLAM::TData * positionData()
	{
		_peaks = FreezeAndGetData();
		UnfreezeData();
		if ( !_peaks.HasFreqBuffer()) return 0;
		const CLAM::Array<CLAM::TData> & data = _peaks.GetFreqBuffer();
		_size = data.Size();
		if (!_size) return 0;
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
		return IsRunning();
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
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT PeakView : public QWidget
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
		// Important: this view is suposing magnitudes are log scale [-inf,0]
		if ( !_dataSource) return;
		const CLAM::TData * freq = _dataSource->positionData();
		if ( !freq) return;
		const CLAM::TData * mag = _dataSource->magnitudeData();
		if ( !mag) return;
		int size = _dataSource->nBins();
		double max=-1000;
		double min=1000;
		for (int i=0; i<size; i++)
		{
			if (max<mag[i]) max=mag[i];
			if (min>mag[i]) min=mag[i];
		}
		min-=3;
		//std::cout << min << ":" << max << std::endl;
		QVector<QPointF> lines;
		QPainter painter(this);
		painter.setPen(QPen(_pointColor,4));
		
		for (int i=0; i<size; i++)
			// TODO: Take the scaling from the datasource
			painter.drawPoint(QPointF(freq[i]*width()/11050+2, mag[i]*height()/min));
		painter.scale(width()/11025.,height()/min);
		painter.translate(10,0);
		painter.drawLines(lines);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.drawLine(10,0,-10,0);
		painter.drawLine(0,100,0,-100);
		painter.setPen(_lineColor);
		for (int i=0; i<size; i++)
			lines 
				<< QPointF(freq[i], min)
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

