#ifndef BFormatRotationZoom_hxx
#define BFormatRotationZoom_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>


/**
 This Processing implements the rotation and zoom of a B-format signal set.
 The rotation is performed on the set X,Y,Z such as the new set of axes X',Y',Z' has X' 
 facing the direction specified by the Azimuth and Elevation angles.
 The Zoom effect (Dominance effect) is applied after rotation and takes place along the X' direction.
 @param[in] azimuth [Control] The counterclockwise angle of rotation on the horizontal plane. In degrees [-360..360]
 @param[in] elevation [Control] The angle of vertical rotation. In degrees [-90..90]
 @param[in] dominance [Control] The parameter related to the level of zoom in the X' direction. A value of 1 leaves the components untouched. Values greater than 1 produce a zoom-in effect, while values less than 1 produce the zoom out. Range [0.25 .. 4]
 @param[in] W [Port] The W input component
 @param[in] X [Port] The X component to be rotated
 @param[in] Y [Port] The Y component to be rotated
 @param[in] Z [Port] The Z component to be rotated
 @param[out] W [Port] The W component after rotation and zoom
 @param[out] X [Port] The X component after rotation and zoom
 @param[out] Y [Port] The Y component after rotation and zoom	
 @param[out] Z [Port] The Z component after rotation and zoom
 @see AmbisonicsConventions 
 @ingroup SpatialAudio

 @todo Split the rotation from the zoom itself or introducing the roll
*/
class BFormatRotationZoom : public CLAM::Processing
{
	
public:
	const char* GetClassName() const { return "BFormatRotationZoom"; }
	CLAM::AudioInPort _Win;//add
	CLAM::AudioInPort _Xin;
	CLAM::AudioInPort _Yin;
	CLAM::AudioInPort _Zin;
	CLAM::AudioOutPort _Wout;
	CLAM::AudioOutPort _Xout;
	CLAM::AudioOutPort _Yout;
	CLAM::AudioOutPort _Zout;
	CLAM::FloatInControl _azimuth;
	CLAM::FloatInControl _elevation;
	CLAM::FloatInControl _roll;
	CLAM::FloatInControl _dominance;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;

	BFormatRotationZoom(const Config& config = Config()) 
		: _Win("W", this)
		, _Xin("X", this)
		, _Yin("Y", this)
		, _Zin("Z", this)
		, _Wout("W", this)
		, _Xout("X", this)
		, _Yout("Y", this)
		, _Zout("Z", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _roll("roll", this) // angle in degrees
		, _dominance("dominance", this) //domination value	
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90);
		_roll.SetBounds(-360, 360);
		_dominance.SetBounds(0.25, 4);
		_roll.DoControl(0);
		_azimuth.DoControl(0);
		_elevation.DoControl(0);
		_dominance.DoControl(1);
//		unsigned buffersize = BackendBufferSize();
//		When buffersize==1024 a 512 delay is added to the W channel. This is strange!
//		there is a dataflow scheduling problem. --Pau
		unsigned buffersize=512;
//std::cout << "BUFFER SIZE " << buffersize << std::endl;
		_Win.SetSize(buffersize);
		_Win.SetHop(buffersize);
		_Xin.SetSize(buffersize);
		_Xin.SetHop(buffersize);
		_Yin.SetSize(buffersize);
		_Yin.SetHop(buffersize);
		_Zin.SetSize(buffersize);
		_Zin.SetHop(buffersize);
		_Wout.SetSize(buffersize);
		_Wout.SetHop(buffersize);
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
		const CLAM::DataArray& W =_Win.GetAudio().GetBuffer();
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
		CLAM::TData* Wout = &_Wout.GetAudio().GetBuffer()[0];

		double dominance=_dominance.GetLastValue();
		// matrix coeficient from Win to Wout
		const double dominanceWW = 0.5*(dominance+1/dominance);
		// matrix coeficient from Xin to Wout
		const double dominanceWX = 1/sqrt(8)*(dominance-1/dominance);
		// matrix coeficient from Win to Xout
		const double dominanceXW = 1/sqrt(2)*(dominance-1/dominance);
		// matrix coeficient from Xin to Xout
		const double dominanceXX = 0.5*(dominance+1/dominance);
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
			
			// zoom phase:
			Wout[i] = dominanceWX * rotatedX + dominanceWW * W[i];
			Xout[i] = dominanceXX * rotatedX + dominanceXW * W[i];
			Yout[i] = rotatedY;
			Zout[i] = rotatedZ;
		}

		_Win.Consume();
		_Xin.Consume();
		_Yin.Consume();
		_Zin.Consume();
		_Wout.Produce();
		_Xout.Produce();
		_Yout.Produce();
		_Zout.Produce();

		return true;
	}

};
#endif

