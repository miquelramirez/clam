#ifndef NetworkCanvas_hxx
#define NetworkCanvas_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QGridLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtCore/QTextStream>
#include "ProcessingBox.hxx"
#include "Wires.hxx"
#include <vector>
#include <CLAM/Network.hxx>

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
		ResizeDrag,
		PanDrag
	};
	NetworkCanvas(QWidget * parent=0)
		: QWidget(parent)
		, _dragStatus(NoDrag)
		, _dragProcessing(0)
		, _dragConnectionIndex(0)
		, _printing(false)
		, _zoomFactor(1.)
		, _network(0)
	{
		setMouseTracking(true);
		setAcceptDrops(true);
		setMinimumSize(200,100);
		resize(600,300);
	   	// Overwritten latter. But some text is needed to enable it.
		setWhatsThis("Dummy");
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

	static quint32 positionFileMagicNumber() { return 0x01026420; }

	virtual ~NetworkCanvas();

	void clear();

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
public slots:
	void print()
	{
		_printing = true;
		QPrinter printer;
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
public:
	void paint(QPainter & painter)
	{
		painter.setRenderHint(QPainter::Antialiasing);
		painter.scale(_zoomFactor,_zoomFactor);
		for (unsigned i = 0; i<_controlWires.size(); i++)
			_controlWires[i]->draw(painter);
		for (unsigned i = 0; i<_portWires.size(); i++)
			_portWires[i]->draw(painter);
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
		int cursorSize = 8;

		double tooltipWidth = _metrics.width(_tooltipText)+2*margin;
		double x = _tooltipPos.x()+cursorSize;
		if (x + tooltipWidth > width())
		x = _tooltipPos.x() - _metrics.width(_tooltipText) - 2*margin;

		double tooltipHeight = _metrics.height()+2*margin;
		double y = _tooltipPos.y() +cursorSize;
		if (y + tooltipHeight > height())
		y = _tooltipPos.y() - tooltipHeight;

		QRectF tooltip(x, y, _metrics.width(_tooltipText)+2*margin, tooltipHeight)  ;
		painter.setBrush(QColor(0xff,0xff,0x90,0xa0));
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
		if (!(event->modifiers()&Qt::ControlModifier)) return;
		startDrag(PanDrag,0,0);
		for (unsigned i = _processings.size(); i--; )
		{
			_processings[i]->startMoving(event->globalPos());
		}
		update();
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
				menu.addAction(QIcon(":/icons/images/remove.png"), "Disconnect",
					this, SLOT(onDisconnect()))->setData(event->pos());
				menu.addAction(QIcon(":/icons/images/editcopy.png"), "Copy connection name",
					this, SLOT(onCopyConnection()))->setData(event->pos());
			}
			if (region==ProcessingBox::nameRegion || 
				region==ProcessingBox::bodyRegion ||
				region==ProcessingBox::resizeHandleRegion)
			{
				menu.addAction(QIcon(":/icons/images/configure.png"), "Configure",
					this, SLOT(onConfigure()))->setData(event->pos());
				menu.addAction(QIcon(":/icons/images/editdelete.png"), "Remove",
					this, SLOT(onDeleteProcessing()))->setData(event->pos());
			}
			menu.exec();
			return;
		}
		QMenu menu(this);
		menu.move(event->globalPos());
		menu.addAction(QIcon(":/icons/images/newprocessing.png"), "Add processing",
			this, SLOT(onNewProcessing()))->setData(event->pos());
		menu.exec();

	}

	void dragEnterEvent(QDragEnterEvent *event)
	{
		event->acceptProposedAction();
	}
	void dragMoveEvent(QDragMoveEvent *event)
	{
		event->acceptProposedAction();
	}
	void dropEvent(QDropEvent *event)
	{
		QString type =  event->mimeData()->text();
		event->acceptProposedAction();
		addProcessing(event->pos(), type);
	}
	void wheelEvent(QWheelEvent * event)
	{
		zoom(event->delta());
	}
	bool event(QEvent * event)
	{
		if (event->type()!=QEvent::WhatsThis) return QWidget::event(event);
		QHelpEvent * helpEvent = (QHelpEvent *) event;
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(helpEvent->pos());
			if (region==ProcessingBox::noRegion) continue;
			QString connectionText("%1:\nDrag it to an %2 to connect them.\n"
					"Hover to see its name\n"
					"The context menu has options for disconnecting and "
					"for copying the connection name for using it on Qt Designer.\n");
			switch (region)
			{
			case ProcessingBox::incontrolsRegion:
				setWhatsThis(connectionText.arg(tr("Incontrol")).arg(tr("outcontrol")));
			break;
			case ProcessingBox::outcontrolsRegion:
				setWhatsThis(connectionText.arg(tr("Outcontrol")).arg(tr("incontrol")));
			break;
			case ProcessingBox::inportsRegion:
				setWhatsThis(connectionText.arg(tr("Inport")).arg(tr("outport")));
			break;
			case ProcessingBox::outportsRegion:
				setWhatsThis(connectionText.arg(tr("Outport")).arg(tr("inport")));
			break;
			case ProcessingBox::resizeHandleRegion:
			case ProcessingBox::nameRegion:
			case ProcessingBox::bodyRegion:
				setWhatsThis(tr(
						"<p>\n"
						"This is a Processing, which encapsulates a processing algorithm.\n"
						"Round connectors are ports, which consumes and produces data; "
						"inlets on the left and outlets on the right.\n"
						"Square connectors are controls which communicate events; "
						"inlets on the top and outlets on the bottom.\n"
						"</p>\n"
						"<p>Available actions</p>"
						"<ul>\n"
						"<li>Right click over the body or over the connectors for context actions.</li>\n"
						"<li>Drag connectors to connect to a complementary one.</li>\n"
						"<li>Double click the name to rename.</li>\n"
						"<li>Drag the name to move the processing box.</li>\n"
						"<li>Drag the handle on the bottom right to resize the processing box.</li>\n"
						"</ul>\n"
						));
			}
			return QWidget::event(event);
		}
		setWhatsThis(tr(
			"<p>This is the 'network canvas'. "
			"Drag here processings from the 'Processing tool box' "
			"and connect them by dragging compatible connectors. "
			"You may pan the canvas by dragging it with the control key pressed.</p>"
			));
		return QWidget::event(event);
	}

	void addProcessing(QPoint point, QString type)
	{
		if (!_network)
		{
			addProcessingBox(type, 0, point);
			return;
		}
		try
		{
			std::string name = _network->AddProcessing(type.toStdString());
			CLAM::Processing & processing = _network->GetProcessing(name);
			addProcessingBox(name.c_str(), &processing, point);
		}
		catch (CLAM::Err & e)
		{
			QMessageBox::critical(this, tr("Error creating a processing"),
				tr("<p>The processing type '<tt>%1</tt>' is not supported.</p>").arg(type));
		}
	}

	void addProcessingBox(const QString & name, CLAM::Processing * processing, QPoint point=QPoint(), QSize size=QSize())
	{
		if (!processing)
		{
			QDialog * portsDialog = new QDialog(this);
			QGridLayout * layout = new QGridLayout(portsDialog);
			const char * labels[] = {
				"Number of inports",
				"Number of outports",
				"Number of incontrols",
				"Number of outcontrols",
				0
			};
			std::vector <QSpinBox*> widgets;
			for (unsigned i = 0; labels[i]; i++) 
			{
				QSpinBox * widget = new QSpinBox(portsDialog);
				widget->setMinimum(0);
				widget->setMaximum(10);
				widget->setValue(2);
				QLabel * label = new QLabel(labels[i], portsDialog);
				layout->addWidget(label, i,0);
				layout->addWidget(widget, i,1);
				widgets.push_back(widget);
			}
			QPushButton * ok = new QPushButton("Ok",portsDialog);
			QPushButton * cancel = new QPushButton("Cancel",portsDialog);
			connect(ok, SIGNAL(clicked()), portsDialog, SLOT(accept()));
			connect(cancel, SIGNAL(clicked()), portsDialog, SLOT(reject()));
			layout->addWidget(cancel,4,0);
			layout->addWidget(ok,4,1);
			if (!portsDialog->exec()) return;
			unsigned nInports= widgets[0]->value();
			unsigned nOutports= widgets[1]->value();
			unsigned nIncontrols= widgets[2]->value();
			unsigned nOutcontrols= widgets[3]->value();
			_processings.push_back(new ProcessingBox(this, name, nInports, nOutports, nIncontrols, nOutcontrols));
		}
		else
			_processings.push_back(new ProcessingBox(this, name, 0, 0, 0, 0));
		_processings.back()->setProcessing(processing);
		_processings.back()->move(point);
		_processings.back()->resize(size);
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
		if (_printing) return QColor(0xf0,0xf0,0xf0);
		return QColor(0x30,0x8f,0x30,0xaf);
	}
	QColor colorBoxBody() const
	{
		if (_printing) return QColor(0xd0,0xd0,0xd0);
		return QColor(0xF9,0xFb,0xF9,0xaf);
	}
	QColor colorResizeHandle() const
	{
		if (_printing) return QColor(0xd0,0xf0,0xd0);
		return QColor(0xf9,0xbb,0xb9);
	}
	QColor colorPortOutline() const
	{
		return QColor(0x53,0x30,0x42);
	}
	QColor colorPort() const
	{
		if (_printing) return QColor(0xd0,0x50,0xa0);
		return QColor(0xa6,0x60,0x84);
	}
	QColor colorControlOutline() const
	{
		return QColor(0x53,0x30,0x42);
	}
	QColor colorControl() const
	{
		return QColor(0xf6,0x60,0x84);
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
	void endConnectionDragTo(ProcessingBox * processing, int connection)
	{
		switch (_dragStatus) 
		{
			case InportDrag:
			{
				addPortConnection(processing, connection, _dragProcessing, _dragConnectionIndex);
			} break;
			case OutportDrag:
			{
				addPortConnection(_dragProcessing, _dragConnectionIndex, processing, connection);
			} break;
			case IncontrolDrag:
			{
				addControlConnection(processing, connection, _dragProcessing, _dragConnectionIndex);
			} break;
			case OutcontrolDrag:
			{
				addControlConnection(_dragProcessing, _dragConnectionIndex, processing, connection);
			} break;
		}
	}
	void addPortConnection(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		if (_network)
		{
			QString out = source->getName() + "." + source->getOutportName(outlet);
			QString in = target->getName() + "." + target->getInportName(inlet);
			if (!_network->ConnectPorts(out.toStdString(), in.toStdString())) return;
		}
		addPortWire(source, outlet, target, inlet);
	}
	void addControlConnection(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		if (_network)
		{
			QString out = source->getName() + "." + source->getOutcontrolName(outlet);
			QString in = target->getName() + "." + target->getIncontrolName(inlet);
			if (!_network->ConnectControls(out.toStdString(), in.toStdString())) return;
		}
		addControlWire(source, outlet, target, inlet);
	}
	void addPortWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		_portWires.push_back(new PortWire(source, outlet, target, inlet));
	}
	void addControlWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		_controlWires.push_back(new ControlWire(source, outlet, target, inlet));
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
				if (_network)
					_network->DisconnectControls(wire->getSourceId(), wire->getTargetId());
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
				if (_network)
					_network->DisconnectControls(wire->getSourceId(), wire->getTargetId());
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
				if (_network)
					_network->DisconnectPorts(wire->getSourceId(), wire->getTargetId());
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
				if (_network)
					_network->DisconnectPorts(wire->getSourceId(), wire->getTargetId());
				delete wire;
				it=_portWires.erase(it);
			}
		}
	}

	void removeProcessing(ProcessingBox * processing)
	{
		if (_network) _network->RemoveProcessing(processing->getName().toStdString());
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

	void loadNetwork(CLAM::Network * network)
	{
		clear();
		_network = network;
		if (!_network) return;
		CLAM::Network::ProcessingsMap::const_iterator it;
		for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
		{
			const std::string & name = it->first;
			CLAM::Processing * processing = it->second;
			addProcessingBox( name.c_str(),  processing );
		}
		// TODO: Refactor this code please!!!
		for (unsigned p = 0; p<_processings.size(); p++)
		{
			std::string producerName = _processings[p]->getName().toStdString();
			CLAM::Processing & producer = _network->GetProcessing(producerName);
			CLAM::OutPortRegistry & outPorts = producer.GetOutPorts();
			for (unsigned op = 0; op<outPorts.Size(); op++)
			{
				CLAM::OutPortBase & outPort = outPorts.GetByNumber(op);
				std::string completeOutName = producerName + "." + outPort.GetName();
				CLAM::Network::NamesList connected = _network->GetInPortsConnectedTo( completeOutName );
				CLAM::Network::NamesList::iterator inName;
				for(inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					CLAM::InPortRegistry & inPorts = consumer.GetInPorts();
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?
					for (unsigned ip=0; ip<consumer.GetInPorts().Size(); ip++)
					{
						CLAM::InPortBase & inPort = inPorts.GetByNumber(ip);
						if (inPort.GetName()!=peerConnection) continue;
						addPortWire(_processings[p], op, consumerBox, ip);
						break;
					}
				}
			}
			CLAM::OutControlRegistry & outControls = producer.GetOutControls();
			for (unsigned op = 0; op<outControls.Size(); op++)
			{
				CLAM::OutControl & outControl = outControls.GetByNumber(op);
				std::string completeOutName = producerName + "." + outControl.GetName();
				CLAM::Network::NamesList connected = _network->GetInControlsConnectedTo( completeOutName );
				CLAM::Network::NamesList::iterator inName;
				for(inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					CLAM::InControlRegistry & inControls = consumer.GetInControls();
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?
					for (unsigned ip=0; ip<consumer.GetInControls().Size(); ip++)
					{
						CLAM::InControl & inControl = inControls.GetByNumber(ip);
						if (inControl.GetName()!=peerConnection) continue;
						addControlWire(_processings[p], op, consumerBox, ip);
						break;
					}
				}
			}
		}
	}
	void savePositions(const QString & filename)
	{
		QFile file(filename);
		file.open(QIODevice::WriteOnly);
		QDataStream stream(&file);
		stream << positionFileMagicNumber();
		for (unsigned i=0; i<_processings.size(); i++)
		{
			stream << _processings[i]->getName();
			stream << _processings[i]->pos();
			stream << _processings[i]->size();
		}
	}
	void loadPositions(const QString & filename)
	{
		QFile file(filename);
		file.open(QIODevice::ReadOnly);
		QDataStream stream(&file);
		quint32 magic;
		stream >> magic;
		if (magic!=positionFileMagicNumber()) return;
		while (!stream.atEnd())
		{
			QString name;
			QPoint pos;
			QSize size;
			stream >> name;
			stream >> pos;
			stream >> size;
			if (stream.status()!=QDataStream::Ok) break;
			ProcessingBox * box = getBox(name);
			if (!box) continue;
			box->move(pos);
			box->resize(size);
		}
	}

	void zoom(int steps)
	{
		_zoomFactor*=1 + steps*.25;
		update();
	}
	void resetZoom()
	{
		_zoomFactor=1.;
		update();
	}
private:
	ProcessingBox * getBox(const QString & name)
	{
		for (unsigned i=0; i<_processings.size(); i++)
			if (_processings[i]->getName()==name) return _processings[i];
		return 0;
	}

private slots:
	void onCopyConnection()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString toCopy;
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			switch (region)
			{
				case ProcessingBox::outportsRegion: 
					toCopy = QString("Outport__%1__%2")
						.arg(_processings[i]->getName())
						.arg(_processings[i]->getOutportName(_processings[i]->portIndexByYPos(point)));
					break;
				case ProcessingBox::inportsRegion: 
					toCopy = QString("Inport__%1__%2")
						.arg(_processings[i]->getName())
						.arg(_processings[i]->getInportName(_processings[i]->portIndexByYPos(point)));
					break;
				case ProcessingBox::incontrolsRegion: 
					toCopy = QString("Incontrol__%1__%2")
						.arg(_processings[i]->getName())
						.arg(_processings[i]->getIncontrolName(_processings[i]->controlIndexByXPos(point)));
					break;
				case ProcessingBox::outcontrolsRegion: 
					toCopy = QString("Outcontrol__%1__%2")
						.arg(_processings[i]->getName())
						.arg(_processings[i]->getOutcontrolName(_processings[i]->controlIndexByXPos(point)));
					break;
			}
			break;
		}
		if (toCopy.isNull()) return;
		QApplication::clipboard()->setText(toCopy);
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
			}
			return;
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
	double _zoomFactor;
	CLAM::Network * _network;
};



#endif//NetworkCanvas_hxx
