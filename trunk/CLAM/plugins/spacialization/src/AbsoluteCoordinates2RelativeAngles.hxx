#ifndef AbsoluteCoordinates2RelativeAngles_hxx 
#define AbsoluteCoordinates2RelativeAngles_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include <iomanip>
#include "Orientation.hxx"

namespace CLAM
{

/**
 Computes the azimuth and elevation angles of
 a point (source) relative to the listener (listener)
 position and orientation.
 Angles are expressed in degrees. 
 Coordinates systems and rotations follow @ref AmbisonicsConventions.
 @param[in] "listener X" [Control] X coord of the listener in absolute coords
 @param[in] "listener Y" [Control] Y coord of the listener in absolute coords
 @param[in] "listener Z" [Control] Z coord of the listener in absolute coords
 @param[in] "listener azimuth" [Control] Azimuth of the listener orientation (degrees)
 @param[in] "listener elevation" [Control] Elevation of the listener orientation (degrees)
 @param[in] "listener roll" [Control] Roll of the listener orientation (degrees)
 @param[in] "source X" [Control] X coord of the source in absolute coords
 @param[in] "source Y" [Control] Y coord of the source in absolute coords
 @param[in] "source Z" [Control] Z coord of the source in absolute coords
 @param[out] "relative azimuth" [Control] Relative azimuth of the source on listener's frame
 @param[out] "relative elevation" [Control] Relative elevation of the source on listener's frame
 @todo Input roll is not considered at all
 @ingroup SpatialAudio
 @see AmbisonicsConventions
*/
class AbsoluteCoordinates2RelativeAngles : public CLAM::Processing
{ 

	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE(Config,2,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,TData,DistanceExponent);
		DYN_ATTRIBUTE(1,public,TData,MinimumDistance);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetDistanceExponent(1.0);
			SetMinimumDistance(1.0);
		};
	};

	Config _config;

	CLAM::FloatInControl _listenerX;
	CLAM::FloatInControl _listenerY;
	CLAM::FloatInControl _listenerZ;
	CLAM::FloatInControl _listenerRoll;
	CLAM::FloatInControl _listenerElevation;
	CLAM::FloatInControl _listenerAzimuth;

	CLAM::FloatInControl _sourceX;
	CLAM::FloatInControl _sourceY;
	CLAM::FloatInControl _sourceZ;
	CLAM::FloatOutControl _sourceElevation;
	CLAM::FloatOutControl _sourceAzimuth;
	CLAM::FloatOutControl _gainBecauseDistance;
public:
	const char* GetClassName() const { return "AbsoluteCoordinates2RelativeAngles"; }
	AbsoluteCoordinates2RelativeAngles(const Config& config = Config()) 
		: _listenerX("listener X", this)
		, _listenerY("listener Y", this)
		, _listenerZ("listener Z", this)		
		, _listenerRoll("listener roll", this)
		, _listenerElevation("listener elevation", this)
		, _listenerAzimuth("listener azimuth", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)		
		, _sourceElevation("relative elevation", this)
		, _sourceAzimuth("relative azimuth", this)
		, _gainBecauseDistance("output gain because of distance", this)
	{
		Configure( config );
	}
 

	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool Do()
	{	

		double sourceX = _sourceX.GetLastValue();
		double sourceY = _sourceY.GetLastValue();
		double sourceZ = _sourceZ.GetLastValue();
		double listenerX = _listenerX.GetLastValue();
		double listenerY = _listenerY.GetLastValue();
		double listenerZ = _listenerZ.GetLastValue();
		double listenerRoll = _listenerRoll.GetLastValue();
		double listenerElevation = _listenerElevation.GetLastValue(); 
		double listenerAzimuth = _listenerAzimuth.GetLastValue();
		Orientation orientation = computeRelativeOrientation(
			listenerX, listenerY, listenerZ,
			listenerAzimuth, listenerElevation, listenerRoll,
			sourceX, sourceY, sourceZ);
		_sourceAzimuth.SendControl( orientation.azimuth );
		_sourceElevation.SendControl( orientation.elevation );
		double distance=getDistance(sourceX,sourceY,sourceZ,listenerX,listenerY,listenerZ);
		const double minimumDistance= ( _config.HasMinimumDistance() ? _config.GetMinimumDistance() : 1.0 );
		if (distance < minimumDistance)
			distance=minimumDistance;
		const double exponent = _config.HasDistanceExponent() ? _config.GetDistanceExponent() : 1.0;
		const double gain=1.0/pow(distance,exponent);
		_gainBecauseDistance.SendControl ( gain );
		return true;
	}


	double getDistance(double sourceX,double sourceY,double sourceZ,double listenerX,double listenerY,double listenerZ)
	{
		return sqrt(pow((sourceX-listenerX),2)+pow((sourceY-listenerY),2)+pow((sourceZ-listenerZ),2));
	}

	static Orientation computeRelativeOrientation(
		double frameX,
		double frameY,
		double frameZ,
		double frameAzimuth,
		double frameElevation,
		double frameRoll,
		double pointX,
		double pointY,
		double pointZ)
	{
		double dx = (pointX - frameX);
		double dy = (pointY - frameY);
		double dz = (pointZ - frameZ);
		double radAzimuth = M_PI/180. * frameAzimuth;
		double cosAzimuth = std::cos(radAzimuth);
		double sinAzimuth = std::sin(radAzimuth);
		double radElevation = M_PI/180. * frameElevation;
		double sinElevation = std::sin(radElevation);
		double cosElevation = std::cos(radElevation);
		double radRoll = M_PI/180. * frameRoll;
		double sinRoll = std::sin(radRoll);
		double cosRoll = std::cos(radRoll);

		double cosASinE = cosAzimuth * sinElevation;
		double sinASinE = sinAzimuth * sinElevation;

		double rotatedX = 
			+ dx * cosAzimuth * cosElevation
			+ dy * sinAzimuth * cosElevation 
			+ dz * sinElevation;
		double rotatedY = 
			+ dx * (- sinAzimuth*cosRoll - cosASinE*sinRoll)
			+ dy * (+ cosAzimuth*cosRoll - sinASinE*sinRoll)
			+ dz * (+ cosElevation*sinRoll);
		double rotatedZ =
			+ dx * (- cosASinE*cosRoll + sinAzimuth*sinRoll)
			+ dy * (- sinASinE*cosRoll - cosAzimuth*sinRoll)
			+ dz * (+ cosElevation*cosRoll);

		Orientation orientation;
		orientation.toPoint(rotatedX, rotatedY, rotatedZ);
		return orientation;
	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config,config);
		return true;
	}

};
} // namespace
#endif // AbsoluteCoordinates2RelativeAngles_hxx
