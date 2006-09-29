#ifndef Vumeter_hxx
#define Vumeter_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>


class Vumeter : public QWidget
{
	enum {
		margin=4
	};
public:
	Vumeter(CLAM::Processing * processing, QWidget * parent=0)
		: QWidget(parent)
		, _monitor(dynamic_cast<CLAM::AudioPortMonitor*>(processing))
		, _energy(0)
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		painter.setBrush(Qt::red);
		painter.setPen(Qt::black);
		int vumeterHeight = int(height()*(energy()*10.));
//		std::cout << "painting.."  << vumeterHeight << std::endl;
		painter.drawRect(margin,height()-vumeterHeight,width()-2*margin,height());
	}
	double energy()
	{
		_energy*=0.5;
		const CLAM::Audio & audio = _monitor->FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = audio.GetBuffer();
		for (int i=0; i<data.Size(); i++)
		{
			const CLAM::TData & bin = data[i];
			_energy+=bin*bin;
		}
		_energy/=data.Size();
		_monitor->UnfreezeData();
		return _energy;
	}
	void timerEvent(QTimerEvent *event)
	{
		update();
	}
private:
	CLAM::AudioPortMonitor * _monitor;
	double _energy;
};



#endif//Vumeter_hxx
