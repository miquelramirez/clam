#ifndef Vumeter_hxx
#define Vumeter_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>


class Vumeter : public QWidget
{
	enum {
		margin=4
	};
public:
	Vumeter(CLAM::Processing * processing, QWidget * parent=0)
		: QWidget(parent)
	{
		new QLabel("Hola",this);
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		painter.setBrush(Qt::red);
		painter.setPen(Qt::black);
		int vumeterHeight = int(height()*(energy()));
		std::cout << "painting.."  << vumeterHeight << std::endl;
		painter.drawRect(margin,height()-vumeterHeight,width()-2*margin,height());
		painter.drawLine(0,3,3,0);
	}
	double energy()
	{
		return _energy;
	}
	void timerEvent(QTimerEvent *event)
	{
		_energy+=.7443;
		_energy=std::fmod(_energy,1.);
		update();
	}
private:
	double _energy;
};



#endif//Vumeter_hxx
