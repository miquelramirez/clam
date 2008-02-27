/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef ImpulseResponseCalculatedOnTheFly_hxx
#define ImpulseResponseCalculatedOnTheFly_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/DirectoryName.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <sstream>

namespace CLAM
{


struct BFormatIR
{
	ImpulseResponse pressure;
	ImpulseResponse vx;
	ImpulseResponse vy;

};
class ImpulseResponseCalculatedOnTheFly : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, DirectoryName, Path);
		DYN_ATTRIBUTE( 2, public, std::string, Prefix);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFrameSize(512);
		};
	};
private:
	
	Config _config;
	OutPort< ImpulseResponse* > _pressureImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _previousPressureImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _vxImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _previousVxImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _vyImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _previousVyImpulseResponseOutPort;
	InControl _emitterX;
	InControl _emitterY;
	InControl _receiverX;
	InControl _receiverY;
	BFormatIR _impulseResponsesA;
	BFormatIR _impulseResponsesB;
	BFormatIR  * _current;
	BFormatIR  * _previous;
	float _currentEmitterX;
	float _currentEmitterY;
	float _currentReceiverX;
	float _currentReceiverY;

public:
	const char* GetClassName() const { return "ImpulseResponseCalculatedOnTheFly"; }
	ImpulseResponseCalculatedOnTheFly(const Config& config = Config()) 
		: _pressureImpulseResponseOutPort("pressure IR", this)
		, _previousPressureImpulseResponseOutPort("previous pressure IR", this)
		, _vxImpulseResponseOutPort("vx IR", this)
		, _previousVxImpulseResponseOutPort("previous vx IR", this)
		, _vyImpulseResponseOutPort("vy IR", this)
		, _previousVyImpulseResponseOutPort("previous vy IR", this)
		, _emitterX("emitterX", this)
		, _emitterY("emitterY", this)
		, _receiverX("receiverX", this)
		, _receiverY("receiverY", this)
		, _current(0)
		, _previous(0)
		, _currentEmitterX(0)
		, _currentEmitterY(0)
		, _currentReceiverX(0)
		, _currentReceiverY(0)
	{
		Configure( config );
		_emitterX.SetBounds(0,1);
		_emitterY.SetBounds(0,1);
		_receiverX.SetBounds(0,1);
		_receiverY.SetBounds(0,1);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		std::cout << "ImpulseResponseCalculatedOnTheFly::ConcreteConfigure"<<std::endl;
		CopyAsConcreteConfig(_config, config);
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	static unsigned map(InControl & control, unsigned limit)
	{
		unsigned result = unsigned(std::floor(control.GetLastValue()*limit));
		return result < limit? result : limit-1;
	}
	bool Do()
	{
		float x1 = _emitterX.GetLastValue();
		float y1 = _emitterY.GetLastValue();
		float x2 = _receiverX.GetLastValue();
		float y2 = _receiverY.GetLastValue();
		float z1 = 0.;
		float z2 = 0.;

		float delta = 0.0020;
		bool changeSnappedIR = fabs(_currentReceiverX-x2) > delta 
			|| fabs(_currentReceiverY-y2) > delta 
			|| fabs(_currentEmitterX-x1) > delta
			|| fabs(_currentEmitterY-y1) > delta
			;
//		std::cout << _currentReceiverX << " " << x2 << " " << fabs(_currentReceiverX-x1) << std::endl;
		
		std::string errorMsg;
		const std::string path = "/home/clamteam/acustica/visualitzador_escena_c++/responses/"; //TODO not abs path
		std::string pressureFile = path+"p_positioned_IR_time.wav";
		std::string vxFile = path+"vx_positioned_IR_time.wav";
		std::string vyFile = path+"vy_positioned_IR_time.wav";
//		std::cout << "IR : "<<x1<<","<<y1<<","<<z1<<" - "<<x2<<","<<y2<<","<<z2<<std::endl;
		std::cout << "." << std::flush;
		if (!_current or changeSnappedIR)
		{
			// swap _current but leave _previous
			_current = _current == &_impulseResponsesA ? &_impulseResponsesB : &_impulseResponsesA;
			if (not _previous) _previous = _current;
			_currentEmitterX = x1;
			_currentEmitterY = y1;
			_currentReceiverX = x2;
			_currentReceiverY = y2;
			std::cout << "|" << std::flush;
			std::ostringstream command;
			command << "(cd ~/acustica/visualitzador_escena_c++ && ./visualitzador "
				<< " --listener-x-pos=" << _currentReceiverX
  				<< " --listener-y-pos=" << _currentReceiverY
  				<< " --source-x-pos=" << _currentEmitterX
				<< " --source-y-pos=" << _currentEmitterY 
				<< " > /dev/null )";
			std::system( command.str().c_str() );
			if (!computeResponseSpectrums(pressureFile, _current->pressure, _config.GetFrameSize(), errorMsg)
				|| !computeResponseSpectrums(vxFile, _current->vx, _config.GetFrameSize(), errorMsg)
				|| !computeResponseSpectrums(vyFile, _current->vy , _config.GetFrameSize(), errorMsg) )
			{
				std::cout << "Error: ImpulseResponseCalculatedOnTheFly::Do " << errorMsg << std::endl;
			}
		}

		_pressureImpulseResponseOutPort.GetData()= &_current->pressure;
		_previousPressureImpulseResponseOutPort.GetData() = &_previous->pressure;
		_vxImpulseResponseOutPort.GetData()= &_current->vx;
		_previousVxImpulseResponseOutPort.GetData()= &_previous->vx;
		_vyImpulseResponseOutPort.GetData()= &_current->vy;
		_previousVyImpulseResponseOutPort.GetData()= &_previous->vy;

		_pressureImpulseResponseOutPort.Produce();
		_previousPressureImpulseResponseOutPort.Produce();
		_vxImpulseResponseOutPort.Produce();
		_previousVxImpulseResponseOutPort.Produce();
		_vyImpulseResponseOutPort.Produce();
		_previousVyImpulseResponseOutPort.Produce();

		_previous=_current;
		return true;
	}
};

} // namespace CLAM


#endif // ImpulseResponseCalculatedOnTheFly_hxx

