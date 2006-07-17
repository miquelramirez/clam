#include "ProcessingBox.hxx"
#include "NetworkCanvas.hxx"

ProcessingBox::~ProcessingBox()
{
}

ProcessingBox::ProcessingBox(NetworkCanvas * parent, const QString & name,
		unsigned nInports, unsigned nOutports,
		unsigned nIncontrols, unsigned nOutcontrols)
	: QWidget((QWidget*)(parent))
	, _canvas(parent)
	, _name(name)
	, _nInports(nInports)
	, _nOutports(nOutports)
	, _nIncontrols(nIncontrols)
	, _nOutcontrols(nOutcontrols)
	, moving(false)
	, resizing(false)
{
	setMouseTracking(true);
	rename(name);
}

void ProcessingBox::paintFromParent(QPainter & painter)
{
	painter.save();
	painter.translate(pos());
	paintBox(painter);
	painter.restore();
}
void ProcessingBox::paintBox(QPainter & painter)
{
	painter.setRenderHint(QPainter::Antialiasing);

	// Box
	painter.setPen(_canvas->colorBoxFrameOutline());
	painter.setBrush(_canvas->colorBoxFrame());
	painter.drawRect(portWidth, controlHeight, width()-2*portWidth, height()-2*controlHeight);
	painter.setBrush(_canvas->colorBoxBody());
	painter.drawRect(portWidth+margin, controlHeight+margin+textHeight, width()-2*(margin+portWidth), height()-textHeight-2*(margin+controlHeight));
	painter.setBrush(_canvas->colorResizeHandle());
	painter.drawRect(width()-portWidth-margin, height()-controlHeight-margin, margin, margin);
	// Ports
	painter.setBrush(_canvas->colorPort());
	painter.setPen(_canvas->colorPortOutline());
	for (unsigned i = 0; i<_nInports; i++)
	{
		painter.drawEllipse(0, margin+controlHeight+i*(portSpacing+portHeight), portWidth, portHeight);
	}
	for (unsigned i = 0; i<_nOutports; i++)
	{
		painter.drawEllipse(width()-portWidth, margin+controlHeight+i*(portSpacing+portHeight), portWidth, portHeight);
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
		painter.drawRect(margin+portWidth+i*(controlSpacing+controlWidth), height()-controlHeight, controlWidth, controlHeight);
	}
	// Text
	painter.setPen(Qt::white);
	painter.drawText(QRect(portWidth+margin, controlHeight+margin, width()-2*margin+portWidth, textHeight), _name);
}
ProcessingBox::Region ProcessingBox::getRegion(const QPoint & point) const
{
	return getRegion(point.x(), point.y());
}
ProcessingBox::Region ProcessingBox::getRegion(int x, int y) const
{
	if (x<0)
		return noRegion;
	if (x>width())
		return noRegion;
	if (y<0)
		return noRegion;
	if (y>height())
		return noRegion;

	if (x<portWidth)
		return inportsRegion;
	if (x>width()-portWidth)
		return outportsRegion;
	if (y>=0 && y<controlHeight)
		return incontrolsRegion;
	if (y<=height() && y>height()-controlHeight)
		return outcontrolsRegion;
	if (y<textHeight+margin+controlHeight)
		return nameRegion;
	if (x>width()-portWidth-margin && y>height()-controlHeight-margin)
		return resizeHandleRegion;
	return bodyRegion;
}
int ProcessingBox::portIndexByYPos(int y)
{
	return (y-margin-controlHeight)/(portHeight+portSpacing);
}
int ProcessingBox::controlIndexByXPos(int x)
{
	return (x-margin-portWidth)/(controlWidth+controlSpacing);
}
// returns the inport connect point in parent coords
QPoint ProcessingBox::getInportPos(unsigned i) const
{
	return QPoint( x(), y()+margin+controlHeight+i*(portSpacing+portHeight) + portHeight/2  );
}
QPoint ProcessingBox::getOutportPos(unsigned i) const
{
	return QPoint( x()+width(), y()+margin+controlHeight+i*(portSpacing+portHeight) + portHeight/2  );
}
QPoint ProcessingBox::getIncontrolPos(unsigned i) const
{
	return QPoint( x()+margin+portWidth+i*(controlSpacing+controlWidth) + controlWidth/2,  y()  );
}
QPoint ProcessingBox::getOutcontrolPos(unsigned i) const
{
	return QPoint( x()+margin+portWidth+i*(controlSpacing+controlWidth) + controlWidth/2, y()+height()  );
}


void ProcessingBox::mousePressEvent(QMouseEvent * event)
{
	int x = event->pos().x();
	int y = event->pos().y();
	Region region = getRegion(x,y);
	if (region==noRegion) return;
	raise();
	// Head
	if (region==nameRegion)
	{
		moving=true;
		originalPosition = pos();
		dragOrigin=event->globalPos();
		setCursor(Qt::SizeAllCursor);
		return;
	}
	// Resize corner
	if (region==resizeHandleRegion)
	{
		resizing=true;
		originalSize = size();
		dragOrigin=event->globalPos();
		setCursor(Qt::SizeFDiagCursor);
		return;
	}
}
void ProcessingBox::mouseMoveEvent(QMouseEvent * event)
{
	setToolTip(QString::null);
	if (moving)
	{
		QPoint dragDelta = event->globalPos() - dragOrigin;
		move(originalPosition + dragDelta);
		((QWidget*)parent())->update();
		return;
	}
	if (resizing)
	{
		QPoint dragDelta = event->globalPos() - dragOrigin;
		resize(
			originalSize.width() + dragDelta.x(),
			originalSize.height() + dragDelta.y()
			);
		((QWidget*)parent())->update();
		return;
	}
	int x = event->pos().x();
	int y = event->pos().y();
	setCursor(Qt::ArrowCursor);
	Region region = getRegion(x,y);
	if (region==inportsRegion)
	{
		setStatusTip(tr("Drag: connects. Double click: removes connections"));
		int index = portIndexByYPos(y);
		if (index<0) return;
		if (index>=_nInports) return;
		setToolTip(tr("Inport %1").arg(index));
		return;
	}
	if (region==outportsRegion)
	{
		setStatusTip(tr("Drag: connects. Double click: removes connections"));
		int index = portIndexByYPos(y);
		if (index<0) return;
		if (index>=_nOutports) return;
		setToolTip(tr("Outport %1").arg(index));
		return;
	}
	if (region==incontrolsRegion)
	{
		setStatusTip(tr("Drag: connects. Double click: removes connections"));
		int index = controlIndexByXPos(x);
		if (index<0) return;
		if (index>=_nIncontrols) return;
		setToolTip(tr("Incontrol %1").arg(index));
		return;
	}
	if (region==outcontrolsRegion)
	{
		setStatusTip(tr("Drag: connects. Double click: removes connections"));
		int index = controlIndexByXPos(x);
		if (index<0) return;
		if (index>=_nOutcontrols) return;
		setToolTip(tr("Outcontrol %1").arg(index));
		return;
	}
	if (region==resizeHandleRegion)
	{
		setCursor(Qt::SizeFDiagCursor);
		setStatusTip(tr("Drag: resize"));
		return;
	}
	if (region==bodyRegion)
	{
		setStatusTip(tr("Double click: configure"));
		return;
	}
	if (region==nameRegion)
	{
		setStatusTip(tr("Drag: move. Double click: rename"));
		return;
	}
}
void ProcessingBox::mouseReleaseEvent(QMouseEvent * event)
{
	if (moving)
	{
		setCursor(Qt::ArrowCursor);
		moving=false;
	}
	if (resizing)
	{
		setCursor(Qt::ArrowCursor);
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
		QString newName = QInputDialog::getText(this, tr("Rename the processing"), tr("New name"), QLineEdit::Normal, _name);
		if (ok && !newName.isEmpty()) rename(newName);
	}
}
void ProcessingBox::rename(const QString & newName)
{
	_name=newName;
	QFontMetrics metrics(font());
	textHeight = metrics.height();
	int minimumHeight = textHeight+2*margin;
	if (minimumHeight<_nInports*(portHeight+portSpacing)) minimumHeight = _nInports*(portHeight+portSpacing);
	if (minimumHeight<_nOutports*(portHeight+portSpacing)) minimumHeight = _nOutports*(portHeight+portSpacing);
	minimumHeight+=2*controlHeight + 2*margin;
	setMinimumSize(metrics.width(_name)+2*(margin+portWidth), minimumHeight );
	((QWidget*)parent())->update();
}


