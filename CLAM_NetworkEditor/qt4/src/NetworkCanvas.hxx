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
	NetworkCanvas(QWidget * parent=0)
		: QWidget(parent)
	{
		setMouseTracking(true);
		setAcceptDrops(true);
		resize(600,300);
		_processings.push_back(new ProcessingBox(this, "Processing1", 2, 2, 2, 3));
		_processings.push_back(new ProcessingBox(this, "Processing2", 4, 5, 1, 2));
		_processings.push_back(new ProcessingBox(this, "Processing3", 2, 0, 2, 0));
		_processings[0]->move(300,200);
		_processings[1]->move(200,10);
		_processings[2]->move(100,200);
		_portWires.push_back(new PortWire(_processings[0],1, _processings[1],3));
		_portWires.push_back(new PortWire(_processings[1],1, _processings[0],1));
		_controlWires.push_back(new ControlWire(_processings[1],1, _processings[0],1));
		_controlWires.push_back(new ControlWire(_processings[1],1, _processings[2],1));
	}

	virtual ~NetworkCanvas();

	void paintEvent(QPaintEvent * event)
	{
		setMinimumSize(200,100);
		QPainter painter(this);
		paint(painter);
	}
	void paint(QPainter & painter)
	{
		for (unsigned i = 0; i<_portWires.size(); i++)
			_portWires[i]->draw(painter);
		for (unsigned i = 0; i<_controlWires.size(); i++)
			_controlWires[i]->draw(painter);
		for (unsigned i = 0; i<_processings.size(); i++)
			_processings[i]->paintFromParent(painter);
	}

	void mouseMoveEvent(QMouseEvent * event)
	{
	}

	void mouseDoubleClickEvent(QMouseEvent * event)
	{
		print();
	}

	void print()
	{
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName("Network.pdf");
		QPrintDialog * dialog = new QPrintDialog(&printer, this);
		dialog->exec();
		QPainter painter;
		painter.begin(&printer);
		paint(painter);
		painter.end();
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
private:
	std::vector<ProcessingBox *> _processings;
	std::vector<PortWire *> _portWires;
	std::vector<ControlWire *> _controlWires;
};



#endif//NetworkCanvas_hxx
