#ifndef Tunner_hxx
#define Tunner_hxx

#include "PeakDataSource.hxx"

#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <cmath>

#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT Tunner : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
	Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor)
	enum Dimensions {
	};
public:
	Tunner(QWidget * parent=0)
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
			"-50%",
			"-40%",
			"-30%",
			"-20%",
			"-10%",
			"0%",
			"+10%",
			"+20%",
			"+30%",
			"+40%",
			"+50%",
			0
		};
		QPainter painter(this);
		painter.translate(width()/2,height());
		painter.save();
		painter.rotate(-90);
		for (int i=0; label[i]; i++)
		{
			QRectF rect = painter.fontMetrics().boundingRect(label[i]);
			rect.translate(-rect.width()/2,-rect.height()/2);
			float x = width()*cos(M_PI*(i)/10)/2;
			float y = height()*sin(M_PI*(i)/10);
			rect.translate(0,-sqrt(x*x+y*y)+rect.height());
			painter.drawText(rect,Qt::AlignTop|Qt::AlignHCenter, label[i]);
			painter.rotate(180/10);
		}
		painter.restore();
		painter.scale(width()/2.,-height());
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.drawEllipse(-1,-1,2,2);
		painter.drawEllipse(QRectF(-.5,-.5,1,1));
		painter.drawLine(QLineF(0,10,0,0));
		for (unsigned i=0; i<=10; i++)
		{
			painter.drawLine(QLineF(0,0,10*cos(i*M_PI/10),10*sin(i*M_PI/10)));
		}
		if ( !_dataSource) return;
		const CLAM::TData * freq = _dataSource->positionData();
		if ( !freq) return;
		const CLAM::TData * mag = _dataSource->magnitudeData();
		if ( !mag) return;
//		std::cout << *freq << " " << *mag << std::endl;
		int size = _dataSource->nBins();
		painter.setPen(_lineColor);
		QVector<QPointF> lines;
		for (int i=0; i<size; i++)
		{
			double angle = M_PI*(-freq[i]+.5);
			lines 
				<< QPointF(0,0)
				<< QPointF(10*mag[i]*cos(angle), 10*mag[i]*sin(angle));
		}
		painter.drawLines(lines);
		painter.setPen(QPen(_pointColor,5./width()));
		for (int i=0; i<size; i++)
		{
			double angle = M_PI*(-freq[i]+.5);
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


#endif//Tunner_hxx
