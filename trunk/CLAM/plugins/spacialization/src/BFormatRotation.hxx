#ifndef BFormatRotation_hxx
#define BFormatRotation_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

/**
 This Processing rotates a B-format signal.
 The rotated B-format is equivalent to the input one but
 expressed in the reference frame of a listener that has been
 rotated the given roll, azimuth and elevation from the original frame.
 In summary, the rotation angles refers to rotation of the listener.
 Roll, azimuth and elevation are applied as in @ref AmbisonicsConventions.
 @param[in] azimuth [Control] in degrees [-360..360]
 @param[in] elevation [Control] in degrees [-90..90]
 @param[in] X [Port] The X component to be rotated
 @param[in] Y [Port] The Y component to be rotated
 @param[in] Z [Port] The Z component to be rotated
 @param[out] X [Port] The X component after rotation
 @param[out] Y [Port] The Y component after rotation
 @param[out] Z [Port] The Z component after rotation
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
	CLAM::FloatInControl _azimuth;
	CLAM::FloatInControl _elevation;
	CLAM::FloatInControl _roll;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;

	BFormatRotation(const Config& config = Config()) 
		: _Xin("X", this)
		, _Yin("Y", this)
		, _Zin("Z", this)
		, _Xout("X", this)
		, _Yout("Y", this)
		, _Zout("Z", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _roll("roll", this) // angle in degrees
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90);
		_roll.SetBounds(-360, 360);
		_roll.DoControl(0);
		_azimuth.DoControl(0);
		_elevation.DoControl(0);
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
		_Zout.SetHop(buffersize);
		
	}
	bool Do()
	{
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
		double elevationRadians = elevationDeg*M_PI/180;
		double azimuthRadians=_azimuth.GetLastValue()*M_PI/180;
		double rollRadians = _roll.GetLastValue()*M_PI/180;
		const double cosAzimuth=std::cos(azimuthRadians);
		const double sinAzimuth=std::sin(azimuthRadians);
		const double cosElevation=std::cos(elevationRadians);
		const double sinElevation=std::sin(elevationRadians);
		const CLAM::DataArray& X =_Xin.GetAudio().GetBuffer();
		const CLAM::DataArray& Y =_Yin.GetAudio().GetBuffer();
		const CLAM::DataArray& Z =_Zin.GetAudio().GetBuffer();
		double sinRoll = std::sin(rollRadians);
		double cosRoll = std::cos(rollRadians);

		double cosASinE = cosAzimuth * sinElevation;
		double sinASinE = sinAzimuth * sinElevation;

		CLAM::TData* Xout = &_Xout.GetAudio().GetBuffer()[0];
		CLAM::TData* Yout = &_Yout.GetAudio().GetBuffer()[0];
		CLAM::TData* Zout = &_Zout.GetAudio().GetBuffer()[0];
		for (int i=0; i<Z.Size(); i++)
		{
			float rotatedX = 
				+ X[i] * cosAzimuth * cosElevation
				+ Y[i] * sinAzimuth * cosElevation 
				+ Z[i] * sinElevation;
			float rotatedY = 
				+ X[i] * (- sinAzimuth*cosRoll - cosASinE*sinRoll)
				+ Y[i] * (+ cosAzimuth*cosRoll - sinASinE*sinRoll)
				+ Z[i] * (+ cosElevation*sinRoll);
			float rotatedZ =
				+ X[i] * (- cosASinE*cosRoll + sinAzimuth*sinRoll)
				+ Y[i] * (- sinASinE*cosRoll - cosAzimuth*sinRoll)
				+ Z[i] * (+ cosElevation*cosRoll);
			Xout[i] = rotatedX;
			Yout[i] = rotatedY;
			Zout[i] = rotatedZ;
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

