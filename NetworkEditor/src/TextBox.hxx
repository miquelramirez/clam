#ifndef TextBox_H
#define TextBox_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

class NetworkCanvas;

class TextBox : public QGraphicsTextItem
{
    Q_OBJECT

	enum
	{
		margin = 5,
	};

	QPoint _dragOrigin;
	QPoint _originalPosition;
	bool _selected;
	NetworkCanvas * _canvas;

public:
	TextBox(NetworkCanvas * canvas=0);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent (QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void keepMoving(const QPoint & delta);
	void move(const QPoint & point);

	void startMoving(const QPoint& scenePoint);
	void setText(const QString& text);

	bool isSelected() { return _selected; }
	void select() { _selected=true; }
	void deselect() { _selected=false; }
};

#endif
