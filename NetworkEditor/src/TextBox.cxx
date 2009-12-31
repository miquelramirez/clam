#include "TextBox.hxx"
#include "NetworkCanvas.hxx"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#include <iostream>

TextBox::TextBox(NetworkCanvas * canvas)
	: _informationText(0)
{
	setFlag(QGraphicsItem::ItemIsMovable,true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	_canvas=canvas;
//	setTextInteractionFlags(Qt::TextEditable);
}

TextBox::~TextBox()
{
	_informationText=0;
}

void TextBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsTextItem::paint(painter, option, widget);

	if (isSelected())
	{
		QRectF rect=QGraphicsTextItem::boundingRect();
		painter->setPen(_canvas->colorBoxFrameText());
		painter->setBrush(_canvas->palette().highlight());
		painter->drawRect(0, 0, margin, margin);
		painter->drawRect(0, rect.size().height(), margin, -margin);
		painter->drawRect(rect.size().width(), 0,  -margin, margin);
		painter->drawRect(rect.size().width(), rect.size().height(), -margin, -margin);
	}
}

void TextBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	_canvas->editingTextBox(this);
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void TextBox::setInformationText(CLAM::InformationText * informationText)
{
	_informationText=informationText;
	setText(QString::fromLocal8Bit(_informationText->text.c_str()));
	setPos(QPoint(_informationText->x, _informationText->y));
}

CLAM::InformationText * TextBox::getInformationText()
{
	return _informationText;
}

void TextBox::move(const QPoint & point)
{
	setPos(point);
	if(_informationText!=0)
	{
		_informationText->x=point.x();
		_informationText->y=point.y();
	}
}

void TextBox::setText(const QString& text)
{
	setPlainText (text);
	adjustSize();
	if(_informationText!=0)
	{
		_informationText->text=text.toStdString();
	}
}
