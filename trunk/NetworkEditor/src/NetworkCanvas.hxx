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
#include <QtGui/QDialogButtonBox>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtGui/QCompleter>
#include <QtCore/QTextStream>
#include "ProcessingBox.hxx"
#include "Wires.hxx"
#include <vector>
#include <algorithm>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include "OutControlSender.hxx" //TODO move to cxx

class AbstractNetworkCanvas : public QWidget
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
		SelectionDrag
	};
	AbstractNetworkCanvas(QWidget * parent=0)
		: QWidget(parent)
		, _zoomFactor(1.)
		, _changed(false)
		, _dragStatus(NoDrag)
		, _dragProcessing(0)
		, _dragConnection(0)
		, _colorBoxFrameText      (0xff,0xff,0xff)
		, _colorBoxFrameOutline   (0x20,0x6f,0x20)
		, _colorBoxFrame          (0x30,0x8f,0x30,0xaf)
		, _colorBoxBody           (0xF9,0xFb,0xF9,0xaf)
		, _colorBoxErrorFrame     (0xff,0x00,0x00,0xa0)
		, _colorBoxErrorBody      (0xff,0x00,0x00,0xa0)
		, _colorResizeHandle      (0xf9,0xbb,0xb9)
		, _colorPortOutline       (0x53,0x30,0x42)
		, _colorPort              (0xa6,0x60,0x84)
		, _colorControlOutline    (0x53,0x30,0x42)
		, _colorControl           (0xf6,0x60,0x84)
		, _colorSelectBoxBody     (0x77,0xff,0x88,0x37)
		, _colorSelectBoxLine     (0x77,0xff,0x88,0xf7)
		, _colorHighlight         (Qt::yellow)
		, _colorForbidenDrop      (0xff,0xaa,0xaa)
		, _colorAllowedDrop       (0xaa,0xff,0xaa)
		, _colorTooltipBody       (0xff,0xff,0x90,0xa0)
		, _colorTooltipLine       (0xff,0xff,0x90,0xff)
		, _colorTooltipText       (Qt::black)
		, _colorPortWire          (0xbb,0x99,0x44)
		, _colorPortWireOutline   (0x50,0x50,0x22)
		, _colorControlWire       (0x4b,0x99,0xb4)
		, _colorControlWireOutline(0x20,0x50,0x52)
	{
		setMouseTracking(true);
		setAcceptDrops(true);
		setMinimumSize(200,100);
		resize(600,300);
	   	// Overwritten latter. But some text is needed to enable it.
		setWhatsThis("Dummy");

		_deleteSelectedAction = new QAction("Delete", this);
		_deleteSelectedAction->setShortcut(QKeySequence(tr("Del")));
		addAction(_deleteSelectedAction);
		connect(_deleteSelectedAction, SIGNAL(triggered()), this, SLOT(removeSelectedProcessings()));

		_selectAllAction = new QAction("Select all", this);
		_selectAllAction->setShortcut(QKeySequence(tr("Ctrl+A")));
		addAction(_selectAllAction);
		connect(_selectAllAction, SIGNAL(triggered()), this, SLOT(onSelectAll()));

		_clearSelectionAction = new QAction("Clear selection", this);
		_clearSelectionAction->setShortcut(QKeySequence(tr("Ctrl+Shift+A")));
		addAction(_clearSelectionAction);
		connect(_clearSelectionAction, SIGNAL(triggered()), this, SLOT(onClearSelections()));
	}
	void raise(ProcessingBox * toRaise)
	{
		std::vector<ProcessingBox*>::iterator search = std::find(_processings.begin(), _processings.end(), toRaise);
		if (search==_processings.end()) return;
		_processings.erase(search);
		_processings.push_back(toRaise);
		toRaise->raiseEmbeded();
	}
	void addPortWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		_portWires.push_back(new PortWire(source, outlet, target, inlet));
	}
	void addControlWire(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		_controlWires.push_back(new ControlWire(source, outlet, target, inlet));
	}
	void clear()
	{
		clearWires();
		for (unsigned i = 0; i<_processings.size(); i++)
			delete _processings[i];
		_processings.clear();
		update();
	}
	void clearWires()
	{
		for (unsigned i = 0; i<_portWires.size(); i++)
			delete _portWires[i];
		_portWires.clear();
		for (unsigned i = 0; i<_controlWires.size(); i++)
			delete _controlWires[i];
		_controlWires.clear();
	}

	QColor colorBoxFrameText() const { return _colorBoxFrameText; }
	QColor colorBoxFrameOutline() const { return _colorBoxFrameOutline; }
	QColor colorBoxFrame() const { return _colorBoxFrame; }
	QColor colorBoxBody() const { return _colorBoxBody; }
	QColor colorBoxErrorFrame() const { return _colorBoxErrorFrame; }
	QColor colorBoxErrorBody() const { return _colorBoxErrorBody; }
	QColor colorResizeHandle() const { return _colorResizeHandle; }
	QColor colorPortOutline() const { return _colorPortOutline; }
	QColor colorPort() const { return _colorPort; }
	QColor colorControlOutline() const { return _colorControlOutline; }
	QColor colorControl() const { return _colorControl; }
	QColor colorSelectBoxBody() const { return _colorSelectBoxBody; }
	QColor colorSelectBoxLine() const { return _colorSelectBoxLine; }
	QColor colorHighlight() const { return _colorHighlight; } 
	QColor colorForbidenDrop() const { return _colorForbidenDrop; }
	QColor colorAllowedDrop() const { return _colorAllowedDrop; }

	QColor colorHighlightConnector(ProcessingBox * processing, ProcessingBox::Region region, int connection)
	{
		switch (_dragStatus) 
		{
			case InportDrag:
				if (region != ProcessingBox::outportsRegion) return _colorForbidenDrop;
				if (canConnectPorts(processing, connection, _dragProcessing, _dragConnection))
					return _colorAllowedDrop;
				return _colorForbidenDrop;
			case OutportDrag:
				if (region != ProcessingBox::inportsRegion) return _colorForbidenDrop;
				if (canConnectPorts(_dragProcessing, _dragConnection, processing, connection))
					return _colorAllowedDrop;
				return _colorForbidenDrop;
			case IncontrolDrag:
				if (region != ProcessingBox::outcontrolsRegion) return _colorForbidenDrop;
				if (canConnectControls(processing, connection, _dragProcessing, _dragConnection))
					return _colorAllowedDrop;
				return _colorForbidenDrop;
			case OutcontrolDrag:
				if (region != ProcessingBox::incontrolsRegion) return _colorForbidenDrop;
				if (canConnectControls(_dragProcessing, _dragConnection, processing, connection))
					return _colorAllowedDrop;
				return _colorForbidenDrop;
			default:
				return _colorHighlight;
		}
	}
// Drawing routines
protected:
	void paint(QPainter & painter)
	{
		_boundingBox=QRect(0,0,1,1);
		for (unsigned i = 0; i<_processings.size(); i++)
			_boundingBox = _boundingBox.unite(QRect(_processings[i]->pos(),_processings[i]->size()));
		for (unsigned i = 0; i<_controlWires.size(); i++)
			_controlWires[i]->expand(_boundingBox);
		for (unsigned i = 0; i<_portWires.size(); i++)
			_portWires[i]->expand(_boundingBox);
		_boundingBox = _boundingBox.unite(QRect(_boundingBox.topLeft(),((QWidget*)parent())->size()/_zoomFactor));
		resize(_boundingBox.size()*_zoomFactor);

		painter.setRenderHint(QPainter::Antialiasing);
		painter.scale(_zoomFactor,_zoomFactor);
		painter.translate(-_boundingBox.topLeft());

		for (unsigned i = 0; i<_controlWires.size(); i++)
			_controlWires[i]->draw(painter);
		for (unsigned i = 0; i<_portWires.size(); i++)
			_portWires[i]->draw(painter);
		for (unsigned i = 0; i<_processings.size(); i++)
			_processings[i]->paintFromParent(painter);
		if (_dragStatus==InportDrag)
			PortWire::draw(painter, _dragPoint, _dragProcessing->getInportPos(_dragConnection));
		if (_dragStatus==OutportDrag)
			PortWire::draw(painter, _dragProcessing->getOutportPos(_dragConnection), _dragPoint);
		if (_dragStatus==IncontrolDrag)
			ControlWire::draw(painter, _dragPoint, _dragProcessing->getIncontrolPos(_dragConnection));
		if (_dragStatus==OutcontrolDrag)
			ControlWire::draw(painter, _dragProcessing->getOutcontrolPos(_dragConnection), _dragPoint);
		drawSelectBox(painter);
		drawTooltip(painter);
	}
	void drawSelectBox(QPainter & painter)
	{
		if (_dragStatus!=SelectionDrag) return;
		painter.setBrush(_colorSelectBoxBody);
		painter.setPen(_colorSelectBoxLine);
		painter.drawRect(QRect(_dragPoint, _selectionDragOrigin));
	}
	void drawTooltip(QPainter & painter)
	{
		if (_tooltipText.isNull()) return;
		QFontMetrics metrics(font());
		int margin =3;
		int cursorSize = 16;

		QRect boundingRect = metrics.boundingRect(QRect(0,0,width(),height()), Qt::AlignLeft, _tooltipText);
		double tooltipWidth = boundingRect.width()+2*margin;
		double x = _tooltipPos.x()+cursorSize;
		if (x + tooltipWidth > width())
			x = _tooltipPos.x() - tooltipWidth;
		if (x<0) x=0;

		double tooltipHeight = boundingRect.height()+2*margin;
		double y = _tooltipPos.y() +cursorSize;
		if (y + tooltipHeight > height())
			y = _tooltipPos.y() - tooltipHeight;
		if (y<0) y=0;

		QRectF tooltip(x, y, tooltipWidth, tooltipHeight)  ;
		painter.setBrush(_colorTooltipBody);
		painter.setPen(_colorTooltipLine);
		painter.drawRect(tooltip);
		painter.setPen(_colorTooltipText);
		painter.drawText(tooltip, Qt::AlignLeft, _tooltipText);
	}

public: // Helpers
	void setToolTip(const QString & text)
	{
		_tooltipText = text;
	}

	QRect translatedRect(QRect rect)
	{
		rect.setSize(rect.size()*_zoomFactor);
		rect.moveTopLeft((rect.topLeft()-_boundingBox.topLeft())*_zoomFactor);
		return rect;
	}
	template <class Event> QPoint translatedPos(Event * event)
	{
		return event->pos()/_zoomFactor+_boundingBox.topLeft();
	}
	template <class Event> QPoint translatedGlobalPos(Event * event)
	{
		return event->globalPos()/_zoomFactor;
	}

public: // Actions
	void clearSelections()
	{
		for (unsigned i=0; i<_processings.size(); i++)
			_processings[i]->deselect();
	}
	void selectAll()
	{
		for (unsigned i=0; i<_processings.size(); i++)
			_processings[i]->select();
	}
	void startMovingSelected(QMouseEvent * event)
	{
		for (unsigned i=0; i<_processings.size(); i++)
		{
			if (!_processings[i]->isSelected()) continue;
			_processings[i]->startMoving(translatedGlobalPos(event));
		}
		setCursor(Qt::SizeAllCursor);
	}
	/**
	 * To be called by the ProcessingBox when some one drops a wire on its connectors.
	 * @pre The processing box has checked that connection is the proper one for the canvas _dragStatus.
	 */
	void endWireDrag(ProcessingBox * processing, int connection)
	{
		switch (_dragStatus) 
		{
			case InportDrag:
			{
				addPortConnection(processing, connection, _dragProcessing, _dragConnection);
			} break;
			case OutportDrag:
			{
				addPortConnection(_dragProcessing, _dragConnection, processing, connection);
			} break;
			case IncontrolDrag:
			{
				addControlConnection(processing, connection, _dragProcessing, _dragConnection);
			} break;
			case OutcontrolDrag:
			{
				addControlConnection(_dragProcessing, _dragConnection, processing, connection);
			} break;
			default:
			{
				CLAM_ASSERT(false, "Ending a wire drag but not in wire drag status");
			} return;
		}
	}
public slots:
	void print()
	{
		// Save current colors
		QColor colorBoxFrameText = _colorBoxFrameText;
		QColor colorBoxFrameOutline = _colorBoxFrameOutline;
		QColor colorBoxFrame = _colorBoxFrame;
		QColor colorBoxBody = _colorBoxBody;
		QColor colorResizeHandle = _colorResizeHandle;
		QColor colorPort = _colorPort;

		// Set printer friendly colors
		_colorBoxFrameText = QColor(0x00,0x00,0x00);
		_colorBoxFrameOutline = QColor(0x00,0x00,0x00);
		_colorBoxFrame = QColor(0xf0,0xf0,0xf0);
		_colorBoxBody = QColor(0xd0,0xd0,0xd0);
		_colorResizeHandle = QColor(0xd0,0xf0,0xd0);
		_colorPort = QColor(0xd0,0x50,0xa0);

		QPrinter printer;
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName("ExportedNetwork.pdf");
		printer.setFullPage(true);
		printer.setCreator( "CLAM NetworkEditor");
		printer.setOrientation(QPrinter::Landscape);
		QPrintDialog * dialog = new QPrintDialog(&printer, this);
		dialog->exec();
		QPainter painter;
		painter.begin(&printer);
		paint(painter);
		painter.end();

		// Restore display colors
		_colorBoxFrameText = colorBoxFrameText;
		_colorBoxFrameOutline = colorBoxFrameOutline;
		_colorBoxFrame = colorBoxFrame;
		_colorBoxBody = colorBoxBody;
		_colorResizeHandle = colorResizeHandle;
		_colorPort = colorPort;
	}
private slots:
	void onClearSelections()
	{
		clearSelections();
		update();
	}
	void onSelectAll()
	{
		selectAll();
		update();
	}
	void removeSelectedProcessings()
	{
		std::vector<ProcessingBox *> toRemove;
		for (unsigned i=0; i<_processings.size(); i++)
			if (_processings[i]->isSelected()) toRemove.push_back(_processings[i]) ;
		for (unsigned i=0; i<toRemove.size(); i++)
			removeProcessing( toRemove[i] );
		update();
	}
public:
	void startDrag(DragStatus status, ProcessingBox * processing, int connection)
	{
		_dragStatus=status;
		_dragProcessing=processing;
		_dragConnection=connection;
	}
	DragStatus dragStatus() const
	{
		return _dragStatus;
	}
	bool renameProcessing(QString oldName, QString newName)
	{
		return networkRenameProcessing(oldName, newName);
	}
	void removeProcessing(ProcessingBox * processing)
	{
		networkRemoveProcessing(processing->getName().toStdString());
		markAsChanged();
		for (std::vector<ControlWire*>::iterator it=_controlWires.begin();
			   	it<_controlWires.end(); )
		{
			ControlWire * wire = *it;
			if ( !wire->involves(processing)) it++;
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
			if ( !wire->involves(processing)) it++;
			else
			{
				delete wire;
				it=_portWires.erase(it);
			}
		}
		delete processing;
		_processings.erase(std::find(_processings.begin(), _processings.end(), processing));
	}
	void zoom(int steps)
	{
		if (steps>=0)
			for (int i=0; i<steps; i++)
				_zoomFactor*=1.0625;
		else
			for (int i=0; i>steps; i--)
				_zoomFactor/=1.0625;
		update();
	}
	void resetZoom()
	{
		_zoomFactor=1.;
		update();
	}
protected:
	ProcessingBox * getBox(const QString & name)
	{
		for (unsigned i=0; i<_processings.size(); i++)
			if (_processings[i]->getName()==name) return _processings[i];
		return 0;
	}
	void addPortConnection(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		QString out = source->getName() + "." + source->getOutportName(outlet);
		QString in = target->getName() + "." + target->getInportName(inlet);
		if (!networkAddPortConnection(out,in)) return;
		addPortWire(source, outlet, target, inlet);
		markAsChanged();
	}
	void addControlConnection(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		QString out = source->getName() + "." + source->getOutcontrolName(outlet);
		QString in = target->getName() + "." + target->getIncontrolName(inlet);
		if ( !networkAddControlConnection(out, in)) return;
		addControlWire(source, outlet, target, inlet);
		markAsChanged();
	}
	void disconnectIncontrol(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<ControlWire*>::iterator it=_controlWires.begin();
			   	it<_controlWires.end();)
		{
			ControlWire * wire = *it;
			if ( !wire->goesTo(processing, index)) it++;
			else
			{
				networkRemoveControlConnection(wire->getSourceId(), wire->getTargetId());
				delete wire;
				it=_controlWires.erase(it);
				markAsChanged();
			}
		}
	}
	void disconnectOutcontrol(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<ControlWire*>::iterator it=_controlWires.begin();
			   	it<_controlWires.end();)
		{
			ControlWire * wire = *it;
			if ( !wire->comesFrom(processing, index)) it++;
			else
			{
				networkRemoveControlConnection(wire->getSourceId(), wire->getTargetId());
				delete wire;
				it=_controlWires.erase(it);
				markAsChanged();
			}
		}
	}
	void disconnectInport(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<PortWire*>::iterator it=_portWires.begin();
			   	it<_portWires.end();)
		{
			PortWire * wire = *it;
			if ( !wire->goesTo(processing, index)) it++;
			else
			{
				networkRemovePortConnection(wire->getSourceId(), wire->getTargetId());
				delete wire;
				it=_portWires.erase(it);
				markAsChanged();
			}
		}
	}
	void disconnectOutport(ProcessingBox * processing, unsigned index)
	{
		for (std::vector<PortWire*>::iterator it=_portWires.begin();
			   	it<_portWires.end();)
		{
			PortWire * wire = *it;
			if ( !wire->comesFrom(processing, index)) it++;
			else
			{
				networkRemovePortConnection(wire->getSourceId(), wire->getTargetId());
				delete wire;
				it=_portWires.erase(it);
				markAsChanged();
			}
		}
	}
protected:
/*
	virtual unsigned processingNInPorts(QString name)
	virtual unsigned processingNOutPorts(QString name)
	virtual unsigned processingNInControls(QString name)
	virtual unsigned processingNOutControls(QString name)
	virtual QString processingInPortName(QString name, unsigned i)
	virtual QString processingInPortTooltip(QString name, unsigned i)
	virtual QString processingInPortPrototyperId(QString name, unsigned i)
*/	
	virtual bool networkRenameProcessing(QString oldName, QString newName)=0;
	virtual void networkRemoveProcessing(const std::string & name) = 0;
	virtual void addProcessing(QPoint point, QString type) = 0;
	virtual bool canConnectPorts(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet) = 0;
	virtual bool canConnectControls(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet) = 0;
	virtual bool networkAddPortConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkAddControlConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkRemovePortConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkRemoveControlConnection(const QString & outlet, const QString & inlet) = 0;

	virtual QColor inportColor(void * processing, unsigned index) const = 0;
	virtual QColor outportColor(void * processing, unsigned index) const = 0;
	virtual QString inportName(void * processing, unsigned index) const = 0;
	virtual QString outportName(void * processing, unsigned index) const = 0;
	virtual QString incontrolName(void * processing, unsigned index) const = 0;
	virtual QString outcontrolName(void * processing, unsigned index) const = 0;
/*
	virtual QString inportTooltip(void * processing, unsigned index) const = 0;
	virtual QString outportTooltip(void * processing, unsigned index) const = 0;
	virtual QString incontrolTooltip(void * processing, unsigned index) const = 0;
	virtual QString outcontrolTooltip(void * processing, unsigned index) const = 0;
*/
	virtual bool isOk(void * processing)=0;
	virtual QString errorMessage(void * processing)=0;

signals:
	void changed();
public:
	bool isChanged()
	{
		return _changed;
	}
	void markAsChanged()
	{
		_changed = true;
		emit changed();
	}
	void clearChanges()
	{
		_changed = false;
	}

public: // Event Handlers

	void mouseMoveEvent(QMouseEvent * event)
	{
		_dragPoint = translatedPos(event);
		setToolTip(0);
		setStatusTip(0);
		setCursor(Qt::ArrowCursor);
		for (unsigned i = _processings.size(); i--; )
			_processings[i]->mouseMoveEvent(event);
		_tooltipPos=_dragPoint;
		update();
	}
	void mousePressEvent(QMouseEvent * event)
	{
		if (event->button()!=Qt::LeftButton) return;
		QPoint translatedPoint = translatedPos(event);
		for (unsigned i = _processings.size(); i--; )
		{
			if (_processings[i]->getRegion(translatedPoint)==ProcessingBox::noRegion) continue;
			_processings[i]->mousePressEvent(event);
			update();
			return;
		}
		if (! (event->modifiers() & Qt::ControlModifier) )
			clearSelections();
		_selectionDragOrigin=translatedPoint;
		startDrag(SelectionDrag,0,0);
		update();
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		if (_dragStatus == SelectionDrag)
		{
			QRect selectionBox (_selectionDragOrigin, _dragPoint);
			for (unsigned i = _processings.size(); i--; )
				if (selectionBox.contains(QRect(_processings[i]->pos(),_processings[i]->size())))
					_processings[i]->select();
		}
		for (unsigned i = _processings.size(); i--; )
			_processings[i]->mouseReleaseEvent(event);
		_dragStatus=NoDrag;
		update();
	}
	void mouseDoubleClickEvent(QMouseEvent * event)
	{
		QPoint translatedPoint = translatedPos(event);
		for (unsigned i = _processings.size(); i--; )
		{
			if (_processings[i]->getRegion(translatedPoint)==ProcessingBox::noRegion) continue;
			_processings[i]->mouseDoubleClickEvent(event);
			update();
			return;
		}
		print();
	}

	void contextMenuEvent(QContextMenuEvent * event)
	{
		QMenu menu(this);
		menu.move(event->globalPos());
		QPoint translatedPoint = translatedPos(event);
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(translatedPoint);
			if (region==ProcessingBox::noRegion) continue;
			if (
				region==ProcessingBox::inportsRegion ||
				region==ProcessingBox::outportsRegion ||
				region==ProcessingBox::incontrolsRegion ||
				region==ProcessingBox::outcontrolsRegion )
			{
				menu.addAction(QIcon(":/icons/images/remove.png"), "Disconnect",
					this, SLOT(onDisconnect()))->setData(translatedPoint);
				menu.addAction(QIcon(":/icons/images/editcopy.png"), "Copy connection name",
					this, SLOT(onCopyConnection()))->setData(translatedPoint);
				if (region==ProcessingBox::incontrolsRegion)
				{
					menu.addAction(QIcon(":/icons/images/hslider.png"), "Add slider",
						this, SLOT(onAddSlider()))->setData(translatedPoint);
				}
				if (region==ProcessingBox::outportsRegion)
				{
					typedef CLAM::ProcessingFactory::Keys Keys;
					menu.addSeparator();
					CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
					unsigned portindex = _processings[i]->portIndexByYPos(translatedPoint);
					Keys keys = factory.GetKeys("port_monitor_type", _processings[i]->getOutportTypeId(portindex));
					for (Keys::const_iterator it=keys.begin(); it!=keys.end(); it++)
					{
						const char* key = it->c_str();
						std::string iconPath = factory.GetValueFromAttribute(key, "icon");
						QIcon icon = QIcon( QString(":/icons/images/%1").arg(iconPath.c_str()) );
						menu.addAction( icon, key, this, SLOT(onAddMonitor()))->setData(translatedPoint);
					}
				}
			}
			if (region==ProcessingBox::nameRegion || 
				region==ProcessingBox::bodyRegion ||
				region==ProcessingBox::resizeHandleRegion)
			{
				menu.addAction(QIcon(":/icons/images/configure.png"), "Configure",
					this, SLOT(onConfigure()))->setData(translatedPoint);
				menu.addAction(QIcon(":/icons/images/rename.png"), "Rename",
					this, SLOT(onRename()))->setData(translatedPoint);
				menu.addAction(QIcon(":/icons/images/editdelete.png"), "Remove",
					this, SLOT(onDeleteProcessing()))->setData(translatedPoint);
			}
			menu.exec();
			return;
		}
		menu.addAction(QIcon(":/icons/images/newprocessing.png"), "Add processing",
			this, SLOT(onNewProcessing()))->setData(translatedPoint);
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
		addProcessing(translatedPos(event), type);
	}
	void wheelEvent(QWheelEvent * event)
	{
		const int deltaUnitsPerDegree = 8;
		const int degreesPerStep = 15;
		int steps = event->delta() / (deltaUnitsPerDegree*degreesPerStep);
		zoom(steps);
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
			case ProcessingBox::noRegion: break; // To avoid warning
			case ProcessingBox::incontrolsRegion:
				setWhatsThis(connectionText.arg(tr("In control")).arg(tr("out control")));
			break;
			case ProcessingBox::outcontrolsRegion:
				setWhatsThis(connectionText.arg(tr("Out control")).arg(tr("in control")));
			break;
			case ProcessingBox::inportsRegion:
				setWhatsThis(connectionText.arg(tr("In port")).arg(tr("out port")));
			break;
			case ProcessingBox::outportsRegion:
				setWhatsThis(connectionText.arg(tr("Out port")).arg(tr("in port")));
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

protected:
	std::vector<ProcessingBox *> _processings;
	std::vector<PortWire *> _portWires;
	std::vector<ControlWire *> _controlWires;
	double _zoomFactor;
	QRect _boundingBox;
private:
	bool _changed;
protected:
	DragStatus _dragStatus;
	ProcessingBox * _dragProcessing;
	unsigned _dragConnection;
	QPoint _dragPoint;
	QPoint _selectionDragOrigin;
	QPoint _tooltipPos;
	QString _tooltipText;
	QAction * _deleteSelectedAction;
	QAction * _selectAllAction;
	QAction * _clearSelectionAction;
	QColor _colorBoxFrameText;
	QColor _colorBoxFrameOutline;
	QColor _colorBoxFrame;
	QColor _colorBoxBody;
	QColor _colorBoxErrorFrame;
	QColor _colorBoxErrorBody;
	QColor _colorResizeHandle;
	QColor _colorPortOutline;
	QColor _colorPort;
	QColor _colorControlOutline;
	QColor _colorControl;
	QColor _colorSelectBoxBody;
	QColor _colorSelectBoxLine;
	QColor _colorHighlight;
	QColor _colorForbidenDrop;
	QColor _colorAllowedDrop;
	QColor _colorTooltipBody;
	QColor _colorTooltipLine;
	QColor _colorTooltipText;
	QColor _colorPortWire;
	QColor _colorPortWireOutline;
	QColor _colorControlWire;
	QColor _colorControlWireOutline;
};

#include <typeinfo>
#include <CLAM/ProcessingDataPlugin.hxx>

class NetworkCanvas : public AbstractNetworkCanvas
{
	Q_OBJECT
public:
	NetworkCanvas(QWidget * parent=0)
		: AbstractNetworkCanvas(parent)
		, _network(0)
	{
	}
	CLAM::Network & network()
	{
		return *_network;
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

	void paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		paint(painter);
	}
public: // Actions

	void addProcessing(QPoint point, QString type)
	{
		if (networkIsDummy())
		{
			addProcessingBox(type, 0, point);
			markAsChanged();
			return;
		}
		try
		{
			std::string name = _network->AddProcessing(type.toStdString());
			CLAM::Processing & processing = _network->GetProcessing(name);
			addProcessingBox(name.c_str(), &processing, point);
			markAsChanged();
		}
		catch (CLAM::Err & e)
		{
			QMessageBox::critical(this, tr("Error creating a processing"),
				tr("<p>The processing type '<tt>%1</tt>' is not supported.</p>").arg(type));
		}
	}
	void addControlSenderProcessing( ProcessingBox * processing, QPoint point )
	{
		if (networkIsDummy()) return;

		unsigned controlIndex = processing->controlIndexByXPos(point);
		QString inControlName = processing->getIncontrolName(controlIndex);
		float defaultValue = processing->getIncontrolDefaultValue(controlIndex);
		float lower = processing->getIncontrolLowerBound(controlIndex);
		float upper = processing->getIncontrolUpperBound(controlIndex);

		std::string controlSenderName  =  inControlName.toStdString();
		// add control-sender processing to network
		if (_network->HasProcessing(controlSenderName) )
		{
			controlSenderName = _network->GetUnusedName(controlSenderName);
		}
		_network->AddProcessing( controlSenderName, "OutControlSender");
		CLAM::Processing & controlSender = _network->GetProcessing( controlSenderName );
		// configure 
		CLAM::OutControlSenderConfig config;
		config.SetMin(lower);
		config.SetMax(upper);
		config.SetStep( std::max( (upper-lower)/200, CLAM::TControlData(0.01)) ); 
		config.SetDefault( defaultValue );
		controlSender.Configure( config );
		// add box to canvas and connect
		addProcessingBox( controlSenderName.c_str(), &controlSender, point+QPoint(0,-100));
		addControlConnection( getBox(controlSenderName.c_str()), 0, processing, controlIndex );

		markAsChanged();
	}
	void addPortMonitorProcessing( ProcessingBox * processing, QPoint point, const QString & monitorType )
	{
		if (networkIsDummy()) return;

		unsigned portIndex = processing->portIndexByYPos(point);
		QString outPortName = processing->getOutportName(portIndex);

		std::string processingId = _network->AddProcessing(monitorType.toStdString() );
		CLAM::Processing & portMonitor = _network->GetProcessing( processingId );
		// add box to canvas and connect
		addProcessingBox( processingId.c_str(), &portMonitor, point+QPoint(100,0));
		addPortConnection(processing, portIndex, getBox(processingId.c_str()), 0);

		markAsChanged();
	}
	virtual QColor inportColor(void * element, unsigned index) const
	{
		if (!element) return colorPort();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& porttype = processing->GetInPorts().GetByNumber(index).GetTypeId();
		return getConnectorColorByType(porttype);
	}
	virtual QColor outportColor(void * element, unsigned index) const
	{
		if (!element) return colorPort();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& porttype = processing->GetOutPorts().GetByNumber(index).GetTypeId();
		return getConnectorColorByType(porttype);
	}
	virtual QString inportName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Inport_%1").arg(index);
		CLAM::InPortBase & port = ((CLAM::Processing*)processing)->GetInPorts().GetByNumber(index);
		return port.GetName().c_str();
	}
	virtual QString outportName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Outport_%1").arg(index);
		CLAM::OutPortBase & port = ((CLAM::Processing*)processing)->GetOutPorts().GetByNumber(index);
		return port.GetName().c_str();
	}
	virtual QString incontrolName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Incontrol_%1").arg(index);
		CLAM::InControl & control = ((CLAM::Processing*)processing)->GetInControls().GetByNumber(index);
		return control.GetName().c_str();
	}
	virtual QString outcontrolName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Outcontrol_%1").arg(index);
		CLAM::OutControl & control = ((CLAM::Processing*)processing)->GetOutControls().GetByNumber(index);
		return control.GetName().c_str();
	}
	virtual bool isOk(void * processing)
	{
		if (!processing) return true;
		return ((CLAM::Processing*)processing)->IsConfigured();
	}
	virtual QString errorMessage(void * processing)
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetConfigErrorMessage().c_str();
	}
private:
	QColor getConnectorColorByType(const std::type_info & type) const
	{
		const char * colorstring = CLAM::ProcessingDataPlugin::colorFor(type).c_str();
		QColor color(colorstring);
		if (color.isValid()) return color;
		return colorPort();
	}

	void addProcessingBox(const QString & name, CLAM::Processing * processing, QPoint point=QPoint(), QSize size=QSize())
	{
		if (!processing)
			_processings.push_back(new ProcessingBox(this, name, 1, 1, 0, 0));
		else
			_processings.push_back(new ProcessingBox(this, name, 0, 0, 0, 0));
		_processings.back()->setProcessing(processing);
		_processings.back()->move(point);
		_processings.back()->resize(size);
	}
protected:
	bool canConnectPorts(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		if (networkIsDummy()) return true;
		QString outName = source->getName() + "." + source->getOutportName(outlet);
		QString inName = target->getName() + "." + target->getInportName(inlet);
		CLAM::OutPortBase & out = _network->GetOutPortByCompleteName(outName.toStdString());
		CLAM::InPortBase & in = _network->GetInPortByCompleteName(inName.toStdString());
		return out.IsConnectableTo(in);
	}
	bool canConnectControls(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet)
	{
		if (networkIsDummy()) return true;
		return true;
	}
	virtual bool networkAddPortConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->ConnectPorts(outlet.toStdString(), inlet.toStdString());
	}
	virtual bool networkAddControlConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->ConnectControls(outlet.toStdString(), inlet.toStdString());
	}
	virtual bool networkRemovePortConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->DisconnectPorts(outlet.toStdString(), inlet.toStdString());
	}
	virtual bool networkRemoveControlConnection(const QString & outlet, const QString & inlet)
	{
		if (networkIsDummy()) return true;
		return _network->DisconnectControls(outlet.toStdString(), inlet.toStdString());
	}
	virtual void networkRemoveProcessing(const std::string & name)
	{
		if (networkIsDummy()) return;
		_network->RemoveProcessing(name);
	}

public:
	bool networkIsDummy() const
	{
		return _network == 0;
	}
	void loadNetwork(CLAM::Network * network)
	{
		clear();
		_network = network;
		clearChanges();
		if (networkIsDummy()) return;
		CLAM::Network::ProcessingsMap::const_iterator it;
		for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
		{
			const std::string & name = it->first;
			CLAM::Processing * processing = it->second;
			addProcessingBox( name.c_str(),  processing );
		}
		refreshWires();
	}

	void refreshWires()
	{
		clearWires();
		// TODO: Refactor this code please!!!
		for (unsigned p = 0; p<_processings.size(); p++)
		{
			std::string producerName = _processings[p]->getName().toStdString();
			CLAM::Processing & producer = _network->GetProcessing(producerName);
			CLAM::OutPortRegistry & outPorts = producer.GetOutPorts();
			for (int op = 0; op<outPorts.Size(); op++)
			{
				CLAM::OutPortBase & outPort = outPorts.GetByNumber(op);
				std::string completeOutName = producerName + "." + outPort.GetName();
				CLAM::Network::NamesList connected = _network->GetInPortsConnectedTo( completeOutName );
				CLAM::Network::NamesList::iterator inName;
				for(inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?

					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					CLAM::InPortRegistry & inPorts = consumer.GetInPorts();
					for (int ip=0; ip<inPorts.Size(); ip++)
					{
						CLAM::InPortBase & inPort = inPorts.GetByNumber(ip);
						if (inPort.GetName()!=peerConnection) continue;
						addPortWire(_processings[p], op, consumerBox, ip);
						break;
					}
				}
			}
			CLAM::OutControlRegistry & outControls = producer.GetOutControls();
			for (int op = 0; op<outControls.Size(); op++)
			{
				CLAM::OutControl & outControl = outControls.GetByNumber(op);
				std::string completeOutName = producerName + "." + outControl.GetName();
				CLAM::Network::NamesList connected = _network->GetInControlsConnectedTo( completeOutName );
				CLAM::Network::NamesList::iterator inName;
				for(inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?

					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					CLAM::InControlRegistry & inControls = consumer.GetInControls();
					for (int ip=0; ip<inControls.Size(); ip++)
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

	static quint32 positionFileMagicNumber() { return 0x01026420; }

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
	virtual bool networkRenameProcessing(QString oldName, QString newName)
	{
		if (networkIsDummy()) return true;
		return _network->RenameProcessing(oldName.toStdString(), newName.toStdString());
	}
private slots:
	void onCopyConnection()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			QString toCopy;
			switch (region)
			{
				case ProcessingBox::outportsRegion: 
					toCopy = _processings[i]->getOutportPrototyperName(point);
					break;
				case ProcessingBox::inportsRegion: 
					toCopy = _processings[i]->getInportPrototyperName(point);
					break;
				case ProcessingBox::incontrolsRegion: 
					toCopy = _processings[i]->getIncontrolPrototyperName(point);
					break;
				case ProcessingBox::outcontrolsRegion: 
					toCopy = _processings[i]->getOutcontrolPrototyperName(point);
					break;
				default:
					return; // Matches a region but not a connector one
			}
			QApplication::clipboard()->setText(toCopy);
			return;
		}
	}
	void onAddSlider()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			if (region!=ProcessingBox::incontrolsRegion) return;

			addControlSenderProcessing(_processings[i], point);
			return;
		}
	}
	void onAddMonitor()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString monitorType = ((QAction*)sender())->text();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			if (region!=ProcessingBox::outportsRegion) return;

			addPortMonitorProcessing(_processings[i], point, monitorType);
			return;
		}
	}
	void onConfigure()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			if ( !_processings[i]->configure()) return;
			if (_network) refreshWires();
			markAsChanged();
			return;
		}
	}
	void onRename()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			if ( !_processings[i]->rename()) return;
			markAsChanged();
			return;
		}
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
				default:
					CLAM_ASSERT(false, "Not a port to disconnect.");
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
		QString type;
		if (!_network)
		{
			type = QInputDialog::getText(this, "Type", "Type", QLineEdit::Normal, "A Processing");
			if (type.isNull()) return;
		}
		else
		{
			QStringList completionList;
			typedef CLAM::ProcessingFactory::Keys FactoryKeys;
			FactoryKeys keys = CLAM::ProcessingFactory::GetInstance().GetKeys();
			for (FactoryKeys::const_iterator it=keys.begin(); it!=keys.end(); it++)
				completionList << QString::fromStdString(*it);
			QDialog dialog;
			dialog.setWindowTitle(tr("Adding a new processing"));
			QVBoxLayout * layout = new QVBoxLayout(&dialog);
			QLineEdit * lineEdit = new QLineEdit(&dialog);
			layout->addWidget(new QLabel(tr("Type"), &dialog));
			QCompleter *completer = new QCompleter(completionList, &dialog);
			completer->setCaseSensitivity(Qt::CaseInsensitive);
			lineEdit->setCompleter(completer);
			layout->addWidget(lineEdit);
			QDialogButtonBox * buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
			connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
			connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
			layout->addWidget(buttons);
			int result = dialog.exec();
			if (result==QDialog::Rejected) return;
			type = lineEdit->text();
			if (type.isEmpty()) return;
		}
		addProcessing(point, type);
	}
private:
	CLAM::Network * _network;
};



#endif//NetworkCanvas_hxx

