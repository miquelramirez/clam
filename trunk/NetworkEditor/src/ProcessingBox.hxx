#ifndef ProcessingBox_hxx
#define ProcessingBox_hxx

#include <QtGui/QWidget>
#include <QtGui/QGraphicsItem>

class NetworkCanvas;

class ProcessingBox : public QGraphicsItem
{
public:
	enum
	{
		margin = 5,
		portWidth = 12,
		portHeight = 12,
		portSpacing = 4,
		controlWidth = 10,
		controlHeight = 10,
		controlSpacing = 6,
		portStep = portSpacing + portHeight,
		portOffset = margin + controlHeight,
		controlStep = controlSpacing + controlWidth,
		controlOffset = margin + portWidth
	};
	enum Region 
	{
		noRegion=0,
		inportsRegion,
		outportsRegion,
		incontrolsRegion,
		outcontrolsRegion,
		nameRegion,
		resizeHandleRegion,
		bodyRegion
	};
	enum ActionMode
	{
		NoAction,
		Resizing,
		Moving
	};
	ProcessingBox(NetworkCanvas * parent, const QString & name,
		   	unsigned nInports, unsigned nOutports,
			unsigned nIncontrols, unsigned nOutcontrols);
	virtual ~ProcessingBox();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setProcessing(void * model);
	void * model() const { return _processing; }
	void paintFromParent(QPainter & painter);
	// Returns the region at a parent pos
	Region getRegion(const QPoint & point) const;
	// Returns the connector index given a parent pos
	int portIndexByYPos(const QPoint & point) const;
	int controlIndexByXPos(const QPoint & point) const;
	// Returns the inport connect point in parent coords
	QPoint getInportPos(unsigned i) const;
	QPoint getOutportPos(unsigned i) const;
	QPoint getIncontrolPos(unsigned i) const;
	QPoint getOutcontrolPos(unsigned i) const;
	// Returns the processing box name
	QString getName() const;
	// Get the full connector name
	QString getInportName(unsigned i) const;
	QString getOutportName(unsigned i) const;
	QString getIncontrolName(unsigned i) const;
	QString getOutcontrolName(unsigned i) const;
	// Get the connector prototyper name at the given parent pos
	QString getInportPrototyperName(const QPoint & point) const;
	QString getOutportPrototyperName(const QPoint & point) const;
	QString getIncontrolPrototyperName(const QPoint & point) const;
	QString getOutcontrolPrototyperName(const QPoint & point) const;
	QString getConnectionPrototyperName(QString kind, QString connectionName) const;

	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) ;

	void move(const QPoint & newPosition);
	void resize(const QSize & newSize);
	void startMoving(const QPoint & initialGlobalPos);
	void keepMoving(const QPoint & delta);
	void doubleClicking(const QPoint & scenePoint);
	QPoint position() const { return _pos; }
	QSize size() const { return _size; }
	void embed(QWidget * widget);
	bool configure();
	bool rename();
	void hover(const QPoint & scenePoint);
	void endWireDrag(const QPoint& scenePoint);

	void raiseEmbeded() {if (_embeded) _embeded->raise();}

	bool isSelected() { return _selected; }
	void select() { _selected=true; }
	void deselect() { _selected=false; }
private:
	void setName(const QString & newName);
	void recomputeMinimumSizes();
	void paintBox(QPainter & painter);
	void drawConnector(QPainter & painter, Region region, unsigned index);
	void refreshConnectors();
	void updateEmbededWidget();
private:
	NetworkCanvas * _canvas;
	void * _processing; ///< Data model associated to the box
	QWidget * _embeded; ///< Embeded widget displayed within the box
	QString _name;
	bool _selected; ///< Whether the widget is selected or not
	unsigned _nInports;
	unsigned _nOutports;
	unsigned _nIncontrols;
	unsigned _nOutcontrols;
	ActionMode _actionMode;
	QPoint dragOrigin;
	QPoint originalPosition;
	QSize originalSize;
	int textHeight;
	QPoint _pos;
	QSize _size;
	QSize _minimumSize;
	Region _highLightRegion;
	unsigned _highLightConnection;
};



#endif//ProcessingBox_hxx

