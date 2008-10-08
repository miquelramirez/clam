#ifndef AbsoluteCoordinates2RelativeAngles_hxx 
#define AbsoluteCoordinates2RelativeAngles_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#define PI 3.14159265

namespace CLAM
{

/** 	This class calculate the angles azimuth and elevation that are relative to the listener. 
	Their values are in degrees. The input values are the source and sink location and the sink's azimuth and elevation angles.**/
class AbsoluteCoordinates2RelativeAngles : public CLAM::Processing
{ 

	CLAM::InControl _targetX;
	CLAM::InControl _targetY;
	CLAM::InControl _targetZ;
	CLAM::InControl _targetAzimuth;
	CLAM::InControl _targetElevation;
	CLAM::InControl _sourceX;
	CLAM::InControl _sourceY;
	CLAM::InControl _sourceZ;

	CLAM::OutControl _sourceAzimuth;
	CLAM::OutControl _sourceElevation;
public:
	const char* GetClassName() const { return "AbsoluteCoordinates2RelativeAngles"; }
	AbsoluteCoordinates2RelativeAngles(const Config& config = Config()) 
		: _targetX("target X", this)
		, _targetY("target Y", this)
		, _targetZ("target Z", this)		
		, _targetAzimuth("target azimuth", this)
		, _targetElevation("target elevation", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)		
		, _sourceAzimuth("source azimuth", this)
		, _sourceElevation("source elevation", this)
	{
		Configure( config );
	}
 
	bool Do()
	{	

/*		const Row & row = _controlSequence[_sequenceIndex];
		//TODO check that _indexTargetX,Y,Z < row.size()
		_targetX.SendControl( row[TargetXColumn] );
		_targetY.SendControl( row[TargetYColumn] );
		_targetZ.SendControl( row[TargetZColumn] );*/
//		_targetAzimuth.SendControl( row[TargetAzimutColumn] );
//		_targetElevation.SendControl( row[TargetElevationColumn] );
/*		_sourceX.SendControl( row[SourceXColumn+3*sourceIndex] );
		_sourceY.SendControl( row[SourceYColumn+3*sourceIndex] );
		_sourceZ.SendControl( row[SourceZColumn+3*sourceIndex] );*/

		float sourceX = _sourceX.GetLastValue();
		float sourceY = _sourceY.GetLastValue();
		float sourceZ = _sourceZ.GetLastValue();
		float targetX = _targetX.GetLastValue();
		float targetY = _targetY.GetLastValue();
		float targetZ = _targetZ.GetLastValue();
		double targetAzimuth = _targetAzimuth.GetLastValue();
		double targetElevation = _targetElevation.GetLastValue()+PI/2; 
//		double targetRoll = 0;
		double dx = (sourceX - targetX);
		double dy = (sourceY - targetY);
		double dz = (sourceZ - targetZ);
		double cosAzimuth = std::cos(targetAzimuth);
		double sinAzimuth = std::sin(targetAzimuth);
		double cosElevation = std::cos(targetElevation);
		double sinElevation = std::sin(targetElevation);
//		double cosRoll = std::cos(targetRoll);
//		double sinRoll = std::sin(targetRoll);

//		double rotatedX = + cosAzimuth*sinElevation * dx + sinAzimuth * dy 	+ cosAzimuth*cosElevation * dz;
//		double rotatedY = - sinAzimuth*sinElevation * dx + cosAzimuth * dy 	- sinAzimuth*cosElevation * dz;
//		double rotatedZ = - cosElevation * dx 		 + /* 0 * vy[i] */  	+ sinElevation  * dz;

		double rotatedX = + cosAzimuth*sinElevation * dx + sinAzimuth * sinElevation * dy+ cosElevation * dz;
		double rotatedY = - sinAzimuth * dx + cosAzimuth * dy - 0 * dz;
		double rotatedZ = - cosElevation * cosAzimuth *dx + -cosElevation*sinAzimuth*dy 	+ sinElevation  * dz;

		// TODO: Test that with target elevation and azimut
		double dazimut = 180./M_PI*std::atan2(rotatedY,rotatedX);
		double delevation = 180./M_PI*std::asin(rotatedZ/std::sqrt(rotatedX*rotatedX+rotatedY*rotatedY+rotatedZ*rotatedZ));
		_sourceAzimuth.SendControl( dazimut );
		_sourceElevation.SendControl( delevation );
		
		return true;
	}

};
} // namespace
#endif // AbsoluteCoordinates2RelativeAngles_hxx
