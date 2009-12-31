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
#include <CLAM/XMLStorage.hxx>
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
		, _scene(0)
		, _maxZ(0)
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
		if (_dragStatus==InportDrag)
			PortWire::draw(*painter, _dragPoint, _dragProcessing->getInportPos(_dragConnection));
		if (_dragStatus==OutportDrag)
			PortWire::draw(*painter, _dragProcessing->getOutportPos(_dragConnection), _dragPoint);
		if (_dragStatus==IncontrolDrag)
			ControlWire::draw(*painter, _dragPoint, _dragProcessing->getIncontrolPos(_dragConnection));
		if (_dragStatus==OutcontrolDrag)
			ControlWire::draw(*painter, _dragProcessing->getOutcontrolPos(_dragConnection), _dragPoint);

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
			_textBoxes[i]->deselect();
	}
	void selectAll()
	{
		for (unsigned i=0; i<_processings.size(); i++)
			_processings[i]->select();
		for (unsigned i=0; i<_textBoxes.size(); i++)
			_textBoxes[i]->select();
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

public:
	virtual bool networkRenameProcessing(QString oldName, QString newName)=0;
	virtual void networkRemoveProcessing(const std::string & name) = 0;
	virtual void networkRemoveTextBox(CLAM::InformationText * textBox) = 0;
	virtual void addProcessing(QPoint point, QString type) = 0;
	virtual void editingTextBox(TextBox * textbox, const QPoint& point=QPoint(0,0)) = 0;
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

	// TODO: Are those generic enough to be virtual?
	virtual bool editConfiguration(ProcessingBox * box) = 0;
	virtual void createAndLinkToInControl( ProcessingBox * processing, QPoint point ) = 0;
	virtual void createAndLinkToOutControl( ProcessingBox * processing, QPoint point ) = 0;
	virtual void createAndLinkToOutPort( ProcessingBox * processing, QPoint point, const QString & processingType) =0;
	virtual void createAndLinkToInPort ( ProcessingBox * processing, QPoint point, const QString & processingType) =0;

	virtual void connectionContextMenu(QMenu * menu, QGraphicsSceneContextMenuEvent * event, ProcessingBox * processing, ProcessingBox::Region region) { }
	virtual void processingContextMenu(QMenu * menu, QGraphicsSceneContextMenuEvent * event, ProcessingBox * processing) { }
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
					_textBoxes[i]->select();
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
	std::vector<TextBox *> _textBoxes;
	std::vector<ProcessingBox *> _processings;
	std::vector<PortWire *> _portWires;
	std::vector<ControlWire *> _controlWires;
	double _zoomFactor;
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
	QGraphicsScene * _scene;
	unsigned long _maxZ;
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
#include <CLAM/BaseNetwork.hxx>
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

		_newTextBoxAction = new QAction(tr("&New Text Box"), this);
		_newTextBoxAction->setShortcut(QKeySequence(tr("Ctrl+t")));
		addAction(_newTextBoxAction);
		connect(_newTextBoxAction, SIGNAL(triggered()), this, SLOT(onNewTextBox()));

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
	CLAM::BaseNetwork & network()
	{
		return *_network;
	}

	virtual ~ClamNetworkCanvas();

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
		CLAM::InControlBase& inControl = processing->GetInControl(index);
		return inControl.DefaultValue();
	}
	float incontrolLowerBound(CLAM::Processing * processing, unsigned index) const
	{
		CLAM::InControlBase& inControl = processing->GetInControl(index);
		return inControl.LowerBound();
	}
	float incontrolUpperBound(CLAM::Processing * processing, unsigned index) const
	{
		CLAM::InControlBase& inControl = processing->GetInControl(index);
		return inControl.UpperBound();
	}

	virtual void createAndLinkToInControl( ProcessingBox * processing, QPoint point )
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

	virtual void createAndLinkToOutControl( ProcessingBox * processing, QPoint point )
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
	
	void createAndLinkToOutPort( ProcessingBox * processing, QPoint point, const QString & processingType)
	{
		if (networkIsDummy()) return;

		std::string type = processingType.toStdString();
		unsigned portIndex = processing->portIndexByYPos(point);
		QString outPortName = processing->getOutportName(portIndex);

		CLAM::Processing * model = (CLAM::Processing*)processing->model();
		if (type.empty())
		{
			// Choose default if any
			if (model->GetOutPort(portIndex).GetTypeId()==typeid(CLAM::TData))
				type = "AudioSink";
			else return;
		}

		std::string processingId = _network->AddProcessing(type);
		CLAM::Processing & portProcessing = _network->GetProcessing( processingId );
		// add box to canvas and connect
		addProcessingBox( processingId.c_str(), &portProcessing, point+QPoint(100,0));
		addPortConnection(processing, portIndex, getBox(processingId.c_str()), 0);

		markAsChanged();
	}

	void createAndLinkToInPort( ProcessingBox * processing, QPoint point, const QString & processingType)
	{
		if (networkIsDummy()) return;

		std::string type = processingType.toStdString();
		unsigned portIndex = processing->portIndexByYPos(point);
		QString inPortName = processing->getInportName(portIndex);
		CLAM::Processing * model = (CLAM::Processing*)processing->model();
		if (type.empty())
		{
			// Choose default if any
			if (model->GetInPort(portIndex).GetTypeId()==typeid(CLAM::TData))
				type = "AudioSource";
			else return;
		}

		std::string processingId = _network->AddProcessing(type);
		CLAM::Processing & portProcessing = _network->GetProcessing( processingId );
		// add box to canvas and connect
		addProcessingBox( processingId.c_str(), &portProcessing, point+QPoint(-200,0));
		addPortConnection(getBox(processingId.c_str()), 0, processing, portIndex);
		markAsChanged();
	}


	virtual QWidget * embededWidgetFor(void * processing);
	virtual unsigned nInports(void * processing) { return ((CLAM::Processing*)processing)->GetNInPorts();}
	virtual unsigned nOutports(void * processing) { return ((CLAM::Processing*)processing)->GetNOutPorts();}
	virtual unsigned nIncontrols(void * processing) { return ((CLAM::Processing*)processing)->GetNInControls();}
	virtual unsigned nOutcontrols(void * processing) { return ((CLAM::Processing*)processing)->GetNOutControls();}
	virtual QColor inportColor(void * element, unsigned index) const
	{
		if (!element) return colorPort();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& porttype = processing->GetInPort(index).GetTypeId();
		return getConnectorColorByType(porttype);
	}
	virtual QColor outportColor(void * element, unsigned index) const
	{
		if (!element) return colorPort();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& porttype = processing->GetOutPort(index).GetTypeId();
		return getConnectorColorByType(porttype);
	}
	virtual QColor incontrolColor(void * element, unsigned index) const
	{
		if (!element) return colorControl();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& controltype = processing->GetInControl(index).GetTypeId();
		return getConnectorColorByType(controltype);
	}
	virtual QColor outcontrolColor(void * element, unsigned index) const
	{
		if (!element) return colorControl();
		CLAM::Processing * processing = (CLAM::Processing*) element;
		const std::type_info& controltype = processing->GetOutControl(index).GetTypeId();
		return getConnectorColorByType(controltype);
	}
	virtual QString inportName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Inport_%1").arg(index);
		CLAM::InPortBase & port = ((CLAM::Processing*)processing)->GetInPort(index);
		return port.GetName().c_str();
	}
	virtual QString outportName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Outport_%1").arg(index);
		CLAM::OutPortBase & port = ((CLAM::Processing*)processing)->GetOutPort(index);
		return port.GetName().c_str();
	}
	virtual QString incontrolName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Incontrol_%1").arg(index);
		CLAM::InControlBase & control = ((CLAM::Processing*)processing)->GetInControl(index);
		return control.GetName().c_str();
	}
	virtual QString outcontrolName(void * processing, unsigned index) const
	{
		if (!processing) return QString("Outcontrol_%1").arg(index);
		CLAM::OutControlBase & control = ((CLAM::Processing*)processing)->GetOutControl(index);
		return control.GetName().c_str();
	}
	template <typename Connector>
	QString connectorTooltip(Connector & connector) const
	{
		const char * typeString = CLAM::ProcessingDataPlugin::displayNameFor(connector.GetTypeId()).c_str();
		return QObject::tr("%1\nType: %2", "Connector tooltip")
			.arg(connector.GetName().c_str())
			.arg(typeString)
			;
	}
	QString outportTooltip(void * processing, unsigned index) const
	{
		if (!processing) return outportName(processing, index);
		CLAM::OutPortBase & port = ((CLAM::Processing*)processing)->GetOutPort(index);
		return connectorTooltip(port);
	}
	QString inportTooltip(void * processing, unsigned index) const
	{
		if (!processing) return inportName(processing, index);
		CLAM::InPortBase & port = ((CLAM::Processing*)processing)->GetInPort(index);
		return connectorTooltip(port);
	}
	QString outcontrolTooltip(void * processing, unsigned index) const
	{
		if (!processing) return outcontrolName(processing, index);
		CLAM::OutControlBase & control = ((CLAM::Processing*)processing)->GetOutControl(index);
		return connectorTooltip(control);
	}
	QString incontrolTooltip(void * processing, unsigned index) const
	{
		CLAM::InControlBase& inControl = ((CLAM::Processing*)processing)->GetInControl(index);
		// TODO: Bound info composing should be moved to the connector
		QString boundInfo = inControl.IsBounded() ? 
			QString("\n(bounds: [%1, %2] )").arg(inControl.LowerBound()).arg(inControl.UpperBound()) :
			"\n(not bounded)";
		return connectorTooltip(inControl) + boundInfo;
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
	virtual QString infoMessage(void * processing)
	{
		if (!processing) return "";
		CLAM::Processing * proc = (CLAM::Processing*)processing;
		std::string key= proc->GetClassName();
		std::string info = "Type: "+key;
		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		CLAM::ProcessingFactory::Pairs pairsFromKey=factory.GetPairsFromKey(key);
		CLAM::ProcessingFactory::Pairs::const_iterator itPairs;
		for(itPairs = pairsFromKey.begin();itPairs!=pairsFromKey.end();itPairs++)
		{
			std::string attribute = itPairs->attribute;
			std::string value = itPairs->value;
			if (attribute=="icon") continue;
			if (attribute=="embedded_svg") continue;
			if (value=="") continue;
			info+="\n"+attribute+": "+value;
		}
		return info.c_str();
	}
	virtual bool networkRenameProcessing(QString oldName, QString newName)
	{
		if (networkIsDummy()) return true;
		return _network->RenameProcessing(oldName.toStdString(), newName.toStdString());
	}


signals:
	void browseUrlRequest();
public:
	const QString getFileNameToOpenWithExternalApplication () const
	{
		QString copyFileNameToOpen = _fileNameToOpen;
		_fileNameToOpen.clear();
		return copyFileNameToOpen;
		//return _fileNameToOpen;
	}
	void requestBrowseUrl(const QString & fileName)
	{
		_fileNameToOpen=fileName;
		emit browseUrlRequest();
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
		_scene->addItem(_processings.back());
		raise(_processings.back());
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
		QString outName = source->getName() + "." + source->getOutcontrolName(outlet);
		QString inName = target->getName() + "." + incontrolName(target->model(),inlet);
		CLAM::OutControlBase & out = _network->GetOutControlByCompleteName(outName.toStdString());
		CLAM::InControlBase & in = _network->GetInControlByCompleteName(inName.toStdString());
		return out.IsLinkable(in);
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
	virtual void networkRemoveTextBox(CLAM::InformationText * informationText)
	{
		if (networkIsDummy()) return;
		_network->removeInformationText(informationText);
	}

public:
	bool networkIsDummy() const
	{
		return _network == 0;
	}
	void loadNetwork(CLAM::BaseNetwork * network)
	{
		clear();
		_network = network;
		clearChanges();
		if (networkIsDummy()) return;
		CLAM::BaseNetwork::ProcessingsMap::const_iterator it;
		for (it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++)
		{
			const std::string & name = it->first;
			CLAM::Processing * processing = it->second;
			addProcessingBox( name.c_str(),  processing );
		}
		
		CLAM::BaseNetwork::InformationTexts::iterator it2;
		for (it2=_network->BeginInformationTexts(); it2!=_network->EndInformationTexts(); it2++)
		{
			TextBox *textBox=new TextBox(this);
			_scene->addItem(textBox);
			textBox->setInformationText(*it2);
			_textBoxes.push_back(textBox);
		}
		refreshWires();
	}


	void reloadNetwork(bool selectAllNew=false)
	{
		if (networkIsDummy()) return;
		CLAM::BaseNetwork::ProcessingsMap::const_iterator it;
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
			unsigned nOutPorts = producer.GetNOutPorts();
			for (unsigned op = 0; op<nOutPorts; op++)
			{
				CLAM::OutPortBase & outPort = producer.GetOutPort(op);
				std::string completeOutName = producerName + "." + outPort.GetName();
				CLAM::BaseNetwork::NamesList connected = _network->GetInPortsConnectedTo( completeOutName );
				CLAM::BaseNetwork::NamesList::iterator inName;
				for(inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?

					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					unsigned nInPorts = consumer.GetNInPorts();
					for (unsigned ip=0; ip<nInPorts; ip++)
					{
						CLAM::InPortBase & inPort = consumer.GetInPort(ip);
						if (inPort.GetName()!=peerConnection) continue;
						addPortWire(_processings[p], op, consumerBox, ip);
						break;
					}
				}
			}
			unsigned nOutControls = producer.GetNOutControls();
			for (unsigned op = 0; op<nOutControls; op++)
			{
				CLAM::OutControlBase & outControl = producer.GetOutControl(op);
				std::string completeOutName = producerName + "." + outControl.GetName();
				CLAM::BaseNetwork::NamesList connected = _network->GetInControlsConnectedTo( completeOutName );
				CLAM::BaseNetwork::NamesList::iterator inName;
				for(inName=connected.begin(); inName!=connected.end(); inName++)
				{
					std::string consumerName = _network->GetProcessingIdentifier(*inName);
					ProcessingBox * consumerBox = getBox(consumerName.c_str());
					if (!consumerBox) continue; // TODO: Error?

					std::string peerConnection = _network->GetConnectorIdentifier(*inName);
					CLAM::Processing & consumer = _network->GetProcessing(consumerName);
					unsigned nInControls = consumer.GetNInControls();
					for (unsigned ip=0; ip<nInControls; ip++)
					{
						CLAM::InControlBase & inControl = consumer.GetInControl(ip);
						if (inControl.GetName()!=peerConnection) continue;
						addControlWire(_processings[p], op, consumerBox, ip);
						break;
					}
				}
			}
		}
	}

	bool updateGeometriesOnXML(QPoint offsetPoint=QPoint(0,0))
	{
		CLAM::BaseNetwork::ProcessingsGeometriesMap processingsGeometriesMap;
		for (unsigned i=0; i<_processings.size(); i++)
		{
			CLAM::BaseNetwork::Geometry processingGeometry;
			QPoint position = _processings[i]->position()-offsetPoint;
			QSize size = _processings[i]->size();
			const std::string name=_processings[i]->getName().toStdString();
			processingGeometry.x=position.x();
			processingGeometry.y=position.y();
			processingGeometry.width=size.width();
			processingGeometry.height=size.height();
			processingsGeometriesMap.insert(CLAM::BaseNetwork::ProcessingsGeometriesMap::value_type(name,processingGeometry));
		}
		return (_network->SetProcessingsGeometries(processingsGeometriesMap));
	}
	bool loadGeometriesFromXML(QPoint offsetPoint = QPoint(0,0))
	{
		const CLAM::BaseNetwork::ProcessingsGeometriesMap & processingsGeometriesMap=_network->GetAndClearGeometries();
		if (processingsGeometriesMap.empty())
			return 0;
		CLAM::BaseNetwork::ProcessingsGeometriesMap::const_iterator it;
		for(it=processingsGeometriesMap.begin();it!=processingsGeometriesMap.end();it++)
		{
			QString name=QString(it->first.c_str());
			const CLAM::BaseNetwork::Geometry & geometry=it->second;
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

	bool embedSVGDiagramsOption()
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
		CLAM::BaseNetwork::NamesList processingsNamesList;
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

		CLAM::BaseNetwork::NamesList::iterator cuttedNamesIterator;
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

			createAndLinkToInControl(_processings[i], point);
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

			createAndLinkToOutControl(_processings[i], point);
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
				createAndLinkToInPort(_processings[i], point, processingType);
				break;
			case ProcessingBox::outportsRegion:
				createAndLinkToOutPort(_processings[i], point, processingType);
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
	void editingTextBox(TextBox * textbox, const QPoint& point=QPoint(0,0))
	{
		QDialog dialog;
		dialog.setWindowTitle(tr("Adding text box."));
		QVBoxLayout * layout = new QVBoxLayout(&dialog);

		QPlainTextEdit * plainText = new QPlainTextEdit(&dialog);
		layout->addWidget(plainText);

		if(textbox) plainText->setPlainText(textbox->toPlainText());

		QDialogButtonBox * buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
		connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
		connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
		layout->addWidget(buttons);
		int result = dialog.exec();
		this->activateWindow();
		if (result==QDialog::Rejected) return;
		
		if(textbox==0) // new text box
		{
			CLAM::InformationText * informationText= new CLAM::InformationText();
			informationText->x=point.x();
			informationText->y=point.y();
			informationText->text=plainText->toPlainText().toStdString();
			_network->addInformationText(informationText);
			textbox = new TextBox(this);
			_scene->addItem(textbox);
			textbox->setInformationText(informationText);
			_textBoxes.push_back(textbox);
		}
		else
		{
			textbox->setText(plainText->toPlainText());
		}
		markAsChanged();
	}
	void onNewTextBox()
	{
		TextBox * textbox = 0;
		QPoint point = ((QAction*)sender())->data().toPoint();
		editingTextBox(textbox, point);
	}

	void onOpenFileWithExternalApplication()
	{
		const QString fileName = ((QAction*)sender())->data().toString();
		QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
	}

	void onBrowseUrl()
	{
		const QString fileName = ((QAction*)sender())->data().toString();
		requestBrowseUrl(fileName);
	}


private:
	std::string outportTypeId(void * processing, unsigned index) const
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetOutPort(index).GetTypeId().name();
	}

	std::string inportTypeId(void * processing, unsigned index) const
	{
		if (!processing) return "";
		return ((CLAM::Processing*)processing)->GetInPort(index).GetTypeId().name();
	}


	virtual void connectionContextMenu(QMenu * menu, QGraphicsSceneContextMenuEvent * event, ProcessingBox * processing, ProcessingBox::Region region)
	{
		connectionContextMenu(menu, event->scenePos().toPoint(), processing, region);
	}
	virtual void connectionContextMenu(QMenu * menu, const QPoint& cursorPosition, ProcessingBox * processing, ProcessingBox::Region region)
	{
		menu->addAction(QIcon(":/icons/images/remove.png"), tr("Disconnect"),
			this, SLOT(onDisconnect()))->setData(cursorPosition);
		menu->addAction(QIcon(":/icons/images/editcopy.png"), tr("Copy connection name"),
			this, SLOT(onCopyConnection()))->setData(cursorPosition);

		if (not processing->model()) return;
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

	virtual void processingContextMenu(QMenu * menu, QGraphicsSceneContextMenuEvent * event, ProcessingBox * processing)
	{
		processingContextMenu(menu, event->scenePos().toPoint(), processing);
	}
	virtual void processingContextMenu(QMenu * menu, const QPoint& point, ProcessingBox * processing)
	{
		menu->addAction(QIcon(":/icons/images/configure.png"), tr("Configure"),
			this, SLOT(onConfigure()))->setData(point);
		menu->addAction(QIcon(":/icons/images/editclear.png"), tr("Rename"),
			this, SLOT(onRename()))->setData(point);
		menu->addAction(_deleteSelectedAction);
		menu->addAction(_copySelectionAction);
		menu->addAction(_cutSelectionAction);

		if (not processing->model()) return;

		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		std::string className=((CLAM::Processing*)processing->model())->GetClassName();
		if (factory.AttributeExists(className,"faust_diagram"))
		{
			QString fileName=QString(factory.GetValueFromAttribute(className,"faust_diagram").c_str());
			menu->addSeparator();
			//menu->addAction(clamProcessingIcon(className),tr("Open diagram with browser"),this,SLOT(onOpenFileWithExternalApplication()))->setData(fileName);
			menu->addAction(
				clamProcessingIcon(className),
				tr("Navigate Faust diagram"),
				this,SLOT(onBrowseUrl())
				)->setData(fileName);
		}
		if (factory.AttributeExists(className,"faust_source_file"))
		{
			QString fileName=QString(factory.GetValueFromAttribute(className,"faust_source_file").c_str());
			menu->addAction(
				clamProcessingIcon(className),
				tr("Open source with editor"),
				this,SLOT(onOpenFileWithExternalApplication())
				)->setData(fileName);
			//menu->addAction(clamProcessingIcon(className),"Recompile plugin");
		}
	}

	virtual void canvasContextMenu(QMenu * menu, QContextMenuEvent * event)
	{
		canvasContextMenu(menu, mapToScene(event->pos()).toPoint());
	}
	virtual void canvasContextMenu(QMenu * menu, const QPoint& point)
	{
		_pasteSelectionAction->setData(point);
		menu->addAction(_pasteSelectionAction);
		_newProcessingAction->setData(point);
		menu->addAction(_newProcessingAction);
		_newTextBoxAction->setData(point);
		menu->addAction(_newTextBoxAction);
	}

	virtual QIcon processingIcon(ProcessingBox * processingBox)
	{
		if (not processingBox->model()) return  QIcon(":/icons/images/processing.svg");
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
	QAction * _newTextBoxAction;
	QAction * _newProcessingAction;
	QAction * _copySelectionAction;
	QAction * _cutSelectionAction;
	QAction * _pasteSelectionAction;
private:
	CLAM::BaseNetwork * _network;
	mutable QString _fileNameToOpen;
	bool _embedSVGDiagramsOption;
};

#endif//NetworkCanvas_hxx

