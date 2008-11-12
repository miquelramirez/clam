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
 a point (source) relative to the listener (target)
 position and orientation.
 Angles are expressed in degrees. 
 Coordinates systems and rotations follow @ref AmbisonicsConventions.
 @param[in] "target X" [Control] X coord of the listener in absolute coords
 @param[in] "target Y" [Control] Y coord of the listener in absolute coords
 @param[in] "target Z" [Control] Z coord of the listener in absolute coords
 @param[in] "target azimuth" [Control] Azimuth of the listener orientation (degrees)
 @param[in] "target elevation" [Control] Elevation of the listener orientation (degrees)
 @param[in] "target roll" [Control] Roll of the listener orientation (degrees)
 @param[in] "source X" [Control] X coord of the source in absolute coords
 @param[in] "source Y" [Control] Y coord of the source in absolute coords
 @param[in] "source Z" [Control] Z coord of the source in absolute coords
 @param[out] "source azimuth" [Control] Relative azimuth of the source on target's frame
 @param[out] "source elevation" [Control] Relative elevation of the source on target's frame
 @todo Input roll is not considered at all
 @ingroup SpatialAudio
 @see AmbisonicsConventions
*/
class AbsoluteCoordinates2RelativeAngles : public CLAM::Processing
{ 

	CLAM::FloatInControl _targetX;
	CLAM::FloatInControl _targetY;
	CLAM::FloatInControl _targetZ;
	CLAM::FloatInControl _targetRoll;
	CLAM::FloatInControl _targetElevation;
	CLAM::FloatInControl _targetAzimuth;

	CLAM::FloatInControl _sourceX;
	CLAM::FloatInControl _sourceY;
	CLAM::FloatInControl _sourceZ;
	CLAM::FloatOutControl _sourceElevation;
	CLAM::FloatOutControl _sourceAzimuth;
public:
	const char* GetClassName() const { return "AbsoluteCoordinates2RelativeAngles"; }
	AbsoluteCoordinates2RelativeAngles(const Config& config = Config()) 
		: _targetX("target X", this)
		, _targetY("target Y", this)
		, _targetZ("target Z", this)		
		, _targetRoll("target roll", this)
		, _targetElevation("target elevation", this)
		, _targetAzimuth("target azimuth", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)		
		, _sourceElevation("source elevation", this)
		, _sourceAzimuth("source azimuth", this)
	{
		Configure( config );
	}
 
	bool Do()
	{	

		double sourceX = _sourceX.GetLastValue();
		double sourceY = _sourceY.GetLastValue();
		double sourceZ = _sourceZ.GetLastValue();
		double targetX = _targetX.GetLastValue();
		double targetY = _targetY.GetLastValue();
		double targetZ = _targetZ.GetLastValue();
		double targetRoll = _targetRoll.GetLastValue();
		//We sum pi/2 because the matrix is for zenith and no elevation. TODO change zenith for elevation
		double targetElevation = _targetElevation.GetLastValue(); 
		double targetAzimuth = _targetAzimuth.GetLastValue();
		Orientation orientation = computeRelativeOrientation(
			targetX, targetY, targetZ,
			targetAzimuth, targetElevation, targetRoll,
			sourceX, sourceY, sourceZ);
		_sourceAzimuth.SendControl( orientation.azimuth );
		_sourceElevation.SendControl( orientation.elevation );
		return true;
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

};
} // namespace
#endif // AbsoluteCoordinates2RelativeAngles_hxx
