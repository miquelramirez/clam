#include "ControlSurfaceWidget.hxx"
#include <QtGui/QLabel>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>


ControlSurfaceWidget::ControlSurfaceWidget(CLAM::Processing * processing)
	: _spinBox1(0)
	, _spinBox2(0)
	, _updating(false)
	, _dragging(false)
{
	_sender = dynamic_cast<CLAM::ControlSurface* >(processing);
	CLAM_ASSERT(_sender, "ControlSurfaceWidget only works "
				"with ControlSurface processings.");
	const CLAM::ControlSurfaceConfig * config = 
		dynamic_cast<const CLAM::ControlSurfaceConfig *>(&_sender->GetConfig());
	CLAM_ASSERT( config, "Unexpected Configuration type for an ControlSurface" );

	QVBoxLayout * mainLayout= new QVBoxLayout;
	setLayout(mainLayout);

	_min1 = config->GetMin1();
	_default1 = config->GetDefault1();
	_max1 = config->GetMax1();
	_min2 = config->GetMin2();
	_default2 = config->GetDefault2();
	_max2 = config->GetMax2();
	mainLayout->setMargin(1);

	_surface = new QLabel;
	mainLayout->addWidget(_surface);
	_surface->setMinimumSize(100,100);
	_surface->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QHBoxLayout * spinLayout = new QHBoxLayout;
	mainLayout->addLayout(spinLayout);
	_spinBox1 = new QDoubleSpinBox;
	spinLayout->addWidget(_spinBox1);

	_spinBox1->setMinimum(_min1);
	_spinBox1->setMaximum(_max1);
	_spinBox1->setValue(_default1);
	_spinBox1->setSingleStep((_max1-_min1)/100);

	connect(_spinBox1, SIGNAL(valueChanged(double)), 
		this, SLOT(spinBoxChanged()));

	_spinBox2 = new QDoubleSpinBox();
	spinLayout->addWidget(_spinBox2);

	_spinBox2->setMinimum(_min2);
	_spinBox2->setMaximum(_max2);
	_spinBox2->setValue(_default2);
	_spinBox2->setSingleStep((_max2-_min2)/100);

	connect(_spinBox2, SIGNAL(valueChanged(double)), 
		this, SLOT(spinBoxChanged()));
}


ControlSurfaceWidget::~ControlSurfaceWidget()
{
}

void ControlSurfaceWidget::moveSurface(int pos1, int pos2)
{
	if (_updating) return;
	_updating = true;

	double value1 = mapX(pos1);
	double value2 = mapY(pos2);
	_spinBox1->setValue(value1);
	_spinBox2->setValue(value2);
	_sender->SendControl(value1, value2);

	_updating = false;
	update();
	emit surfaceMoved(value1, value2);
}

void ControlSurfaceWidget::spinBoxChanged()
{
	if (_updating) return;
	_updating = true;

	double value1 = _spinBox1->value(); 
	double value2 = _spinBox2->value();
	_sender->SendControl(value1, value2);

	_updating = false;
	update();
	emit surfaceMoved(value1, value1);
}

void ControlSurfaceWidget::paintEvent(QPaintEvent * event)
{
	const unsigned xSize = width()-pointSize;
	const unsigned ySize = _spinBox1->y()-pointSize;
	QPainter painter(this);
	painter.setBrush(Qt::red);
	painter.setPen(Qt::black);
	QPointF point(
		pointSize/2+(_spinBox1->value()-_min1)*xSize/(_max1-_min1),
		pointSize/2+(_spinBox2->value()-_min2)*ySize/(_max2-_min2));
	painter.drawRoundRect(point.x()-pointSize/2, point.y()-pointSize/2,pointSize,pointSize,2,2);
}

void ControlSurfaceWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->y()>_spinBox1->y()) return;
	moveSurface(event->x(), event->y());
}
void ControlSurfaceWidget::mouseMoveEvent(QMouseEvent * event)
{
	moveSurface(event->x(), event->y());
}
void ControlSurfaceWidget::mouseReleaseEvent(QMouseEvent * event)
{
	moveSurface(event->x(), event->y());
}

double ControlSurfaceWidget::mapX(int x) const
{
	if (x<pointSize/2) return _min1;
	if (x>width()-pointSize/2) return _max1;
	return _min1+(x-pointSize/2)*(_max1-_min1)/(width()-pointSize);
}
double ControlSurfaceWidget::mapY(int y) const
{
	if (y<pointSize/2) return _min2;
	if (y>_spinBox1->y()-pointSize/2) return _max2;
	return _min2+(y-pointSize/2)*(_max2-_min2)/(_spinBox1->y()-pointSize);
}


