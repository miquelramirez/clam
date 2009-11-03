/*
 * Copyright (c) 2009 Fundacio Barcelona Media Universitat Pompeu Fabra.
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
#include "ControlIntervalMapper.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlIntervalMapper",
		"category", "Controls",
		"description", "Receives a control and maps it using two intervals, the interval of the incoming value and the interval of the new value",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlIntervalMapper> reg = metadata;
}

void ControlIntervalMapperConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetInputMin( 0.0 );
	SetInputMax( 1.0 );
	SetOutputMin( 0.0 );
	SetOutputMax( 1.0 );
}

ControlIntervalMapper::ControlIntervalMapper()
	: _inControl( "input_control", this , &ControlIntervalMapper::InControlCallback )
	, _outControl( "mapped_control", this )
	, _min(0.)
	, _max(1.)
	, _newmin(0.)
	, _newmax(1.)
{
	Configure( _config );	
}

ControlIntervalMapper::ControlIntervalMapper( const ControlIntervalMapperConfig& cfg ) 
	: _inControl( "input_control", this , &ControlIntervalMapper::InControlCallback )
	, _outControl( "mapped_control", this )
	, _min(0.)
	, _max(1.)
	, _newmin(0.)
	, _newmax(1.)
{ 
	Configure( cfg );
}

bool ControlIntervalMapper::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( _config, cfg );
	_min = _config.GetInputMin();
	_max = _config.GetInputMax();
	_newmin = _config.GetOutputMin();
	_newmax = _config.GetOutputMax();
	_inControl.SetBounds(_min,_max);
	return true; 		
}

void ControlIntervalMapper::InControlCallback(const TControlData & value)
{
	TControlData newval = (( value - _min) / (_max - _min)) * 
		( _newmax - _newmin) + _newmin;
	_outControl.SendControl( newval );
}

bool ControlIntervalMapper::Do()
{
	return true;
}
}

