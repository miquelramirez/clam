#include "ProcessingBox.hxx"
#include "NetworkCanvas.hxx"

ProcessingBox::~ProcessingBox()
{
}

ProcessingBox::ProcessingBox(NetworkCanvas * parent, const QString & name,
		unsigned nInports, unsigned nOutports,
		unsigned nIncontrols, unsigned nOutcontrols)
//	: QWidget((QWidget*)(parent))
	: _canvas(parent)
	, _name(name)
	, _nInports(nInports)
	, _nOutports(nOutports)
	, _nIncontrols(nIncontrols)
	, _nOutcontrols(nOutcontrols)
	, moving(false)
	, resizing(false)
{
//	setMouseTracking(true); //TODO;
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
	if (minimumHeight<_nInports*(portHeight+portSpacing)) minimumHeight = _nInports*(portHeight+portSpacing);
	if (minimumHeight<_nOutports*(portHeight+portSpacing)) minimumHeight = _nOutports*(portHeight+portSpacing);
	minimumHeight+=2*controlHeight + 2*margin;
	_minimumSize = QSize(metrics.width(_name)+2*(margin+portWidth), minimumHeight );
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
	painter.setPen(Qt::white);
	painter.drawText(QRect(portWidth+margin, controlHeight+margin, _size.width()-2*margin+portWidth, textHeight), _name);
}
ProcessingBox::Region ProcessingBox::getRegion(const QPoint & point) const
{
	return getRegion(point.x(), point.y());
}
ProcessingBox::Region ProcessingBox::getRegion(int x, int y) const
{
	x = x - _pos.x();
	y = y - _pos.y();
	if (x<0)
		return noRegion;
	if (x>_size.width())
		return noRegion;
	if (y<0)
		return noRegion;
	if (y>_size.height())
		return noRegion;

	if (x<portWidth)
		return inportsRegion;
	if (x>_size.width()-portWidth)
		return outportsRegion;
	if (y>=0 && y<controlHeight)
		return incontrolsRegion;
	if (y<=_size.height() && y>_size.height()-controlHeight)
		return outcontrolsRegion;
	if (y<textHeight+margin+controlHeight)
		return nameRegion;
	if (x>_size.width()-portWidth-margin && y>_size.height()-controlHeight-margin)
		return resizeHandleRegion;
	return bodyRegion;
}
int ProcessingBox::portIndexByYPos(int y)
{
	y-=_pos.y();
	return (y-margin-controlHeight)/(portHeight+portSpacing);
}
int ProcessingBox::controlIndexByXPos(int x)
{
	x-=_pos.x();
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


void ProcessingBox::mousePressEvent(QMouseEvent * event)
{
	Region region = getRegion(event->pos());
	if (region==noRegion) return;
	_canvas->raise(this);
	// Head
	if (region==nameRegion)
	{
		moving=true;
		originalPosition = _pos;
		dragOrigin=event->globalPos();
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
	if (resizing)
	{
		QPoint dragDelta = event->globalPos() - dragOrigin;
		resize(QSize(
			originalSize.width() + dragDelta.x(),
			originalSize.height() + dragDelta.y()
			));
		return;
	}
	int x = event->pos().x();
	int y = event->pos().y();
	Region region = getRegion(event->pos());
	if (region==noRegion) return;
	_canvas->setCursor(Qt::ArrowCursor);
	if (region==inportsRegion)
	{
		int index = portIndexByYPos(y);
		if (index<0) return;
		if (index>=_nInports) return;
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(QObject::tr("Inport %1").arg(index));
		return;
	}
	if (region==outportsRegion)
	{
		int index = portIndexByYPos(y);
		if (index<0) return;
		if (index>=_nOutports) return;
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(QObject::tr("Outport %1").arg(index));
		return;
	}
	if (region==incontrolsRegion)
	{
		int index = controlIndexByXPos(x);
		if (index<0) return;
		if (index>=_nIncontrols) return;
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(QObject::tr("Incontrol %1").arg(index));
		return;
	}
	if (region==outcontrolsRegion)
	{
		int index = controlIndexByXPos(x);
		if (index<0) return;
		if (index>=_nOutcontrols) return;
		_highLightRegion=region;
		_highLightConnection=index;
		_canvas->setToolTip(QObject::tr("Outcontrol %1").arg(index));
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
}
void ProcessingBox::mouseDoubleClickEvent(QMouseEvent * event)
{
	int x = event->pos().x();
	int y = event->pos().y();
	Region region = getRegion(x,y);
	if (region==nameRegion) 
	{
		bool ok;
		QString newName = QInputDialog::getText(_canvas, QObject::tr("Rename the processing"), QObject::tr("New name"), QLineEdit::Normal, _name);
		if (ok && !newName.isEmpty()) rename(newName);
	}
}


