#include "ProcessingBox.hxx"
#include "NetworkCanvas.hxx"

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>
#include <QtGui/QMouseEvent>
#include <QtGui/QInputDialog>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsSceneContextMenuEvent>
#include <CLAM/ProcessingFactory.hxx> 

static std::string processingBoxRegionName(ProcessingBox::Region region)
{
	switch(region)
	{
		case ProcessingBox::nameRegion:        return "nameRegion";
		case ProcessingBox::bodyRegion:        return "bodyRegion";
		case ProcessingBox::resizeHandleRegion:return "resizeHandleRegion";
		case ProcessingBox::inportsRegion:     return "inportsRegion";
		case ProcessingBox::outportsRegion:    return "outportsRegion";
		case ProcessingBox::incontrolsRegion:  return "incontrolsRegion";
		case ProcessingBox::outcontrolsRegion: return "outcontrolsRegion";
		case ProcessingBox::noRegion:          return "noRegion";
		default:                               return "invalidRegion";
	}
}

static std::string processingBoxAction(ProcessingBox::ActionMode action)
{
	switch(action)
	{
		case ProcessingBox::NoAction:        return "NoAction";
		case ProcessingBox::Resizing:        return "Resizing";
		case ProcessingBox::Moving:        return "Moving";
		default:                               return "Which drag??";
	}
}

static std::string networkCanvasDragMode(NetworkCanvas::DragStatus dragMode)
{
	switch(dragMode)
	{
		case NetworkCanvas::NoDrag:        return "NoDrag";
		case NetworkCanvas::MoveDrag:        return "MoveDrag";
		case NetworkCanvas::ResizeDrag:    return "ResizeDrag";
		case NetworkCanvas::SelectionDrag:    return "SelectionDrag";
		case NetworkCanvas::InportDrag:     return "InportDrag";
		case NetworkCanvas::OutportDrag:    return "OutportDrag";
		case NetworkCanvas::IncontrolDrag:  return "IncontrolDrag";
		case NetworkCanvas::OutcontrolDrag: return "OutcontrolDrag";
		default:                               return "Which drag??";
	}
}

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

QRectF ProcessingBox::boundingRect() const
{	
	return QRectF(_pos.x(), _pos.y(), _size.width(), _size.height());
}
void ProcessingBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	paintFromParent(*painter);
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
	embedZone.translate(_pos);
	embedZone = _canvas->translatedRect(embedZone);
	_embeded->setGeometry(embedZone);
}

void ProcessingBox::paintFromParent(QPainter & painter)
{
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
	updateEmbededWidget();
}
void ProcessingBox::move(const QPoint & point)
{
	_pos=point;
	updateEmbededWidget();
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
	painter.drawRect(portWidth, controlHeight, _size.width()-2*portWidth, _size.height()-2*controlHeight);
	painter.setBrush(boxBodyColor);
	painter.drawRect(controlOffset, portOffset+textHeight, _size.width()-2*controlOffset, _size.height()-textHeight-2*portOffset);
	painter.setBrush(_canvas->colorResizeHandle());
	painter.drawRect(_size.width()-controlOffset, _size.height()-portOffset, margin, margin);
		
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
	painter.setPen(_canvas->colorControlOutline());
	for (unsigned i = 0; i<_nIncontrols; i++)
	{
		painter.setBrush(_canvas->incontrolColor(_processing,i));
		drawConnector(painter, incontrolsRegion, i);
	}
	for (unsigned i = 0; i<_nOutcontrols; i++)
	{
		painter.setBrush(_canvas->outcontrolColor(_processing,i));
		drawConnector(painter, outcontrolsRegion, i);
	}

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

void ProcessingBox::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QPoint scenePoint = event->scenePos().toPoint();
	Region region = getRegion(scenePoint);
	std::cout << "PB::mouseMove " << _name.toStdString() << " " << processingBoxRegionName(region) << " " << processingBoxAction(_actionMode) << std::endl;
	if (region==noRegion)
	{
		event->ignore(); // Let other children to take it
		return;
	}
	_canvas->raise(this);
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
		_canvas->startMovingSelected(scenePoint);
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
		return;
	}
	if (region==resizeHandleRegion)
	{
		_actionMode = Resizing;
		originalSize = _size;
		dragOrigin=scenePoint;
		_canvas->setCursor(Qt::SizeFDiagCursor);
		return;
	}
	// connection actions
	if (region==inportsRegion)
	{
		int index = portIndexByYPos(scenePoint);
		_canvas->startDrag(NetworkCanvas::InportDrag, this, index);
		return;
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(scenePoint);
		_canvas->startDrag(NetworkCanvas::OutportDrag, this, index);
		return;
	}
	if (region==incontrolsRegion)
	{
		int index = controlIndexByXPos(scenePoint);
		_canvas->startDrag(NetworkCanvas::IncontrolDrag, this, index);
		return;
	}
	if (region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(scenePoint);
		_canvas->startDrag(NetworkCanvas::OutcontrolDrag, this, index);
		return;
	}
}
void ProcessingBox::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	std::cout << "PB::mouseMove " << _name.toStdString() << std::endl;
	QPoint scenePoint = event->scenePos().toPoint();

	if (_actionMode==Moving)
	{
		if(!_selected)
		{
			_canvas->clearSelections();
			return;
		}
		QPoint delta = scenePoint - dragOrigin;
		_canvas->keepMovingSelected(delta);
		return;
	}
	if (_actionMode==Resizing)
	{
		_canvas->setCursor(Qt::SizeFDiagCursor);
		QPoint dragDelta = scenePoint - dragOrigin;
		resize(QSize(
			originalSize.width() + dragDelta.x(),
			originalSize.height() + dragDelta.y()
			));
		return;
	}
}
void ProcessingBox::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	std::cout << "PB::mouseRelease " << _name.toStdString() << std::endl;
	if (_actionMode==Moving or _actionMode==Resizing)
	{
		_canvas->setCursor(Qt::ArrowCursor);
		_actionMode = NoAction;
	}
}
void ProcessingBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	std::cout << "PB::mouseDoubleClick " << _name.toStdString() << std::endl;
	QPoint point = event->scenePos().toPoint();
	doubleClicking(point);
}
void ProcessingBox::contextMenuEvent( QGraphicsSceneContextMenuEvent * event )
{
	QMenu menu(_canvas);
	QPoint scenePoint = event->scenePos().toPoint();
	Region region = getRegion(scenePoint);
	std::cout << "PB::contextMenu " << _name.toStdString() << " " << processingBoxRegionName(region) <<  std::endl;

	switch (region)
	{
		case ProcessingBox::inportsRegion:
		case ProcessingBox::outportsRegion:
		case ProcessingBox::incontrolsRegion:
		case ProcessingBox::outcontrolsRegion:
			_canvas->connectionContextMenu(&menu, event, this, region);
			menu.exec(event->screenPos());
			return;
		case ProcessingBox::nameRegion:
		case ProcessingBox::bodyRegion:
		case ProcessingBox::resizeHandleRegion:
			if (not isSelected())
			{
				std::cout << "updating selection on context menu" << std::endl;
				if (! (event->modifiers() & Qt::ControlModifier) )
					_canvas->clearSelections();
				select();
				update();
			}
			_canvas->processingContextMenu(&menu, event, this);
			menu.exec(event->screenPos());
		default:return;
	}
}

void ProcessingBox::startMoving(const QPoint & initialGlobalPos)
{
	_actionMode = Moving;
	originalPosition = _pos;
	dragOrigin = initialGlobalPos;
}
void ProcessingBox::keepMoving(const QPoint & delta)
{
	if (_actionMode==Moving)
		move(originalPosition + delta);
}
void ProcessingBox::stopMoving()
{
	if (_actionMode==Moving)
		_actionMode = NoAction;
}
void ProcessingBox::hover(const QPoint & scenePoint)
{
	_highLightRegion=noRegion;
	Region region = getRegion(scenePoint);
	if (region==noRegion) return;
	switch (region)
	{	
		case noRegion:
			break;		// it should not reach this point, is handled by a previous conditional....
		case inportsRegion:
		{
			int index = portIndexByYPos(scenePoint);
			_highLightRegion=region;
			_highLightConnection=index;
			_canvas->setToolTip(_canvas->inportTooltip(_processing, index));
			break;
		}
		case outportsRegion:
		{
			int index = portIndexByYPos(scenePoint);
			_highLightRegion=region;
			_highLightConnection=index;
			_canvas->setToolTip(_canvas->outportTooltip(_processing, index));
			break;
		}
		case incontrolsRegion:
		{
			int index = controlIndexByXPos(scenePoint);
			_highLightRegion=region;
			_highLightConnection=index;
			_canvas->setToolTip(_canvas->incontrolTooltip(_processing, index));
			break;
		}
		case outcontrolsRegion:
		{	
			int index = controlIndexByXPos(scenePoint);
			_highLightRegion=region;
			_highLightConnection=index;
			_canvas->setToolTip(_canvas->outcontrolTooltip(_processing, index));
			break;
		}
		case resizeHandleRegion:
		{
			if(_actionMode!=Moving)
				_canvas->setCursor(Qt::SizeFDiagCursor);
			_canvas->setStatusTip(QObject::tr("Drag: resize"));
			break;
		}
		case bodyRegion:
		{
			if (not _canvas->isOk(_processing)) 
				_canvas->setToolTip(_canvas->errorMessage(_processing));
			_canvas->setStatusTip(QObject::tr("Double click: configure. Left click: Processing menu"));
			break;
		}
		case nameRegion:
		{
			if (not _canvas->isOk(_processing))
			{
				_canvas->setToolTip(_canvas->errorMessage(_processing));
			}
			else
			{
				setMetadataToolTip();
			}
			_canvas->setStatusTip(QObject::tr("Drag: move. Double click: rename. Left click: Processing menu"));
			break;
		}
		return;
	}
}
void ProcessingBox::setMetadataToolTip()
{
	QString tooltipText=QString("Type: ");
	std::string key=((CLAM::Processing*)_processing)->GetClassName();
	tooltipText+=QString(key.c_str());
	CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
	CLAM::ProcessingFactory::Pairs pairsFromKey=factory.GetPairsFromKey(key);
	CLAM::ProcessingFactory::Pairs::const_iterator itPairs;
	for(itPairs = pairsFromKey.begin();itPairs!=pairsFromKey.end();itPairs++)
	{
		std::string attribute = itPairs->attribute;
		std::string value = itPairs->value;
		if(attribute!="icon" && value!=key && value!="")
		{
			tooltipText+=QString("\n");
			tooltipText+=QString(attribute.c_str());
			tooltipText+=QString(": ");
			tooltipText+=QString(value.c_str());
		}
	}
	_canvas->setToolTip(tooltipText);
}
void ProcessingBox::doubleClicking(const QPoint & scenePoint)
{
	Region region = getRegion(scenePoint);
	if (region==nameRegion) rename();
	if (region==incontrolsRegion)
	{
		_canvas->addControlSenderProcessing(this, scenePoint);
	}
	if (region==outcontrolsRegion)
	{
		_canvas->addControlPrinterProcessing(this, scenePoint);
	}
	if (region==inportsRegion)
	{
		int index = portIndexByYPos(scenePoint);
		if (((CLAM::Processing*)_processing)->GetInPort(index).GetTypeId()==typeid(CLAM::TData))
			_canvas->addLinkedProcessingSender(this,scenePoint,"AudioSource");
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(scenePoint);
		if (((CLAM::Processing*)_processing)->GetOutPort(index).GetTypeId()==typeid(CLAM::TData))
			_canvas->addLinkedProcessingReceiver(this,scenePoint,"AudioSink");
	}
}
void ProcessingBox::endWireDrag(const QPoint& scenePoint)
{
	Region region = getRegion(scenePoint);

	if (_canvas->dragStatus()==NetworkCanvas::OutportDrag && region==inportsRegion)
	{
		int index = portIndexByYPos(scenePoint);
		_canvas->endWireDrag(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::InportDrag &&  region==outportsRegion)
	{
		int index = portIndexByYPos(scenePoint);
		_canvas->endWireDrag(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::OutcontrolDrag && region==incontrolsRegion)
	{
		int index = controlIndexByXPos(scenePoint);
		_canvas->endWireDrag(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::IncontrolDrag && region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(scenePoint);
		_canvas->endWireDrag(this, index);
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

