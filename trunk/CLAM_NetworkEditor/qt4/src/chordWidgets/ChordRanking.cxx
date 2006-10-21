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

#include <CLAM/Factory.hxx>
#include "ChordRanking.hxx"

static CLAM::Factory<CLAM::Processing>::Registrator<ChordRankingMonitor> registrator("ChordRanking");

#include "ChordRanking.hxx"
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <iostream>
#include <CLAM/Pool.hxx>

CLAM::VM::ChordRanking::~ChordRanking()
{
}
CLAM::VM::ChordRanking::ChordRanking(QWidget * parent) :
	QWidget(parent)
{
	_data = 0;
	_dataSource = 0;
	_font.setFamily("sans-serif");
	_font.setPointSize(11);
	_updatePending=0;
	_nBins=0;
	_maxValue=1;
//	setWhatsThis(tr("TODOI"));
	startTimer(50);
}

void CLAM::VM::ChordRanking::updateIfNeeded()
{
	if (!_updatePending++) update();
}

void CLAM::VM::ChordRanking::setDataSource(FloatArrayDataSource & dataSource )
{
	_dataSource = &dataSource;
	_nBins = _dataSource->nBins();
}

void CLAM::VM::ChordRanking::clearData()
{
	_maxValue=1;
}

void CLAM::VM::ChordRanking::timerEvent(QTimerEvent *event)
{
	if ( !_dataSource) return;
	if ( !_dataSource->isEnabled()) return;
	updateIfNeeded();
}

struct IndirectSorter {
	const CLAM::TData * _values;
public:
	IndirectSorter (const CLAM::TData * values)
		: _values(values) {}
	int operator() (unsigned a, unsigned b)
	{
		return _values[a]>_values[b];
	} 
};

void CLAM::VM::ChordRanking::paintEvent(QPaintEvent * event)
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
	unsigned barSize = QFontMetrics(_font).height()+4;
	const unsigned margin=2;
	unsigned barOffset = barSize+margin;
	
	QPainter painter(this);
	_maxValue*=0.95;
	if (_maxValue<1e-5) _maxValue=1;
	for (unsigned i = 0; i < size; i++)
		if (_data[i]>=_maxValue) _maxValue=_data[i];
	std::vector<unsigned> indexes(size);
	for (unsigned i=0; i<size; i++)
		indexes[i]=i;
	std::sort(indexes.begin(), indexes.end(), IndirectSorter(_data));

	for (unsigned i=0; i<size; i++)
	{
		QRect barRect(margin,i*barOffset+margin, (width()-2*margin)*_data[indexes[i]]/_maxValue,barSize);
		painter.drawRect(barRect);
		painter.drawText(margin, (i+1)*barOffset,_dataSource->getLabel(indexes[i]).c_str());
	}
	_dataSource->release();
}
