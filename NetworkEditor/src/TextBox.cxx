#include "TextBox.hxx"
#include "NetworkCanvas.hxx"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#include <iostream>

TextBox::TextBox(NetworkCanvas * canvas)
	: _modelText(0)
{
	setFlag(QGraphicsItem::ItemIsMovable,true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
//TODO: David, this flag doesn't exists in Qt 4.5 !? (http://doc.trolltech.com/4.5/qgraphicsitem.html#GraphicsItemFlag-enum)
//	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	_canvas=canvas;
//	setTextInteractionFlags(Qt::TextEditable);
}

TextBox::~TextBox()
{
	_modelText=0;
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
	_canvas->editTextBox(this);
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void TextBox::setInformationText(void * informationText)
{
	_modelText=informationText;
}

void * TextBox::getInformationText()
{
	return _modelText;
}

QVariant TextBox::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change==ItemPositionHasChanged)
	{
		QPointF newPos = value.toPointF();
		_canvas->networkUpdateTextBox(_modelText, toPlainText(), pos());
	}
	return QGraphicsTextItem::itemChange(change, value);
}

void TextBox::setText(const QString& text)
{
	setPlainText (text);
	adjustSize();
	_canvas->networkUpdateTextBox(_modelText, toPlainText(), pos());
}

