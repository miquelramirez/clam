#ifndef Tunner_hxx
#define Tunner_hxx

#include <vector>
#include <CLAM/DataTypes.hxx>
#include "PeakView.hxx"

#include <CLAM/PortMonitor.hxx>
class TunnerMonitor : public CLAM::PortMonitor<std::pair<CLAM::TData,CLAM::TData> >, public PeakDataSource
{
	const char* GetClassName() const { return "Tunner"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const CLAM::TData * orderData()
	{
		_fasor = FreezeAndGetData();
		UnfreezeData();
		return &(_fasor.first);
	}
	const CLAM::TData * magnitudeData()
	{
		return &(_fasor.second);
	}
	unsigned nBins() const
	{
		return 1;
	}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
	}
private:
	std::pair<CLAM::TData,CLAM::TData> _fasor;
};



#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>


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
			"+50",
			"+40",
			"+30",
			"+20",
			"+10",
			"0",
			"-10",
			"-20",
			"-30",
			"-40",
			"-50",
			0
		};
		QPainter painter(this);
		painter.translate(width()/2,height());
		for (int i=0; label[i]; i++)
		{
			double angle = -M_PI*i/10;
			painter.drawText(QPointF(.4*width()*cos(angle), .8*height()*sin(angle)), label[i]);
		}
		painter.scale(width()/2.,-height());
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
		const CLAM::TData * freq = _dataSource->orderData();
		if ( !freq) return;
		const CLAM::TData * mag = _dataSource->magnitudeData();
		if ( !mag) return;
//		std::cout << *freq << " " << *mag << std::endl;
		int size = _dataSource->nBins();
		painter.setPen(_lineColor);
		QVector<QPointF> lines;
		for (int i=0; i<size; i++)
		{
			double angle = M_PI*(freq[i]+.5);
			lines 
				<< QPointF(0,0)
				<< QPointF(10*mag[i]*cos(angle), 10*mag[i]*sin(angle));
		}
		painter.drawLines(lines);
		painter.setPen(QPen(_pointColor,5./width()));
		for (int i=0; i<size; i++)
		{
			double angle = M_PI*(freq[i]+.5);
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
