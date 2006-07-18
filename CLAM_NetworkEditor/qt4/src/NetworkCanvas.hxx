#ifndef NetworkCanvas_hxx
#define NetworkCanvas_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QMouseEvent>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include "ProcessingBox.hxx"
#include <vector>
#include <iostream>
#include <cmath>

class PortWire
{
public:
	PortWire(ProcessingBox * source, unsigned outport, ProcessingBox * target, unsigned inport)
		: _source(source)
		, _target(target)
		, _outport(outport)
		, _inport(inport)
	{
	}
	void draw(QPainter & painter)
	{
		QPoint source = _source->getOutportPos(_outport);
		QPoint target = _target->getInportPos(_inport);
		draw(painter, source, target);
	}
	static void draw(QPainter & painter, QPoint source, QPoint target)
	{
		QPainterPath path;
		int minTangentSize=abs(target.y()-source.y());
		if (minTangentSize>150) minTangentSize=150;
		if (target.x()<source.x()) minTangentSize=150;
		int tangentOut=target.x();
		if (tangentOut<source.x()+minTangentSize) tangentOut = source.x()+minTangentSize;
		int tangentIn=source.x();
		if (tangentIn>target.x()-minTangentSize) tangentIn = target.x()-minTangentSize;
		path.moveTo(source);
		path.cubicTo(tangentOut, source.y(), tangentIn, target.y(), target.x(), target.y());
		painter.strokePath(path, QPen(QBrush(QColor(0x50,0x50,0x22)), 5));
		painter.strokePath(path, QPen(QBrush(QColor(0xbb,0x99,0x44)), 3));
	}
private:
	ProcessingBox * _source;
	ProcessingBox * _target;
	unsigned _outport;
	unsigned _inport;
};

class ControlWire
{
public:
	ControlWire(ProcessingBox * source, unsigned outcontrol, ProcessingBox * target, unsigned incontrol)
		: _source(source)
		, _target(target)
		, _outcontrol(outcontrol)
		, _incontrol(incontrol)
	{
	}
	void draw(QPainter & painter)
	{
		QPoint source = _source->getOutcontrolPos(_outcontrol);
		QPoint target = _target->getIncontrolPos(_incontrol);
		draw(painter, source, target);
	}
	static void draw(QPainter & painter, QPoint source, QPoint target)
	{
		QPainterPath path;
		int minTangentSize=abs(target.x()-source.x());
		if (minTangentSize>150) minTangentSize=150;
		if (target.y()<source.y()) minTangentSize=150;
		int tangentOut=target.y();
		if (tangentOut<source.y()+minTangentSize) tangentOut = source.y()+minTangentSize;
		int tangentIn=source.y();
		if (tangentIn>target.y()-minTangentSize) tangentIn = target.y()-minTangentSize;
		path.moveTo(source);
		path.cubicTo(source.x(), tangentOut, target.x(), tangentIn, target.x(), target.y());
		painter.strokePath(path, QPen(QBrush(QColor(0x20,0x50,0x52)), 4));
		painter.strokePath(path, QPen(QBrush(QColor(0x4b,0x99,0xb4)), 2));
	}
private:
	ProcessingBox * _source;
	ProcessingBox * _target;
	unsigned _outcontrol;
	unsigned _incontrol;
};

class NetworkCanvas : public QWidget
{

	Q_OBJECT
public:
	enum DragStatus 
	{
		NoDrag,
		InportDrag,
		OutportDrag,
		IncontrolDrag,
		OutcontrolDrag,
		MoveDrag,
		ResizeDrag
	};
	NetworkCanvas(QWidget * parent=0)
		: QWidget(parent)
		, _dragStatus(NoDrag)
		, _dragProcessing(0)
		, _dragConnectionIndex(0)
	{
		setMouseTracking(true);
		setAcceptDrops(true);
		setMinimumSize(200,100);
		resize(600,300);
		example1();
	}

	void example1()
	{
		_processings.push_back(new ProcessingBox(this, "Processing1", 2, 2, 2, 3));
		_processings.push_back(new ProcessingBox(this, "Processing2", 4, 5, 1, 2));
		_processings.push_back(new ProcessingBox(this, "Processing3", 2, 0, 2, 0));
		_processings[0]->move(QPoint(300,200));
		_processings[1]->move(QPoint(200,10));
		_processings[2]->move(QPoint(100,200));
		addPortWire(_processings[0],1, _processings[1],3);
		addPortWire(_processings[1],1, _processings[0],1);
		addControlWire(_processings[1],1, _processings[0],1);
		addControlWire(_processings[1],1, _processings[2],1);
	}

	virtual ~NetworkCanvas();

	void raise(ProcessingBox * toRaise)
	{
		std::vector<ProcessingBox*>::iterator search = std::find(_processings.begin(), _processings.end(), toRaise);
		if (search==_processings.end()) return;
		_processings.erase(search);
		_processings.push_back(toRaise);
	}

	void paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		paint(painter);
	}
	void print()
	{
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName("Network.pdf");
		printer.setFullPage(true);
		printer.setCreator( "CLAM NetworkEditor");
		printer.setOrientation(QPrinter::Landscape);
		QPrintDialog * dialog = new QPrintDialog(&printer, this);
		dialog->exec();
		QPainter painter;
		painter.begin(&printer);
		paint(painter);
		painter.end();
	}
	void paint(QPainter & painter)
	{
		painter.setRenderHint(QPainter::Antialiasing);
		for (unsigned i = 0; i<_portWires.size(); i++)
			_portWires[i]->draw(painter);
		for (unsigned i = 0; i<_controlWires.size(); i++)
			_controlWires[i]->draw(painter);
		for (unsigned i = 0; i<_processings.size(); i++)
			_processings[i]->paintFromParent(painter);
		if (_dragStatus==InportDrag)
			PortWire::draw(painter, _dragPoint, _dragProcessing->getInportPos(_dragConnectionIndex));
		if (_dragStatus==OutportDrag)
			PortWire::draw(painter, _dragProcessing->getOutportPos(_dragConnectionIndex), _dragPoint);
		if (_dragStatus==IncontrolDrag)
			ControlWire::draw(painter, _dragPoint, _dragProcessing->getIncontrolPos(_dragConnectionIndex));
		if (_dragStatus==OutcontrolDrag)
			ControlWire::draw(painter, _dragProcessing->getOutcontrolPos(_dragConnectionIndex), _dragPoint);
	}

	void mouseMoveEvent(QMouseEvent * event)
	{
		_dragPoint = event->pos();
		setToolTip(0);
		setStatusTip(0);
		for (unsigned i = _processings.size(); i--; )
			_processings[i]->mouseMoveEvent(event);
		update();
	}

	void mousePressEvent(QMouseEvent * event)
	{
		for (unsigned i = _processings.size(); i--; )
		{
			if (_processings[i]->getRegion(event->pos())==ProcessingBox::noRegion) continue;
			_processings[i]->mousePressEvent(event);
			update();
			return;
		}
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		for (unsigned i = _processings.size(); i--; )
			_processings[i]->mouseReleaseEvent(event);
		_dragStatus=NoDrag;
		update();
	}
	void mouseDoubleClickEvent(QMouseEvent * event)
	{
		for (unsigned i = _processings.size(); i--; )
		{
			if (_processings[i]->getRegion(event->pos())==ProcessingBox::noRegion) continue;
			_processings[i]->mouseDoubleClickEvent(event);
			update();
			return;
		}
		print();
	}


	void dragEnterEvent(QDragEnterEvent *event)
	{
		std::cout << "Drag enter" << std::endl;
		event->acceptProposedAction();
	}
	void dragMoveEvent(QDragMoveEvent *event)
	{
		std::cout << "Draggin" << std::endl;
		event->acceptProposedAction();
	}
	void dropEvent(QDropEvent *event)
	{

	}

	QColor colorBoxFrameOutline() const
	{
		return QColor(0x20,0x6f,0x20);
	}
	QColor colorBoxFrame() const
	{
		return QColor(0x30,0x8f,0x30,0xaf);
	}
	QColor colorBoxBody() const
	{
		return QColor(0xF9,0xFb,0xF9,0xaf);
	}
	QColor colorResizeHandle() const
	{
		return QColor(0xf9,0xbb,0xb9);
	}
	QColor colorPortOutline() const
	{
		return QColor(0x53,0x30,0x42);
	}
	QColor colorPort() const
	{
		return QColor(0xa6,0x60,0x84);
	}
	QColor colorControl() const
	{
		return QColor(0xf6,0x60,0x84);
	}
	QColor colorControlOutline() const
	{
		return QColor(0x53,0x30,0x42);
	}

	void startDrag(DragStatus status, ProcessingBox * processing, int connection)
	{
		_dragStatus=status;
		_dragProcessing=processing;
		_dragConnectionIndex=connection;
	}
	DragStatus dragStatus() const
	{
		return _dragStatus;
	}
	void endConnectionTo(ProcessingBox * processing, int connection)
	{
		switch (_dragStatus) 
		{
			case InportDrag:
			{
				addPortWire(processing, connection, _dragProcessing, _dragConnectionIndex);
			} break;
			case OutportDrag:
			{
				addPortWire(_dragProcessing, _dragConnectionIndex, processing, connection);
			} break;
			case IncontrolDrag:
			{
				addControlWire(processing, connection, _dragProcessing, _dragConnectionIndex);
			} break;
			case OutcontrolDrag:
			{
				addControlWire(_dragProcessing, _dragConnectionIndex, processing, connection);
			} break;
		}
	}
	void addPortWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		_portWires.push_back(new PortWire(source, outlet, target, inlet));
	}
	void addControlWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		_controlWires.push_back(new ControlWire(source, outlet, target, inlet));
	}

private:
	std::vector<ProcessingBox *> _processings;
	std::vector<PortWire *> _portWires;
	std::vector<ControlWire *> _controlWires;
	DragStatus _dragStatus;
	ProcessingBox * _dragProcessing;
	unsigned _dragConnectionIndex;
	QPoint _dragPoint;
};



#endif//NetworkCanvas_hxx
