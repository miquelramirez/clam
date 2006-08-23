#ifndef ProcessingBox_hxx
#define ProcessingBox_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QInputDialog>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <CLAM/Processing.hxx>

class NetworkCanvas;

class ProcessingBox 
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

	void setProcessing(CLAM::Processing * processing);
	void paintFromParent(QPainter & painter);
	Region getRegion(const QPoint & point) const;
	int portIndexByYPos(const QPoint & point);
	int controlIndexByXPos(const QPoint & point);
	// returns the inport connect point in parent coords
	QPoint getInportPos(unsigned i) const;
	QPoint getOutportPos(unsigned i) const;
	QPoint getIncontrolPos(unsigned i) const;
	QPoint getOutcontrolPos(unsigned i) const;
	QString getName() const;
	QString getInportName(unsigned i) const;
	QString getOutportName(unsigned i) const;
	QString getIncontrolName(unsigned i) const;
	QString getOutcontrolName(unsigned i) const;

	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseDoubleClickEvent(QMouseEvent * event);
	void rename(const QString & newName);
	void move(const QPoint & newPosition);
	void resize(const QSize & newSize);
	void startMoving(const QPoint & initialGlobalPos);
	QPoint pos() { return _pos; }
	QSize size() { return _size; }
	void embed(QWidget * widget);
	bool configure();
private:
	void recomputeMinimumSizes();
	void paintBox(QPainter & painter);
	void drawConnector(QPainter & painter, Region region, unsigned index);
	void refreshConnectors();
	void updateEmbededWidget();
private:
	NetworkCanvas * _canvas;
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
	QPoint _pos;
	QSize _size;
	QSize _minimumSize;
	Region _highLightRegion;
	unsigned _highLightConnection;
	CLAM::Processing * _processing;
	QWidget * _embeded;
};



#endif//ProcessingBox_hxx
