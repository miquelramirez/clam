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

class NetworkCanvas;

class ProcessingBox 
{
public:
	enum
	{
		portWidth = 12,
		portHeight = 12,
		portSpacing = 4,
		controlWidth = 10,
		controlHeight = 10,
		controlSpacing = 6,
		margin = 5
	};
	enum Region 
	{
		inportsRegion,
		outportsRegion,
		incontrolsRegion,
		outcontrolsRegion,
		nameRegion,
		resizeHandleRegion,
		bodyRegion,
		noRegion
	};
	ProcessingBox(NetworkCanvas * parent, const QString & name,
		   	unsigned nInports, unsigned nOutports,
			unsigned nIncontrols, unsigned nOutcontrols);
	virtual ~ProcessingBox();

	void paintFromParent(QPainter & painter);
	void paintBox(QPainter & painter);
	Region getRegion(const QPoint & point) const;
	int portIndexByYPos(const QPoint & point);
	int controlIndexByXPos(const QPoint & point);
	// returns the inport connect point in parent coords
	QPoint getInportPos(unsigned i) const;
	QPoint getOutportPos(unsigned i) const;
	QPoint getIncontrolPos(unsigned i) const;
	QPoint getOutcontrolPos(unsigned i) const;

	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseDoubleClickEvent(QMouseEvent * event);
	void rename(const QString & newName);
	void move(const QPoint & newPosition);
	void resize(const QSize & newSize);
private:
	NetworkCanvas * _canvas;
	QString _name;
	unsigned _nInports;
	unsigned _nOutports;
	unsigned _nIncontrols;
	unsigned _nOutcontrols;
	bool moving;
	QPoint dragOrigin;
	QPoint originalPosition;
	QSize originalSize;
	bool resizing;
	int textHeight;
	QPoint _pos;
	QSize _size;
	QSize _minimumSize;
	Region _highLightRegion;
	unsigned _highLightConnection;
};



#endif//ProcessingBox_hxx
