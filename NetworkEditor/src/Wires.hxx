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
	/**
		@arg separation X distance with ports and y distance with controls
		@arg disalignment y distance with ports and x distance with controls
		@return magnitude of the vector that is tangent to the wire at the connector
	*/
	static int tangentSize(int separation, int disalignment)
	{
		int minTangentSize=abs(disalignment);
		if (minTangentSize>50) minTangentSize=50;
		if (separation<=0) minTangentSize=150;

		int tangentSize = separation/3;
		if (tangentSize<minTangentSize) tangentSize = minTangentSize;
		return tangentSize;
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
		int tanSize = tangentSize(target.x()-source.x(), target.y()-source.y());

		// We use tangentY instead of plain source.y() in order to avoid a qt bezier bug
		int tangentY = source.y();
		if (std::abs((double)(source.y()-target.y()))<7) tangentY+=6;

		QPainterPath path;
		path.moveTo(source);
		path.cubicTo(source.x()+tanSize, tangentY, target.x()-tanSize, target.y(), target.x(), target.y());
		painter.strokePath(path, QPen(QBrush(QColor(0x50,0x50,0x22)), 6));
		painter.strokePath(path, QPen(QBrush(QColor(0xbb,0x99,0x44)), 4));
	}
	QString getTargetId()
	{
		return _target->getName()+"."+_target->getInportName(_inlet);
	}
	QString getSourceId()
	{
		return _source->getName()+"."+_source->getOutportName(_outlet);
	}
	void expand(QRect & rect)
	{
		const int wireBend=40;
		QPoint source = _source->getOutportPos(_outlet);
		QPoint target = _target->getInportPos(_inlet);
		if (rect.left()>target.x()-wireBend)
			rect.setLeft(target.x()-wireBend);
		if (rect.right()<source.x()+wireBend)
			rect.setRight(source.x()+wireBend);
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
		int tanSize = tangentSize(target.y()-source.y(), target.x()-source.x());

		// We use tangentX instead of plain source.x() in order to avoid a qt bezier bug
		int tangentX = source.x();
		if (std::abs((double)(source.x()-target.x()))<7) tangentX+=6;

		QPainterPath path;
		path.moveTo(source);
		path.cubicTo(tangentX, source.y()+tanSize, target.x(), target.y()-tanSize, target.x(), target.y());
		painter.strokePath(path, QPen(QBrush(QColor(0x20,0x50,0x52)), 4));
		painter.strokePath(path, QPen(QBrush(QColor(0x4b,0x99,0xb4)), 2));
	}
	QString getTargetId()
	{
		return _target->getName()+"."+_target->getIncontrolName(_inlet);
	}
	QString getSourceId()
	{
		return _source->getName()+"."+_source->getOutcontrolName(_outlet);
	}
	void expand(QRect & rect)
	{
		const int wireBend=40;
		QPoint source = _source->getOutcontrolPos(_outlet);
		QPoint target = _target->getIncontrolPos(_inlet);
		if (rect.top()>target.y()-wireBend)
			rect.setTop(target.y()-wireBend);
		if (rect.bottom()<source.y()+wireBend)
			rect.setBottom(source.y()+wireBend);
	}
};

#endif//Wires_hxx

