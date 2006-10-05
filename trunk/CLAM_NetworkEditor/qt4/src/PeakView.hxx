#ifndef PeakView_hxx
#define PeakView_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>


class PeakViewWidget : public QWidget
{
	enum Dimensions {
	};
public:
	PeakViewWidget(CLAM::Processing * processing, QWidget * parent=0)
		: QWidget(parent)
		, _monitor(dynamic_cast<CLAM::PortMonitor<CLAM::SpectralPeakArray>*>(processing))
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		if ( !_monitor) return;
		CLAM::SpectralPeakArray peaks = _monitor->FreezeAndGetData();
		_monitor->UnfreezeData();
		if ( !peaks.HasMagBuffer()) return;
		if ( !peaks.HasFreqBuffer()) return;
		const CLAM::DataArray & mag = peaks.GetMagBuffer();
		const CLAM::DataArray & freq = peaks.GetFreqBuffer();
		int size = mag.Size();
		QVector<QPointF> lines;
		QPainter painter(this);
		painter.setPen(QPen(Qt::red,4));
		for (int i=0; i<size; i++)
			painter.drawPoint(freq[i]*width()/11050+2, -mag[i]*height()/50.);
		painter.scale(width()/11025.,-height()/50.);
		painter.translate(10,0);
		painter.drawLines(lines);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.drawLine(10,0,-10,0);
		painter.drawLine(0,50,0,-50);
		painter.setPen(Qt::black);
		for (int i=0; i<size; i++)
			lines 
				<< QPointF(freq[i], -50)
				<< QPointF(freq[i], mag[i]);
		painter.drawLines(lines);
	}
	void timerEvent(QTimerEvent *event)
	{
		if ( !_monitor) return;
		if (_monitor->GetExecState() != CLAM::Processing::Running)
			return;
		update();
	}
private:
	CLAM::PortMonitor<CLAM::SpectralPeakArray> * _monitor;
	std::vector<CLAM::TData> _data;
};



#endif//PeakView_hxx
