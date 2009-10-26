#ifndef BFormat2Sixteen_hxx
#define BFormat2Sixteen_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

/**
 Converts pressure and velocity input into N equiangular surround
 channels with the first one at the front and going to the left.
 @param NChannels [Config] The number of channels that are to be generated. That configuration parameter changes number of output ports the processing will have and the separation among the speakers.
 @param[in] W [Port] W to be reproduced at the sweet-spot
 @param[in] X [Port] X ambisonics component
 @param[in] Y [Port] Y ambisonics component
 @param[out] "AAA" [Ports] Audio signals to be emitted by the speaker at AAA. Here AAA is the position of the speaker.
 @bug It should take W,X,Y,Z instead pressure and velocity according to conventions
 @todo Review the rotation direction with the conventions.
 @ingroup SpatialAudio
*/
class BFormat2Sixteen : public CLAM::Processing
{ 
protected:
/* TODO add enum paramter with kind of exhibition setup
 
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, NChannels);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetNChannels(12);
		};
	};
*/
	CLAM::AudioInPort _w;
	CLAM::AudioInPort _x;
	CLAM::AudioInPort _y;
	CLAM::AudioInPort _z;
	std::vector<double> _sinAzimuths;
	std::vector<double> _cosAzimuths;
	std::vector<double> _sinElevations;
	std::vector<double> _cosElevations;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	unsigned _nChannels;
	Config _config;

public:
	const char* GetClassName() const { return "BFormat2Sixteen"; }
	BFormat2Sixteen(const Config& config = Config()) 
		: _w("W", this)
		, _x("X", this)
		, _y("Y", this)
		, _z("Z", this)
		, _nChannels(0)
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
		RemovePorts();
		_sinElevations.clear();
		_cosElevations.clear();
		_sinAzimuths.clear();
		_cosAzimuths.clear();

		unsigned buffersize = BackendBufferSize();

		struct SpeakerPositions {
			const char * name;
			float azimuth;
			float elevation;
		} speaker[] =
		{
			{"Back Horizontal", 180., 0.},
			{"Base Back Left", 135., -45.},
			{"Base Back Right", -135, -45.},
			{"Base Front Left", 45., -45.},
			{"Base Front Right", -45., -45.},
			{"Front Horizontal", 0., 0.},
			{"Front Left Horizontal", 60., 0.},
			{"Front Right Horizontal", -60., 0.},
			{"Rear Left Horizontal", 120., 0.},
			{"Rear Right Horizontal", -120., 0.},
			{"Top Back Left", 135., 45.},
			{"Top Back Right", -135., 45.},
			{"Top Front Right", -45., 45.},
			{"Top Front Left", 45., 45.}, 
			{"Top", 0., 89.},
			{0, 0., 0.}
		};
		for (unsigned i=0; speaker[i].name; i++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( speaker[i].name, this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );

			_sinAzimuths.push_back( std::sin( M_PI*speaker[i].azimuth/180 ) );
			_cosAzimuths.push_back( std::cos( M_PI*speaker[i].azimuth/180 ) );
			_sinElevations.push_back( std::sin( M_PI*speaker[i].elevation/180 ) );
			_cosElevations.push_back( std::cos( M_PI*speaker[i].elevation/180 ) );
		}
		_nChannels=_outputs.size();
		
		
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
		CLAM::TData* channels[_nChannels];
		const float wGain = 1.;
		const float velocitiesGain = 1.;

		for (unsigned channel=0; channel<_nChannels; channel++)
			channels[channel] = &_outputs[channel]->GetAudio().GetBuffer()[0];

		for (int i=0; i<w.Size(); i++)
		{
			for (unsigned channel=0; channel<_nChannels; channel++)
			{

				channels[channel][i] = 
					w[i] * wGain + (
					x[i] * _cosAzimuths[channel] * _cosElevations[channel] + 
					y[i] * _sinAzimuths[channel] * _cosElevations[channel] +
					z[i] * _sinElevations[channel]
					) * velocitiesGain;
				channels[channel][i] /= _nChannels;
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
		OutPorts::iterator it;
		for(it=_outputs.begin(); it!=_outputs.end(); it++)
			delete *it;
		_outputs.clear();
		// Delete ports from Processing (base class) register
		GetOutPorts().Clear();
	}
	~BFormat2Sixteen()
	{
		RemovePorts();
	}



};
#endif

