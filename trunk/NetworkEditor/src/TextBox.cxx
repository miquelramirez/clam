#include "TextBox.hxx"
#include "NetworkCanvas.hxx"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

TextBox::TextBox(NetworkCanvas * canvas):_selected(false)
					, _informationText(0)
{
	_canvas=canvas;
}

void TextBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsTextItem::paint(painter, option, widget);

	if (_selected)
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

void TextBox::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QPoint scenePoint = event->scenePos().toPoint();

	if (event->modifiers() & Qt::ControlModifier )
	{
		_selected=!_selected;
		if (!_selected) return;
	}
	if (!_selected)
	{
		_canvas->clearSelections();
		_selected=true;
	}
	_canvas->startMovingSelected(scenePoint);
}

void TextBox::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{

}

void TextBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	_canvas->editingTextBox(this);
}

void TextBox::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if(!_selected)
	{
		_canvas->clearSelections();
		return;
	}

	QPoint scenePoint = event->scenePos().toPoint();
	QPoint _dragDelta = scenePoint - _dragOrigin;
	_canvas->keepMovingSelected(_dragDelta);
}

void TextBox::keepMoving(const QPoint & delta)
{
	move(_originalPosition + delta);
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

void TextBox::startMoving(const QPoint& scenePoint)
{
	_originalPosition = pos().toPoint();
	_dragOrigin = scenePoint;
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