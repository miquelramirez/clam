#ifndef NSurround_hxx
#define NSurround_hxx
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
class NSurround : public CLAM::Processing
{ 
protected:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, NChannels);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetNChannels(0);
		};
	};

	CLAM::AudioInPort _w;
	CLAM::AudioInPort _x;
	CLAM::AudioInPort _y;
	std::vector<double> _sinAlphas;
	std::vector<double> _cosAlphas;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	unsigned _nChannels;
	Config _config;

public:
	const char* GetClassName() const { return "NSurround"; }
	NSurround(const Config& config = Config()) 
		: _w("W", this)
		, _x("X", this)
		, _y("Y", this)
		, _nChannels(15)
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
		_sinAlphas.clear();
		_cosAlphas.clear();

		_nChannels=_config.GetNChannels();
		unsigned buffersize = BackendBufferSize();
		for (unsigned i=0; i<_nChannels; i++)
		{
			double angle = 360.*i/_nChannels;
			double rad = M_PI*angle/180;
			_sinAlphas.push_back( std::sin(rad) );
			_cosAlphas.push_back( std::cos(rad) );
	//		std::cout << "sin "<< angle << " "<< std::sin(rad) << std::endl;

			std::ostringstream nameStream;
			nameStream << "out " << angle;
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( nameStream.str(), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		
		_w.SetSize(buffersize);
		_w.SetHop(buffersize);
		_x.SetSize(buffersize);
		_x.SetHop(buffersize);
		_y.SetSize(buffersize);
		_y.SetHop(buffersize);
		return true;
	}
 
	bool Do()
	{
		const CLAM::DataArray& w =_w.GetAudio().GetBuffer();
		const CLAM::DataArray& x =_x.GetAudio().GetBuffer();
		const CLAM::DataArray& y =_y.GetAudio().GetBuffer();
		const float sqrt2 = sqrt(2);
		const float sqrtN = sqrt(_nChannels);
		CLAM::TData* channels[_nChannels];
	
		for (unsigned channel=0; channel<_nChannels; channel++)
			channels[channel] = &_outputs[channel]->GetAudio().GetBuffer()[0];


		for (int i=0; i<w.Size(); i++)
		{
			for (unsigned channel=0; channel<_nChannels; channel++)
			{
				channels[channel][i] = 1./sqrtN * ( sqrt2 * w[i] + x[i] * _cosAlphas[channel] + y[i] * _sinAlphas[channel] ) ;
			}
		}

		_w.Consume();
		_x.Consume();
		_y.Consume();
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
	~NSurround()
	{
		RemovePortsAndControls();
	}



};
#endif

