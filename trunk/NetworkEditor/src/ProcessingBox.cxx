#include "ProcessingBox.hxx"
#include "NetworkCanvas.hxx"

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QInputDialog>

ProcessingBox::~ProcessingBox()
{
	if (_embeded) delete _embeded;
}

ProcessingBox::ProcessingBox(NetworkCanvas * parent, const QString & name,
		unsigned nInports, unsigned nOutports,
		unsigned nIncontrols, unsigned nOutcontrols)
	: _canvas(parent)
	, _processing(0)
	, _embeded(0)
	, _name(name)
	, _selected(false)
	, _nInports(nInports)
	, _nOutports(nOutports)
	, _nIncontrols(nIncontrols)
	, _nOutcontrols(nOutcontrols)
	, _actionMode(NoAction)
	, _highLightRegion(noRegion)
{
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

void ProcessingBox::setProcessing(void * model)
{
	_processing = model;
	QWidget * embeded = _canvas->embededWidgetFor(_processing);
	embed(embeded);
	refreshConnectors();
}

void ProcessingBox::refreshConnectors()
{
	if (!_processing) return;
	_nInports = _canvas->nInports(_processing);
	_nOutports = _canvas->nOutports(_processing);
	_nIncontrols = _canvas->nIncontrols(_processing);
	_nOutcontrols = _canvas->nOutcontrols(_processing);
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
	if (_embeded && embededMinHeight<_embeded->minimumHeight())
		embededMinHeight = _embeded->minimumHeight();
	if (_embeded && embededMinHeight<_embeded->minimumSizeHint().height())
		embededMinHeight = _embeded->minimumSizeHint().height();
	int minimumHeight = textHeight+embededMinHeight;
	int inportsHeight = _nInports*portStep;
	int outportsHeight = _nOutports*portStep;
	if (minimumHeight<inportsHeight) minimumHeight = inportsHeight;
	if (minimumHeight<outportsHeight) minimumHeight = outportsHeight;
	minimumHeight += 2*portOffset;

	int minimumWidth = metrics.width(_name);
	if (_embeded && minimumWidth<_embeded->minimumWidth())
		minimumWidth = _embeded->minimumWidth();
	if (_embeded && minimumWidth<_embeded->minimumSizeHint().width())
		minimumWidth = _embeded->minimumSizeHint().width();
	int incontrolsWidth = _nIncontrols*controlStep;
	int outcontrolsWidth = _nOutcontrols*controlStep;
	if (minimumWidth<incontrolsWidth) minimumWidth = incontrolsWidth;
	if (minimumWidth<outcontrolsWidth) minimumWidth = outcontrolsWidth;
	minimumWidth += 2*controlOffset;

	// If exists hint size and is greater that minimum use instead of (until 500x500)
	if (_embeded && _embeded->sizeHint().isValid())
	{
		//std::cout<<"minimum size: "<<minimumWidth<<"; "<<minimumHeight<<std::endl;
		const double maximumEmbededWidth=std::max(500,minimumWidth);
		const double maximumEmbededHeight=std::max(500,minimumHeight);
		//std::cout<<"maximum size: "<<maximumEmbededWidth<<"; "<<maximumEmbededHeight<<std::endl;
		double factor=1;
		const double embededHintWidth = _embeded->sizeHint().width();
		const double embededHintHeight = _embeded->sizeHint().height();
		//std::cout<<"hint size: "<<embededHintWidth<<"; "<<embededHintHeight<<std::endl;

		if (embededHintWidth>maximumEmbededWidth)
			factor=(maximumEmbededWidth/embededHintWidth);
		if (embededHintHeight>maximumEmbededHeight)
			factor=std::min(factor,(maximumEmbededHeight/embededHintHeight));
		minimumHeight=std::max(minimumHeight,(int)(embededHintHeight*factor));
		minimumWidth=std::max(minimumWidth,(int)(embededHintWidth*factor));
		//std::cout<<"Factor: "<<factor<<std::endl;
		//std::cout<<"resize: "<<minimumWidth<<"; "<<minimumHeight<<std::endl;
	}

	_minimumSize = QSize(minimumWidth, minimumHeight );
	resize(_size);
	_canvas->update();
}

void ProcessingBox::paintBox(QPainter & painter)
{
	QColor boxBodyColor = _canvas->isOk(_processing)? _canvas->colorBoxBody() : _canvas->colorBoxErrorBody();
	QColor boxFrameColor = _canvas->isOk(_processing)? _canvas->colorBoxFrame() : _canvas->colorBoxErrorFrame();
	// Box
	painter.setPen( _canvas->colorBoxFrameOutline());
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
	painter.setPen(_canvas->colorPortOutline());
	for (unsigned i = 0; i<_nInports; i++)
	{
		painter.setBrush( _canvas->inportColor(_processing,i) );
		drawConnector(painter, inportsRegion, i);
	}
	for (unsigned i = 0; i<_nOutports; i++)
	{
		painter.setBrush( _canvas->outportColor(_processing,i) );
		drawConnector(painter, outportsRegion, i);
	}
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
	//Selection
	if (_selected)
	{
		painter.setBrush( _canvas->palette().highlight() );
		painter.drawRect(0, 0, margin, margin);
		painter.drawRect(0, _size.height(), margin, -margin);
		painter.drawRect(_size.width(), 0,  -margin, margin);
		painter.drawRect(_size.width(), _size.height(), -margin, -margin);
	}
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
		if (event->modifiers() & Qt::ControlModifier )
		{
			_selected=!_selected;
			if (!_selected) return;
		}
		if (!_selected)
		{
			_canvas->clearSelections();
			_selected=true;
		}
		_canvas->startMovingSelected(event);
		return;
	}
	if (region==bodyRegion)
	{
		if (event->modifiers() & Qt::ControlModifier )
		{
			_selected=!_selected;
		}
		else
		{
			_canvas->clearSelections();
			_selected=true;
		}
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
	if (_actionMode==Moving)
	{
		_canvas->setCursor(Qt::SizeAllCursor);
		QPoint dragDelta = _canvas->translatedGlobalPos(event) - dragOrigin;
		move(originalPosition + dragDelta);
		return;
	}
	if (_actionMode==Resizing)
	{
		_canvas->setCursor(Qt::SizeFDiagCursor);
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
		
		_canvas->setToolTip(_canvas->inportTooltip(_processing, index));
		return;
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(_canvas->outportTooltip(_processing, index));
		return;
	}
	if (region==incontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(_canvas->incontrolTooltip(_processing, index));
		return;
	}
	if (region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(_canvas->outcontrolTooltip(_processing, index));
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
		if (not _canvas->isOk(_processing)) 
			_canvas->setToolTip(_canvas->errorMessage(_processing));
		_canvas->setStatusTip(QObject::tr("Double click: configure. Left click: Processing menu"));
		return;
	}
	if (region==nameRegion)
	{
		if (not _canvas->isOk(_processing)) _canvas->setToolTip(_canvas->errorMessage(_processing));
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
		_canvas->endWireDrag(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::InportDrag &&  region==outportsRegion)
	{
		int index = portIndexByYPos(_canvas->translatedPos(event));
		_canvas->endWireDrag(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::OutcontrolDrag && region==incontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_canvas->endWireDrag(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::IncontrolDrag && region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(_canvas->translatedPos(event));
		_canvas->endWireDrag(this, index);
	}
}
void ProcessingBox::mouseDoubleClickEvent(QMouseEvent * event)
{
	QPoint point =_canvas->translatedPos(event);
	Region region = getRegion(point);
	if (region==nameRegion) rename();
	if (region==incontrolsRegion)
	{
		_canvas->addControlSenderProcessing(this, point);
	}
	if (region==outcontrolsRegion)
	{
		_canvas->addControlPrinterProcessing(this, point);
	}
}

bool ProcessingBox::rename()
{
	bool ok=true;
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

QString ProcessingBox::getInportName(unsigned index) const
{
	return _canvas->inportName(_processing, index);
}
QString ProcessingBox::getOutportName(unsigned index) const
{
	return _canvas->outportName(_processing, index);
}
QString ProcessingBox::getOutcontrolName(unsigned index) const
{
	return _canvas->outcontrolName(_processing, index);
}
QString ProcessingBox::getIncontrolName(unsigned index) const
{
	return _canvas->incontrolName(_processing, index);
}

QString ProcessingBox::getInportPrototyperName(const QPoint & point) const
{
	return getConnectionPrototyperName("InPort", _canvas->inportName(_processing, portIndexByYPos(point)));
}
QString ProcessingBox::getOutportPrototyperName(const QPoint & point) const
{
	return getConnectionPrototyperName("OutPort", _canvas->outportName(_processing, portIndexByYPos(point)));
}
QString ProcessingBox::getIncontrolPrototyperName(const QPoint & point) const
{
	return getConnectionPrototyperName("InControl", _canvas->incontrolName(_processing, controlIndexByXPos(point)));
}
QString ProcessingBox::getOutcontrolPrototyperName(const QPoint & point) const
{
	return getConnectionPrototyperName("OutControl", _canvas->outcontrolName(_processing, controlIndexByXPos(point)));
}

QString ProcessingBox::getConnectionPrototyperName(QString kind, QString connectionName) const
{
	return QString("%1__%2__%3")
		.arg(kind)
		.arg(getName().replace(" ","___"))
		.arg(connectionName.replace(" ","___"));
}

bool ProcessingBox::configure()
{
	if (_processing) return _canvas->editConfiguration(this);

	// TODO: move this to the dummy canvas

	Ui::DummyProcessingConfig ui;
	QDialog * configDialog = new QDialog(_canvas);
	ui.setupUi(configDialog);
	ui.inports->setValue(_nInports);
	ui.outports->setValue(_nOutports);
	ui.incontrols->setValue(_nIncontrols);
	ui.outcontrols->setValue(_nOutcontrols);
	if ( !configDialog->exec()) return false;
	// TODO: clear connections
	_nInports= ui.inports->value();
	_nOutports= ui.outports->value();
	_nIncontrols= ui.incontrols->value();
	_nOutcontrols= ui.outcontrols->value();
	recomputeMinimumSizes();
	return true;
}

