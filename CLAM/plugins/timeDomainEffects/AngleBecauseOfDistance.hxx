/*
 * Copyright (c) 2010 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef ANGLEBECAUSEOFDISTANCE_INCLUDED
#define ANGLEBECAUSEOFDISTANCE_INCLUDED

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include <vector>

namespace CLAM
{

class AngleBecauseOfDistance : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, float, SourceSize);
		DYN_ATTRIBUTE( 1, public, float, SourceDistance);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetSourceSize(1.0);
			SetSourceDistance(1.0);
		}
	};
private:
	Config _config;
	FloatInControl _sourceSize;
	FloatInControl _sourceDistance;
	FloatOutControl _sourceThickness;
	
	void InControlCallback(const TControlData & value)
	{
		TControlData size = _sourceSize.GetLastValue();
		TControlData distance = _sourceDistance.GetLastValue();
		if (distance == 0) return;
		float angle=(180.0/M_PI)*(size/distance);
		//distance < 0.001 ? angle=(180.0/M_PI)*(size/0.001) : angle=(180.0/M_PI)*(size/distance);
		if (angle < 4.5)  angle=4.5;		
		(angle > 360) ? _sourceThickness.SendControl(360.0):_sourceThickness.SendControl(angle);		
	}

public:
	

	AngleBecauseOfDistance(const Config& config = Config()) 
		: _sourceSize("source Size in mts", this, &AngleBecauseOfDistance::InControlCallback) 
		, _sourceDistance("source Distance in mts", this, &AngleBecauseOfDistance::InControlCallback) 
		, _sourceThickness("source thickness", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
		
		_sourceSize.SetBounds(0.001, 100.0);
		_sourceDistance.SetBounds(0.0, 50.0);

		_sourceSize.DoControl( _config.GetSourceSize() );		
		_sourceDistance.DoControl( _config.GetSourceDistance() );
		
		return true;
	}


	const char* GetClassName() const { return "AngleBecauseOfDistance"; }

	virtual ~AngleBecauseOfDistance(){}

	const ProcessingConfig & GetConfig() const {return _config;}	

	bool Do() 
	{
		return true;
	}
 
};
} // namespace CLAM
#endif

