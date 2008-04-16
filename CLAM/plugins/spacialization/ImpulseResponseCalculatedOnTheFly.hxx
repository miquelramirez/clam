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
#include <CLAM/Filename.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <sstream>
#include <string>

namespace CLAM
{


struct BFormatIR
{
	ImpulseResponse W;
	ImpulseResponse X;
	ImpulseResponse Y;
	ImpulseResponse Z;

};
class ImpulseResponseCalculatedOnTheFly : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 7, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, Filename, Model3DFile);
		DYN_ATTRIBUTE( 2, public, unsigned, GridDivisions);
		DYN_ATTRIBUTE( 3, public, unsigned, NRays);
		DYN_ATTRIBUTE( 4, public, unsigned, NRebounds);
		DYN_ATTRIBUTE( 5, public, float, IrLength);
		DYN_ATTRIBUTE( 6, public, CLAM::Text, ExtraOptions);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFrameSize(512);
			SetModel3DFile("entorns/salo_simplificat.dat");
			SetGridDivisions(500);
			SetNRays(200);
			SetNRebounds(20);
			SetIrLength(1.0);
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
	OutPort< ImpulseResponse* > _vzImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _previousVzImpulseResponseOutPort;
	InControl _emitterX;
	InControl _emitterY;
	InControl _emitterZ;
	InControl _receiverX;
	InControl _receiverY;
	InControl _receiverZ;
	BFormatIR _impulseResponsesA;
	BFormatIR _impulseResponsesB;
	BFormatIR  * _current;
	BFormatIR  * _previous;
	float _currentEmitterX;
	float _currentEmitterY;
	float _currentEmitterZ;
	float _currentReceiverX;
	float _currentReceiverY;
	float _currentReceiverZ;
	float _delta;

public:
	const char* GetClassName() const { return "ImpulseResponseCalculatedOnTheFly"; }
	ImpulseResponseCalculatedOnTheFly(const Config& config = Config()) 
		: _pressureImpulseResponseOutPort("pressure IR", this)
		, _previousPressureImpulseResponseOutPort("previous pressure IR", this)
		, _vxImpulseResponseOutPort("vx IR", this)
		, _previousVxImpulseResponseOutPort("previous vx IR", this)
		, _vyImpulseResponseOutPort("vy IR", this)
		, _previousVyImpulseResponseOutPort("previous vy IR", this)
		, _vzImpulseResponseOutPort("vz IR", this)
		, _previousVzImpulseResponseOutPort("previous vz IR", this)
		, _emitterX("emitterX", this)
		, _emitterY("emitterY", this)
		, _emitterZ("emitterZ", this)
		, _receiverX("receiverX", this)
		, _receiverY("receiverY", this)
		, _receiverZ("receiverZ", this)
		, _current(0)
		, _previous(0)
		, _currentEmitterX(0)
		, _currentEmitterY(0)
		, _currentEmitterZ(0)
		, _currentReceiverX(0)
		, _currentReceiverY(0)
		, _currentReceiverZ(0)
		, _delta(1)
	{
		Configure( config );
		_emitterX.SetBounds(0,1);
		_emitterY.SetBounds(0,1);
		_emitterZ.SetBounds(0,1);
		_receiverX.SetBounds(0,1);
		_receiverY.SetBounds(0,1);
		_receiverZ.SetBounds(0,1);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		std::cout << "ImpulseResponseCalculatedOnTheFly::ConcreteConfigure"<<std::endl;
		CopyAsConcreteConfig(_config, config);
		_emitterX.DoControl(0.2);
		_emitterY.DoControl(0.5);
		_emitterZ.DoControl(0.2);
		_receiverX.DoControl(0.5);
		_receiverY.DoControl(0.5);
		_receiverZ.DoControl(0.2);
		_delta = 1./_config.GetGridDivisions();
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
		float z1 = _emitterZ.GetLastValue();
		float x2 = _receiverX.GetLastValue();
		float y2 = _receiverY.GetLastValue();
		float z2 = _receiverZ.GetLastValue();

		bool changeSnappedIR = fabs(_currentReceiverX-x2) > _delta 
			|| fabs(_currentReceiverY-y2) > _delta 
			|| fabs(_currentEmitterX-x1) > _delta
			|| fabs(_currentEmitterY-y1) > _delta
			;
//		std::cout << _currentReceiverX << " " << x2 << " " << fabs(_currentReceiverX-x1) << std::endl;
		
		std::string errorMsg;
		std::string wFile = "w_IR.wav";
		std::string xFile = "x_IR.wav";
		std::string yFile = "y_IR.wav";
		std::string zFile = "z_IR.wav";
//		std::cout << "IR : "<<x1<<","<<y1<<","<<z1<<" - "<<x2<<","<<y2<<","<<z2<<std::endl;
		std::cout << "." << std::flush;
		if (!_current or changeSnappedIR)
		{
			// swap _current but leave _previous
			_current = _current == &_impulseResponsesA ? &_impulseResponsesB : &_impulseResponsesA;
			if (not _previous) _previous = _current;
			_currentEmitterX = x1;
			_currentEmitterY = y1;
			_currentEmitterZ = z1;
			_currentReceiverX = x2;
			_currentReceiverY = y2;
			_currentReceiverZ = z2;
			std::cout << "|" << std::flush;
			std::ostringstream command;
			command << "visualitzador "
				<< " --compute-ir"
				<< " --model-file=" << _config.GetModel3DFile()
				<< " --listener-x-pos=" << _currentReceiverX
  				<< " --listener-y-pos=" << _currentReceiverY
  				<< " --listener-z-pos=" << _currentReceiverZ
  				<< " --source-x-pos=" << _currentEmitterX
				<< " --source-y-pos=" << _currentEmitterY 
				<< " --source-z-pos=" << _currentEmitterZ
				;
			if (_config.HasNRebounds()) command << " --num-rebounds=" <<  _config.GetNRebounds();
			if (_config.HasNRays()) command << " --num-rays=" << _config.GetNRays();
			if (_config.HasIrLength()) command << " --ir-length=" << _config.GetIrLength();
			if (_config.HasExtraOptions()) command << " " << _config.GetExtraOptions() << " ";
			command << " > /dev/null";
			// std::cout << command.str() << std::endl;
			int error = std::system( command.str().c_str() );
			if (error)
			{
				std::cout << "ERROR: ImpulseResponseCalculatedOnTheFly::Do at visualitzador execution!!" <<std::endl;
				std::cout << "Offending command:\n"<<  command.str() << std::endl;
				return false;
			}
			if (!computeResponseSpectrums(wFile, _current->W, _config.GetFrameSize(), errorMsg)
				|| !computeResponseSpectrums(xFile, _current->X, _config.GetFrameSize(), errorMsg)
				|| !computeResponseSpectrums(yFile, _current->Y , _config.GetFrameSize(), errorMsg) 
				|| !computeResponseSpectrums(zFile, _current->Z , _config.GetFrameSize(), errorMsg) )
			{
				std::cout << "ERROR: ImpulseResponseCalculatedOnTheFly::Do can not open IR files." << errorMsg << std::endl;
				return false;
			}
		}

		_pressureImpulseResponseOutPort.GetData()= &_current->W;
		_previousPressureImpulseResponseOutPort.GetData() = &_previous->W;
		_vxImpulseResponseOutPort.GetData()= &_current->X;
		_previousVxImpulseResponseOutPort.GetData()= &_previous->X;
		_vyImpulseResponseOutPort.GetData()= &_current->Y;
		_previousVyImpulseResponseOutPort.GetData()= &_previous->Y;
		_vzImpulseResponseOutPort.GetData()= &_current->Z;
		_previousVzImpulseResponseOutPort.GetData()= &_previous->Z;

		_pressureImpulseResponseOutPort.Produce();
		_previousPressureImpulseResponseOutPort.Produce();
		_vxImpulseResponseOutPort.Produce();
		_previousVxImpulseResponseOutPort.Produce();
		_vyImpulseResponseOutPort.Produce();
		_previousVyImpulseResponseOutPort.Produce();
		_vzImpulseResponseOutPort.Produce();
		_previousVzImpulseResponseOutPort.Produce();

		_previous=_current;
		return true;
	}
};

} // namespace CLAM


#endif // ImpulseResponseCalculatedOnTheFly_hxx

