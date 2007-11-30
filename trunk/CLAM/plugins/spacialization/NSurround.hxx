#ifndef NSurround_hxx
#define NSurround_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

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

	CLAM::AudioInPort _p;
	CLAM::AudioInPort _vx;
	CLAM::AudioInPort _vy;
	CLAM::InControl _beta;
	std::vector<double> _sinAlphas;
	std::vector<double> _cosAlphas;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	unsigned _nChannels;
	Config _config;

	NSurround(int) 
		: _p("p", this)
		, _vx("vx", this)
		, _vy("vy", this)
		, _beta("beta", this)
	{}
public:
	const char* GetClassName() const { return "NSurround"; }
	NSurround(const Config& config = Config()) 
		: _p("p", this)
		, _vx("vx", this)
		, _vy("vy", this)
		, _beta("beta", this) // angle in degrees
		, _nChannels(15)
	{
		Configure( config );
		_beta.SetBounds(-360, 360); //a complete spin on each slider direction
		
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);
		RemovePortsAndControls();

		_nChannels=_config.GetNChannels();
		std::vector<int> angles;
		for (unsigned i=0; i<_nChannels; i++)
		{
			double angle = 360.*i/_nChannels;
			angles.push_back(int(angle));
			double rad = M_PI*angle/180;
			_sinAlphas.push_back( std::sin(rad) );
			_cosAlphas.push_back( std::cos(rad) );
			std::cout << "sin "<< angle << " "<< std::sin(rad) << std::endl;
		}
		unsigned buffersize = BackendBufferSize();
		for (unsigned i=0; i<_nChannels; i++)
		{
			std::stringstream stream;
			stream << angles[i];
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( "out "+stream.str(), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		
		_p.SetSize(buffersize);
		_p.SetHop(buffersize);
		_vx.SetSize(buffersize);
		_vx.SetHop(buffersize);
		_vy.SetSize(buffersize);
		_vy.SetHop(buffersize);
		return true;
	}
 
	bool Do()
	{
		const double beta=_beta.GetLastValue()*M_PI/180; //conversion. beta is in radians
		const double cosBeta=std::cos(beta);
		const double sinBeta=std::sin(beta);
		const CLAM::DataArray& p =_p.GetAudio().GetBuffer();
		const CLAM::DataArray& vx =_vx.GetAudio().GetBuffer();
		const CLAM::DataArray& vy =_vy.GetAudio().GetBuffer();
		for (int i=0; i<p.Size(); i++)
		{
			double ux = vx[i] * cosBeta - vy[i] * sinBeta;
			double uy = vx[i] * sinBeta + vy[i] * cosBeta;
			CLAM::TData* channels[_nChannels];
			for (unsigned channel=0; channel<_nChannels; channel++)
				channels[channel] = &_outputs[channel]->GetAudio().GetBuffer()[0];

			for (unsigned channel=0; channel<_nChannels; channel++)
			{
				channels[channel][i] = 0.5*( p[i] - ux * _cosAlphas[channel] - uy * _sinAlphas[channel] );
			}
		}

		_p.Consume();
		_vx.Consume();
		_vy.Consume();
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

