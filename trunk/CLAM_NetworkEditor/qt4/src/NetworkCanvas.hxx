#ifndef NetworkCanvas_hxx
#define NetworkCanvas_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QMouseEvent>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QMenu>
#include "ProcessingBox.hxx"
#include <vector>
#include <iostream>
#include <cmath>

class PortWire
{
public:
	PortWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
		: _source(source)
		, _target(target)
		, _outlet(outlet)
		, _inlet(inlet)
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
private:
	ProcessingBox * _source;
	ProcessingBox * _target;
	unsigned _outlet;
	unsigned _inlet;
};

class ControlWire
{
public:
	ControlWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
		: _source(source)
		, _target(target)
		, _outlet(outlet)
		, _inlet(inlet)
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
		path.moveTo(source);
		path.cubicTo(source.x(), tangentOut, target.x(), tangentIn, target.x(), target.y());
		painter.strokePath(path, QPen(QBrush(QColor(0x20,0x50,0x52)), 4));
		painter.strokePath(path, QPen(QBrush(QColor(0x4b,0x99,0xb4)), 2));
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
private:
	ProcessingBox * _source;
	ProcessingBox * _target;
	unsigned _outlet;
	unsigned _inlet;
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
		, _printing(false)
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
		_printing = true;
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
		_printing = false;
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
		drawTooltip(painter);
	}

	void drawTooltip(QPainter & painter)
	{
		if (_tooltipText.isNull()) return;
		QFontMetrics _metrics(font());
		int margin =3;
		double tooltipWidth = _metrics.width(_tooltipText)+2*margin;
		double tooltipHeight = _metrics.height()+2*margin;
		double x = _tooltipPos.x()+16;
		if (x + tooltipWidth > width())
		x = _tooltipPos.x() - _metrics.width(_tooltipText) - 2*margin;
		double y = _tooltipPos.y() +16;
		if (y + tooltipHeight > height())
		y = _tooltipPos.y() - tooltipHeight;

		QRectF tooltip(x, y, _metrics.width(_tooltipText)+2*margin, tooltipHeight)  ;
		painter.setBrush(QColor(0xff,0xff,0x90,0x90));
		painter.setPen(QColor(0xff,0xff,0x90,0xff));
		painter.drawRect(tooltip);
		painter.setPen(Qt::black);
		painter.drawText(tooltip, Qt::AlignCenter, _tooltipText);
	}

	void setToolTip(const QString & text)
	{
		_tooltipText = text;
	}


	void mouseMoveEvent(QMouseEvent * event)
	{
		_dragPoint = event->pos();
		setToolTip(0);
		setStatusTip(0);
		for (unsigned i = _processings.size(); i--; )
			_processings[i]->mouseMoveEvent(event);
		update();
		_tooltipPos=_dragPoint;
	}

	void mousePressEvent(QMouseEvent * event)
	{
		if (event->button()!=Qt::LeftButton) return;
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

	void contextMenuEvent(QContextMenuEvent * event)
	{
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(event->pos());
			if (region==ProcessingBox::noRegion) continue;
			QMenu menu(this);
			menu.move(event->globalPos());
			if (
				region==ProcessingBox::inportsRegion ||
				region==ProcessingBox::outportsRegion ||
				region==ProcessingBox::incontrolsRegion ||
				region==ProcessingBox::outcontrolsRegion )
			{
				menu.addAction(QIcon("src/images/remove.png"), "Disconnect",
					this, SLOT(onDisconnect()))->setData(event->pos());
				menu.addAction(QIcon("src/images/editcopy.png"), "Copy",
					this, SLOT(onCopyConnection()))->setData(event->pos());
			}
			if (region==ProcessingBox::nameRegion || 
				region==ProcessingBox::bodyRegion ||
				region==ProcessingBox::resizeHandleRegion)
			{
				menu.addAction(QIcon("src/images/configure.png"), "Configure",
					this, SLOT(onConfigure()))->setData(event->pos());
				menu.addAction(QIcon("src/images/editdelete.png"), "Delete",
					this, SLOT(onDeleteProcessing()))->setData(event->pos());
			}
			menu.exec();
			return;
		}
		QMenu menu(this);
		menu.move(event->globalPos());
		menu.addAction(QIcon("src/images/newprocessing.png"), "Add processing",
			this, SLOT(onNewProcessing()))->setData(event->pos());
		menu.exec();

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
		QString type =  event->mimeData()->text();
		addProcessing(event->pos(), type);
		event->acceptProposedAction();
	}

	void addProcessing(QPoint point, QString type)
	{
		unsigned nInports= QInputDialog::getInteger(this, type, "Number of inports",2,0,10);
		unsigned nOutports= QInputDialog::getInteger(this, type, "Number of outports",2,0,10);
		unsigned nIncontrols= QInputDialog::getInteger(this, type, "Number of incontrols",2,0,10);
		unsigned nOutcontrols= QInputDialog::getInteger(this, type, "Number of outcontrols",2,0,10);
		_processings.push_back(new ProcessingBox(this, type, nInports, nOutports, nIncontrols, nOutcontrols));
		_processings.back()->move(point);
	}

	QColor colorBoxFrameText() const
	{
		if (_printing) return QColor(0x00,0x00,0x00);
		return QColor(0xff,0xff,0xff);
	}
	QColor colorBoxFrameOutline() const
	{
		if (_printing) return QColor(0x00,0x00,0x00);
		return QColor(0x20,0x6f,0x20);
	}
	QColor colorBoxFrame() const
	{
		if (_printing) return QColor(0xd0,0xd0,0xd0);
		return QColor(0x30,0x8f,0x30,0xaf);
	}
	QColor colorBoxBody() const
	{
		if (_printing) return QColor(0xe0,0xe0,0xe0);
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
	ProcessingBox * addProcessing(const QString & type, int x, int y)
	{
		_processings.push_back(new ProcessingBox(this, type, 4, 5, 2, 2));
		_processings.back()->move(QPoint(x,y));
		return _processings.back();
	}

	void disconnectIncontrol(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<ControlWire*>::iterator it=_controlWires.begin();
			   	it<_controlWires.end();)
		{
			ControlWire * wire = *it;
			if (!wire->goesTo(processing, index)) it++;
			else
			{
				delete wire;
				it=_controlWires.erase(it);
			}
		}
	}
	void disconnectOutcontrol(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<ControlWire*>::iterator it=_controlWires.begin();
			   	it<_controlWires.end();)
		{
			ControlWire * wire = *it;
			if (!wire->comesFrom(processing, index)) it++;
			else
			{
				delete wire;
				it=_controlWires.erase(it);
			}
		}
	}
	void disconnectInport(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<PortWire*>::iterator it=_portWires.begin();
			   	it<_portWires.end();)
		{
			PortWire * wire = *it;
			if (!wire->goesTo(processing, index)) it++;
			else
			{
				delete wire;
				it=_portWires.erase(it);
			}
		}
	}
	void disconnectOutport(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<PortWire*>::iterator it=_portWires.begin();
			   	it<_portWires.end();)
		{
			PortWire * wire = *it;
			if (!wire->comesFrom(processing, index)) it++;
			else
			{
				delete wire;
				it=_portWires.erase(it);
			}
		}
	}

	void removeProcessing(ProcessingBox * processing)
	{
		for (std::vector<ControlWire*>::iterator it=_controlWires.begin();
			   	it<_controlWires.end(); )
		{
			ControlWire * wire = *it;
			if (!wire->involves(processing)) it++;
			else
			{
				delete wire;
				it=_controlWires.erase(it);
			}
		}
		for (std::vector<PortWire*>::iterator it=_portWires.begin();
			   	it<_portWires.end(); )
		{
			PortWire * wire = *it;
			if (!wire->involves(processing)) it++;
			else
			{
				delete wire;
				it=_portWires.erase(it);
			}
		}
		delete processing;
		_processings.erase(std::find(_processings.begin(), _processings.end(), processing));
	}

private slots:
	void onCopyConnection()
	{
	}
	void onConfigure()
	{
	}
	void onDisconnect()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			switch (region)
			{
				case ProcessingBox::noRegion: 
					continue;
				case ProcessingBox::inportsRegion:
					disconnectInport(_processings[i], _processings[i]->portIndexByYPos(point));
				break;
				case ProcessingBox::outportsRegion:
					disconnectOutport(_processings[i], _processings[i]->portIndexByYPos(point));
				break;
				case ProcessingBox::incontrolsRegion:
					disconnectIncontrol(_processings[i], _processings[i]->controlIndexByXPos(point));
				break;
				case ProcessingBox::outcontrolsRegion:
					disconnectOutcontrol(_processings[i], _processings[i]->controlIndexByXPos(point));
				break;
			return;
			}
		}
	}
	void onDeleteProcessing()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region == ProcessingBox::noRegion) continue;
			removeProcessing(_processings[i]);
			return;
		}
	}
	void onNewProcessing()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString type = QInputDialog::getText(this, "Type", "Type", QLineEdit::Normal, "A Processing");
		if (type.isNull()) return;
		addProcessing(point, type);
	}
	
private:
	std::vector<ProcessingBox *> _processings;
	std::vector<PortWire *> _portWires;
	std::vector<ControlWire *> _controlWires;
	DragStatus _dragStatus;
	ProcessingBox * _dragProcessing;
	unsigned _dragConnectionIndex;
	QPoint _dragPoint;
	QPoint _tooltipPos;
	QString _tooltipText;
	bool _printing;
	
};



#endif//NetworkCanvas_hxx
