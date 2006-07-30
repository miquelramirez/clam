#include "ProcessingBox.hxx"
#include "NetworkCanvas.hxx"

ProcessingBox::~ProcessingBox()
{
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
	, moving(false)
	, resizing(false)
{
	rename(name);
}

void ProcessingBox::paintFromParent(QPainter & painter)
{
	painter.save();
	painter.translate(_pos);
	paintBox(painter);
	painter.restore();
}
void ProcessingBox::move(const QPoint & point)
{
	_pos=point;
}
void ProcessingBox::resize(const QSize & size)
{
	_size=size.expandedTo(_minimumSize);
}
void ProcessingBox::rename(const QString & newName)
{
	_name=newName;
	QFontMetrics metrics(_canvas->font());
	textHeight = metrics.height();

	int minimumHeight = textHeight+2*margin;
	int inportsHeight = _nInports*(portHeight+portSpacing);
	int outportsHeight = _nOutports*(portHeight+portSpacing);
	if (minimumHeight<inportsHeight) minimumHeight = inportsHeight;
	if (minimumHeight<outportsHeight) minimumHeight = outportsHeight;
	minimumHeight += 2*controlHeight + 2*margin;

	int minimumWidth = metrics.width(_name);
	int incontrolsWidth = _nIncontrols*(controlWidth+controlSpacing);
	int outcontrolsWidth = _nOutcontrols*(controlWidth+controlSpacing);
	if (minimumWidth<incontrolsWidth) minimumWidth = incontrolsWidth;
	if (minimumWidth<outcontrolsWidth) minimumWidth = outcontrolsWidth;
	minimumWidth += 2*portWidth + 2*margin;

	_minimumSize = QSize(minimumWidth, minimumHeight );
	resize(_size);
	_canvas->update();
}
void ProcessingBox::paintBox(QPainter & painter)
{
	painter.setRenderHint(QPainter::Antialiasing);

	// Box
	painter.setPen(_canvas->colorBoxFrameOutline());
	painter.setBrush(_canvas->colorBoxFrame());
	painter.drawRect(portWidth, controlHeight, _size.width()-2*portWidth, _size.height()-2*controlHeight);
	painter.setBrush(_canvas->colorBoxBody());
	painter.drawRect(portWidth+margin, controlHeight+margin+textHeight, _size.width()-2*(margin+portWidth), _size.height()-textHeight-2*(margin+controlHeight));
	painter.setBrush(_canvas->colorResizeHandle());
	painter.drawRect(_size.width()-portWidth-margin, _size.height()-controlHeight-margin, margin, margin);
	// Ports
	painter.setBrush(_canvas->colorPort());
	painter.setPen(_canvas->colorPortOutline());
	for (unsigned i = 0; i<_nInports; i++)
	{
		painter.drawEllipse(0, margin+controlHeight+i*(portSpacing+portHeight), portWidth, portHeight);
	}
	for (unsigned i = 0; i<_nOutports; i++)
	{
		painter.drawEllipse(_size.width()-portWidth, margin+controlHeight+i*(portSpacing+portHeight), portWidth, portHeight);
	}
	// Controls
	painter.setBrush(_canvas->colorControl());
	painter.setPen(_canvas->colorControlOutline());
	for (unsigned i = 0; i<_nIncontrols; i++)
	{
		painter.drawRect(margin+portWidth+i*(controlSpacing+controlWidth), 0, controlWidth, controlHeight);
	}
	for (unsigned i = 0; i<_nOutcontrols; i++)
	{
		painter.drawRect(margin+portWidth+i*(controlSpacing+controlWidth), _size.height()-controlHeight, controlWidth, controlHeight);
	}
	painter.setBrush(Qt::yellow); // TODO; colorPortHightLight
	painter.setPen(_canvas->colorPortOutline());
	if (_highLightRegion == inportsRegion)
		painter.drawEllipse(0, margin+controlHeight+_highLightConnection*(portSpacing+portHeight), portWidth, portHeight);
	if (_highLightRegion == outportsRegion)
		painter.drawEllipse(_size.width()-portWidth, margin+controlHeight+_highLightConnection*(portSpacing+portHeight), portWidth, portHeight);
	if (_highLightRegion == incontrolsRegion)
		painter.drawRect(margin+portWidth+_highLightConnection*(controlSpacing+controlWidth), 0, controlWidth, controlHeight);
	if (_highLightRegion == outcontrolsRegion)
		painter.drawRect(margin+portWidth+_highLightConnection*(controlSpacing+controlWidth), _size.height()-controlHeight, controlWidth, controlHeight);
	// Text
	painter.setPen(_canvas->colorBoxFrameText());
	painter.drawText(QRect(portWidth+margin, controlHeight+margin, _size.width()-2*margin+portWidth, textHeight), _name);
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
		if (y<margin+controlHeight) return noRegion;
		if (y>=margin+controlHeight+_nInports*(portHeight+portSpacing)) return noRegion;
		return inportsRegion;
	}
	if (x>_size.width()-portWidth)
	{
		if (y<margin+controlHeight) return noRegion;
		if (y>=margin+controlHeight+_nOutports*(portHeight+portSpacing)) return noRegion;
		return outportsRegion;
	}
	if (y>=0 && y<controlHeight)
	{
		if (x<margin+portHeight) return noRegion;
		if (x>=margin+portHeight+_nIncontrols*(controlHeight+controlSpacing)) return noRegion;
		return incontrolsRegion;
	}
	if (y<=_size.height() && y>_size.height()-controlHeight)
	{
		if (x<margin+portHeight) return noRegion;
		if (x>=margin+portHeight+_nOutcontrols*(controlHeight+controlSpacing)) return noRegion;
		return outcontrolsRegion;
	}
	if (y<textHeight+margin+controlHeight)
		return nameRegion;
	if (x>_size.width()-portWidth-margin && y>_size.height()-controlHeight-margin)
		return resizeHandleRegion;
	return bodyRegion;
}
int ProcessingBox::portIndexByYPos(const QPoint & point)
{
	int y = point.y()-_pos.y();
	return (y-margin-controlHeight)/(portHeight+portSpacing);
}
int ProcessingBox::controlIndexByXPos(const QPoint & point)
{
	int x = point.x()-_pos.x();
	return (x-margin-portWidth)/(controlWidth+controlSpacing);
}
// returns the inport connect point in parent coords
QPoint ProcessingBox::getInportPos(unsigned i) const
{
	return QPoint( _pos.x(), _pos.y()+margin+controlHeight+i*(portSpacing+portHeight) + portHeight/2  );
}
QPoint ProcessingBox::getOutportPos(unsigned i) const
{
	return QPoint( _pos.x()+_size.width(), _pos.y()+margin+controlHeight+i*(portSpacing+portHeight) + portHeight/2  );
}
QPoint ProcessingBox::getIncontrolPos(unsigned i) const
{
	return QPoint( _pos.x()+margin+portWidth+i*(controlSpacing+controlWidth) + controlWidth/2,  _pos.y()  );
}
QPoint ProcessingBox::getOutcontrolPos(unsigned i) const
{
	return QPoint( _pos.x()+margin+portWidth+i*(controlSpacing+controlWidth) + controlWidth/2, _pos.y()+_size.height()  );
}

void ProcessingBox::startMoving(const QPoint & initialGlobalPos)
{
	moving = true;
	originalPosition = _pos;
	dragOrigin = initialGlobalPos;
}

void ProcessingBox::mousePressEvent(QMouseEvent * event)
{
	Region region = getRegion(event->pos());
	if (region==noRegion) return;
	_canvas->raise(this);
	// Head
	if (region==nameRegion)
	{
		startMoving(event->globalPos());
		_canvas->setCursor(Qt::SizeAllCursor);
		return;
	}
	// Resize corner
	if (region==resizeHandleRegion)
	{
		resizing=true;
		originalSize = _size;
		dragOrigin=event->globalPos();
		_canvas->setCursor(Qt::SizeFDiagCursor);
		return;
	}
	if (region==inportsRegion)
	{
		int index = portIndexByYPos(event->pos());
		_canvas->startDrag(NetworkCanvas::InportDrag, this, index);
		return;
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(event->pos());
		_canvas->startDrag(NetworkCanvas::OutportDrag, this, index);
		return;
	}
	if (region==incontrolsRegion)
	{
		int index = controlIndexByXPos(event->pos());
		_canvas->startDrag(NetworkCanvas::IncontrolDrag, this, index);
		return;
	}
	if (region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(event->pos());
		_canvas->startDrag(NetworkCanvas::OutcontrolDrag, this, index);
		return;
	}
}
void ProcessingBox::mouseMoveEvent(QMouseEvent * event)
{
	_highLightRegion=noRegion;;
	if (moving)
	{
		QPoint dragDelta = event->globalPos() - dragOrigin;
		move(originalPosition + dragDelta);
		return;
	}
	if (_canvas->dragStatus()==NetworkCanvas::PanDrag)
	{
		QPoint dragDelta = event->globalPos() - dragOrigin;
		move(originalPosition + dragDelta);
		return;
	}
	if (resizing)
	{
		QPoint dragDelta = event->globalPos() - dragOrigin;
		resize(QSize(
			originalSize.width() + dragDelta.x(),
			originalSize.height() + dragDelta.y()
			));
		return;
	}
	Region region = getRegion(event->pos());
	if (region==noRegion) return;
	_canvas->setCursor(Qt::ArrowCursor);
	if (region==inportsRegion)
	{
		int index = portIndexByYPos(event->pos());
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(getInportName(index));
		return;
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(event->pos());
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(getOutportName(index));
		return;
	}
	if (region==incontrolsRegion)
	{
		int index = controlIndexByXPos(event->pos());
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(getIncontrolName(index));
		return;
	}
	if (region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(event->pos());
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
		_canvas->setStatusTip(QObject::tr("Double click: configure"));
		return;
	}
	if (region==nameRegion)
	{
		_canvas->setStatusTip(QObject::tr("Drag: move. Double click: rename"));
		return;
	}
}
void ProcessingBox::mouseReleaseEvent(QMouseEvent * event)
{
	if (moving)
	{
		_canvas->setCursor(Qt::ArrowCursor);
		moving=false;
	}
	if (resizing)
	{
		_canvas->setCursor(Qt::ArrowCursor);
		resizing=false;
	}
	Region region = getRegion(event->pos());
	if (_canvas->dragStatus()==NetworkCanvas::OutportDrag && region==inportsRegion)
	{
		int index = portIndexByYPos(event->pos());
		_canvas->endConnectionTo(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::InportDrag &&  region==outportsRegion)
	{
		int index = portIndexByYPos(event->pos());
		_canvas->endConnectionTo(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::OutcontrolDrag && region==incontrolsRegion)
	{
		int index = controlIndexByXPos(event->pos());
		_canvas->endConnectionTo(this, index);
	}
	if (_canvas->dragStatus()==NetworkCanvas::IncontrolDrag && region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(event->pos());
		_canvas->endConnectionTo(this, index);
	}
}
void ProcessingBox::mouseDoubleClickEvent(QMouseEvent * event)
{
	Region region = getRegion(event->pos());
	if (region==nameRegion) 
	{
		bool ok;
		QString newName = QInputDialog::getText(_canvas, QObject::tr("Rename the processing"), QObject::tr("New name"), QLineEdit::Normal, _name);
		if (ok && !newName.isEmpty()) rename(newName);
	}
}

QString ProcessingBox::getName() const
{
	return _name;
}

QString ProcessingBox::getOutportName(unsigned index) const
{
	return QString("Outport_%1").arg(index);
}
QString ProcessingBox::getInportName(unsigned index) const
{
	return QString("Inport_%1").arg(index);
}
QString ProcessingBox::getOutcontrolName(unsigned index) const
{
	return QString("Outcontrol_%1").arg(index);
}
QString ProcessingBox::getIncontrolName(unsigned index) const
{
	return QString("Incontrol_%1").arg(index);
}


