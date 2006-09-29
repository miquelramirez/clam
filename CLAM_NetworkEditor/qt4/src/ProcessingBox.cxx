#include "ProcessingBox.hxx"
#include "NetworkCanvas.hxx"
#include "Vumeter.hxx"
#include "ControlSenderWidget.hxx"
#include <QtGui/QSlider>
#include <QtDebug>

ProcessingBox::~ProcessingBox()
{
	if (_embeded) delete _embeded;
}

ProcessingBox::ProcessingBox(NetworkCanvas * parent, const QString & name,
		unsigned nInports, unsigned nOutports,
		unsigned nIncontrols, unsigned nOutcontrols)
	: _canvas(parent)
	, _name(name)
	, _nInports(nInports)
	, _nOutports(nOutports)
	, _nIncontrols(nIncontrols)
	, _nOutcontrols(nOutcontrols)
	, _actionMode(NoAction)
	, _highLightRegion(noRegion)
	, _processing(0)
	, _embeded(0)
{
//	embed(new QSlider(Qt::Horizontal));
	setName(name);
	recomputeMinimumSizes();
}

void ProcessingBox::embed(QWidget * widget)
{
	if (_embeded) delete _embeded;
	_embeded = widget;
	if (!_embeded) return;
	_embeded->setParent(_canvas);
	_embeded->show();
}

void ProcessingBox::updateEmbededWidget()
{
	if (!_embeded) return;
	QRect embedZone(
			controlOffset, portOffset + textHeight,
   			_size.width()-2*controlOffset, _size.height()-textHeight-2*portOffset);
	embedZone.translate(pos());
	embedZone = _canvas->translatedRect(embedZone);
	_embeded->setGeometry(embedZone);
}

void ProcessingBox::paintFromParent(QPainter & painter)
{
	updateEmbededWidget();
	painter.save();
	painter.translate(_pos);
	paintBox(painter);
	painter.restore();
}
void ProcessingBox::setProcessing(CLAM::Processing * processing)
{
	_processing = processing;
	QWidget * embeded = 0;
	if (_processing and _processing->GetClassName()==std::string("OutControlSender"))
		embeded = new ControlSenderWidget(_processing);
	if (_processing and _processing->GetClassName()==std::string("AudioPortMonitor"))
		embeded = new Vumeter(_processing);
	embed(embeded);
	refreshConnectors();
}

void ProcessingBox::refreshConnectors()
{
	if (!_processing) return;
	_nInports = _processing->GetInPorts().Size();
	_nOutports = _processing->GetOutPorts().Size();
	_nIncontrols = _processing->GetInControls().Size();
	_nOutcontrols = _processing->GetOutControls().Size();
	recomputeMinimumSizes();
}

void ProcessingBox::resize(const QSize & size)
{
	_size=size.expandedTo(_minimumSize);
}
void ProcessingBox::move(const QPoint & point)
{
	_pos=point;
}

void ProcessingBox::setName(const QString & newName)
{
	_name=newName;
	recomputeMinimumSizes();
}
void ProcessingBox::recomputeMinimumSizes()
{
	QFontMetrics metrics(_canvas->font());
	textHeight = metrics.height();
	int embededMinHeight = 2*margin;
	if (_embeded and embededMinHeight<_embeded->minimumHeight())
		embededMinHeight = _embeded->minimumHeight();
	if (_embeded and embededMinHeight<_embeded->minimumSizeHint().height())
		embededMinHeight = _embeded->minimumSizeHint().height();
	int minimumHeight = textHeight+embededMinHeight;
	int inportsHeight = _nInports*portStep;
	int outportsHeight = _nOutports*portStep;
	if (minimumHeight<inportsHeight) minimumHeight = inportsHeight;
	if (minimumHeight<outportsHeight) minimumHeight = outportsHeight;
	minimumHeight += 2*portOffset;

	int minimumWidth = metrics.width(_name);
	if (_embeded and minimumWidth<_embeded->minimumWidth())
		minimumWidth = _embeded->minimumWidth();
	if (_embeded and minimumWidth<_embeded->minimumSizeHint().width())
		minimumWidth = _embeded->minimumSizeHint().width();
	int incontrolsWidth = _nIncontrols*controlStep;
	int outcontrolsWidth = _nOutcontrols*controlStep;
	if (minimumWidth<incontrolsWidth) minimumWidth = incontrolsWidth;
	if (minimumWidth<outcontrolsWidth) minimumWidth = outcontrolsWidth;
	minimumWidth += 2*controlOffset;

	_minimumSize = QSize(minimumWidth, minimumHeight );
	resize(_size);
	_canvas->update();
}
QColor execBodyColor(QColor defaultColor, CLAM::Processing * processing)
{
	if (not processing) return defaultColor;
	if (processing->GetExecState() == CLAM::Processing::Unconfigured)
		return QColor(0xff,0xa0,0xa0,0x70);
	return defaultColor;
}

QColor execFrameColor(QColor defaultColor, CLAM::Processing * processing)
{
	if (not processing) return defaultColor;
	if (processing->GetExecState() == CLAM::Processing::Unconfigured)
		return QColor(0xff,0x00,0x00,0xa0);
	return defaultColor;
}

void ProcessingBox::paintBox(QPainter & painter)
{
	painter.setRenderHint(QPainter::Antialiasing);

	QColor boxBodyColor = execBodyColor(_canvas->colorBoxBody(),_processing);
	QColor boxFrameColor = execFrameColor(_canvas->colorBoxFrame(),_processing);
	// Box
	painter.setPen(_canvas->colorBoxFrameOutline());
	painter.setBrush(boxFrameColor);
	painter.drawRect(portWidth, controlHeight,
		   	_size.width()-2*portWidth, _size.height()-2*controlHeight);
	painter.setBrush(boxBodyColor);
	painter.drawRect(controlOffset, portOffset+textHeight,
		   	_size.width()-2*controlOffset, _size.height()-textHeight-2*portOffset);
	painter.setBrush(_canvas->colorResizeHandle());
	painter.drawRect(_size.width()-controlOffset, _size.height()-portOffset,
		   	margin, margin);
	// Ports
	painter.setBrush(_canvas->colorPort());
	painter.setPen(_canvas->colorPortOutline());
	for (unsigned i = 0; i<_nInports; i++)
		drawConnector(painter, inportsRegion, i);
	for (unsigned i = 0; i<_nOutports; i++)
		drawConnector(painter, outportsRegion, i);
	// Controls
	painter.setBrush(_canvas->colorControl());
	painter.setPen(_canvas->colorControlOutline());
	for (unsigned i = 0; i<_nIncontrols; i++)
		drawConnector(painter, incontrolsRegion, i);
	for (unsigned i = 0; i<_nOutcontrols; i++)
		drawConnector(painter, outcontrolsRegion, i);

	// Highlights
	if (_highLightRegion != noRegion)
	{
		painter.setBrush(_canvas->colorHighlightConnector(this, _highLightRegion, _highLightConnection));
		painter.setPen(_canvas->colorPortOutline());
		drawConnector(painter, _highLightRegion, _highLightConnection);
	}

	// Text
	painter.setPen(_canvas->colorBoxFrameText());
	painter.drawText(QRect(controlOffset, portOffset,
			_size.width()-2*controlOffset, textHeight), _name);
}

void ProcessingBox::drawConnector(QPainter & painter, Region region, unsigned index)
{
	switch (region)
	{
	case inportsRegion:
		painter.drawEllipse(
				0, portOffset+index*portStep,
			   	portWidth, portHeight);
	return;
	case outportsRegion:
		painter.drawEllipse(
				_size.width()-portWidth, portOffset+index*portStep,
			   	portWidth, portHeight);
	return;
	case incontrolsRegion:
		painter.drawRect(
				controlOffset+index*controlStep, 0,
			   	controlWidth, controlHeight);
	return;
	case outcontrolsRegion:
		painter.drawRect(
				controlOffset+index*controlStep, _size.height()-controlHeight,
			   	controlWidth, controlHeight);
	return;
	default:
	return;
	}
}

ProcessingBox::Region ProcessingBox::getRegion(const QPoint & point) const
{
	int x = point.x()-_pos.x();
	int y = point.y()-_pos.y();
	if (x<0)
		return noRegion;
	if (x>_size.width())
		return noRegion;
	if (y<0)
		return noRegion;
	if (y>_size.height())
		return noRegion;

	if (x<portWidth)
	{
		if (y<portOffset) return noRegion;
		if (y>=portOffset+_nInports*portStep) return noRegion;
		return inportsRegion;
	}
	if (x>_size.width()-portWidth)
	{
		if (y<portOffset) return noRegion;
		if (y>=portOffset+_nOutports*portStep) return noRegion;
		return outportsRegion;
	}
	if (y>=0 && y<controlHeight)
	{
		if (x<controlOffset) return noRegion;
		if (x>=controlOffset+_nIncontrols*controlStep) return noRegion;
		return incontrolsRegion;
	}
	if (y<=_size.height() && y>_size.height()-controlHeight)
	{
		if (x<controlOffset) return noRegion;
		if (x>=controlOffset+_nOutcontrols*controlStep) return noRegion;
		return outcontrolsRegion;
	}
	if (y<textHeight+portOffset)
		return nameRegion;
	if (x>_size.width()-controlOffset && y>_size.height()-portOffset)
		return resizeHandleRegion;
	return bodyRegion;
}


int ProcessingBox::portIndexByYPos(const QPoint & point) const
{
	int y = point.y()-_pos.y();
	return (y-portOffset)/portStep;
}
int ProcessingBox::controlIndexByXPos(const QPoint & point) const
{
	int x = point.x()-_pos.x();
	return (x-controlOffset)/controlStep;
}
// returns the inport connect point in parent coords
QPoint ProcessingBox::getInportPos(unsigned i) const
{
	return _pos + QPoint( 0, portOffset+i*portStep + portHeight/2  );
}
QPoint ProcessingBox::getOutportPos(unsigned i) const
{
	return _pos + QPoint( _size.width(), portOffset+i*portStep + portHeight/2  );
}
QPoint ProcessingBox::getIncontrolPos(unsigned i) const
{
	return _pos + QPoint( controlOffset+i*controlStep + controlWidth/2, 0 );
}
QPoint ProcessingBox::getOutcontrolPos(unsigned i) const
{
	return _pos + QPoint( controlOffset+i*controlStep + controlWidth/2, _size.height()  );
}

void ProcessingBox::startMoving(const QPoint & initialGlobalPos)
{
	_actionMode = Moving;
	originalPosition = _pos;
	dragOrigin = initialGlobalPos;
}

void ProcessingBox::mousePressEvent(QMouseEvent * event)
{
	Region region = getRegion(_canvas->translatedPos(event));
	if (region==noRegion) return;
	_canvas->raise(this);
	// Head
	if (region==nameRegion)
	{
		startMoving(_canvas->translatedGlobalPos(event));
		_canvas->setCursor(Qt::SizeAllCursor);
		return;
	}
	// Resize corner
	if (region==resizeHandleRegion)
	{
		_actionMode = Resizing;
		originalSize = _size;
		dragOrigin=_canvas->translatedGlobalPos(event);
		_canvas->setCursor(Qt::SizeFDiagCursor);
		return;
	}
	if (region==inportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_canvas->startDrag(NetworkCanvas::InportDrag, this, index);
		return;
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_canvas->startDrag(NetworkCanvas::OutportDrag, this, index);
		return;
	}
	if (region==incontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_canvas->startDrag(NetworkCanvas::IncontrolDrag, this, index);
		return;
	}
	if (region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_canvas->startDrag(NetworkCanvas::OutcontrolDrag, this, index);
		return;
	}
}
void ProcessingBox::mouseMoveEvent(QMouseEvent * event)
{
	_highLightRegion=noRegion;;
	if (_actionMode==Moving || _canvas->dragStatus()==NetworkCanvas::PanDrag)
	{
		QPoint dragDelta = _canvas->translatedGlobalPos(event) - dragOrigin;
		move(originalPosition + dragDelta);
		return;
	}
	if (_actionMode==Resizing)
	{
		QPoint dragDelta = _canvas->translatedGlobalPos(event) - dragOrigin;
		resize(QSize(
			originalSize.width() + dragDelta.x(),
			originalSize.height() + dragDelta.y()
			));
		return;
	}
	Region region = getRegion(_canvas->translatedPos(event));
	if (region==noRegion) return;
	_canvas->setCursor(Qt::ArrowCursor);
	if (region==inportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(getInportName(index));
		return;
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(getOutportName(index));
		return;
	}
	if (region==incontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(getIncontrolName(index));
		return;
	}
	if (region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(getOutcontrolName(index));
		return;
	}
	if (region==resizeHandleRegion)
	{
		_canvas->setCursor(Qt::SizeFDiagCursor);
		_canvas->setStatusTip(QObject::tr("Drag: resize"));
		return;
	}
	if (region==bodyRegion)
	{
		if (_processing and _processing->GetExecState()==CLAM::Processing::Unconfigured)
			_canvas->setToolTip(_processing->GetConfigErrorMessage().c_str());
		_canvas->setStatusTip(QObject::tr("Double click: configure. Left click: Processing menu"));
		return;
	}
	if (region==nameRegion)
	{
		if (_processing and _processing->GetExecState()==CLAM::Processing::Unconfigured)
			_canvas->setToolTip(_processing->GetConfigErrorMessage().c_str());
		_canvas->setStatusTip(QObject::tr("Drag: move. Double click: rename. Left click: Processing menu"));
		return;
	}
}
void ProcessingBox::mouseReleaseEvent(QMouseEvent * event)
{
	if (_actionMode==Moving)
	{
		_canvas->setCursor(Qt::ArrowCursor);
		_actionMode = NoAction;
	}
	if (_actionMode==Resizing)
	{
		_canvas->setCursor(Qt::ArrowCursor);
		_actionMode = NoAction;
	}
	Region region = getRegion(_canvas->translatedPos(event));
	if (_canvas->dragStatus()==NetworkCanvas::OutportDrag && region==inportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_canvas->endConnectionDragTo(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::InportDrag &&  region==outportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_canvas->endConnectionDragTo(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::OutcontrolDrag && region==incontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_canvas->endConnectionDragTo(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::IncontrolDrag && region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_canvas->endConnectionDragTo(this, index);
	}
}
void ProcessingBox::mouseDoubleClickEvent(QMouseEvent * event)
{
	Region region = getRegion(_canvas->translatedPos(event));
	if (region==nameRegion) rename();
}

bool ProcessingBox::rename()
{
	bool ok;
	QString newName = QInputDialog::getText(_canvas, QObject::tr("Rename the processing"), QObject::tr("New name"), QLineEdit::Normal, _name);
	if (!ok) return false;
	if (newName.isEmpty()) return false;
	if (!_canvas->renameProcessing(_name, newName))
	{
		QMessageBox::critical(_canvas, QObject::tr("Naming processing"),
			QObject::tr("A processing already exists with this name."));
		return false;
	}
	setName(newName);
	_canvas->markAsChanged();
	return true;
}

QString ProcessingBox::getName() const
{
	return _name;
}

QString ProcessingBox::getOutportName(unsigned index) const
{
	if (!_processing) return QString("Outport_%1").arg(index);
	CLAM::OutPortRegistry & outPorts = _processing->GetOutPorts();
	return outPorts.GetByNumber(index).GetName().c_str();
}
QString ProcessingBox::getInportName(unsigned index) const
{
	if (!_processing) return QString("Inport_%1").arg(index);
	CLAM::InPortRegistry & inPorts = _processing->GetInPorts();
	return inPorts.GetByNumber(index).GetName().c_str();
}
QString ProcessingBox::getOutcontrolName(unsigned index) const
{
	if (!_processing) return QString("Outcontrol_%1").arg(index);
	CLAM::OutControlRegistry & outControls = _processing->GetOutControls();
	return outControls.GetByNumber(index).GetName().c_str();
}
QString ProcessingBox::getIncontrolName(unsigned index) const
{
	if (!_processing) return QString("Incontrol_%1").arg(index);
	CLAM::InControlRegistry & inControls = _processing->GetInControls();
	return inControls.GetByNumber(index).GetName().c_str();
}
QString ProcessingBox::getPrototyperInportName(const QPoint & point) const
{
	return QString("Inport__%1__%2")
		.arg(getName())
		.arg(getInportName(portIndexByYPos(point)));
}
QString ProcessingBox::getPrototyperOutportName(const QPoint & point) const
{
	return QString("Outport__%1__%2")
		.arg(getName())
		.arg(getOutportName(portIndexByYPos(point)));
}
QString ProcessingBox::getPrototyperIncontrolName(const QPoint & point) const
{
	return QString("Incontrol__%1__%2")
		.arg(getName())
		.arg(getIncontrolName(controlIndexByXPos(point)));
}
QString ProcessingBox::getPrototyperOutcontrolName(const QPoint & point) const
{
	return QString("Outcontrol__%1__%2")
		.arg(getName())
		.arg(getOutcontrolName(controlIndexByXPos(point)));
}

#include "ConfiguratorLauncher.hxx"
#include "uic_DummyProcessingConfig.hxx"
#include <CLAM/Factory.hxx>

bool ProcessingBox::configure()
{
	if (_processing)
	{
		const CLAM::ProcessingConfig & config = _processing->GetConfig();
		ConfiguratorLauncher * launcher=0;
	   	try
		{
			launcher = CLAM::Factory<ConfiguratorLauncher>::GetInstance()
				.CreateSafe(config.GetClassName());
		}
		catch (CLAM::ErrFactory & e)
		{
			QMessageBox::critical(_canvas, _canvas->tr("Configuring"),
				   	_canvas->tr("No configuration dialog available for this processing"));
			return false;
		}
		if (not launcher->Launch(*_processing,_name)) return false;
		setProcessing(_processing);
		return true;
	}
	else
	{
		Ui::DummyProcessingConfig ui;
		QDialog * configDialog = new QDialog(_canvas);
		ui.setupUi(configDialog);
		ui.inports->setValue(_nInports);
		ui.outports->setValue(_nOutports);
		ui.incontrols->setValue(_nIncontrols);
		ui.outcontrols->setValue(_nOutcontrols);
		if (not configDialog->exec()) return false;
		// TODO: clear connections
		_nInports= ui.inports->value();
		_nOutports= ui.outports->value();
		_nIncontrols= ui.incontrols->value();
		_nOutcontrols= ui.outcontrols->value();
		recomputeMinimumSizes();
		return true;
	}
}
