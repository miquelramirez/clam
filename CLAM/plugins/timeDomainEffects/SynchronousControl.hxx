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

#ifndef SynchronousControl_hxx
#define SynchronousControl_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/AudioInPort.hxx>
#include <cmath>

namespace CLAM
{

class SynchronousControl : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 1, public, unsigned, ControlsPerSecond);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();		
			SetSampleRate(48000);		
			SetControlsPerSecond(24);
		};
	};

	Config _config;
	FloatInControl _inControl;
	FloatOutControl _outControl;
	InPort<Audio> _syncIn;

	unsigned _samplesPerControl;
	unsigned _sampleCount;
	unsigned _frameSize;

public:
	SynchronousControl(const Config& config = Config()) 
		: _inControl("in control", this)
		, _outControl("out control",this) 
		, _syncIn("sync", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_frameSize=BackendBufferSize();
		_samplesPerControl = _config.GetSampleRate()/_config.GetControlsPerSecond();
		_sampleCount=0;
		
		_inControl.DoControl(0.);
		
		return true;
	}
	
	const char* GetClassName() const { return "SynchronousControl"; }

	virtual ~SynchronousControl(){}

	const CLAM::ProcessingConfig & GetConfig() const { return _config; }

	bool Do()
	{
		_sampleCount += _frameSize;
		if (_sampleCount>=_samplesPerControl)
		{
			_outControl.SendControl(_inControl.GetLastValue());
			_sampleCount -= _samplesPerControl;
		}
		_syncIn.Consume();
		return true;
	}
	
};

} //namespace
#endif
