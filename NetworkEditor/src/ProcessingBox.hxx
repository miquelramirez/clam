#ifndef ProcessingBox_hxx
#define ProcessingBox_hxx

#include <QtGui/QWidget>
#include <QtGui/QGraphicsItem>

class NetworkCanvas;
class QGraphicsProxyWidget;

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
		iconRegion,
		bodyRegion
	};
	enum ActionMode
	{
		NoAction,
		Resizing,
		Moving,
		Linking
	};
	ProcessingBox(NetworkCanvas * parent, const QString & name,
		   	unsigned nInports, unsigned nOutports,
			unsigned nIncontrols, unsigned nOutcontrols);
	virtual ~ProcessingBox();

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setProcessing(void * model);
	void * model() const { return _processing; }
	void paintFromParent(QPainter & painter);
	// Returns the region at a parent pos
	Region getRegion(const QPoint & point) const;
	Region getItemRegion(const QPoint & point) const;
	// Returns the connector index given a parent pos
	int portIndexByYPos(const QPoint & point) const;
	int controlIndexByXPos(const QPoint & point) const;
	int portIndexByItemYPos(const QPoint & point) const;
	int controlIndexByItemXPos(const QPoint & point) const;
	// Returns the inport connect point in parent coords
	QPoint getInportPos(unsigned i) const;
	QPoint getOutportPos(unsigned i) const;
	QPoint getIncontrolPos(unsigned i) const;
	QPoint getOutcontrolPos(unsigned i) const;
	// Returns whether the port is highlighted
	bool isInPortHighlighted(unsigned index) const;
	bool isOutPortHighlighted(unsigned index) const;
	bool isInControlHighlighted(unsigned index) const;
	bool isOutControlHighlighted(unsigned index) const;
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
	QPoint position() const { return pos().toPoint(); }
	QSize size() const { return _size; }
	void embed(QWidget * widget);
	bool configure();
	bool rename();
	void hover(const QPoint & scenePoint);
	void setMetadataToolTip();
	void endWireDrag(const QPoint& scenePoint);
	void raiseEmbededWidget() { if (_embeded) _embeded->raise(); }

	void select()     { setSelected(true); }
	void deselect()   { setSelected(false); }

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
	QGraphicsProxyWidget * _embededProxy; ///< Proxy to the embeded widget displayed within the box
	QString _name;
	unsigned _nInports;
	unsigned _nOutports;
	unsigned _nIncontrols;
	unsigned _nOutcontrols;
	ActionMode _actionMode;
	QPoint dragOrigin;
	QPoint originalPosition;
	QSize originalSize;
	int textHeight;
	QSize _size;
	QSize _minimumSize;
	Region _highLightRegion;
	unsigned _highLightConnection;
};



#endif//ProcessingBox_hxx

