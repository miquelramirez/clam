#include "ControlSurfaceWidget.hxx"
#include <QtGui/QLabel>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include "EmbededWidgets.hxx"

static CLAM::EmbededWidgetCreator <ControlSurfaceWidget> reg("ControlSurface");


ControlSurfaceWidget::ControlSurfaceWidget(CLAM::Processing * processing, QWidget * parent)
	: QWidget(parent)
	, _spinBoxX(0)
	, _spinBoxY(0)
	, _sender(0)
	, _minX(0)
	, _minY(0)
	, _maxX(1)
	, _maxY(1)
	, _defaultX(0)
	, _defaultY(0)
	, _updating(false)
	, _dragging(false)
	, _areSpinBoxesHidden(false)
	, _pointPenColor(Qt::black)
	, _pointBrushColor(Qt::red)
{
	if (processing)
	{
		_sender = dynamic_cast<CLAM::ControlSurface* >(processing);
		CLAM_ASSERT(_sender, "ControlSurfaceWidget only works "
					"with ControlSurface processings.");
		const CLAM::ControlSurfaceConfig * config = 
			dynamic_cast<const CLAM::ControlSurfaceConfig *>(&_sender->GetConfig());
		CLAM_ASSERT( config, "Unexpected Configuration type for an ControlSurface" );

		_minX = config->GetMinX();
		_defaultX = config->GetDefaultX();
		_maxX = config->GetMaxX();
		_minY = config->GetMinY();
		_defaultY = config->GetDefaultY();
		_maxY = config->GetMaxY();
	}
	QVBoxLayout * mainLayout= new QVBoxLayout;
	setLayout(mainLayout);
	mainLayout->setMargin(1);

	_surface = new QLabel;
	mainLayout->addWidget(_surface);
	_surface->setMinimumSize(100,100);
	_surface->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QHBoxLayout * spinLayout = new QHBoxLayout;
	mainLayout->addLayout(spinLayout);
	_spinBoxX = new QDoubleSpinBox;
	spinLayout->addWidget(_spinBoxX);

	_spinBoxX->setMinimum(_minX);
	_spinBoxX->setMaximum(_maxX);
	_spinBoxX->setValue(_defaultX);
	_spinBoxX->setSingleStep((_maxX-_minX)/100);

	connect(_spinBoxX, SIGNAL(valueChanged(double)), 
		this, SLOT(spinBoxChanged()));

	_spinBoxY = new QDoubleSpinBox();
	spinLayout->addWidget(_spinBoxY);

	_spinBoxY->setMinimum(_minY);
	_spinBoxY->setMaximum(_maxY);
	_spinBoxY->setValue(_defaultY);
	_spinBoxY->setSingleStep((_maxY-_minY)/100);

	hideSpinBoxes(false);

	connect(_spinBoxY, SIGNAL(valueChanged(double)), 
		this, SLOT(spinBoxChanged()));
}

void ControlSurfaceWidget::hideSpinBoxes(bool hide)
{
	_areSpinBoxesHidden=hide;
	if (_areSpinBoxesHidden)
	{
		_spinBoxX->hide();
		_spinBoxY->hide();
	}
	else
	{
		_spinBoxX->show();
		_spinBoxY->show();
	}
	update();
}

bool ControlSurfaceWidget::spinBoxesHidden()
{
	return _areSpinBoxesHidden;
}


ControlSurfaceWidget::~ControlSurfaceWidget()
{
}

void ControlSurfaceWidget::moveSurface(int posX, int posY)
{
	if (_updating) return;
	_updating = true;

	double valueX = mapX(posX);
	double valueY = mapY(posY);
	_spinBoxX->setValue(valueX);
	_spinBoxY->setValue(valueY);
	if (_sender)
		_sender->SendControl(valueX, valueY);

	_updating = false;
	update();
	emit surfaceMoved(valueX, valueY);
	emit updatedX(valueX);
	emit updatedY(valueY);
}

void ControlSurfaceWidget::spinBoxChanged()
{
	if (_updating) return;
	_updating = true;

	double valueX = _spinBoxX->value(); 
	double valueY = _spinBoxY->value();
	if (_sender)
		_sender->SendControl(valueX, valueY);

	_updating = false;
	update();
	emit surfaceMoved(valueX, valueY);
	emit updatedX(valueX);
	emit updatedY(valueY);
}

void ControlSurfaceWidget::paintEvent(QPaintEvent * event)
{
	const unsigned xSize = width()-pointSize;
	const unsigned ySize = (_spinBoxX->isHidden() ? height() : _spinBoxX->y())-pointSize;
	QPainter painter(this);
	painter.setBrush(_pointBrushColor);
	painter.setPen(_pointPenColor);
	QPointF point(
		pointSize/2+(_spinBoxX->value()-_minX)*xSize/(_maxX-_minX),
		pointSize/2+(_maxY-_spinBoxY->value())*ySize/(_maxY-_minY));
	painter.drawRoundRect(point.x()-pointSize/2, point.y()-pointSize/2,pointSize,pointSize,2,2);
}

void ControlSurfaceWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->y()>_spinBoxX->y()) return;
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
	if (x<pointSize/2) return _minX;
	if (x>width()-pointSize/2) return _maxX;
	return _minX+(x-pointSize/2)*(_maxX-_minX)/(width()-pointSize);
}
double ControlSurfaceWidget::mapY(int y) const
{
	int enabledHeight= _spinBoxX->isHidden() ? height() : _spinBoxX->y();
	if (y<pointSize/2) return _maxY;
	if (y>enabledHeight-pointSize/2) return _minY;
	return _maxY-(y-pointSize/2)*(_maxY-_minY)/(enabledHeight-pointSize);
}


