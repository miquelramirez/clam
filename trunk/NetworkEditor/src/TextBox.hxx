#ifndef TextBox_H
#define TextBox_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <CLAM/BaseNetwork.hxx>

class NetworkCanvas;

class TextBox : public QGraphicsTextItem
{
    Q_OBJECT

	enum
	{
		margin = 5,
	};

	NetworkCanvas * _canvas;
	CLAM::InformationText * _informationText;

public:
	TextBox(NetworkCanvas * canvas=0);
	~TextBox();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void move(const QPoint & point);

 	void setInformationText(CLAM::InformationText * informationText);
	CLAM::InformationText * getInformationText();
	void setText(const QString& text);

	void select()     { setSelected(true); }
	void deselect()   { setSelected(false);}
};

#endif
