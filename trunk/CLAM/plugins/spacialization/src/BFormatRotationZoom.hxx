#ifndef BFormatRotationZoom_hxx
#define BFormatRotationZoom_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>


/**
 @todo Document BFormatRotationZoom
 @ingroup SpatialAudio
*/
class BFormatRotationZoom : public CLAM::Processing
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
	const char* GetClassName() const { return "BFormatRotationZoom"; }
	CLAM::AudioInPort _Win;//add
	CLAM::AudioInPort _Xin;
	CLAM::AudioInPort _Yin;
	CLAM::AudioInPort _Zin;
	CLAM::AudioOutPort _Wout;//add
	CLAM::AudioOutPort _Xout;
	CLAM::AudioOutPort _Yout;
	CLAM::AudioOutPort _Zout;
	CLAM::InControl _azimuth;
	CLAM::InControl _elevation;
	CLAM::InControl _dominance;//add
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	Config _config;
	bool _isPassiveRotation;
public:
	BFormatRotationZoom(const Config& config = Config()) 
		: _Win("p", this)//add
		, _Xin("vx", this)
		, _Yin("vy", this)
		, _Zin("vz", this)
		, _Wout("p", this)//add
		, _Xout("vx", this)
		, _Yout("vy", this)
		, _Zout("vz", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _dominance("dominance", this) //domination value	//add
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(0, 180); //a complete spin on each slider direction
		_dominance.SetBounds(0.25, 4);//add
//		unsigned buffersize = BackendBufferSize();
//		When buffersize==1024 a 512 delay is added to the W channel. This is strange!
//		there is a dataflow scheduling problem. --Pau
		unsigned buffersize=512;
std::cout << "BUFFER SIZE " << buffersize << std::endl;
		_Win.SetSize(buffersize);//add
		_Win.SetHop(buffersize);//add
		_Xin.SetSize(buffersize);
		_Xin.SetHop(buffersize);
		_Yin.SetSize(buffersize);
		_Yin.SetHop(buffersize);
		_Zin.SetSize(buffersize);
		_Zin.SetHop(buffersize);
		_Wout.SetSize(buffersize);//add
		_Wout.SetHop(buffersize);//add
		_Xout.SetSize(buffersize);
		_Xout.SetHop(buffersize);
		_Yout.SetSize(buffersize);
		_Yout.SetHop(buffersize);
		_Zout.SetSize(buffersize);
		_Zin.SetHop(buffersize);
		
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
//		std::cout << "BFormatRotation::ConcreteConfigure()"<<std::endl;
		_isPassiveRotation = _config.GetIsPassiveRotation();
		return true;
	}
	bool Do()
	{
		double azimuth=_azimuth.GetLastValue()*M_PI/180; //conversion. azimuth is in radians
		const double givenElevation=_elevation.GetLastValue()*M_PI/180; //conversion. elevation is in radians
		double elevation;
		double dominance=_dominance.GetLastValue();//add
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
//TODO fix: eliminate param
_isPassiveRotation=true;
//azimuth *= -1;

		const double cosAzimuth=std::cos(azimuth);
		const double sinAzimuth=std::sin(azimuth);
		const double cosElevation=std::cos(elevation);
		const double sinElevation=std::sin(elevation);
		const CLAM::DataArray& p =_Win.GetAudio().GetBuffer();//add
		const CLAM::DataArray& vz =_Zin.GetAudio().GetBuffer();
		const CLAM::DataArray& vx =_Xin.GetAudio().GetBuffer();
		const CLAM::DataArray& vy =_Yin.GetAudio().GetBuffer();
		for (int i=0; i<vz.Size(); i++)
		{
			CLAM::TData* Wout = &_Wout.GetAudio().GetBuffer()[0];//add
			CLAM::TData* Xout = &_Xout.GetAudio().GetBuffer()[0];
			CLAM::TData* Yout = &_Yout.GetAudio().GetBuffer()[0];
			CLAM::TData* Zout = &_Zout.GetAudio().GetBuffer()[0];

			//insert dominance gain	
			Wout[i] = 0.5*(dominance+1/dominance) * p[i] + 1/sqrt(8)*(dominance-1/dominance)*(cosAzimuth*sinElevation * vx[i] + sinAzimuth * vy[i] - cosAzimuth*cosElevation * vz[i]);//add		
			Xout[i] = 0.5*(dominance+1/dominance)*(cosAzimuth*sinElevation * vx[i] + sinAzimuth * vy[i] - cosAzimuth*cosElevation * vz[i])+1/sqrt(2)*(dominance-1/dominance)*p[i]; //add
			Yout[i] = -sinAzimuth*sinElevation * vx[i] + cosAzimuth * vy[i] + sinAzimuth*cosElevation * vz[i];
			Zout[i] = cosElevation * vx[i] + /* 0 * vy[i] */  + sinElevation  * vz[i];
		}

		_Win.Consume();//add
		_Xin.Consume();
		_Yin.Consume();
		_Zin.Consume();
		_Wout.Produce();//add
		_Xout.Produce();
		_Yout.Produce();
		_Zout.Produce();

		return true;
	}

};
#endif

