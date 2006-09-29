#ifndef Vumeter_hxx
#define Vumeter_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>


class Vumeter : public QWidget
{
	enum Dimensions {
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
		int vumeterHeight = int(height()*(energy()*10));
//		std::cout << "painting.."  << vumeterHeight << "\tenergy: "<<_energy << std::endl;
		painter.drawRect(margin,height()-vumeterHeight,width()-2*margin,height());
	}
	double energy()
	{
		_energy*=0.5;
		const CLAM::Audio & audio = _monitor->FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = audio.GetBuffer();
		unsigned size = data.Size();
		if (not size)
		{
			_monitor->UnfreezeData();
			_energy = 0;
			return _energy;
		}
		for (unsigned i=0; i<size; i++)
		{
			const CLAM::TData & bin = data[i];
			_energy+=bin*bin*0.5;
		}
		_energy /= size;
		_monitor->UnfreezeData();

		return _energy;
	}
	void timerEvent(QTimerEvent *event)
	{
		if (_monitor->GetExecState() != CLAM::Processing::Running)
			return;
		update();
	}
private:
	CLAM::AudioPortMonitor * _monitor;
	double _energy;
};



#endif//Vumeter_hxx
