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
	_data = 0;
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

void CLAM::VM::BarGraph::clearData()
{
}

void CLAM::VM::BarGraph::timerEvent(QTimerEvent *event)
{
	if ( !_dataSource) return;
	if ( !_dataSource->isEnabled()) return;
	updateIfNeeded();
}

void CLAM::VM::BarGraph::paintEvent(QPaintEvent * event)
{
	_updatePending=0;
	if ( !_dataSource) return;
	const CLAM::TData * _data = _dataSource->frameData();
	if ( !_data)
	{
		_dataSource->release();
		return;
	}
	int size = _dataSource->nBins();
	double minValue=_dataSource->lowerBound();
	double maxValue=_dataSource->upperBound();
	const unsigned margin=2;
	float barWidth = (width()-margin)/float(size);
	float maxBarSize = height()-2*margin;
	float zeroPos = margin + maxBarSize*maxValue/(maxValue-minValue);
	QPainter painter(this);
	painter.drawLine(0,zeroPos,width(),zeroPos);
	for (unsigned i=0; i<size; i++)
	{
		if (_data[i]!=_data[i])
			continue; // NaN
		double valuePosition = margin + maxBarSize*(maxValue-_data[i])/(maxValue-minValue);
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
	_dataSource->release();
}

