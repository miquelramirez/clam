#ifndef PolarChromaPeaks_hxx
#define PolarChromaPeaks_hxx

#include "PeakDataSource.hxx"

#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT PolarChromaPeaks : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
	Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor)
	enum Dimensions {
	};
public:
	PolarChromaPeaks(QWidget * parent=0)
		: QWidget(parent)
		, _dataSource(0)
		, _lineColor(Qt::black)
		, _pointColor(Qt::red)
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		const char * label[] = {
			"G",
			"G#",
			"A",
			"A#",
			"B",
			"C",
			"C#",
			"D",
			"D#",
			"E",
			"F",
			"F#",
		};
		QPainter painter(this);
		painter.setRenderHint(QPainter::NonCosmeticDefaultPen,false);
		painter.translate(width()/2,height()/2);
		for (int i=0; i<12; i++)
		{
			double angle = -M_PI*i/6;
			painter.drawText(QPointF(.4*width()*cos(angle), .4*height()*sin(angle)), label[i]);
		}
		painter.scale(width()/2.,-height()/2.);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.drawEllipse(-1,-1,2,2);
		painter.drawEllipse(QRectF(-.5,-.5,1,1));
		painter.drawLine(QLineF(10,10*sqrt(3.),-10,-10*sqrt(3.)));
		painter.drawLine(QLineF(10*sqrt(3.),10,-10*sqrt(3.),-10));
		painter.drawLine(QLineF(10,-10*sqrt(3.),-10,10*sqrt(3.)));
		painter.drawLine(QLineF(10*sqrt(3.),-10,-10*sqrt(3.),10));
		painter.drawLine(QLineF(10,0,-10,0));
		painter.drawLine(QLineF(0,10,0,-10));
		if ( !_dataSource) return;
		const CLAM::TData * freq = _dataSource->positionData();
		if ( !freq) return;
		const CLAM::TData * mag = _dataSource->magnitudeData();
		if ( !mag) return;
		int size = _dataSource->nBins();
		painter.setPen(_lineColor);
		QVector<QPointF> lines;
		for (int i=0; i<size; i++)
		{
			double angle = M_PI*freq[i]/6;
			lines 
				<< QPointF(0,0)
				<< QPointF(10*mag[i]*cos(angle), 10*mag[i]*sin(angle));
		}
		painter.drawLines(lines);
		painter.setPen(QPen(_pointColor,5./width()));
		for (int i=0; i<size; i++)
		{
			double angle = M_PI*freq[i]/6;
			painter.drawPoint(QPointF(10*mag[i]*cos(angle), 10*mag[i]*sin(angle)));
		}
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


#endif//PolarChromaPeaks_hxx
