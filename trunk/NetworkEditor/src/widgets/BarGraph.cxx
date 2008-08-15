/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "BarGraph.hxx"

#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <iostream>
#include <algorithm>

static CLAM::VM::FloatArrayDataSource & dummySource()
{
	static CLAM::TData dummyData[] = {.4, .3, .5, .7, .8, .9, .6, .3, .2, .1, .0};
	static CLAM::VM::DummyFloatArrayDataSource dummy(11, dummyData);
	return dummy;
}

CLAM::VM::BarGraph::~BarGraph()
{
}
CLAM::VM::BarGraph::BarGraph(QWidget * parent, FloatArrayDataSource * source)
	: QWidget(parent)
	, _barGradientBegin(Qt::white)
	, _barGradientEnd(Qt::black)
{
	setDataSource(source ? *source : dummySource());
	_updatePending=0;
	startTimer(50);
}

void CLAM::VM::BarGraph::updateIfNeeded()
{
	if (!_updatePending++) update();
}

void CLAM::VM::BarGraph::setDataSource(FloatArrayDataSource & dataSource )
{
	_dataSource = &dataSource;
}

void CLAM::VM::BarGraph::noDataSource()
{
	_dataSource = 0;
}

void CLAM::VM::BarGraph::clearData()
{
}

void CLAM::VM::BarGraph::timerEvent(QTimerEvent *event)
{
	if ( !_dataSource) return;
	if ( !_dataSource->isEnabled()) return;
	updateIfNeeded();
}
void CLAM::VM::BarGraph::findBounds(double & min, double & max, unsigned size, const CLAM::TData * data)
{
	// First try to use the ones provided by the data source
	if (_dataSource->hasUpperBound() && _dataSource->hasLowerBound())
	{
		min=_dataSource->lowerBound();
		max=_dataSource->upperBound();
		return;
	}
	// No data, keep default value
	if (!data) return;
	if (!size) return;
	min=+1e10;
	max=-1e10;
	for (unsigned i=0; i<size; i++)
	{
		if (data[i]!=data[i]) continue; // NaN
		if (data[i]<min) min=data[i];
		if (data[i]>max) max=data[i];
	}
	// If positive, 0 as reference
	if (min>0) min=0;
	// If also negative, make it simetrical
	if (min*max<0)
	{
		if (min>-max) min=-max;
		if (max<-min) max=-min;
	}
	// No valid number found
	if (min>max)
	{
		max=-5;
		min=0;
	}
	// if constant 0, wide the scope
	if (min==max) max=1;
}

void CLAM::VM::BarGraph::paintEvent(QPaintEvent * event)
{
	_updatePending=0;
	if ( !_dataSource) return;
	const TData * data = _dataSource->frameData();
	if ( !data)
	{
		_dataSource->release();
		return;
	}
	int size = _dataSource->nBins();
	double minValue=0;
	double maxValue=1;
	findBounds(minValue, maxValue, size, data);
	const unsigned margin=2;
	float barWidth = (width()-margin)/float(size);
	float maxBarSize = height()-2*margin;
	float zeroPos = margin + maxBarSize*maxValue/(maxValue-minValue);
	QPainter painter(this);
	if (barWidth<margin+5)
	{
		QPolygonF polygon;
		for (unsigned i=0; i<size; i++)
		{
			if (data[i]!=data[i]) continue; // NaN
			double valuePosition = margin + maxBarSize*(maxValue-data[i])/(maxValue-minValue);
			polygon << QPointF(i*barWidth+margin, valuePosition);
		}
		polygon << QPointF((size-1)*barWidth+margin, zeroPos);
		polygon << QPointF(margin, zeroPos);
		QLinearGradient linearGrad(
			QPointF(0,height()-margin),
			QPointF(0,margin));
		linearGrad.setColorAt(0, _barGradientEnd);
		linearGrad.setColorAt(zeroPos/height(), _barGradientBegin);
		linearGrad.setColorAt(1, _barGradientEnd);
		painter.setBrush(linearGrad);
		painter.drawPolygon(polygon);
	}
	else
	{
		painter.drawLine(0,zeroPos,width(),zeroPos);
		for (unsigned i=0; i<size; i++)
		{
			if (data[i]!=data[i]) continue; // NaN
			double valuePosition = margin + maxBarSize*(maxValue-data[i])/(maxValue-minValue);
			QRectF barRect(
				i*barWidth+margin, zeroPos,
				barWidth-margin, valuePosition-zeroPos);
			QLinearGradient linearGrad(
				QPointF(0,zeroPos),
				QPointF(0,valuePosition));
			linearGrad.setColorAt(0, _barGradientBegin);
			linearGrad.setColorAt(1, _barGradientEnd);
			painter.setBrush(linearGrad);
			painter.drawRect(barRect);
		}
	}
	_dataSource->release();
}

