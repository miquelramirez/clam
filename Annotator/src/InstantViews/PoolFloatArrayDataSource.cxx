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

#include "FrameDivision.hxx"
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
	_data.resize(0);
	_nFrames=0;
	_frameDivision=0;
	_frameData=0;
	_currentFrame=0;
	_nBins = 0;
	_firstBinOffset = 0;
	_binGap = 0;
}

void CLAM::VM::PoolFloatArrayDataSource::setDataSource(const CLAM_Annotator::Project & project, const std::string & scope, const std::string & name)
{
	_name = name;
	_scope = scope;
	const CLAM_Annotator::SchemaAttribute & attribute =
		project.GetAttributeScheme(scope,name);
	const CLAM_Annotator::SchemaAttribute & parent =
		project.GetParentAttribute(scope);
	_parentName = parent.GetName();
	_parentScope = parent.GetScope();

	if (attribute.HasBinLabels())
	{
		const std::list<std::string> & binLabels=
			attribute.GetBinLabels();
		_binLabels.assign(binLabels.begin(), binLabels.end());
	}
	
	// default nBins to binLabels.size if NBins attribute isn't set
	_nBins = attribute.HasNBins() ? attribute.GetNBins() : _binLabels.size();

	// default FirstBinOffset to 0 if it wasn't set
	_firstBinOffset = attribute.HasFirstBinOffset()? attribute.GetFirstBinOffset() : 0;
	
	_binGap = attribute.HasBinGap()? attribute.GetBinGap() : 0;
	
}

void CLAM::VM::PoolFloatArrayDataSource::updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)
{
	_frameData = 0;
	_samplingRate = samplingRate;
	_nFrames = data.GetNumberOfContexts(_scope);
	_frameDivision = 
		data.GetReadPool<CLAM_Annotator::FrameDivision>(_parentScope, _parentName);
	const CLAM::DataArray * arrays =
		data.GetReadPool<CLAM::DataArray>(_scope,_name);

	_data.resize(_nFrames*_nBins);
	for (unsigned frame =0; frame < _nFrames; frame++)
	{
		const CLAM::DataArray & array = arrays[frame];
		for (unsigned i=0; i<_nBins; i++)
		{
			// TODO: This nBins is and adhoc hack for normalization
			double value = array[i]*_nBins;
			_data[frame*_nBins+i] = value;
		}
	}
	_frameData = &_data[0];
}

bool CLAM::VM::PoolFloatArrayDataSource::setCurrentTime(double timeMiliseconds)
{
	unsigned newFrame = _frameDivision ? _frameDivision->GetItem(timeMiliseconds*_samplingRate): 0;
	if (_nFrames==0) newFrame = 0;
	else if (newFrame>=_nFrames) newFrame=_nFrames-1;
	_frameData = getData()? getData()+_nBins*newFrame : 0;
	if (newFrame == _currentFrame) return false;
	_currentFrame = newFrame;
	return true;
}


