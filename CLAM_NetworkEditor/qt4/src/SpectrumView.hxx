#ifndef SpectrumView_hxx
#define SpectrumView_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>


class SpectrumViewWidget : public QWidget
{
	enum Dimensions {
	};
public:
	SpectrumViewWidget(CLAM::Processing * processing, QWidget * parent=0)
		: QWidget(parent)
		, _monitor(dynamic_cast<CLAM::PortMonitor<CLAM::Spectrum>*>(processing))
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		QPolygonF _line;
		QPainter painter(this);
		painter.scale(width(),height()/7.0);
		painter.setPen(Qt::black);
		const CLAM::Spectrum & spectrum = _monitor->FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = spectrum.GetMagBuffer();
		std::cout << &data << std::endl;
		int size = data.Size();
		for (int i=0; i<size; i++)
			_line << QPointF(double(i)/size, -std::log10(data[i]));
		_monitor->UnfreezeData();
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		if (_monitor->GetExecState() != CLAM::Processing::Running)
			return;
		update();
	}
private:
	CLAM::PortMonitor<CLAM::Spectrum> * _monitor;
	std::vector<CLAM::TData> _data;
};



#endif//SpectrumView_hxx
