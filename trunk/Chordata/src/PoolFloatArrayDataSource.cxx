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
#include "PoolFloatArrayDataSource.hxx"

CLAM::VM::PoolFloatArrayDataSource::PoolFloatArrayDataSource()
	: _nFrames(0)
	, _frameDivision(0)
	, _samplingRate(44100)
	, _frameData(0)
	, _nBins(0)
	, _binGap(0)
	, _firstBinOffset(0)
	, _currentFrame(0)
{
}

void CLAM::VM::PoolFloatArrayDataSource::clearData()
{
	_nFrames=0;
	_frameDivision=0;
	_frameData=0;
	_currentFrame=0;
	_nBins = 0;
	_firstBinOffset = 0;
	_binGap = 0;
	_storage = 0;
}

void CLAM::VM::PoolFloatArrayDataSource::setDataSource(unsigned nBins, CLAM::TData binGap, CLAM::TData firstBinOffset, std::vector<std::string> binLabels)
{
	_nBins = nBins;
	_binGap = binGap;
	_firstBinOffset = firstBinOffset;
	_binLabels.assign(binLabels.begin(), binLabels.end());	
}

void CLAM::VM::PoolFloatArrayDataSource::setStorage(FloatVectorStorage *storage, CLAM::TData samplingRate, CLAM_Annotator::FrameDivision *frameDivision, unsigned nFrames)
{
	_storage = storage;
	_frameData = &_storage->Data()[0];
	_samplingRate = samplingRate;
	_nFrames = nFrames;
	_frameDivision = frameDivision;
}

bool CLAM::VM::PoolFloatArrayDataSource::setCurrentTime(double timeMiliseconds)
{
	unsigned newFrame = _frameDivision ? _frameDivision->GetItem(timeMiliseconds*_samplingRate): 0;
	if (_nFrames==0) newFrame = 0;
	else if (newFrame>=_nFrames) newFrame=_nFrames-1;
	_frameData = getData() ? getData() + (_nBins + unsigned(_binGap)) * newFrame + unsigned(_firstBinOffset) : 0;
	if (newFrame == _currentFrame) return false;
	_currentFrame = newFrame;
	return true;
}
