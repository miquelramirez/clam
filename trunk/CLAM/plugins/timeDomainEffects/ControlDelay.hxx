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

#ifndef CONTROLDELAY_INCLUDED
#define CONTROLDELAY_INCLUDED

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include <vector>

namespace CLAM
{

class ControlDelay : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, float, Delay);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetDelay(50);
		}
	};
private:
	Config _config;
	FloatInControl _inControl;
	FloatOutControl _outControl;
	FloatInControl _delayControl;

	typedef long long unsigned Index;
	typedef std::vector<TControlData> DelayBuffer;
	DelayBuffer _delayBuffer;
	Index _delayBufferSize, _readIndex, _writeIndex;
	
	void InControlCallback(const TControlData & value)
	{
		TControlData delay = _delayControl.GetLastValue();
		setDelay(delay);

		_outControl.SendControl(delayLine(_inControl.GetLastValue()));		
	}

	void setDelay(float delaySamples) 
	{
		CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
		Index delayInSamples = round(delaySamples);
		
		if (delayInSamples > _delayBufferSize) 
			return;
		
		Index readIndex = (_writeIndex - delayInSamples) % _delayBufferSize ;

		if (_readIndex % _delayBufferSize == readIndex)
			return;
		
		_readIndex = readIndex;					
		return;
	}
	
	TControlData delayLine(TControlData x)
	{	
		CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
		Index writeindex = _writeIndex++ % _delayBufferSize;
		Index readindex = _readIndex++ % _delayBufferSize;

		_delayBuffer[writeindex] = x;
		TData y = _delayBuffer[readindex];
									
		return y;
	}


public:
	

	ControlDelay(const Config& config = Config()) 
		: _inControl("in control", this, &ControlDelay::InControlCallback) 
		, _outControl("out control", this)
		, _delayControl("Delay in Samples", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
						
			
		_delayBuffer.resize(65535);
		_delayBufferSize = _delayBuffer.size(); 
		_readIndex = _writeIndex = (_delayBufferSize-1); 
		std::fill(_delayBuffer.begin(), _delayBuffer.end(), 0.);

		//_inControl.DoControl(0.);
		_delayControl.DoControl(_config.GetDelay());

		return true;
	}


	const char* GetClassName() const { return "ControlDelay"; }

	virtual ~ControlDelay(){}

	const ProcessingConfig & GetConfig() const {return _config;}	

	bool Do() 
	{
		return true;
	}
 
};
} // namespace CLAM
#endif

