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
#include <CLAM/Pool.hxx>
#include <CLAM/Array.hxx>
#include "PoolFloatArrayDataSource.hxx"

CLAM::VM::PoolFloatArrayDataSource::PoolFloatArrayDataSource()
	: _nFrames(0)
	, _frameDivision(0)
	, _samplingRate(44100)
	, _frameData(0)
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
}

void CLAM::VM::PoolFloatArrayDataSource::setDataSource(const CLAM_Annotator::Project & project, const std::string & scope, const std::string & name)
{
	_name = name;
	_scope = scope;
	_project = & project;
	const std::list<std::string> & binLabels=
		project.GetAttributeScheme(scope,name).GetBinLabels();
	_binLabels.assign(binLabels.begin(), binLabels.end());
}

void CLAM::VM::PoolFloatArrayDataSource::updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)
{
	_frameData = 0;
	_samplingRate = samplingRate;
	_nFrames = data.GetNumberOfContexts(_scope);
	const CLAM_Annotator::SchemaAttribute & parent =
		_project->GetParentAttribute(_scope);
	_frameDivision = 
		data.GetReadPool<CLAM_Annotator::FrameDivision>(
			parent.GetScope(),
			parent.GetName()
		);
	const CLAM::DataArray * arrays =
		data.GetReadPool<CLAM::DataArray>(_scope,_name);
	unsigned nBins = _binLabels.size();
	_data.resize(_nFrames*nBins);
	for (unsigned frame =0; frame < _nFrames; frame++)
	{
		const CLAM::DataArray & array = arrays[frame];
		for (unsigned i=0; i<nBins; i++)
		{
			// TODO: This nBins is and adhoc hack for normalization
			double value = array[i]*nBins;
			_data[frame*nBins+i] = value;
		}
	}
	_frameData = &_data[0];
}

bool CLAM::VM::PoolFloatArrayDataSource::setCurrentTime(double timeMiliseconds)
{
	unsigned newFrame = _frameDivision ? _frameDivision->GetItem(timeMiliseconds*_samplingRate): 0;
	if (_nFrames==0) newFrame = 0;
	else if (newFrame>=_nFrames) newFrame=_nFrames-1;
	_frameData = getData()? getData()+_binLabels.size()*newFrame : 0;
	if (newFrame == _currentFrame) return false;
	_currentFrame = newFrame;
	return true;
}


