#ifndef NetworkCanvas_hxx
#define NetworkCanvas_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QMouseEvent>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

const static int margin =5;

class ProcessingBox : public QWidget
{
	Q_OBJECT
public:
	enum {
		portWidth = 12,
		portHeight = 12,
		portSpacing = 4,
		controlWidth = 10,
		controlHeight = 12,
		controlSpacing = 4
	};
	ProcessingBox(QWidget * parent, const QString & name,
		   	unsigned nInports, unsigned nOutports,
			unsigned nIncontrols, unsigned nOutcontrols)
		: QWidget(parent)
		, _name(name)
		, _nInports(nInports)
		, _nOutports(nOutports)
		, _nIncontrols(nIncontrols)
		, _nOutcontrols(nOutcontrols)
		, moving(false)
		, resizing(false)
	{
		setMouseTracking(true);
		QFontMetrics metrics(font());
		textHeight = metrics.height();
		int minimumHeight = textHeight+2*margin;
		if (minimumHeight<_nInports*(portHeight+portSpacing)) minimumHeight = _nInports*(portHeight+portSpacing);
		if (minimumHeight<_nOutports*(portHeight+portSpacing)) minimumHeight = _nOutports*(portHeight+portSpacing);
		minimumHeight+=2*controlHeight + 2*margin;
		setMinimumSize(metrics.width(_name)+2*(margin+portWidth), minimumHeight );
	}
	virtual ~ProcessingBox() {}

	void paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		QColor runningProcesingFrame(0x30,0x8f,0x30);
		QColor runningProcesingFrameBorder(0x20,0x6f,0x20);

		// Box
		painter.setPen(runningProcesingFrameBorder);
		painter.setBrush(runningProcesingFrame);
		painter.drawRect(portWidth, controlHeight, width()-2*portWidth, height()-2*controlHeight);
		painter.setBrush(QColor(0x99,0xbb,0x99));
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
			painter.drawEllipse(width(), margin+controlHeight+i*(portSpacing+portHeight), -portWidth, portHeight);
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
			painter.drawRect(margin+portWidth+i*(controlSpacing+controlWidth), height(), controlWidth, -controlHeight);
		}
		// Text
		painter.setPen(Qt::white);
		painter.drawText(QRect(portWidth+margin, controlHeight+margin, width()-2*margin+portWidth, textHeight), _name);
	}
	void mousePressEvent(QMouseEvent * event)
	{
		int x = event->pos().x();
		int y = event->pos().y();
		if (x<portWidth)
		{
			std::cout << "Clicking inports" << std::endl;
			return;
		}
		if (x>width()-portWidth)
		{
			std::cout << "Clicking outports" << std::endl;
			return;
		}
		if (y<controlHeight)
		{
			std::cout << "Clicking incontrols" << std::endl;
			return;
		}
		if (y>height()-controlHeight)
		{
			std::cout << "Clicking outcontrols" << std::endl;
			return;
		}
		raise();
		// Resize corner
		if (x>width()-portWidth-margin && y>height()-controlHeight-margin)
		{
			resizing=true;
			dragOrigin=event->pos();
			originalSize=size();
			setCursor(Qt::SizeFDiagCursor);
			return;
		}
		// Head
		if (y<textHeight+margin+controlHeight)
		{
			moving=true;
			dragOrigin=event->pos();
			setCursor(Qt::SizeAllCursor);
			return;
		}
	}
	int portIndexByYPos(int y)
	{
		return (y-margin-controlHeight)/(portHeight+portSpacing);
	}
	int controlIndexByXPos(int x)
	{
		return (x-margin-portWidth)/(controlWidth+controlSpacing);
	}
	void mouseMoveEvent(QMouseEvent * event)
	{
		if (moving)
		{
			move(pos() - dragOrigin + event->pos());
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
		if (x<portWidth)
		{
			int index = portIndexByYPos(y);
			if (index<0) return;
			if (index>=_nInports) return;
			std::cout << "Hovering inport " << index << std::endl;
			return;
		}
		if (x>width()-portWidth)
		{
			int index = portIndexByYPos(y);
			if (index<0) return;
			if (index>=_nOutports) return;
			std::cout << "Hovering outport " << index << std::endl;
			return;
		}
		if (y<controlHeight)
		{
			int index = controlIndexByXPos(x);
			if (index<0) return;
			if (index>=_nIncontrols) return;
			std::cout << "Hovering incontrol" << index << std::endl;
			return;
		}
		if (y>height()-controlHeight)
		{
			int index = controlIndexByXPos(x);
			if (index<0) return;
			if (index>=_nOutcontrols) return;
			std::cout << "Hovering outcontrol" << index << std::endl;
			return;
		}
		// Resize corner
		if (x>width()-portWidth-margin && y>height()-controlHeight-margin)
		{
			setCursor(Qt::SizeFDiagCursor);
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
private:
	QString _name;
	unsigned _nInports;
	unsigned _nOutports;
	unsigned _nIncontrols;
	unsigned _nOutcontrols;
	bool moving;
	QPoint dragOrigin;
	QSize originalSize;
	bool resizing;
	int textHeight;
};

class NetworkCanvas : public QWidget
{

	Q_OBJECT
public:
	NetworkCanvas(QWidget * parent=0)
		: QWidget(parent)
		, _font("Arial", 10)
		, _metrics(_font)
		, _tooltipText(QString::null)
		, _captionWidth(90)
	{
		setMouseTracking(true);
		resize(600,300);
		_processings.push_back(new ProcessingBox(this, "Processing1", 2, 2, 2, 3));
		_processings.push_back(new ProcessingBox(this, "Processing2", 4, 5, 1, 2));
		_processings.push_back(new ProcessingBox(this, "Processing3", 2, 0, 2, 0));
		_processings[0]->move(300,200);
		_processings[1]->move(200,10);
		_processings[2]->move(100,200);
	}

	virtual ~NetworkCanvas();

	void paintEvent(QPaintEvent * event)
	{
		setMinimumSize(200,100);
		QPainter painter(this);
		paintPortConnection(painter, 0,1, 1,3);
		paintPortConnection(painter, 1,1, 0,1);
		paintControlConnection(painter, 1,1, 0,1);
		paintControlConnection(painter, 1,1, 2,1);
	}
	void paintPortConnection(QPainter & painter, unsigned module1, unsigned outport, unsigned module2, unsigned inport)
	{
		QPoint source = _processings[module1]->getOutportPos(outport);
		QPoint target = _processings[module2]->getInportPos(inport);
		painter.setRenderHint(QPainter::Antialiasing);
		QPainterPath path;
		int minTangentSize=abs(target.y()-source.y());
		if (minTangentSize>150) minTangentSize=150;
		if (target.x()<source.x()) minTangentSize=150;
		int tangentOut=target.x();
		if (tangentOut<source.x()+minTangentSize) tangentOut = source.x()+minTangentSize;
		int tangentIn=source.x();
		if (tangentIn>target.x()-minTangentSize) tangentIn = target.x()-minTangentSize;
		path.moveTo(source);
		path.cubicTo(tangentOut, source.y(), tangentIn, target.y(), target.x(), target.y()); // 3 = portHeight/2
		painter.strokePath(path, QPen(QBrush(QColor(0x50,0x50,0x22)), 5));
		painter.strokePath(path, QPen(QBrush(QColor(0xbb,0x99,0x44)), 3));
	}
	void paintControlConnection(QPainter & painter, unsigned module1, unsigned outcontrol, unsigned module2, unsigned incontrol)
	{
		QPoint source = _processings[module1]->getOutcontrolPos(outcontrol);
		QPoint target = _processings[module2]->getIncontrolPos(incontrol);
		painter.setRenderHint(QPainter::Antialiasing);
		QPainterPath path;
		int minTangentSize=abs(target.x()-source.x());
		if (minTangentSize>150) minTangentSize=150;
		if (target.y()<source.y()) minTangentSize=150;
		int tangentOut=target.y();
		if (tangentOut<source.y()+minTangentSize) tangentOut = source.y()+minTangentSize;
		int tangentIn=source.y();
		if (tangentIn>target.y()-minTangentSize) tangentIn = target.y()-minTangentSize;
		path.moveTo(source);
		path.cubicTo(source.x(), tangentOut, target.x(), tangentIn, target.x(), target.y()); // 3 = portHeight/2
		painter.strokePath(path, QPen(QBrush(QColor(0x20,0x50,0x52)), 5));
		painter.strokePath(path, QPen(QBrush(QColor(0x4b,0x99,0xb4)), 3));
	}

	void mouseMoveEvent(QMouseEvent * event)
	{
	}

	void mouseDoubleClickEvent(QMouseEvent * event)
	{
	}




public slots:

private:
	// Drawing properties
	QFont _font;
	QFontMetrics _metrics;
	unsigned _captionWidth;
	int _rulerHeight;
	int _tooltipX;
	int _tooltipY;
	QString _tooltipText;
	std::vector<ProcessingBox *> _processings;
};



#endif//NetworkCanvas_hxx
