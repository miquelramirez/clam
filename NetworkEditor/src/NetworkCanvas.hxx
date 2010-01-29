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
#include "TextBox.hxx"
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <CLAM/Assert.hxx>
#include <iostream>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QResizeEvent>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>

class NetworkCanvas : public QGraphicsView
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
		: QGraphicsView(parent)
		, _changed(false)
		, _zoomFactor(1.)
		, _dragStatus(NoDrag)
		, _dragProcessing(0)
		, _dragConnection(0)
		, _dragBusMode(false)
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
		, _scene(0)
		, _maxZ(0)
		, _enabledViableConnectionMenu(false)
	{
		_scene=new QGraphicsScene(this);
		_scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
		setScene(_scene);
		setAlignment(Qt::AlignLeft | Qt::AlignTop);
		setRenderHint(QPainter::Antialiasing);
		setTransformationAnchor ( QGraphicsView::NoAnchor );
		viewport()->setStyleSheet("background-color: transparent;");
		
		setMouseTracking(true);
		setAcceptDrops(true);
		setMinimumSize(200,100);
		resize(600,300);
	   	// Overwritten latter. But some text is needed to enable it.
		setWhatsThis("Dummy");

		setGreenColorsForBoxes();

		_deleteSelectedAction = new QAction(QIcon(":/icons/images/editdelete.png"),tr("Delete"), this);
		_deleteSelectedAction->setShortcut(QKeySequence(tr("Del")));
		addAction(_deleteSelectedAction);
		connect(_deleteSelectedAction, SIGNAL(triggered()), this, SLOT(removeSelection()));

		_selectAllAction = new QAction(tr("Select all"), this);
		_selectAllAction->setShortcut(QKeySequence(tr("Ctrl+A")));
		addAction(_selectAllAction);
		connect(_selectAllAction, SIGNAL(triggered()), this, SLOT(onSelectAll()));

		_clearSelectionAction = new QAction(tr("Clear selection"), this);
		_clearSelectionAction->setShortcut(QKeySequence(tr("Ctrl+Shift+A")));
		addAction(_clearSelectionAction);
		connect(_clearSelectionAction, SIGNAL(triggered()), this, SLOT(onClearSelections()));
	}
	virtual ~NetworkCanvas();

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
		toRaise->raiseEmbededWidget();
		toRaise->setZValue(++_maxZ);
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
		{
 			_scene->removeItem(_processings[i]);
 			delete _processings[i];
		}
		_processings.clear();
		for (unsigned i = 0; i<_textBoxes.size(); i++)
		{
 			_scene->removeItem(_textBoxes[i]);
			delete _textBoxes[i];
		}
		_textBoxes.clear();
		update();
		_maxZ=0;
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

	void resizeEvent(QResizeEvent * event)
	{
		viewport()->resize(event->size());
		QGraphicsView::resizeEvent(event);
		updateEmbededWidgets();
	}
	void recomputeSceneRect()
	{
		QRect boundingBox(0,0,1,1);
		for (unsigned i = 0; i<_processings.size(); i++)
			boundingBox = boundingBox.unite(QRect(_processings[i]->position(),_processings[i]->size()));
		for (unsigned i = 0; i<_controlWires.size(); i++)
			_controlWires[i]->expand(boundingBox);
		for (unsigned i = 0; i<_portWires.size(); i++)
			_portWires[i]->expand(boundingBox);
		_scene->setSceneRect(boundingBox);
	}
	void paintEvent ( QPaintEvent * event )
	{
		recomputeSceneRect();
		QGraphicsView::paintEvent(event);
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
		if (_tooltipText.isEmpty()) return;
		QFontMetrics metrics(font());
		int margin =3;
		int cursorSize = 16;

		QPoint viewPortTopLeft=mapToScene(0,0).toPoint();
		QPoint viewPortBottomRight=mapToScene(viewport()->width(), viewport()->height()).toPoint();

		QRect boundingRect = metrics.boundingRect(QRect(0,0,width(),height()), Qt::AlignLeft, _tooltipText);
		double tooltipWidth = boundingRect.width()+2*margin;
		double x = _tooltipPos.x()+cursorSize;
		if (x + tooltipWidth > viewPortBottomRight.x())
			x = _tooltipPos.x() - tooltipWidth;
		if (x<viewPortTopLeft.x()) x=viewPortTopLeft.x();

		double tooltipHeight = boundingRect.height()+2*margin;
		double y = _tooltipPos.y() +cursorSize;
		if (y + tooltipHeight > viewPortBottomRight.y())
			y = _tooltipPos.y() - tooltipHeight;
		if (y<viewPortTopLeft.y()) y=viewPortTopLeft.y();

		QRectF tooltip(x, y, tooltipWidth, tooltipHeight)  ;
		painter.setBrush(_colorTooltipBody);
		painter.setPen(_colorTooltipLine);
		painter.drawRect(tooltip);
		painter.setPen(_colorTooltipText);
		painter.drawText(tooltip, Qt::AlignLeft, _tooltipText);
	}
	void drawForeground ( QPainter * painter, const QRectF & rect )
	{
		for (unsigned i = 0; i<_controlWires.size(); i++)
			if (_controlWires[i]->highlighted()) _controlWires[i]->draw(*painter);
		for (unsigned i = 0; i<_portWires.size(); i++)
			if (_portWires[i]->highlighted()) _portWires[i]->draw(*painter);
		if (_dragStatus==InportDrag)
		{
			unsigned n = _dragBusMode ? nInports(_dragProcessing->model())-_dragConnection : 1;
			for (unsigned i=0; i<n; i++)
				PortWire::draw(*painter, _dragPoint+QPoint(0,i*ProcessingBox::portStep), _dragProcessing->getInportPos(_dragConnection+i));
		}
		if (_dragStatus==OutportDrag)
		{
			unsigned n = _dragBusMode ? nOutports(_dragProcessing->model())-_dragConnection : 1;
			for (unsigned i=0; i<n; i++)
				PortWire::draw(*painter, _dragProcessing->getOutportPos(_dragConnection+i), _dragPoint+QPoint(0,i*ProcessingBox::portStep));
		}
		if (_dragStatus==IncontrolDrag)
		{
			unsigned n = _dragBusMode ? nIncontrols(_dragProcessing->model())-_dragConnection : 1;
			for (unsigned i=0; i<n; i++)
				ControlWire::draw(*painter, _dragPoint+QPoint(i*ProcessingBox::controlStep,0), _dragProcessing->getIncontrolPos(_dragConnection+i));
		}
		if (_dragStatus==OutcontrolDrag)
		{
			unsigned n = _dragBusMode ? nOutcontrols(_dragProcessing->model())-_dragConnection : 1;
			for (unsigned i=0; i<n; i++)
				ControlWire::draw(*painter, _dragProcessing->getOutcontrolPos(_dragConnection+i), _dragPoint+QPoint(i*ProcessingBox::controlStep,0));
		}

		drawSelectBox(*painter);
		drawTooltip(*painter);
	}
	void drawBackground ( QPainter * painter, const QRectF & rect )
	{
		for (unsigned i = 0; i<_controlWires.size(); i++)
			_controlWires[i]->draw(*painter);
		for (unsigned i = 0; i<_portWires.size(); i++)
			_portWires[i]->draw(*painter);
	}

public: // Helpers
	void setToolTip(const QString & text)
	{
		_tooltipText = text;
	}

	QRect translatedRect(const QRect & rect)
	{
		QPoint topLeft = mapFromScene(rect.topLeft())+viewport()->pos();
		QPoint bottomRight = mapFromScene(rect.bottomRight())+viewport()->pos();

		return QRect(topLeft, bottomRight);
	}
protected:
	/// Returns the processing under a given (scene) point.
	/// If more than one are available returns the top most one.
	ProcessingBox * processingUnder(const QPoint & point)
	{
		for (unsigned i = _processings.size(); i--; )
		{
			ProcessingBox::Region region = _processings[i]->getRegion(point);
			if (region==ProcessingBox::noRegion) continue;
			return _processings[i];
		}
		return 0;
	}
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
		setCursor(Qt::ArrowCursor);
		for (unsigned i=0; i<_processings.size(); i++)
			_processings[i]->deselect();
		for (unsigned i=0; i<_textBoxes.size(); i++)
			_textBoxes[i]->setSelected(false);
	}
	void selectAll()
	{
		for (unsigned i=0; i<_processings.size(); i++)
			_processings[i]->select();
		for (unsigned i=0; i<_textBoxes.size(); i++)
			_textBoxes[i]->setSelected(false);
	}
	void setBusDragging(bool isSet)
	{
		_dragBusMode=isSet;
	}
	/**
	 * To be called by the ProcessingBox when some one drops a wire on its connectors.
	 * @pre The processing box has checked that connection is the proper one for the canvas _dragStatus.
	 */
	void endWireDrag(ProcessingBox * processing, int connection)
	{
		ProcessingBox * from, * to;
		int fromConnection, toConnection;
		unsigned n=1;
		switch (_dragStatus)
		{
			case InportDrag:
			case IncontrolDrag:
				from = processing;
				to = _dragProcessing;
				fromConnection = connection;
				toConnection = _dragConnection;
				break;
			case OutportDrag:
			case OutcontrolDrag:
				from = _dragProcessing;
				to = processing;
				fromConnection = _dragConnection;
				toConnection = connection;
				break;
			default:
				CLAM_ASSERT(false, "Ending a wire drag but not in wire drag status");
				return;
		}
		switch (_dragStatus) 
		{
			case InportDrag:
			case OutportDrag:
				if (_dragBusMode) n = std::min(
					nInports(to->model())-toConnection,
					nOutports(from->model())-fromConnection);
				for (unsigned i=0; i<n; i++)
					addPortConnection(from, fromConnection+i, to, toConnection+i);
				break;
			case OutcontrolDrag:
			case IncontrolDrag:
				if (_dragBusMode) n = std::min(
					nIncontrols(to->model())-toConnection,
					nOutcontrols(from->model())-fromConnection);
				for (unsigned i=0; i<n; i++)
					addControlConnection(from, fromConnection+i, to, toConnection+i);
				break;
			default:
				CLAM_ASSERT(false, "Ending a wire drag but not in wire drag status");
				return;
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
		setWhiteColorsForBoxes();

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
		render(&painter);
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
	void removeSelection()
	{
		std::vector<ProcessingBox *> toRemoveP;
		for (unsigned i=0; i<_processings.size(); i++)
			if (_processings[i]->isSelected()) toRemoveP.push_back(_processings[i]) ;
		for (unsigned i=0; i<toRemoveP.size(); i++)
			removeProcessing( toRemoveP[i] );

		std::vector<TextBox *> toRemoveT;
		for (unsigned i=0; i<_textBoxes.size(); i++)
			if (_textBoxes[i]->isSelected()) toRemoveT.push_back(_textBoxes[i]) ;
		for (unsigned i=0; i<toRemoveT.size(); i++)
			removeTextBox( toRemoveT[i] );

		update();
	}
public:
	void startDrag(DragStatus status, ProcessingBox * processing, int connection)
	{
		_dragStatus=status;
		_dragProcessing=processing;
		_dragConnection=connection;
		_dragBusMode = false;
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
		networkRemoveProcessing(processing->getName());
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
	void removeTextBox(TextBox * textBox)
	{
		networkRemoveTextBox(textBox->getInformationText());
		delete textBox;
		_textBoxes.erase(std::find(_textBoxes.begin(), _textBoxes.end(), textBox));
		markAsChanged();
	}
	void zoom(int steps)
	{
		if (steps>=0)
			for (int i=0; i<steps; i++)
				_zoomFactor*=1.0625;
		else
			for (int i=0; i>steps; i--)
				_zoomFactor/=1.0625;
		resetTransform();
		scale(_zoomFactor,_zoomFactor);
		updateEmbededWidgets();
		update();
	}
	void resetZoom()
	{
		resetTransform();
		_zoomFactor=1.;
		updateEmbededWidgets();
		update();
	}
	void updateEmbededWidgets()
	{
		// Change the position just to trigger the updateEmbededGeometry of each processing box
		for (unsigned i=0; i<_processings.size(); i++)
			_processings[i]->move(_processings[i]->position());
	}
	const QPixmap getPixmapOfCanvas(Qt::TransformationMode mode = Qt::FastTransformation)
	{
		QPixmap pixmap(size());
		QPainter painter;
		painter.begin(&pixmap);
		render(&painter);
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
				boundingBox = boundingBox.unite(QRect(_processings[i]->position(),_processings[i]->size()));
		return boundingBox;
	}
protected:
	void addProcessing(QPoint point, QString type)
	{
		QString name = networkAddProcessing(type);
		if (name.isEmpty())
		{
			QMessageBox::critical(this, tr("Error creating a processing"),
				tr("<p>The processing type '<tt>%1</tt>' is not supported.</p>").arg(type));
			return;
		}
		addProcessingBox(name, networkProcessing(name), point);
		markAsChanged();
	}
	void addProcessingBox(const QString & name, void * processing, QPoint point=QPoint(), QSize size=QSize())
	{
		if (!processing)
			_processings.push_back(new ProcessingBox(this, name, 1, 1, 0, 0));
		else
			_processings.push_back(new ProcessingBox(this, name, 0, 0, 0, 0));
		_processings.back()->setProcessing(processing);
		_processings.back()->move(point);
		_processings.back()->resize(size);
		_scene->addItem(_processings.back());
		raise(_processings.back());
	}
protected slots:
	void onNewProcessing()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString type = askProcessingType();
		if (type.isEmpty()) return;
		addProcessing(point, type);
	}
	void onConfigure()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		ProcessingBox * processing = processingUnder(point);
		if (not processing) return;
		if (not processing->configure()) return;
		markAsChanged();
	}
	void onRename()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		ProcessingBox * processing = processingUnder(point);
		if (!processing) return;
		if (!processing->rename()) return;
		markAsChanged();
	}
	void onDisconnect()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		ProcessingBox * processing = processingUnder(point);
		if (not processing) return;
		ProcessingBox::Region region = processing->getRegion(point);
		switch (region)
		{
			case ProcessingBox::inportsRegion:
				disconnectInport(processing, processing->portIndexByYPos(point));
			break;
			case ProcessingBox::outportsRegion:
				disconnectOutport(processing, processing->portIndexByYPos(point));
			break;
			case ProcessingBox::incontrolsRegion:
				disconnectIncontrol(processing, processing->controlIndexByXPos(point));
			break;
			case ProcessingBox::outcontrolsRegion:
				disconnectOutcontrol(processing, processing->controlIndexByXPos(point));
			break;
			default:
				CLAM_WARNING(false, "Not a port to disconnect.");
		}
	}
	void onAddLinkedProcessing()
	{
		QPoint point = ((QAction*)sender())->data().toPoint();
		QString processingType = ((QAction*)sender())->text();
		ProcessingBox * processing = processingUnder(point);
		ProcessingBox::Region region = processing->getRegion(point);
		switch (region)
		{
			case ProcessingBox::outcontrolsRegion:
				createAndLinkToOutControl(processing, point);
				break;
			case ProcessingBox::incontrolsRegion:
				createAndLinkToInControl(processing, point);
				break;
			case ProcessingBox::inportsRegion:
				createAndLinkToInPort(processing, point, processingType);
				break;
			case ProcessingBox::outportsRegion:
				createAndLinkToOutPort(processing, point, processingType);
				break;
			default:
				;
		}
	}


public:
	virtual void * networkProcessing(const QString & name)=0; // Returns the named module
	virtual QString networkAddProcessing(const QString & type)=0; // creates a module, returns name
	virtual bool networkRenameProcessing(QString oldName, QString newName)=0;
	virtual void networkRemoveProcessing(const QString & name) = 0;
	virtual void networkRemoveTextBox(void * textBox) = 0;
	virtual void addTextBox(const QPoint& point) = 0;
	virtual void editTextBox(TextBox * textbox) = 0;
	virtual void networkUpdateTextBox(void * modelText, const QString & text, const QPointF & pos) = 0;
	virtual bool canConnectPorts(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet) = 0;
	virtual bool canConnectControls(ProcessingBox * source, unsigned outlet, ProcessingBox * target, unsigned inlet) = 0;
	virtual bool networkAddPortConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkAddControlConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkRemovePortConnection(const QString & outlet, const QString & inlet) = 0;
	virtual bool networkRemoveControlConnection(const QString & outlet, const QString & inlet) = 0;

	virtual QIcon processingIcon(ProcessingBox * processingBox) =0;
	virtual unsigned nInports(void * processing) = 0;
	virtual unsigned nOutports(void * processing) = 0;
	virtual unsigned nIncontrols(void * processing) = 0;
	virtual unsigned nOutcontrols(void * processing) = 0;
	virtual QColor inportColor(void * processing, unsigned index) const = 0;
	virtual QColor outportColor(void * processing, unsigned index) const = 0;
	virtual QColor incontrolColor(void * processing, unsigned index) const = 0;
	virtual QColor outcontrolColor(void * processing, unsigned index) const = 0;
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
	virtual QString infoMessage(void * processing)=0;
	virtual QWidget * embededWidgetFor(void * processing) = 0;
	virtual QString askProcessingType()=0;

	// TODO: Are those generic enough to be virtual?
	virtual bool editConfiguration(ProcessingBox * box) = 0;
	virtual void createAndLinkToInControl( ProcessingBox * processing, QPoint point ) = 0;
	virtual void createAndLinkToOutControl( ProcessingBox * processing, QPoint point ) = 0;
	virtual void createAndLinkToOutPort( ProcessingBox * processing, QPoint point, const QString & processingType) =0;
	virtual void createAndLinkToInPort ( ProcessingBox * processing, QPoint point, const QString & processingType) =0;

	virtual void networkConnectionContextMenu(QMenu * menu, const QPoint & pos, ProcessingBox * processing, ProcessingBox::Region region) { }
	virtual void processingContextMenu(QMenu * menu, const QPoint & pos, ProcessingBox * processing) { }
	virtual void canvasContextMenu(QMenu * menu, const QPoint & pos) { }

	void connectionContextMenu(QMenu * menu, const QPoint & cursorPosition, ProcessingBox * processing, ProcessingBox::Region region)
	{
		menu->addAction(QIcon(":/icons/images/remove.png"), tr("Disconnect"),
			this, SLOT(onDisconnect()))->setData(cursorPosition);
		networkConnectionContextMenu(menu, cursorPosition, processing, region);
		connectionPopulateConnectToMenu(menu, cursorPosition, processing, region);
	}

	void connectionPopulateConnectToMenu(QMenu * menu, const QPoint& cursorPosition, ProcessingBox * processing, ProcessingBox::Region region)
	{
		if (not _enabledViableConnectionMenu) return;

		menu->addAction(tr("Connect to"))->setSeparator(true);
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
		//std::cout << "NC::mouseMove" << std::endl;
		_dragPoint = mapToScene(event->pos()).toPoint();

		setToolTip(0);
		setStatusTip(0);

		// TODO: This is so ugly, turn it state based
		if(cursor().shape()!=Qt::ClosedHandCursor)
			setCursor(Qt::ArrowCursor);

		QGraphicsView::mouseMoveEvent(event);
		for (unsigned i = _processings.size(); i--; )
			_processings[i]->hover(mapToScene(event->pos()).toPoint());
		_tooltipPos=_dragPoint;
		update();
		//std::cout << "NC::mouseMove done" << std::endl;
	}
	void mousePressEvent(QMouseEvent * event)
	{
		if (event->button()!=Qt::LeftButton) return;
		//std::cout << "NC::mousePress" << std::endl;
		QGraphicsView::mousePressEvent(event);
		if (event->isAccepted()) return;
		//std::cout << "NC::mousePress background" << std::endl;
		if (not (event->modifiers() & Qt::ControlModifier))
			clearSelections();
		_selectionDragOrigin=mapToScene(event->pos()).toPoint();
		startDrag(SelectionDrag,0,0);
		update();
		//std::cout << "NC::mousePress done" << std::endl;
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		//std::cout << "NC::mouseRelease" << std::endl;
		if (_dragStatus == SelectionDrag)
		{
			QRect selectionBox (_selectionDragOrigin, _dragPoint);
			for (unsigned i = _processings.size(); i--; )
				if (selectionBox.contains(QRect(_processings[i]->position(),_processings[i]->size())))
					_processings[i]->select();
			for (unsigned i = _textBoxes.size(); i--; )
				if (selectionBox.contains(QRect(_textBoxes[i]->pos().toPoint(), _textBoxes[i]->boundingRect().size().toSize())))
					_textBoxes[i]->setSelected(false);
		}
		QGraphicsView::mouseReleaseEvent(event);
		QPointF scenePointF=mapToScene(event->pos());
		ProcessingBox * processingBox=(ProcessingBox*)_scene->itemAt(scenePointF);
		if(processingBox)
			processingBox->endWireDrag(scenePointF.toPoint());
		_dragStatus=NoDrag;
		update();
		//std::cout << "NC::mouseRelease done" << std::endl;
	}
	void mouseDoubleClickEvent(QMouseEvent * event)
	{
		if (_scene->itemAt(mapToScene(event->pos())))
			QGraphicsView::mouseDoubleClickEvent(event);
		else
			print();
	}

	void contextMenuEvent(QContextMenuEvent * event)
	{
		QGraphicsView::contextMenuEvent(event);
		if (event->isAccepted()) return;
		QMenu menu(this);
		canvasContextMenu(&menu, mapToScene(event->pos()).toPoint());
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
		addProcessing(mapToScene(event->pos()).toPoint(), type);
	}
	void scrollContentsBy( int dx, int dy )
	{
		QGraphicsView::scrollContentsBy(dx,dy);
		updateEmbededWidgets();
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
		//std::cout << "Event: " << typeid(*event).name() << std::endl;
		if (event->type()!=QEvent::WhatsThis) return QGraphicsView::event(event);
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
			case ProcessingBox::iconRegion:
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
					"<li>Double click on the icon to configure it.</li>\n"
					"</ul>\n"
					));
			}
			return QGraphicsView::event(event);
		}
		setWhatsThis(tr(
			"<p>This is the 'network canvas'. "
			"Drag here processings from the 'Processing tool box' "
			"and connect them by dragging compatible connectors. "
			"You may pan the canvas by dragging it with the control key pressed.</p>"
			));
		return QGraphicsView::event(event);
	}
	
protected:
	std::vector<ProcessingBox *> _processings;
	std::vector<PortWire *> _portWires;
	std::vector<ControlWire *> _controlWires;
	std::vector<TextBox *> _textBoxes;
private:
	bool _changed;
	double _zoomFactor;
	DragStatus _dragStatus;
	ProcessingBox * _dragProcessing;
	unsigned _dragConnection;
	QPoint _dragPoint;
	QPoint _selectionDragOrigin;
	QPoint _tooltipPos;
	QString _tooltipText;
	bool _dragBusMode;
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
	QGraphicsScene * _scene;
	unsigned long _maxZ;
protected:
	bool _enabledViableConnectionMenu;
	QAction * _deleteSelectedAction;
	QAction * _selectAllAction;
	QAction * _clearSelectionAction;
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

#endif//NetworkCanvas_hxx

