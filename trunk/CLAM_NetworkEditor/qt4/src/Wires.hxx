#ifndef Wires_hxx
#define Wires_hxx

#include "ProcessingBox.hxx"
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <cmath>

class Wire
{
public:
	Wire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
		: _source(source)
		, _target(target)
		, _outlet(outlet)
		, _inlet(inlet)
	{
	}
	bool involves(ProcessingBox * processing)
	{
		return processing == _source || processing == _target;
	}
	bool comesFrom(ProcessingBox * source, unsigned outlet)
	{
		return source == _source && _outlet == outlet;
	}
	bool goesTo(ProcessingBox * target, unsigned inlet)
	{
		return target == _target && _inlet == inlet;
	}
protected:
	ProcessingBox * _source;
	ProcessingBox * _target;
	unsigned _outlet;
	unsigned _inlet;
};

class PortWire : public Wire
{
public:
	PortWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
		: Wire(source, outlet, target, inlet)
	{
	}
	void draw(QPainter & painter)
	{
		QPoint source = _source->getOutportPos(_outlet);
		QPoint target = _target->getInportPos(_inlet);
		draw(painter, source, target);
	}
	static void draw(QPainter & painter, QPoint source, QPoint target)
	{
		QPainterPath path;
		int minTangentSize=abs(target.y()-source.y());
		if (minTangentSize>150) minTangentSize=150;
		if (target.x()<=source.x()) minTangentSize=150;
		int tangentOut=target.x();
		if (tangentOut<source.x()+minTangentSize) tangentOut = source.x()+minTangentSize;
		int tangentIn=source.x();
		if (tangentIn>target.x()-minTangentSize) tangentIn = target.x()-minTangentSize;

		// We use tangentY instead of plain source.y() in order to avoid a qt bezier bug
		int tangentY = source.y();
		if (std::abs(source.y()-target.y())<7) tangentY+=6;

		path.moveTo(source);
		path.cubicTo(tangentOut, tangentY, tangentIn, target.y(), target.x(), target.y());
		painter.strokePath(path, QPen(QBrush(QColor(0x50,0x50,0x22)), 6));
		painter.strokePath(path, QPen(QBrush(QColor(0xbb,0x99,0x44)), 4));
	}
	std::string getTargetId()
	{
		return (_target->getName()+"."+_target->getInportName(_inlet)).toStdString();
	}
	std::string getSourceId()
	{
		return (_source->getName()+"."+_source->getOutportName(_outlet)).toStdString();
	}
};

class ControlWire : public Wire
{
public:
	ControlWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
		: Wire(source, outlet, target, inlet)
	{
	}
	void draw(QPainter & painter)
	{
		QPoint source = _source->getOutcontrolPos(_outlet);
		QPoint target = _target->getIncontrolPos(_inlet);
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

		// We use tangentX instead of plain source.x() in order to avoid a qt bezier bug
		int tangentX = source.x();
		if (std::abs(source.x()-target.x())<7) tangentX+=6;

		path.moveTo(source);
		path.cubicTo(tangentX, tangentOut, target.x(), tangentIn, target.x(), target.y());
		painter.strokePath(path, QPen(QBrush(QColor(0x20,0x50,0x52)), 4));
		painter.strokePath(path, QPen(QBrush(QColor(0x4b,0x99,0xb4)), 2));
	}
	std::string getTargetId()
	{
		return (_target->getName()+"."+_target->getIncontrolName(_inlet)).toStdString();
	}
	std::string getSourceId()
	{
		return (_source->getName()+"."+_source->getOutcontrolName(_outlet)).toStdString();
	}
};

#endif//Wires_hxx
