#ifndef ProcessingBox_hxx
#define ProcessingBox_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QInputDialog>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

class NetworkCanvas;

class ProcessingBox : public QWidget
{
	Q_OBJECT
public:
	enum
	{
		portWidth = 12,
		portHeight = 12,
		portSpacing = 4,
		controlWidth = 10,
		controlHeight = 10,
		controlSpacing = 6,
		margin = 5
	};
	enum Region 
	{
		inportsRegion,
		outportsRegion,
		incontrolsRegion,
		outcontrolsRegion,
		nameRegion,
		resizeHandleRegion,
		bodyRegion,
		noRegion
	};
	ProcessingBox(NetworkCanvas * parent, const QString & name,
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
	virtual ~ProcessingBox();

	void paintEvent(QPaintEvent * event)
	{
		return;
		QPainter painter(this);
		paintBox(painter);
	}
	void paintFromParent(QPainter & painter)
	{
		painter.save();
		painter.translate(x(),y());
		paintBox(painter);
		painter.restore();
	}
	void paintBox(QPainter & painter)
	{
		painter.setRenderHint(QPainter::Antialiasing);

		// Box
		painter.setPen(QColor(0x20,0x6f,0x20));
		painter.setBrush(QColor(0x30,0x8f,0x30,0xaf));
		painter.drawRect(portWidth, controlHeight, width()-2*portWidth, height()-2*controlHeight);
		painter.setBrush(QColor(0xF9,0xFb,0xF9,0xaf));
		painter.drawRect(portWidth+margin, controlHeight+margin+textHeight, width()-2*(margin+portWidth), height()-textHeight-2*(margin+controlHeight));
		painter.setBrush(QColor(0xb9,0xbb,0xb9));
		painter.drawRect(width()-portWidth-margin, height()-controlHeight-margin, margin, margin);
		// Ports
		painter.setBrush(QColor(0xa6,0x60,0x84));
		painter.setPen(QColor(0x53,0x30,0x42));
		for (unsigned i = 0; i<_nInports; i++)
		{
			painter.drawEllipse(0, margin+controlHeight+i*(portSpacing+portHeight), portWidth, portHeight);
		}
		for (unsigned i = 0; i<_nOutports; i++)
		{
			painter.drawEllipse(width()-portWidth, margin+controlHeight+i*(portSpacing+portHeight), portWidth, portHeight);
		}
		// Controls
		painter.setBrush(QColor(0xf6,0x60,0x84));
		painter.setPen(QColor(0x53,0x30,0x42));
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
	Region getRegion(const QPoint & point) const
	{
		return getRegion(point.x(), point.y());
	}
	Region getRegion(int x, int y) const
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
	int portIndexByYPos(int y)
	{
		return (y-margin-controlHeight)/(portHeight+portSpacing);
	}
	int controlIndexByXPos(int x)
	{
		return (x-margin-portWidth)/(controlWidth+controlSpacing);
	}
	// returns the inport connect point in parent coords
	QPoint getInportPos(unsigned i) const
	{
		return QPoint( x(), y()+margin+controlHeight+i*(portSpacing+portHeight) + portHeight/2  );
	}
	QPoint getOutportPos(unsigned i) const
	{
		return QPoint( x()+width(), y()+margin+controlHeight+i*(portSpacing+portHeight) + portHeight/2  );
	}
	QPoint getIncontrolPos(unsigned i) const
	{
		return QPoint( x()+margin+portWidth+i*(controlSpacing+controlWidth) + controlWidth/2,  y()  );
	}
	QPoint getOutcontrolPos(unsigned i) const
	{
		return QPoint( x()+margin+portWidth+i*(controlSpacing+controlWidth) + controlWidth/2, y()+height()  );
	}


	void mousePressEvent(QMouseEvent * event)
	{
		int x = event->pos().x();
		int y = event->pos().y();
		Region region = getRegion(x,y);
		if (region==noRegion) return;
		raise();
		// Resize corner
		if (region==resizeHandleRegion)
		{
			resizing=true;
			dragOrigin=event->pos();
			originalSize=size();
			setCursor(Qt::SizeFDiagCursor);
			return;
		}
		// Head
		if (region==nameRegion)
		{
			moving=true;
			originalPosition = pos();
			dragOrigin=event->globalPos();
			setCursor(Qt::SizeAllCursor);
			return;
		}
	}
	void mouseMoveEvent(QMouseEvent * event)
	{
		setToolTip(QString::null);
		if (moving)
		{
			move(originalPosition - dragOrigin + event->globalPos());
			((QWidget*)parent())->update();
			return;
		}
		int x = event->pos().x();
		int y = event->pos().y();
		if (resizing)
		{
			resize(
				originalSize.width() - dragOrigin.x() + x,
				originalSize.height() - dragOrigin.y() + y
				);
			((QWidget*)parent())->update();
			return;
		}
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
	void mouseReleaseEvent(QMouseEvent * event)
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
	void mouseDoubleClickEvent(QMouseEvent * event)
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
	void rename(const QString & newName)
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
private:
	NetworkCanvas * _canvas;
	QString _name;
	unsigned _nInports;
	unsigned _nOutports;
	unsigned _nIncontrols;
	unsigned _nOutcontrols;
	bool moving;
	QPoint dragOrigin;
	QPoint originalPosition;
	QSize originalSize;
	bool resizing;
	int textHeight;
};



#endif//ProcessingBox_hxx
