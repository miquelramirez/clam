#ifndef ControlSurfaceWidget_hxx
#define ControlSurfaceWidget_hxx

#include <cmath>
#include "ControlSurface.hxx"
#include <QtGui/QWidget>

class QLabel;
class QDoubleSpinBox;

class ControlSurfaceWidget : public QWidget
{
	Q_OBJECT
public:
	ControlSurfaceWidget(CLAM::Processing * processing);
	~ControlSurfaceWidget();
	void paintEvent(QPaintEvent * event); 
	void mousePressEvent(QMouseEvent * event); 
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event) ;

private slots:
	void moveSurface(int pos1, int pos2);
	void spinBoxChanged();
signals:
	void surfaceMoved(int pos1, int pos2);

private:
	double mapX(int x) const;
	double mapY(int y) const;
	enum { pointSize=10 };
	QLabel * _surface;
	QDoubleSpinBox * _spinBox1;
	QDoubleSpinBox * _spinBox2;
	CLAM::ControlSurface * _sender;
	double _min1;
	double _default1;
	double _max1;
	double _step1;
	double _min2;
	double _default2;
	double _max2;
	double _step2;
	bool _updating;
	bool _dragging;
};

#endif//ControlSurfaceWidget_hxx

