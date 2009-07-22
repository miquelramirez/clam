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

#ifndef RoomImpulseResponseSimulator_hxx
#define RoomImpulseResponseSimulator_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Filename.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include "Orientation.hxx" // SH definition
#include <vector>
#include <sstream>
#include <string>

#include <scene.hxx>

namespace CLAM
{

/**
 Simulates the impulse response from a source position to a listener point
 within a 3D acoustic environment.
 First order ambisonics components of the ImpulseResponse are computed.
 This processing is disabled in GPL binaries as it relies in a propietary
 raytracing library. Feel free to implement your own one.

 The processing detects when the "synchronization" input is silence to
 save computations meanwhile.
 Also a new impulse response is computed just when the positions move
 a given delta from the last computed one.
 Such a delta is equal to the full size in any dimension divided by
 the GridDivisions parameter.

 @todo Turn GridDivisions into a more meaningfull parameter (Hysteresis?)
 @todo Document RoomImpulseResponseSimulator
 @todo Document Model 3D format or providing a reference

 @param FrameSize [Config] The size used to split the ImpulseResponses
 @param Model3DFile [Config] The file containing the geometry to be simulated
 @param GridDivisions [Config] The number of divisions of the full space
 @param NRays [Config] Number of rays to cast
 @param NRebounds [Config] Number of rebounds to consider for each ray
 @param IrLength [Config] length in seconds of the impulse response (0 for no limit)
 @param Order [Config] Order of Ambisonics to generate the output (0,1,2 or 3)
 @param ExtraOptions [Config] deprecated parameter (for backward compatibility with commandline version)
 @param SeparateDirectSoundAndReverb [Config] When true direct sound won't be on the impulse response, direct sounds related controls are still sent
 @param SupressInitialDelay [Config] If true, sample 0 of the impulse response matches the direct sound time.
 @param[in] "synchronization" [Port] The audio to be convolved just to detect silences
 @param[in] "source X" [Control] X coordinate of the sound source (normalized [0..1])
 @param[in] "source Y" [Control] Y coordinate of the sound source (normalized [0..1])
 @param[in] "source Z" [Control] Z coordinate of the sound source (normalized [0..1])
 @param[in] "listener X" [Control] X coordinate of the listener (normalized [0..1])
 @param[in] "listener Y" [Control] Y coordinate of the listener (normalized [0..1])
 @param[in] "listener Z" [Control] Z coordinate of the listener (normalized [0..1])
 @param[out] "W IR" [Port] The W component of the impulse response
 @param[out] "X IR" [Port] The X component of the impulse response
 @param[out] "Y IR" [Port] The Y component of the impulse response
 @param[out] "Z IR" [Port] The Z component of the impulse response
 @param[out] "directSoundPressure" [Control] The amplification value for the direct sound
 @see RoomImpulseResponseSimulatorCommandLine, ImpulseResponseDatabaseFetcher, AmbisonicsConventions
 @ingroup SpatialAudio
*/
class RoomImpulseResponseSimulator : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 10, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, InFilename, Model3DFile);
		DYN_ATTRIBUTE( 2, public, unsigned, GridDivisions);
		DYN_ATTRIBUTE( 3, public, unsigned, NRays);
		DYN_ATTRIBUTE( 4, public, unsigned, NRebounds);
		DYN_ATTRIBUTE( 5, public, float, IrLength);
		DYN_ATTRIBUTE( 6, public, unsigned, Order);
		DYN_ATTRIBUTE( 7, public, CLAM::Text, ExtraOptions);
		DYN_ATTRIBUTE( 8, public, bool, SeparateDirectSoundAndReverb);
		DYN_ATTRIBUTE( 9, public, bool, SupressInitialDelay);
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
			SetOrder(1);
			SetSeparateDirectSoundAndReverb(false);
			SetSupressInitialDelay(false);
		}
	};
	typedef std::vector<ImpulseResponse> HoaIR;

private:
	enum {nCachedIRs=3};
	Config _config;
	AudioInPort _syncAudio;
	typedef OutPort<ImpulseResponse*> IROutPort;
	typedef std::vector< IROutPort* > ImpulseResponseOutPorts;
	ImpulseResponseOutPorts _outputs;
	FloatInControl _sourceX;
	FloatInControl _sourceY;
	FloatInControl _sourceZ;
	FloatInControl _listenerX;
	FloatInControl _listenerY;
	FloatInControl _listenerZ;
	FloatOutControl _directSoundPressure;
	HoaIR _impulseResponses[nCachedIRs];
	HoaIR  * _current;
	HoaIR  * _previous;
	float _currentSourceX;
	float _currentSourceY;
	float _currentSourceZ;
	float _currentListenerX;
	float _currentListenerY;
	float _currentListenerZ;
	float _delta;
	unsigned _currentCacheIndex;
	Scene * _scene;

public:
	const char* GetClassName() const { return "RoomImpulseResponseSimulator"; }
	RoomImpulseResponseSimulator(const Config& config = Config()) 
		: _syncAudio("synchronization", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)
		, _listenerX("listener X", this)
		, _listenerY("listener Y", this)
		, _listenerZ("listener Z", this)
		, _directSoundPressure("directSoundPressure", this)
		, _current(0)
		, _previous(0)
		, _currentSourceX(0)
		, _currentSourceY(0)
		, _currentSourceZ(0)
		, _currentListenerX(0)
		, _currentListenerY(0)
		, _currentListenerZ(0)
		, _delta(1)
		, _currentCacheIndex(0)
		, _scene(0)
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		if (not _config.HasOrder())
		{
			_config.AddOrder();
			_config.UpdateData();
			_config.SetOrder(1);
		}
		unsigned order = _config.GetOrder();
		if (order>3) return AddConfigErrorMessage(
			"Ambisonics orders beyond 3rd are not supported");
		SphericalHarmonicsDefinition *sh = Orientation::sphericalHarmonics();
		unsigned i=0;
		for (;sh[i].name; i++)
		{
			if (sh[i].order > order) break;
			if (i<_outputs.size()) continue;
			IROutPort * port = new IROutPort(sh[i].name+std::string(" IR"), this);
			_outputs.push_back( port );
		}
		unsigned actualSize=i;
		for (;i<_outputs.size(); i++)
			delete _outputs[i];
		_outputs.resize(actualSize);

		_syncAudio.SetSize(_config.GetFrameSize());
		_syncAudio.SetHop(_config.GetFrameSize());
		_sourceX.DoControl(0.2);
		_sourceY.DoControl(0.5);
		_sourceZ.DoControl(0.2);
		_listenerX.DoControl(0.5);
		_listenerY.DoControl(0.5);
		_listenerZ.DoControl(0.2);
		_delta = 1./_config.GetGridDivisions();

		std::ifstream modelcheck( _config.GetModel3DFile().c_str() );
		if ( not modelcheck.is_open() )
		{
			std::string err("Could not open 3D model file: "); 
			err += _config.GetModel3DFile();
			AddConfigErrorMessage(err);
			return false;
		}

		Settings settings;
		settings.repeatable = true; // TODO: It should be configurable
		settings.extra_bound = 0.5;
		settings.human_height = 1.70;
		settings.model_filename = _config.GetModel3DFile();
		settings.num_rays = _config.GetNRays();
		settings.num_rebounds = _config.GetNRebounds();
		settings.physical_output = 0;
		settings.ir_length = _config.GetIrLength();
		CLAM_WARNING(_config.HasExtraOptions() and _config.GetExtraOptions() != "",
			"Extra options not supported");
		settings.ignore_sources_models = true;
		settings.use_osc = false;
		settings.use_dat = false;
		settings.hoa_order = _config.HasOrder()?_config.GetOrder():1;

		if (_scene) delete _scene;
		_scene = new Scene(settings);

		return true;
	}
	~RoomImpulseResponseSimulator()
	{
		if (_scene) delete _scene;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	static unsigned map(FloatInControl & control, unsigned limit)
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

		float x1 = _sourceX.GetLastValue();
		float y1 = _sourceY.GetLastValue();
		float z1 = _sourceZ.GetLastValue();
		float x2 = _listenerX.GetLastValue();
		float y2 = _listenerY.GetLastValue();
		float z2 = _listenerZ.GetLastValue();

		bool changeSnappedIR
			=  fabs(_currentListenerX-x2) > _delta 
			|| fabs(_currentListenerY-y2) > _delta 
			|| fabs(_currentListenerZ-z2) > _delta 
			|| fabs(_currentSourceX-x1) > _delta
			|| fabs(_currentSourceY-y1) > _delta
			|| fabs(_currentSourceZ-z1) > _delta
			;

		// If we already have one but the movement is small enough, keep it
		if (_current and not changeSnappedIR) return true;

//		std::cout << _currentListenerX << " " << x2 << " " << fabs(_currentListenerX-x1) << std::endl;
		
//		std::cout << "IR : "<<x1<<","<<y1<<","<<z1<<" - "<<x2<<","<<y2<<","<<z2<<std::endl;
		// swap _current but leave _previous
		_current = &_impulseResponses[_currentCacheIndex];
		_currentCacheIndex = (_currentCacheIndex+1) % nCachedIRs;
		if (not _previous) _previous = _current;
		_currentSourceX = x1;
		_currentSourceY = y1;
		_currentSourceZ = z1;
		_currentListenerX = x2;
		_currentListenerY = y2;
		_currentListenerZ = z2;
		std::cout << "|" << std::flush;
		_scene->setReceiverPos(_currentListenerX,_currentListenerY,_currentListenerZ);
		_scene->setComputedSource(0);
		_scene->clearSources();
		_scene->appendSource(_currentSourceX,_currentSourceY,_currentSourceZ);
		std::string responsesPath = "";
		double directSoundPressure = 0;
		unsigned initialDelay = 0;
		unsigned offsetToStrip = 0;
		if (_config.GetSeparateDirectSoundAndReverb())
		{

			_scene->computeDirectSoundOverTime();
			directSoundPressure = _scene->getDirectSoundPressure();
			initialDelay = _scene->getDirectSoundDelayInSamples();
			_scene->raytracingReverbOverTime(responsesPath, "IR" );
		}
		else
		{
			_scene->raytracingOverTime(responsesPath, "IR" );
			initialDelay = _scene->getDirectSoundDelayInSamples();
			directSoundPressure = _scene->getDirectSoundPressure();
		}

		if ( _config.GetSupressInitialDelay())
		{
			offsetToStrip = initialDelay;
		}
#if 0 // for debugging TODO remove
		std::cout << "IR size " << _scene->getTimeResponse_P().size() << std::endl;
		std::cout << "offset to strip " << offsetToStrip << std::endl;
		for (unsigned i=offsetToStrip; i<offsetToStrip+1000; i++)
			std::cout << _scene->getTimeResponse_P()[i] << " ";
		std::cout << std::endl;
#endif

		std::string errorMsg;
		SphericalHarmonicsDefinition *sh = Orientation::sphericalHarmonics();
		_current->resize(_outputs.size());
		const unsigned sampleRate=44100;
		for (unsigned component = 0; component < _outputs.size(); component++)
		{
			const std::vector<double> & response =
				_scene->getTimeResponse(sh[component].order, sh[component].zProjection*sh[component].sign);
			if (computeResponseSpectrums(response, (*_current)[component], _config.GetFrameSize(), errorMsg, sampleRate, offsetToStrip)) continue;
			std::cout << "ERROR: RoomImpulseResponseSimulator::Do can not open IR files.\n" << errorMsg << std::endl;
			return false;
		}

		_directSoundPressure.SendControl(directSoundPressure);
		return true;
	}
public:
	bool Do()
	{
		bool ok = computeNewIRIfNeeded();
		CLAM_ASSERT(ok, "Error reading the IR");

		if (not _previous) _previous = _current;
		for (unsigned component = 0; component < _outputs.size(); component++)
			_outputs[component]->GetData() = &(*_current)[component];

		_syncAudio.Consume();
		for (unsigned component = 0; component < _outputs.size(); component++)
			_outputs[component]->Produce();
		
		_previous=_current;
		return true;
	}
};

} // namespace CLAM


#endif // RoomImpulseResponseSimulator_hxx

