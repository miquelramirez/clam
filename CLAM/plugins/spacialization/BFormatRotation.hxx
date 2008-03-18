#ifndef BFormatRotation_hxx
#define BFormatRotation_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

class BFormatRotation : public CLAM::Processing
{
protected:
	class Config : public CLAM::ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, bool, IsPassiveRotation);
    	protected:
	    void DefaultInit()
	    {
		AddAll();
		UpdateData();
		SetIsPassiveRotation(true);
	    };
	};

	
public:
	const char* GetClassName() const { return "BFormatRotation"; }
	CLAM::AudioInPort _vx;
	CLAM::AudioInPort _vy;
	CLAM::AudioInPort _vz;
	CLAM::InControl _azimuth;
	CLAM::InControl _elevation;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	unsigned _nChannels;
	Config _config;
	bool _isPassiveRotation;
public:
	BFormatRotation(const Config& config = Config()) 
		: _vx("vx", this)
		, _vy("vy", this)
		, _vz("vz", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _nChannels(3)
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(0, 180); //a complete spin on each slider direction
		std::vector<std::string> names;
		names.push_back("vx");
		names.push_back("vy");
		names.push_back("vz");
		unsigned buffersize = BackendBufferSize();
		for (unsigned i=0; i<_nChannels; i++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( names[i], this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		_vx.SetSize(buffersize);
		_vx.SetHop(buffersize);
		_vy.SetSize(buffersize);
		_vy.SetHop(buffersize);
		_vz.SetSize(buffersize);
		_vz.SetHop(buffersize);
		
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		std::cout << "BFormatRotation::ConcreteConfigure()"<<std::endl;
		_isPassiveRotation = _config.GetIsPassiveRotation();
		return true;
	}
	bool Do()
	{
		double azimuth=_azimuth.GetLastValue()*M_PI/180; //conversion. azimuth is in radians
		const double givenElevation=_elevation.GetLastValue()*M_PI/180; //conversion. elevation is in radians
		double elevation;
		if (givenElevation>M_PI)
		{
			std::cout << "WARNING: elevation should be smaller than 180" << std::endl;
			elevation = M_PI;
		}
		else if (givenElevation<0)
		{
			std::cout << "WARNING: elevation should be greater than 0" << std::endl;
			elevation = 0;
		}
		else 
			elevation = givenElevation;
		if (not _isPassiveRotation)
		{
			azimuth *= -1;
			elevation *= -1;
		}
		const double cosAzimuth=std::cos(azimuth);
		const double sinAzimuth=std::sin(azimuth);
		const double cosElevation=std::cos(elevation);
		const double sinElevation=std::sin(elevation);
		const CLAM::DataArray& vz =_vz.GetAudio().GetBuffer();
		const CLAM::DataArray& vx =_vx.GetAudio().GetBuffer();
		const CLAM::DataArray& vy =_vy.GetAudio().GetBuffer();
		for (int i=0; i<vz.Size(); i++)
		{
			CLAM::TData* out[_nChannels];
			for (unsigned channel=0; channel<_nChannels; channel++)
				out[channel] = &_outputs[channel]->GetAudio().GetBuffer()[0];
			out[0][i] = cosAzimuth*sinElevation * vx[i] + sinAzimuth * vy[i] -cosAzimuth*cosElevation * vz[i]; 
			out[1][i] = -sinAzimuth*sinElevation * vx[i] + cosAzimuth * vy[i] + sinAzimuth*cosElevation * vz[i];
			out[2][i] = cosElevation * vx[i] + 0 * vy[i] + sinElevation  * vz[i];
		}

		_vz.Consume();
		_vx.Consume();
		_vy.Consume();
		// Tell the ports this is done
		for (unsigned channel=0; channel<_nChannels; channel++)
			_outputs[channel]->Produce();
		
		return true;
	}

};
#endif

