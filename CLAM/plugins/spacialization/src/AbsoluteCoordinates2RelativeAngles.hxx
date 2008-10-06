#ifndef AbsoluteCoordinates2RelativeAngles_hxx 
#define AbsoluteCoordinates2RelativeAngles_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>

namespace CLAM
{
class AbsoluteCoordinates2RelativeAngles : public CLAM::Processing
{ 

	CLAM::InControl _targetX;
	CLAM::InControl _targetY;
	CLAM::InControl _targetZ;
	CLAM::InControl _targetAzimuth;
	CLAM::InControl _targetZenit;
	CLAM::InControl _sourceX;
	CLAM::InControl _sourceY;
	CLAM::InControl _sourceZ;

	CLAM::OutControl _sourceAzimuth;
	CLAM::OutControl _sourceZenith;
public:
	const char* GetClassName() const { return "AbsoluteCoordinates2RelativeAngles"; }
	AbsoluteCoordinates2RelativeAngles(const Config& config = Config()) 
		: _targetX("target X", this)
		, _targetY("target Y", this)
		, _targetZ("target Z", this)		
		, _targetAzimuth("target azimuth", this)
		, _targetZenit("target elevation", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)		
		, _sourceAzimuth("source azimuth", this)
		, _sourceZenith("source elevation", this)
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
//		_targetZenith.SendControl( row[TargetZenitColumn] );
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
		double targetZenit = _targetZenit.GetLastValue(); 
		double dx = (sourceX - targetX);
		double dy = (sourceY - targetY);
		double dz = (sourceZ - targetZ);
		double cosAzimuth = std::cos(targetAzimuth);
		double sinAzimuth = std::sin(targetAzimuth);
		double cosZenith = std::cos(targetZenit);
		double sinZenith = std::sin(targetZenit);
		double rotatedX = + cosAzimuth*sinZenith * dx + sinAzimuth * dy - cosAzimuth*cosZenith * dz;
		double rotatedY = - sinAzimuth*sinZenith * dx + cosAzimuth * dy + sinAzimuth*cosZenith * dz;
		double rotatedZ = + cosZenith * dx + /* 0 * vy[i] */  + sinZenith  * dz;

		// TODO: Test that with target elevation and azimut
		double dazimut = 180./M_PI*std::atan2(rotatedY,rotatedX);
		double delevation = 180./M_PI*std::asin(rotatedZ/std::sqrt(rotatedX*rotatedX+rotatedY*rotatedY+rotatedZ*rotatedZ));

		_sourceAzimuth.SendControl( dazimut );
		_sourceZenith.SendControl( delevation );
		
		return true;
	}

};
} // namespace
#endif // AbsoluteCoordinates2RelativeAngles_hxx
