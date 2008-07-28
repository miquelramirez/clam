#ifndef NetworkCanvas_hxx
#define NetworkCanvas_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QInputDialog>
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
#include <CLAM/Assert.hxx>
#include <CLAM/XMLStorage.hxx>
#include <iostream>




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
		SelectionDrag
	};
	NetworkCanvas(QWidget * parent=0)
		: QWidget(parent)
		, _zoomFactor(1.)
		, _changed(false)
		, _dragStatus(NoDrag)
		, _dragProcessing(0)
		, _dragConnection(0)
		, _colorBoxErrorFrame     (0xff,0x00,0x00,0xa0)
		, _colorBoxErrorBody      (0xff,0x00,0x00,0xa0)
		, _colorPortOutline       (0x53,0x30,0x42)
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
		setGreenColorsForBoxes();

		setMouseTracking(true);
		setAcceptDrops(true);
		setMinimumSize(200,100);
		resize(600,300);
	   	// Overwritten latter. But some text is needed to enable it.
		setWhatsThis("Dummy");

		_deleteSelectedAction = new QAction(QIcon(":/icons/images/editdelete.png"),tr("Delete"), this);
		_deleteSelectedAction->setShortcut(QKeySequence(tr("Del")));
		addAction(_deleteSelectedAction);
		connect(_deleteSelectedAction, SIGNAL(triggered()), this, SLOT(removeSelectedProcessings()));

		_selectAllAction = new QAction(tr("Select all"), this);
		_selectAllAction->setShortcut(QKeySequence(tr("Ctrl+A")));
		addAction(_selectAllAction);
		connect(_selectAllAction, SIGNAL(triggered()), this, SLOT(onSelectAll()));

		_clearSelectionAction = new QAction(tr("Clear selection"), this);
		_clearSelectionAction->setShortcut(QKeySequence(tr("Ctrl+Shift+A")));
		addAction(_clearSelectionAction);
		connect(_clearSelectionAction, SIGNAL(triggered()), this, SLOT(onClearSelections()));
	}
	void setGreenColorsForBoxes()
	{
		_colorBoxFrameText = QColor(0xff,0xff,0xff);
		_colorBoxFrameOutline = QColor(0x20,0x6f,0x20);
		_colorBoxFrame = QColor(0x30,0x8f,0x30,0xaf);
		_colorBoxBody = QColor(0xF9,0xFb,0xF9,0xaf);
		_colorResizeHandle = QColor(0xf9,0xbb,0xb9);
		_colorPort = QColor(0xa6,0x60,0x84);
	}
	void setWhiteColorsForBoxes()
	{
		_colorBoxFrameText = QColor(0x00,0x00,0x00);
		_colorBoxFrameOutline = QColor(0x00,0x00,0x00);
		_colorBoxFrame = QColor(0xf0,0xf0,0xf0);
		_colorBoxBody = QColor(0xd0,0xd0,0xd0);
		_colorResizeHandle = QColor(0xd0,0xf0,0xd0);
		_colorPort = QColor(0xd0,0x50,0xa0);
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
protected:
	/// Given a connector region it return the complementary one.
	/// Returns noRegion if the region is not a connector region.
	ProcessingBox::Region peerRegion(ProcessingBox::Region region)
	{
		switch (region)
		{
			case ProcessingBox::outcontrolsRegion:
				return ProcessingBox::incontrolsRegion;
			case ProcessingBox::incontrolsRegion:
				return ProcessingBox::outcontrolsRegion;
			case ProcessingBox::outportsRegion:
				return ProcessingBox::inportsRegion;
			case ProcessingBox::inportsRegion:
				return ProcessingBox::outportsRegion;
			default:
				return ProcessingBox::noRegion;
		}
	}
	/// Being region a connector region, it returns the number of connectors the processing has in it.
	/// @todo: move this method to the ProcessingBox
	unsigned nConnectors(void * processing, ProcessingBox::Region region)
	{
		switch (region)
		{
			case ProcessingBox::outcontrolsRegion:
				return nOutcontrols(processing);
			case ProcessingBox::incontrolsRegion:
				return nIncontrols(processing);
			case ProcessingBox::outportsRegion:
				return nOutports(processing);
			case ProcessingBox::inportsRegion:
				return nInports(processing);
			default:
				return 0;
		}
	}
	/// Being region a connector region, it returns the name of connector in processing at position index.
	/// @todo: move this method to the ProcessingBox
	QString connectorName(void * processing, ProcessingBox::Region region, unsigned index)
	{
			switch (region)
			{
				case ProcessingBox::outcontrolsRegion:
					return outcontrolName(processing, index);
				case ProcessingBox::incontrolsRegion:
					return incontrolName(processing, index);
				case ProcessingBox::outportsRegion:
					return outportName(processing, index);
				case ProcessingBox::inportsRegion:
					return inportName(processing, index);
				default:
					// it should never reach this point
					return "Error";
			}
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
	const QPixmap getPixmapOfCanvas(Qt::TransformationMode mode = Qt::FastTransformation)
	{
		QPixmap pixmap(size());
		QPainter painter;
		painter.begin(&pixmap);
		paint(painter);
		painter.end();
		// if greater than 800x800, rescale it
		if (size()!=size().boundedTo(QSize(800,800)))
			return pixmap.scaled(800,800,Qt::KeepAspectRatio, mode);
		return pixmap;
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
	// get a bounded QRect of selection:
	virtual QRect getSelectionBoundingRect()
	{
		QRect boundingBox;
		for (unsigned i = 0; i<_processings.size(); i++)
			if (_processings[i]->isSelected())
				boundingBox = boundingBox.unite(QRect(_processings[i]->pos(),_processings[i]->size()));
		return boundingBox;
	}

public:
	virtual bool networkRenameProcessing(QString oldName, QString newName)=0;
	virtual void networkRemoveProcessing(const std::string & name) = 0;
	virtual void addProcessing(QPoint point, QString type) = 0;
	virtual bool canConnectPorts(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet) = 0;
	virtual bool canConnectControls(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet) = 0;
	virtual bool networkAddPortConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkAddControlConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkRemovePortConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkRemoveControlConnection(const QString & outlet, const QString & inlet) = 0;

	virtual unsigned nInports(void * processing) = 0;
	virtual unsigned nOutports(void * processing) = 0;
	virtual unsigned nIncontrols(void * processing) = 0;
	virtual unsigned nOutcontrols(void * processing) = 0;
	virtual QColor inportColor(void * processing, unsigned index) const = 0;
	virtual QColor outportColor(void * processing, unsigned index) const = 0;
	virtual QString inportName(void * processing, unsigned index) const = 0;
	virtual QString outportName(void * processing, unsigned index) const = 0;
	virtual QString incontrolName(void * processing, unsigned index) const = 0;
	virtual QString outcontrolName(void * processing, unsigned index) const = 0;
	virtual QString inportTooltip(void * processing, unsigned index) const = 0;
	virtual QString outportTooltip(void * processing, unsigned index) const = 0;
	virtual QString incontrolTooltip(void * processing, unsigned index) const = 0;
	virtual QString outcontrolTooltip(void * processing, unsigned index) const = 0;
	virtual bool isOk(void * processing)=0;
	virtual QString errorMessage(void * processing)=0;
	virtual QWidget * embededWidgetFor(void * processing) = 0;

	// TODO: Are those generic enough to be virtual?
	virtual bool editConfiguration(ProcessingBox * box) = 0;
	virtual void addControlSenderProcessing( ProcessingBox * processing, QPoint point ) = 0;
	virtual void addControlPrinterProcessing( ProcessingBox * processing, QPoint point ) = 0;

	virtual void connectionContextMenu(QMenu * menu, QContextMenuEvent * event, ProcessingBox * processing, ProcessingBox::Region region) { }
	virtual void processingContextMenu(QMenu * menu, QContextMenuEvent * event, ProcessingBox * processing) { }
	virtual void canvasContextMenu(QMenu * menu, QContextMenuEvent * event) { }


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
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(translatedPos(event));
			switch (region)
			{
				case ProcessingBox::inportsRegion:
				case ProcessingBox::outportsRegion:
				case ProcessingBox::incontrolsRegion:
				case ProcessingBox::outcontrolsRegion:
					connectionContextMenu(&menu, event, _processings[i], region);
					menu.exec(event->globalPos());
				return;

				case ProcessingBox::nameRegion:
				case ProcessingBox::bodyRegion:
				case ProcessingBox::resizeHandleRegion:
					if (not _processings[i]->isSelected())
					{
						std::cout << "updating selection on context menu" << std::endl;
						if (! (event->modifiers() & Qt::ControlModifier) )
							clearSelections();
						_processings[i]->select();
						update();
					}
					processingContextMenu(&menu, event, _processings[i]);
					menu.exec(event->globalPos());
				return;

				default: continue;
			}
		}
		canvasContextMenu(&menu, event);
		menu.exec(event->globalPos());
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


class JackNetworkCanvas : public NetworkCanvas
{
	Q_OBJECT
public:
	JackNetworkCanvas(QWidget * parent=0)
		: NetworkCanvas(parent)
	{
	}
};

#include <typeinfo>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingDataPlugin.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include "OutControlSender.hxx"
#include "Configurator.hxx"
#include "uic_DummyProcessingConfig.hxx"


class ClamNetworkCanvas : public NetworkCanvas
{
	Q_OBJECT
public:
	ClamNetworkCanvas(QWidget * parent=0)
		: NetworkCanvas(parent)
		, _network(0)
		, _fileNameToOpen(0)
	{
		setWindowState(windowState() ^ Qt::WindowFullScreen);

		_newProcessingAction = new QAction(tr("&New Processing"), this);
		_newProcessingAction->setShortcut(QKeySequence(tr("Ctrl+Space")));
		addAction(_newProcessingAction);
		connect(_newProcessingAction, SIGNAL(triggered()), this, SLOT(onNewProcessing()));

		_copySelectionAction = new QAction(QIcon(":/icons/images/editcopy.png"), tr("&Copy"), this);
		_copySelectionAction->setShortcut(QKeySequence(tr("Ctrl+C")));
		addAction(_copySelectionAction);
		connect(_copySelectionAction, SIGNAL(triggered()), this, SLOT (onCopyProcessingsToClipboard()));

		_cutSelectionAction = new QAction(QIcon(":/icons/images/editcut.png"), tr("Cu&t"), this);
		_cutSelectionAction->setShortcut(QKeySequence(tr("Ctrl+X")));
		addAction(_cutSelectionAction);
		connect(_cutSelectionAction, SIGNAL(triggered()), this, SLOT (onCutProcessingsToClipboard()));

		_pasteSelectionAction = new QAction(QIcon(":/icons/images/editcopy.png"), tr("&Paste"), this);
		_pasteSelectionAction->setShortcut(QKeySequence(tr("Ctrl+V")));
		addAction(_pasteSelectionAction);
		connect(_pasteSelectionAction, SIGNAL(triggered()), this, SLOT (onPasteProcessingsFromClipboard()));
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

	virtual ~ClamNetworkCanvas();

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
	bool editConfiguration(ProcessingBox * box)
	{
		CLAM::Processing * processing = (CLAM::Processing *) box->model();
		CLAM::ProcessingConfig * config = (CLAM::ProcessingConfig*) processing->GetConfig().DeepCopy();
		Configurator configurator(*config);
		if (!configurator.exec())
		{
			delete config;
			return false;
		}
		network().ConfigureProcessing( box->getName().toStdString(), *config);	
		box->setProcessing(processing);
		delete config;
		return true;
	}
	float incontrolDefaultValue(CLAM::Processing * processing, unsigned index) const //TODO remove
	{
		CLAM::InControl& inControl = processing->GetInControls().GetByNumber(index);
		return inControl.DefaultValue();
	}
	float incontrolLowerBound(CLAM::Processing * processing, unsigned index) const
	{
		CLAM::InControl& inControl = processing->GetInControls().GetByNumber(index);
		return inControl.LowerBound();
	}
	float incontrolUpperBound(CLAM::Processing * processing, unsigned index) const
	{
		CLAM::InControl& inControl = processing->GetInControls().GetByNumber(index);
		return inControl.UpperBound();
	}

	virtual void addControlSenderProcessing( ProcessingBox * processing, QPoint point )
	{
		if (networkIsDummy()) return;

		unsigned controlIndex = processing->controlIndexByXPos(point);
		QString inControlName = processing->getIncontrolName(controlIndex);
		CLAM::Processing * model = (CLAM::Processing*) processing->model();
		float defaultValue = incontrolDefaultValue(model,controlIndex);
		float lower = incontrolLowerBound(model,controlIndex);
		float upper = incontrolUpperBound(model,controlIndex);

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

	virtual void addControlPrinterProcessing( ProcessingBox * processing, QPoint point )
	{
		if (networkIsDummy()) return;

		unsigned controlIndex = processing->controlIndexByXPos(point);
		QString outControlName = processing->getOutcontrolName(controlIndex);
		std::string controlPrinterName  =  outControlName.toStdString();
		// add Control Printer processing to network
		if (_network->HasProcessing(controlPrinterName) )
		{
			controlPrinterName = _network->GetUnusedName(controlPrinterName);
		}
		_network->AddProcessing( controlPrinterName, "ControlPrinter");
		CLAM::Processing & controlPrinter = _network->GetProcessing( controlPrinterName );
		// add box to canvas and connect
		addProcessingBox( controlPrinterName.c_str(), &controlPrinter, point+QPoint(0,100));
		addControlConnection( processing, controlIndex, getBox(controlPrinterName.c_str()), 0 );

		markAsChanged();
	}
	
	void addLinkedProcessingReceiver( ProcessingBox * processing, QPoint point, const QString & processingType, unsigned nInPort=0 )
	{
		if (networkIsDummy()) return;

		unsigned portIndex = processing->portIndexByYPos(point);
		QString outPortName = processing->getOutportName(portIndex);

		std::string processingId = _network->AddProcessing(processingType.toStdString() );
		CLAM::Processing & portProcessing = _network->GetProcessing( processingId );
		// add box to canvas and connect
		addProcessingBox( processingId.c_str(), &portProcessing, point+QPoint(100,0));
		addPortConnection(processing, portIndex, getBox(processingId.c_str()), nInPort);

		markAsChanged();
	}

	void addLinkedProcessingSender( ProcessingBox * processing, QPoint point, const QString & processingType, unsigned nOutPort=0 )
	{
		if (networkIsDummy()) return;

		unsigned portIndex = processing->portIndexByYPos(point);
		QString inPortName = processing->getInportName(portIndex);

		std::string processingId = _network->AddProcessing(processingType.toStdString() );
		CLAM::Processing & portProcessing = _network->GetProcessing( processingId );
		// add box to canvas and connect
		addProcessingBox( processingId.c_str(), &portProcessing, point+QPoint(-200,0));
		addPortConnection(getBox(processingId.c_str()), nOutPort, processing, portIndex);
		markAsChanged();
	}


	virtual QWidget * embededWidgetFor(void * processing);
	virtual unsigned nInports(void * processing) { return ((CLAM::Processing*)processing)->GetInPorts().Size();}
	virtual unsigned nOutports(void * processing) { return ((CLAM::Processing*)processing)->GetOutPorts().Size();}
	virtual unsigned nIncontrols(void * processing) { return ((CLAM::Processing*)processing)->GetInControls().Size();}
	virtual unsigned nOutcontrols(void * processing) { return ((CLAM::Processing*)processing)->GetOutControls().Size();}
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
	QString outportTooltip(void * processing, unsigned index) const
	{
		if (!processing) return outportName(processing, index);
		CLAM::OutPortBase & port = ((CLAM::Processing*)processing)->GetOutPorts().GetByNumber(index);
		const char * typeString = CLAM::ProcessingDataPlugin::displayNameFor(port.GetTypeId()).c_str();
		return QObject::tr("%1\nType: %3","Outport tooltip")
			.arg(port.GetName().c_str())
			.arg(typeString)
			;
	}
	QString inportTooltip(void * processing, unsigned index) const
	{
		if (!processing) return inportName(processing, index);
		CLAM::InPortBase & port = ((CLAM::Processing*)processing)->GetInPorts().GetByNumber(index);
		const char * typeString = CLAM::ProcessingDataPlugin::displayNameFor(port.GetTypeId()).c_str();
		return QObject::tr("%1\nType: %3","Inport tooltip")
			.arg(port.GetName().c_str())
			.arg(typeString)
			;
	}
	QString outcontrolTooltip(void * processing, unsigned index) const
	{
		return outcontrolName(processing, index);
	}
	QString incontrolTooltip(void * processing, unsigned index) const
	{
		CLAM::InControl& inControl = ((CLAM::Processing*)processing)->GetInControls().GetByNumber(index);
		QString boundInfo = inControl.IsBounded() ? 
			QString(" (bounds: [%1, %2] )").arg(inControl.LowerBound()).arg(inControl.UpperBound()) :
			" (not bounded)";
		return incontrolName(processing, index)+boundInfo;
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
	virtual bool networkRenameProcessing(QString oldName, QString newName)
	{
		if (networkIsDummy()) return true;
		return _network->RenameProcessing(oldName.toStdString(), newName.toStdString());
	}


signals:
	void openFileWithExternalApplicationRequest();
public:
	void requestOpenFileWithExternalApplication (const QString & fileName )
	{
		_fileNameToOpen.clear();
		_fileNameToOpen=fileName;
		emit openFileWithExternalApplicationRequest();
	}
	const QString getFileNameToOpenWithExternalApplication () const
	{
		QString copyFileNameToOpen = _fileNameToOpen;
		_fileNameToOpen.clear();
		return copyFileNameToOpen;
		//return _fileNameToOpen;
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
		QString inName = target->getName() + "." + inportName(target->model(),inlet);
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


	void reloadNetwork(bool selectAllNew=false)
	{
		if (networkIsDummy()) return;
		CLAM::Network::ProcessingsMap::const_iterator it;
		for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
		{
			const std::string & name = it->first;
			if (getBox(QString(name.c_str())))
				continue; // if the processing exists in canvas, skip it
			CLAM::Processing * processing = it->second;
			addProcessingBox( name.c_str(),  processing );
			if (selectAllNew)
				getBox(name.c_str())->select();
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

	bool updateGeometriesOnXML(QPoint offsetPoint=QPoint(0,0))
	{
		CLAM::Network::ProcessingsGeometriesMap processingsGeometriesMap;
		for (unsigned i=0; i<_processings.size(); i++)
		{
			CLAM::Network::Geometry processingGeometry;
			QPoint position = _processings[i]->pos()-offsetPoint;
			QSize size = _processings[i]->size();
			const std::string name=_processings[i]->getName().toStdString();
			processingGeometry.x=position.x();
			processingGeometry.y=position.y();
			processingGeometry.width=size.width();
			processingGeometry.height=size.height();
			processingsGeometriesMap.insert(CLAM::Network::ProcessingsGeometriesMap::value_type(name,processingGeometry));
		}
		return (_network->SetProcessingsGeometries(processingsGeometriesMap));
	}
	bool loadGeometriesFromXML(QPoint offsetPoint = QPoint(0,0))
	{
		const CLAM::Network::ProcessingsGeometriesMap & processingsGeometriesMap=_network->GetAndClearGeometries();
		if (processingsGeometriesMap.empty())
			return 0;
		CLAM::Network::ProcessingsGeometriesMap::const_iterator it;
		for(it=processingsGeometriesMap.begin();it!=processingsGeometriesMap.end();it++)
		{
			QString name=QString(it->first.c_str());
			const CLAM::Network::Geometry & geometry=it->second;
			QPoint position=offsetPoint+QPoint(geometry.x,geometry.y);
			QSize size=QSize(geometry.width,geometry.height);
			ProcessingBox * box=getBox(name);
			box->move(position);
			box->resize(size);
		}
		return 1;
	}

	void setEmbedSVGDiagramsOption(bool state)
	{
		_embedSVGDiagramsOption=state;
	}

	bool getEmbedSVGDiagramsOption()
	{
		return _embedSVGDiagramsOption;
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

	void onCopyProcessingsToClipboard(bool cut=false)
	{
		std::ostringstream streamXMLBuffer;
		CLAM::Network::NamesList processingsNamesList;
		// Copy selected processings on networkToCopy
		for (unsigned i=0; i<_processings.size();i++)
		{
			if (!_processings[i]->isSelected())
				continue;
			const std::string name=(_processings[i]->getName()).toStdString();
			processingsNamesList.push_back(name);
		}
		if (_network->UpdateSelections(processingsNamesList))
			return;
		updateGeometriesOnXML(getSelectionBoundingRect().topLeft());
		CLAM::XmlStorage::Dump(*_network,"network",streamXMLBuffer);

		QApplication::clipboard()->setText(QString(streamXMLBuffer.str().c_str()));
		if (!cut) return;

		CLAM::Network::NamesList::iterator cuttedNamesIterator;
		for(cuttedNamesIterator=processingsNamesList.begin();
			cuttedNamesIterator!=processingsNamesList.end();
			cuttedNamesIterator++)
		{
			removeProcessing(getBox(QString((*cuttedNamesIterator).c_str())));
		}
	}

	void onCutProcessingsToClipboard()
	{
		onCopyProcessingsToClipboard(true);
	}

	void onPasteProcessingsFromClipboard()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString text=QApplication::clipboard()->text();
		std::stringstream streamXMLBuffer;
		streamXMLBuffer << text.toStdString();

		try
		{
			_network->setPasteMode();
			CLAM::XMLStorage::Restore(*_network, streamXMLBuffer);
		}
		catch(CLAM::XmlStorageErr &e)
		{
			QMessageBox::critical(this, tr("Error pasting from clipboard"), 
					tr("<p>An error occurred while pasting clipboard content.<p>"
						"<p><b>%1</b></p>").arg(e.what()));
			return;
		}
		clearSelections();
		bool selectAllNew=true;
		reloadNetwork(selectAllNew);
		loadGeometriesFromXML(point);
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

	void onAddControlPrinter()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			if (region!=ProcessingBox::outcontrolsRegion) return;

			addControlPrinterProcessing(_processings[i], point);
			return;
		}
	}

	void onAddLinkedProcessing()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString processingType = ((QAction*)sender())->text();
		for (unsigned i = _processings.size();i--;)
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			switch(region)
			{
			case ProcessingBox::inportsRegion:
				addLinkedProcessingSender(_processings[i], point, processingType);
				break;
			case ProcessingBox::outportsRegion:
				addLinkedProcessingReceiver(_processings[i], point, processingType);
				break;
			default:
				continue; // Not a port connections region
			}
			return;
		}
	}

	void onProcessingsConnectTo()
	{
		QMap<QString, QVariant> receiveMap=((QAction*)sender())->data().toMap();
		ProcessingBox * sourceBox = getBox(receiveMap["outprocessing"].toString());
		ProcessingBox * targetBox = getBox(receiveMap["inprocessing"].toString());
		unsigned sourceConnector = receiveMap["outlet"].toUInt();
		unsigned targetConnector = receiveMap["inlet"].toUInt();
		bool isPort = receiveMap["isPort"].toBool();
		if (isPort)
			addPortConnection(sourceBox,sourceConnector,targetBox,targetConnector);
		else
			addControlConnection(sourceBox,sourceConnector,targetBox,targetConnector);
		update();
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
			this->activateWindow();
			if (result==QDialog::Rejected) return;
			type = lineEdit->text();
			if (type.isEmpty()) return;
		}
		addProcessing(point, type);
	}

	void onOpenFileWithExternalApplication()
	{
		const QString fileName = ((QAction*)sender())->data().toString();
		requestOpenFileWithExternalApplication(fileName);
	}


private:
	std::string outportTypeId(void * processing, unsigned index) const
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetOutPorts().GetByNumber(index).GetTypeId().name();
	}

	std::string inportTypeId(void * processing, unsigned index) const
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetInPorts().GetByNumber(index).GetTypeId().name();
	}


	virtual void connectionContextMenu(QMenu * menu, QContextMenuEvent * event, ProcessingBox * processing, ProcessingBox::Region region)
	{
		QPoint cursorPosition = translatedPos(event);
		menu->addAction(QIcon(":/icons/images/remove.png"), tr("Disconnect"),
			this, SLOT(onDisconnect()))->setData(cursorPosition);
		menu->addAction(QIcon(":/icons/images/editcopy.png"), tr("Copy connection name"),
			this, SLOT(onCopyConnection()))->setData(cursorPosition);
		if (region==ProcessingBox::incontrolsRegion)
		{
			menu->addAction(QIcon(":/icons/images/hslider.png"), tr("Add slider"),
				this, SLOT(onAddSlider()))->setData(cursorPosition);
		}
		if (region==ProcessingBox::outcontrolsRegion)
		{
			menu->addAction(QIcon(":/icons/images/processing.png"), tr("Add control printer"),
				this, SLOT(onAddControlPrinter()))->setData(cursorPosition);
		}
		if (region==ProcessingBox::outportsRegion)
		{
			typedef CLAM::ProcessingFactory::Keys Keys;
			menu->addSeparator();
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			unsigned portindex = processing->portIndexByYPos(cursorPosition);
			std::string outportType = outportTypeId(processing->model(),portindex);
			Keys keys = factory.GetKeys("port_monitor_type", outportType);
			for (Keys::const_iterator it=keys.begin(); it!=keys.end(); it++)
			{
				const char* key = it->c_str();
				menu->addAction( clamProcessingIcon(key), key,
					this, SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
			}
			if (outportType==typeid(CLAM::TData).name()) // Check for audio port
			{
				const char* key="AudioSink";
				menu->addAction( clamProcessingIcon(key), key,
					this, SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
			}
		}
		if (region==ProcessingBox::inportsRegion)
		{
			unsigned portindex = processing->portIndexByYPos(cursorPosition);
			std::string inportType=inportTypeId(processing->model(),portindex);

			if (inportType==typeid(CLAM::TData).name()) // Check for audio port
			{
				const char* key="AudioSource";
				menu->addAction( clamProcessingIcon(key), key,
					this ,SLOT(onAddLinkedProcessing()))->setData(cursorPosition);
			}
		}
		// Check for compatible connectors and add "Connect To" submenus
		{
			bool isPortConnection = region == ProcessingBox::outportsRegion || region == ProcessingBox::inportsRegion;
			bool selfIsInput = region == ProcessingBox::incontrolsRegion || region == ProcessingBox::inportsRegion;

			// Self reference connector
			ProcessingBox * selfProcessing = processing; // Just an alias to be symmetric to 'peer'
			unsigned selfConnection = isPortConnection ?
					selfProcessing->portIndexByYPos(cursorPosition):
					selfProcessing->controlIndexByXPos(cursorPosition);

			// Peer: To be given values along the bucle
			ProcessingBox * peerProcessing;
			unsigned peerConnection;

			// Choosing in/out in respect of self/peer
			ProcessingBox ** inProcessing  = selfIsInput? &selfProcessing : &peerProcessing;
			ProcessingBox ** outProcessing = selfIsInput? &peerProcessing : &selfProcessing;
			unsigned * inlet  = selfIsInput ? &selfConnection : &peerConnection;
			unsigned * outlet = selfIsInput ? &peerConnection : &selfConnection;
			
			menu->addSeparator();
			for (unsigned i = 0; i<_processings.size(); i++ )
			{
				peerProcessing =_processings[i];
				if (peerProcessing==selfProcessing) continue; // TODO: Loops not allowed by now

				typedef QMap<QString, QVariant> ConnectionMap;
				QList<ConnectionMap> listConnectionsMap;
				unsigned nPeerConnectors = nConnectors(peerProcessing->model(), peerRegion(region));
				for (peerConnection=0; peerConnection<nPeerConnectors; peerConnection++)
				{
					// Filter out incompatible connections
					if (isPortConnection and not canConnectPorts(*outProcessing, *outlet, *inProcessing, *inlet))
						continue;
					if (not isPortConnection and not canConnectControls(*outProcessing, *outlet, *inProcessing, *inlet))
						continue;

					QString peerConnectionName=connectorName(peerProcessing->model(), peerRegion(region), peerConnection);
					ConnectionMap connectionMap;
					connectionMap["outprocessing"]=(*outProcessing)->getName();	// QString
					connectionMap["inprocessing"]=(*inProcessing)->getName();	// QString
					connectionMap["outlet"]=*outlet;			//unsigned
					connectionMap["inlet"]=*inlet;			//unsigned
					connectionMap["isPort"]=isPortConnection;			//bool
					connectionMap["connectionName"]=peerConnectionName;		//QString
					listConnectionsMap.push_back(connectionMap);
				}

				if (listConnectionsMap.empty())
					continue; // no compatible connections, skip
				QIcon icon = processingIcon(peerProcessing);
				if (listConnectionsMap.size()==1) // one compatible connection
				{
					ConnectionMap connectionMap = listConnectionsMap.front();
					QString connectionName=connectionMap["connectionName"].toString();
					menu->addAction(icon,
						tr("Connect to %1.%2")
							.arg(peerProcessing->getName())
							.arg(connectionName),
						this, SLOT(onProcessingsConnectTo()))->setData(connectionMap);
					continue;
				}
				//more than one compatible connection:
				QMenu * submenu=menu->addMenu(icon,
					tr("Connect to %1").arg(peerProcessing->getName()));
				QList<ConnectionMap>::const_iterator itConnectionsMaps=listConnectionsMap.constBegin();
				while (itConnectionsMaps!=listConnectionsMap.constEnd())
				{
					ConnectionMap connectionMap= *itConnectionsMaps;
					QString connectionName=connectionMap["connectionName"].toString();
					submenu->addAction(connectionName,
						this, SLOT(onProcessingsConnectTo()))->setData(connectionMap);
					itConnectionsMaps++;
				}
			}
		}
	}
	virtual void processingContextMenu(QMenu * menu, QContextMenuEvent * event, ProcessingBox * processing)
	{
		menu->addAction(QIcon(":/icons/images/configure.png"), tr("Configure"),
			this, SLOT(onConfigure()))->setData(translatedPos(event));
		menu->addAction(QIcon(":/icons/images/editclear.png"), tr("Rename"),
			this, SLOT(onRename()))->setData(translatedPos(event));
		menu->addAction(_deleteSelectedAction);
		menu->addAction(_copySelectionAction);
		menu->addAction(_cutSelectionAction);

		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		std::string className=((CLAM::Processing*)processing->model())->GetClassName();
		if (factory.AttributeExists(className,"faust_diagram"))
		{
			QString fileName=QString(factory.GetValueFromAttribute(className,"faust_diagram").c_str());
			menu->addSeparator();
			menu->addAction(clamProcessingIcon(className),"Open diagram with browser",this,SLOT(onOpenFileWithExternalApplication()))->setData(fileName);
		}
		if (factory.AttributeExists(className,"faust_source_file"))
		{
			QString fileName=QString(factory.GetValueFromAttribute(className,"faust_source_file").c_str());
			menu->addAction(clamProcessingIcon(className),"Open source with editor",this,SLOT(onOpenFileWithExternalApplication()))->setData(fileName);
			//menu->addAction(clamProcessingIcon(className),"Recompile plugin");
		}

	}
	virtual void canvasContextMenu(QMenu * menu, QContextMenuEvent * event)
	{
		_pasteSelectionAction->setData(translatedPos(event));
		menu->addAction(_pasteSelectionAction);
		_newProcessingAction->setData(translatedPos(event));
		menu->addAction(_newProcessingAction);
	}
	virtual QIcon processingIcon(ProcessingBox * processingBox)
	{
		const char* className=((CLAM::Processing*)processingBox->model())->GetClassName();
		return clamProcessingIcon(className);
	}
private:
	QIcon clamProcessingIcon(const std::string & className)
	{
		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		if (not factory.AttributeExists(className,"icon")) return QIcon();
		std::string iconPath=factory.GetValueFromAttribute(className,"icon");
		return QIcon( QString(":/icons/images/%1").arg(iconPath.c_str()));
	}



protected:
	QAction * _newProcessingAction;
	QAction * _copySelectionAction;
	QAction * _cutSelectionAction;
	QAction * _pasteSelectionAction;
private:
	CLAM::Network * _network;
	mutable QString _fileNameToOpen;
	bool _embedSVGDiagramsOption;
};

#endif//NetworkCanvas_hxx

