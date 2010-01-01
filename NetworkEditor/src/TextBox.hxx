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

	NetworkCanvas * _canvas;
	void * _modelText;

public:
	TextBox(NetworkCanvas * canvas=0);
	~TextBox();

 	void setInformationText(void * informationText);
	void * getInformationText();
	void setText(const QString& text);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	QVariant itemChange(GraphicsItemChange change, const QVariant & value);
};

#endif
