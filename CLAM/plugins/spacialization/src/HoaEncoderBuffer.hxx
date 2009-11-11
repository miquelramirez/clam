#ifndef HoaEncoderBuffer_hxx
#define HoaEncoderBuffer_hxx
#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>
#include "Orientation.hxx"

/**
 Encodes an audio signal in Higher Order Ambisonics (HOA).
 Considers the audio a plane wave coming from the direction
 indicated by azimuth and elevation controls (in degrees).
 
 @param[in] Order [Config] The Ambisonics order to generate [0,1,2,3]
 @param[in] UseFuMa [Config] Whether to normalize by the Fu-Ma factors.
 @param[in] Input [Port] Audio to be encoded.
 @param[out] XX [Port] Ambisonics component were XX is one of W,X,Y,Z,R,S,T,U,V,K,L,M,N,O,P,Q.
 @param[in] azimuth [Control] Azimuth in degrees acording #AmbisonicsConventions
 @param[in] elevation [Control] Elevation in degrees acording #AmbisonicsConventions
 @ingroup SpatialAudio
 @see AmbisonicsConventions
*/

class HoaEncoderBuffer : public CLAM::Processing
{
	typedef std::vector<CLAM::OutPort<CLAM::Audio>*> OutPorts;

	CLAM::InPort<CLAM::Audio> _input;	// input with Buffer
	OutPorts _outputs;			// output with Buffer

	CLAM::FloatInControl _azimuth;
	CLAM::FloatInControl _elevation;

public:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE(Config,2,ProcessingConfig);
		DYN_ATTRIBUTE(0, public, unsigned, Order);
		DYN_ATTRIBUTE(1, public, bool, UseFuMa);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetOrder(1);
			SetUseFuMa(true);
		};
	};
private:
	Config _config;

public:
	const char* GetClassName() const { return "HoaEncoderBuffer"; }
	HoaEncoderBuffer(const Config& config = Config()) 
		: _input("Input", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90);
		_azimuth.SetDefaultValue(0);
		_elevation.SetDefaultValue(0);
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);
		unsigned order = _config.GetOrder();
		if (order>3) return AddConfigErrorMessage(
			"Ambisonics orders beyond 3rd are not supported");
		CLAM::SphericalHarmonicsDefinition *sh = CLAM::Orientation::sphericalHarmonics();
		unsigned i=0;
		for (;sh[i].name; i++)
		{
			if (sh[i].order > order) break;
			if (i<_outputs.size()) continue;
			CLAM::OutPort<CLAM::Audio> * port = new CLAM::OutPort<CLAM::Audio>( sh[i].name, this);
			port->SetSize(1);
			port->SetHop(1);
			_outputs.push_back( port );
		}
		unsigned actualSize=i;
		for (;i<_outputs.size(); i++)
			delete _outputs[i];
		_outputs.resize(actualSize);

		_elevation.DoControl(0.);
		_azimuth.DoControl(0.);
		_input.SetSize(1);
		_input.SetHop(1);
		return true;
	}
	
	const CLAM::ProcessingConfig & GetConfig() const { return _config; }

	bool Do()
	{
		CLAM::Orientation incidence(_azimuth.GetLastValue(), _elevation.GetLastValue());

		const CLAM::DataArray& input = _input.GetData().GetBuffer();
		CLAM::SphericalHarmonicsDefinition *sh = CLAM::Orientation::sphericalHarmonics();

		for (unsigned i=0; i<_outputs.size(); i++)
		{
			double gainToApply = incidence.sphericalHarmonic(sh[i]);
			if (_config.GetUseFuMa()) gainToApply *= sh[i].weightFuMa;
			CLAM::Audio & audioOut = _outputs[i]->GetData();
			audioOut.SetSize(input.Size());
			CLAM::DataArray& out =audioOut.GetBuffer();
			for (int sample=0; sample<input.Size(); sample++)
				out[sample] = input[sample]*gainToApply;
			_outputs[i]->Produce();
		}
		_input.Consume();
		return true;
	}
	~HoaEncoderBuffer()
	{
		for (unsigned i=0; i<_outputs.size(); i++)
			delete _outputs[i];
	}

};
#endif

