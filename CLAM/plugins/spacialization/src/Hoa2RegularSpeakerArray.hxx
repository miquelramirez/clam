#ifndef Hoa2RegularSpeakerArray_hxx
#define Hoa2RegularSpeakerArray_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Filename.hxx>
#include <cmath>
#include "Orientation.hxx"
#include <fstream>

class SpeakerLayout
{
	std::vector<std::string> _names;
	std::vector<CLAM::Orientation> _orientations;
	bool error(std::string & errorMsg, const std::string & message)
	{
		errorMsg += message;
		errorMsg += "\n";
		return false;
	}
public:
	unsigned size() const { return _names.size(); }
	const std::string & name(unsigned i) const { return _names[i]; }
	const CLAM::Orientation & orientation(unsigned i) const { return _orientations[i]; }
	void add(double azimuthDegrees, double elevationDegrees, const std::string & name)
	{
		std::cout << elevationDegrees << " " << azimuthDegrees << " '" << name << "'" << std::endl;
		_orientations.push_back(CLAM::Orientation(azimuthDegrees, elevationDegrees));
		_names.push_back(name);
	}
	bool load(
			const std::string & path,
			std::string & errorMsg )
	{
		if (path.empty()) return error(errorMsg, "No speaker layout file specified");
		std::ifstream file(path.c_str());
		if (!file) return error(errorMsg, "Could not open the speaker layout file "+path);
		_orientations.clear();
		_names.clear();
		while (true)
		{
			double elevation;
			double azimuth;
			file >> elevation;
			file >> azimuth;
			if (!file) break;
			file >> std::ws;
			std::string name;
			std::getline(file, name);
			// TODO: Issue an error if illegal port name
			add(azimuth, elevation, name);
		}
		return true;
	}
};



/**
 Decodes a Nth order signal for a regular array of speakers.
 @param NChannels [Config] The number of speakers that are to be generated. That configuration parameter changes number of output ports the processing will have and the separation among the speakers.
 @param[in] W [Port] W to be reproduced at the sweet-spot
 @param[in] X [Port] X ambisonics component
 @param[in] Y [Port] Y ambisonics component
 @param[out] "AAA" [Ports] Audio signals to be emitted by the speaker at AAA. Here AAA is the angle in degrees for each one of the speakers.
 @bug It should take W,X,Y,Z instead pressure and velocity according to conventions
 @todo Review the rotation direction with the conventions.
 @ingroup SpatialAudio
*/
class Hoa2RegularSpeakerArray : public CLAM::Processing
{ 
protected:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, Order);
		DYN_ATTRIBUTE( 1, public, CLAM::InFilename, SpeakerLayout);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetOrder(1);
		};
	};
	SpeakerLayout _layout;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	unsigned _nChannels;
	Config _config;
	CLAM::AudioInPort _w;
	CLAM::AudioInPort _x;
	CLAM::AudioInPort _y;
	CLAM::AudioInPort _z;

public:
	const char* GetClassName() const { return "Hoa2RegularSpeakerArray"; }
	Hoa2RegularSpeakerArray(const Config& config = Config()) 
		: _nChannels(0)
		, _w("W", this)
		, _x("X", this)
		, _y("Y", this)
		, _z("Z", this)
	{
		Configure( config );

	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);
		std::string errorMessage;
		if (not _layout.load(_config.GetSpeakerLayout(), errorMessage))
			return AddConfigErrorMessage(errorMessage);

		unsigned buffersize = BackendBufferSize();

		// Set up the outputs according to the layout
		// TODO: Keep the connections
		RemovePorts();
		for (unsigned speaker=0; speaker<_layout.size(); speaker++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( _layout.name(speaker), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		_nChannels=_outputs.size();

		// TODO: configurable ambisonics order
		_w.SetSize(buffersize);
		_w.SetHop(buffersize);
		_x.SetSize(buffersize);
		_x.SetHop(buffersize);
		_y.SetSize(buffersize);
		_y.SetHop(buffersize);
		_z.SetSize(buffersize);
		_z.SetHop(buffersize);
		return true;
	}
 
	bool Do()
	{
		const CLAM::DataArray& w =_w.GetAudio().GetBuffer();
		const CLAM::DataArray& x =_x.GetAudio().GetBuffer();
		const CLAM::DataArray& y =_y.GetAudio().GetBuffer();
		const CLAM::DataArray& z =_z.GetAudio().GetBuffer();
		//const float sqrt2 = sqrt(2);
		//const float sqrtN = sqrt(_nChannels);
		CLAM::TData* speakers[_nChannels];

		for (unsigned speaker=0; speaker<_nChannels; speaker++)
			speakers[speaker] = &_outputs[speaker]->GetAudio().GetBuffer()[0];

		const unsigned nComponents = 4;
		double componentWeight[nComponents];
		CLAM::SphericalHarmonicsDefinition * sh = CLAM::Orientation::sphericalHarmonics();

		for (unsigned speaker=0; speaker<_outputs.size(); speaker++)
		{
			for (unsigned hoaComponent=0; hoaComponent<nComponents; hoaComponent++)
				componentWeight[hoaComponent] = _layout.orientation(speaker).sphericalHarmonic(sh[hoaComponent]);
			for (int sample=0; sample<w.Size(); sample++)
			{
				speakers[speaker][sample] =
					w[sample] * componentWeight[0] +
					x[sample] * componentWeight[1] +
					y[sample] * componentWeight[2] +
					z[sample] * componentWeight[3] ;
			}
		}
		_w.Consume();
		_x.Consume();
		_y.Consume();
		_z.Consume();
		// Tell the ports this is done
		for (unsigned channel=0; channel<_nChannels; channel++)
			_outputs[channel]->Produce();
		
		return true;
	}
	void RemovePorts()
	{
		for (unsigned speaker=0; speaker<_outputs.size(); speaker++)
			delete _outputs[speaker];
		_outputs.clear();
	}
	~Hoa2RegularSpeakerArray()
	{
		RemovePorts();
	}
};
#endif

