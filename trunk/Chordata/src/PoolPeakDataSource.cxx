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

#include <typeinfo>
#include <CLAM/Pool.hxx>
#include <CLAM/Array.hxx>
#include "PoolPeakDataSource.hxx"

#include <iostream> // debug

CLAM::VM::PoolPeakDataSource::PoolPeakDataSource()
	: _nFrames(0)
	, _frameDivision(0)
	, _samplingRate(44100)
	, _positionFrameData(0)
	, _magnitudeFrameData(0)
	, _nBins(0)
	, _currentFrame(0)
{
}

void CLAM::VM::PoolPeakDataSource::clearData()
{
	_frameDataIndex.clear();
	_nFrames=0;
	_frameDivision=0;
	_positionFrameData=0;
	_magnitudeFrameData=0;
	_currentFrame=0;
	_nBins = 0;
	_storage = 0;
}

void CLAM::VM::PoolPeakDataSource::setDataSource(unsigned nBins)
{
	_nBins = nBins;
}

void CLAM::VM::PoolPeakDataSource::setStorage(
	FloatPairVectorStorage *storage,
	TData samplingRate,
	CLAM_Annotator::FrameDivision * frameDivision)
{
	_storage = storage;
	_positionFrameData = 0;
	_magnitudeFrameData = 0;
	_nFrames = _storage->PositionStorage().size();
	_frameDivision = frameDivision;

	_frameDataIndex.resize(_nFrames);
	Storage::iterator positionIt = _storage->PositionStorage().begin();
	Storage::iterator magnitudeIt = _storage->MagnitudeStorage().begin();
	for (unsigned i=0; i<_nFrames; i++)
		_frameDataIndex[i]=std::make_pair(&*positionIt++, &*magnitudeIt++);
}

bool CLAM::VM::PoolPeakDataSource::setCurrentTime(double timeMiliseconds)
{
	unsigned newFrame = _frameDivision ? _frameDivision->GetItem(timeMiliseconds*_samplingRate): 0;
	if (_nFrames==0) newFrame = 0;
	else if (newFrame>=_nFrames) newFrame=_nFrames-1;

	_nBins =  _nFrames ? _frameDataIndex[newFrame].first->size() : 0;
	_positionFrameData = _nBins ? &(*_frameDataIndex[newFrame].first)[0] : 0;
	_magnitudeFrameData = _nBins ? &(*_frameDataIndex[newFrame].second)[0] : 0;

	if (newFrame == _currentFrame) return false;
	_currentFrame = newFrame;
	return true;
}
