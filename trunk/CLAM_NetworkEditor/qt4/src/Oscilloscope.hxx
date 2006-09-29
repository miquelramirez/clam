#ifndef Oscilloscope_hxx
#define Oscilloscope_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>


class Oscilloscope : public QWidget
{
	enum {
		margin=4
	};
public:
	Oscilloscope(CLAM::Processing * processing, QWidget * parent=0)
		: QWidget(parent)
		, _monitor(dynamic_cast<CLAM::AudioPortMonitor*>(processing))
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		std::cout << "Painting..." << std::endl;
		QPolygonF _line;
		QPainter painter(this);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.translate(0,height()/2);
		painter.scale(width(),height()/2);
		painter.drawLine(0,0,1,0);

		painter.setPen(Qt::black);
		const CLAM::Audio & audio = _monitor->FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = audio.GetBuffer();
		int size = data.Size();
		for (int i=0; i<size; i++)
		{
			const CLAM::TData & bin = data[i];
			_line << QPointF(double(i)/size, bin);
		}
		_monitor->UnfreezeData();
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		update();
	}
private:
	CLAM::AudioPortMonitor * _monitor;
	std::vector<CLAM::TData> _data;
};



#endif//Oscilloscope_hxx
