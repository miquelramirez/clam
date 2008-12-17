#ifndef BFormat2Cube_hxx
#define BFormat2Cube_hxx
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
 @param[out] "out AAA" [Ports] Audio signals to be emitted by the speaker at AAA. Here AAA is the angle in degrees for each one of the speakers.
 @bug It should take W,X,Y,Z instead pressure and velocity according to conventions
 @todo Review the rotation direction with the conventions.
 @ingroup SpatialAudio
*/
class BFormat2Cube : public CLAM::Processing
{ 
protected:
/*
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, NChannels);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetNChannels(8);
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
	const char* GetClassName() const { return "BFormat2Cube"; }
	BFormat2Cube(const Config& config = Config()) 
		: _w("W", this)
		, _x("X", this)
		, _y("Y", this)
		, _z("Z", this)
		, _nChannels(8)
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
		RemovePortsAndControls();
		_sinElevations.clear();
		_cosElevations.clear();
		_sinAzimuths.clear();
		_cosAzimuths.clear();

//		_nChannels=_config.GetNChannels();
		unsigned buffersize = BackendBufferSize();
#if 0
		for (unsigned i=0; i<_nChannels; i++)
		{
			double angle = 360.*i/_nChannels;
			double rad = M_PI*angle/180;
			_sinElevations.push_back( std::sin(rad) );
			_cosElevations.push_back( std::cos(rad) );
			_sinAzimuths.push_back( std::sin(rad) );
			_cosAzimuths.push_back( std::cos(rad) );
	//		std::cout << "sin "<< angle << " "<< std::sin(rad) << std::endl;

			std::ostringstream nameStream;
			nameStream << "out " << angle;
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( nameStream.str(), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}	
		
#endif

		struct SpeakerPositions {
			const char * name;
			float azimuth;
			float elevation;
		} speaker[] =
		{
		// The following angles are "encoding" angles not ITU exhibiton setup angles.
			{"Base Front Left", 45., -35.2},
			{"Base Front Right", -45., -35.2},
			{"Base Back Left", 135., -35.2},
			{"Base Back Right", -135, -35.2},
			{"Top Front Left", 45., 35.2},
			{"Top Front Right", -45., 35.2},
			{"Top Back Left", 135., 35.2},
			{"Top Back Right", -135., 35.2},
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
		const float sqrt2 = sqrt(2);
		const float sqrtN = sqrt(_nChannels);
		CLAM::TData* channels[_nChannels];
		const float wGain = 1.;
		const float velocitiesGain = 2.;

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
#if 0
				switch (channel)
				{
					case 0: // base l
						channels[channel][i] = w[i] + x[i] + y[i] - z[i];
						break;
					case 1: // base r
						channels[channel][i] = w[i] + x[i] - y[i] - z[i];
						break;
					case 2: // base bl
						channels[channel][i] = w[i] - x[i] + y[i] - z[i];
						break;
					case 3: // base br
						channels[channel][i] = w[i] - x[i] - y[i] - z[i];
						break;
					case 4: // top l
						channels[channel][i] = w[i] + x[i] + y[i] + z[i];
						break;
					case 5: // top r
						channels[channel][i] = w[i] + x[i] - y[i] + z[i];
						break;
					case 6: // top bl
						channels[channel][i] = w[i] - x[i] + y[i] + z[i];
						break;
					case 7: // top br
						channels[channel][i] = w[i] - x[i] -y[i] + z[i];
						break;
					default:
						std::cout << "ERROR at BFormat2Cube: bad channel number."<< std::endl;
						break;
				}
#endif 
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
	void RemovePortsAndControls()
	{
		OutPorts::iterator it;
		for(it=_outputs.begin(); it!=_outputs.end(); it++)
			delete *it;
		_outputs.clear();
		// Delete ports from Processing (base class) register
		GetOutPorts().Clear();
	}
	~BFormat2Cube()
	{
		RemovePortsAndControls();
	}



};
#endif

