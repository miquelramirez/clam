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
#include <CLAM/AudioInPort.hxx>
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
		DYNAMIC_TYPE_USING_INTERFACE( Config, 9, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, InFilename, Model3DFile);
		DYN_ATTRIBUTE( 2, public, unsigned, GridDivisions);
		DYN_ATTRIBUTE( 3, public, unsigned, NRays);
		DYN_ATTRIBUTE( 4, public, unsigned, NRebounds);
		DYN_ATTRIBUTE( 5, public, float, IrLength);
		DYN_ATTRIBUTE( 6, public, CLAM::Text, ExtraOptions);
		DYN_ATTRIBUTE( 7, public, bool, StripDirectSound);
		DYN_ATTRIBUTE( 8, public, bool, SupressInitialDelay);
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
			SetStripDirectSound(false);
			SetSupressInitialDelay(false);
		};
	};
private:
	enum {nCachedIRs=3};
	Config _config;
	AudioInPort _syncAudio;
	OutPort< ImpulseResponse* > _WImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _XImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _YImpulseResponseOutPort;
	OutPort< ImpulseResponse* > _ZImpulseResponseOutPort;
	InControl _emitterX;
	InControl _emitterY;
	InControl _emitterZ;
	InControl _receiverX;
	InControl _receiverY;
	InControl _receiverZ;
	BFormatIR _impulseResponses[nCachedIRs];
	BFormatIR  * _current;
	BFormatIR  * _previous;
	float _currentEmitterX;
	float _currentEmitterY;
	float _currentEmitterZ;
	float _currentReceiverX;
	float _currentReceiverY;
	float _currentReceiverZ;
	float _delta;
	int _irCount;
	unsigned _currentCacheIndex;

public:
	const char* GetClassName() const { return "ImpulseResponseCalculatedOnTheFly"; }
	ImpulseResponseCalculatedOnTheFly(const Config& config = Config()) 
		: _syncAudio("synchronization", this)
		, _WImpulseResponseOutPort("pressure IR", this)
		, _XImpulseResponseOutPort("vx IR", this)
		, _YImpulseResponseOutPort("vy IR", this)
		, _ZImpulseResponseOutPort("vz IR", this)
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
		, _irCount(0)
		, _currentCacheIndex(0)
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
		//TODO use _config params instead of hardcoded
		_syncAudio.SetSize(512);
		_syncAudio.SetHop(512);
		//end TODO
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
private:
	bool sincAudioIsSilence()
	{
		const CLAM::DataArray& audio = _syncAudio.GetAudio().GetBuffer();
		for (int i=0; i<audio.Size(); i++)
			if (audio[i]>0.00001) return false;
		return true;
	}
	bool computeNewIRIfNeeded()
	{
		// If we already have one and we are in silence, keep it
		if (_current and sincAudioIsSilence())
		{
			std::cout << "_" << std::flush;
			return true;
		}
		std::cout << "." << std::flush;

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

		// If we already have one but the movement is small enough, keep it
		if (_current and not changeSnappedIR) return true;

//		std::cout << _currentReceiverX << " " << x2 << " " << fabs(_currentReceiverX-x1) << std::endl;
		
//		std::cout << "IR : "<<x1<<","<<y1<<","<<z1<<" - "<<x2<<","<<y2<<","<<z2<<std::endl;
		// swap _current but leave _previous
		_current = &_impulseResponses[_currentCacheIndex];
		_currentCacheIndex = (_currentCacheIndex+1) % nCachedIRs;
		if (not _previous) _previous = _current;
		_currentEmitterX = x1;
		_currentEmitterY = y1;
		_currentEmitterZ = z1;
		_currentReceiverX = x2;
		_currentReceiverY = y2;
		_currentReceiverZ = z2;
		std::cout << "|" << std::flush;
		std::ostringstream command;
		command << "raytracing "
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
		if (_config.HasStripDirectSound() and _config.GetStripDirectSound()) command << " --reverb";
		command << " > /dev/null";
		// std::cout << command.str() << std::endl;
		int error = std::system( command.str().c_str() );
		if (error)
		{
			std::cout << "ERROR: ImpulseResponseCalculatedOnTheFly::Do at visualitzador execution!!" <<std::endl;
			std::cout << "Offending command:\n"<<  command.str() << std::endl;
			return false;
		}
		std::string wFile = "w_IR.wav";
		std::string xFile = "x_IR.wav";
		std::string yFile = "y_IR.wav";
		std::string zFile = "z_IR.wav";
		std::string wFileTrimmed = "w_IR_trimmed.wav";
		std::string xFileTrimmed = "x_IR_trimmed.wav";
		std::string yFileTrimmed = "y_IR_trimmed.wav";
		std::string zFileTrimmed = "z_IR_trimmed.wav";
		std::string fileW = wFile;
		std::string fileX = xFile;
		std::string fileY = yFile;
		std::string fileZ = zFile;
		if (_config.HasSupressInitialDelay() and _config.GetSupressInitialDelay()) // Trim initial silences with sox. Can be safely desabled
		{
			std::system( ("sox -t wav " + wFile + " -t wav " + wFileTrimmed + " silence 1, 0.00000001, 0.01 ").c_str() );
			std::system( ("sox -t wav " + xFile + " -t wav " + xFileTrimmed + " silence 1, 0.00000001, 0.01 ").c_str() );
			std::system( ("sox -t wav " + yFile + " -t wav " + yFileTrimmed + " silence 1, 0.00000001, 0.01 ").c_str() );
			std::system( ("sox -t wav " + zFile + " -t wav " + zFileTrimmed + " silence 1, 0.00000001, 0.01 ").c_str() );
			std::cout << " ------------------- trimming ------------------- " << std::endl;
			fileW = wFileTrimmed;
			fileX = xFileTrimmed;
			fileY = yFileTrimmed;
			fileZ = zFileTrimmed;
		}
		std::string errorMsg;
		if (!computeResponseSpectrums(fileW, _current->W, _config.GetFrameSize(), errorMsg)
			|| !computeResponseSpectrums(fileX, _current->X, _config.GetFrameSize(), errorMsg)
			|| !computeResponseSpectrums(fileY, _current->Y , _config.GetFrameSize(), errorMsg) 
			|| !computeResponseSpectrums(fileZ, _current->Z , _config.GetFrameSize(), errorMsg) )
		{
			std::cout << "ERROR: ImpulseResponseCalculatedOnTheFly::Do can not open IR files.\n" << errorMsg << std::endl;
			return false;
		}
		if (false) // save all IRs
		{
			std::cout << "saving IR "<<_irCount<<std::endl;
			std::ostringstream os;
			os << "cp "<<wFile<<" "<<wFile<<_irCount<<".wav";
			std::system(os.str().c_str());
			_irCount++;
		}
		return true;
	}
public:
	bool Do()
	{
		bool ok = computeNewIRIfNeeded();
		CLAM_ASSERT(ok, "Error reading the IR");

		if (not _previous) _previous = _current;

		_WImpulseResponseOutPort.GetData()= &_current->W;
		_XImpulseResponseOutPort.GetData()= &_current->X;
		_YImpulseResponseOutPort.GetData()= &_current->Y;
		_ZImpulseResponseOutPort.GetData()= &_current->Z;

		_syncAudio.Consume();
		_WImpulseResponseOutPort.Produce();
		_XImpulseResponseOutPort.Produce();
		_YImpulseResponseOutPort.Produce();
		_ZImpulseResponseOutPort.Produce();
		
		_previous=_current;
		return true;
	}
};

} // namespace CLAM


#endif // ImpulseResponseCalculatedOnTheFly_hxx

