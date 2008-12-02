#ifndef BFormatRotation_hxx
#define BFormatRotation_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

/**
 This Processing rotates a B-format source
 @todo BFormatRotation explain how the rotations are performed. 
 Rotation is "passive", meaning that is the listener who rotates and not the sources.
 @param[in] azimuth [Control]
 @param[in] elevation [Control]
 @param[in] vx [Port] The X component to be rotated
 @param[in] vy [Port] The Y component to be rotated
 @param[in] vz [Port] The Z component to be rotated
 @param[out] vx [Port] The X component after rotation
 @param[out] vy [Port] The Y component after rotation
 @param[out] vz [Port] The Z component after rotation
 @see AmbisonicsConventions
 @ingroup SpatialAudio
*/
class BFormatRotation : public CLAM::Processing
{
	
public:
	const char* GetClassName() const { return "BFormatRotation"; }
	CLAM::AudioInPort _Xin;
	CLAM::AudioInPort _Yin;
	CLAM::AudioInPort _Zin;
	CLAM::AudioOutPort _Xout;
	CLAM::AudioOutPort _Yout;
	CLAM::AudioOutPort _Zout;
	CLAM::InControl _azimuth;
	CLAM::InControl _elevation;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	Config _config;
public:
	BFormatRotation(const Config& config = Config()) 
		: _Xin("vx", this)
		, _Yin("vy", this)
		, _Zin("vz", this)
		, _Xout("vx", this)
		, _Yout("vy", this)
		, _Zout("vz", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90); //a complete spin on each slider direction
//		unsigned buffersize = BackendBufferSize();
//		When buffersize==1024 a 512 delay is added to the W channel. This is strange!
//		there is a dataflow scheduling problem. --Pau
		unsigned buffersize=512;
//std::cout << "BUFFER SIZE " << buffersize << std::endl;
		_Xin.SetSize(buffersize);
		_Xin.SetHop(buffersize);
		_Yin.SetSize(buffersize);
		_Yin.SetHop(buffersize);
		_Zin.SetSize(buffersize);
		_Zin.SetHop(buffersize);
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
		return true;
	}
	bool Do()
	{
		double azimuth=_azimuth.GetLastValue()*M_PI/180; //conversion. azimuth is in radians
		double elevationDeg = _elevation.GetLastValue(); 
		if (elevationDeg > 90.)
		{
			std::cout << "WARNING: elevation should be smaller than 90, but was " << elevationDeg << std::endl;
			elevationDeg = 90.;
		}
		else if (elevationDeg<-90.)
		{
			std::cout << "WARNING: elevation should be greater than -90, but was " << elevationDeg << std::endl;
			elevationDeg = -90.;
		}
		double elevation = elevationDeg*M_PI/180; //conversion. elevation is in radians
		const double cosAzimuth=std::cos(azimuth);
		const double sinAzimuth=std::sin(azimuth);
		const double cosElevation=std::cos(elevation);
		const double sinElevation=std::sin(elevation);
		const CLAM::DataArray& vz =_Zin.GetAudio().GetBuffer();
		const CLAM::DataArray& vx =_Xin.GetAudio().GetBuffer();
		const CLAM::DataArray& vy =_Yin.GetAudio().GetBuffer();
		const double roll = 0.;
		double sinRoll = std::sin(roll);
		double cosRoll = std::cos(roll);

		double cosASinE = cosAzimuth * sinElevation;
		double sinASinE = sinAzimuth * sinElevation;

		CLAM::TData* Xout = &_Xout.GetAudio().GetBuffer()[0];
		CLAM::TData* Yout = &_Yout.GetAudio().GetBuffer()[0];
		CLAM::TData* Zout = &_Zout.GetAudio().GetBuffer()[0];
//		std::cout<<"Elevation: "<<elevation*180/M_PI<<std::endl;
//		std::cout<<"Azimuth: "<<azimuth*180/M_PI<<std::endl; 
		for (int i=0; i<vz.Size(); i++)
		{
#if 0
 
			Xout[i] = cosAzimuth*cosElevation * vx[i] + sinAzimuth * vy[i] - cosAzimuth*sinElevation * vz[i]; 
			Yout[i] = -sinAzimuth*cosElevation * vx[i] + cosAzimuth * vy[i] + sinAzimuth*sinElevation * vz[i];
			Zout[i] = sinElevation * vx[i] + /* 0 * vy[i] */  + cosElevation  * vz[i];
#endif

			Xout[i] = 
				+ vx[i] * cosAzimuth * cosElevation
				+ vy[i] * sinAzimuth * cosElevation 
				+ vz[i] * sinElevation;
			Yout[i] = 
				+ vx[i] * (- sinAzimuth*cosRoll - cosASinE*sinRoll)
				+ vy[i] * (+ cosAzimuth*cosRoll - sinASinE*sinRoll)
				+ vz[i] * (+ cosElevation*sinRoll);
			Zout[i] =
				+ vx[i] * (- cosASinE*cosRoll + sinAzimuth*sinRoll)
				+ vy[i] * (- sinASinE*cosRoll - cosAzimuth*sinRoll)
				+ vz[i] * (+ cosElevation*cosRoll);

		}

		_Xin.Consume();
		_Yin.Consume();
		_Zin.Consume();
		_Xout.Produce();
		_Yout.Produce();
		_Zout.Produce();

		return true;
	}

};
#endif

