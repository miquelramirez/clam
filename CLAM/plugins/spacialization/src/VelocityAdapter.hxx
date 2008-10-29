#ifndef VelocityAdapter_hxx
#define VelocityAdapter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>

/**
 Inverts the left-right orientation (Y) of a 2D acoustic velocity signal.
 @param[in] "beta input" [Control] Rotation to be applied.
 @param[in] "Vx input" [Port] X component of the velocity.
 @param[in] "VY input" [Port] Y component of the velocity.
 @param[out] "beta output" [Control] Inverted rotation (changes the sign).
 @param[out] "Vx output" [Port] Inverted X component of the velocity (doesn't change).
 @param[out] "Vy output" [Port] Inverted Y component of the velocity (changes the sign).
 @deprecated It won't have sense once we uses AmbisonicsConventions for everything
 @todo Generalize VelocityAdapter to something like Physical2BFormat
 @ingroup SpatialAudio
*/ 
class VelocityAdapter : public CLAM::Processing
{ 
	CLAM::AudioInPort _vxIn;
	CLAM::AudioOutPort _vxOut;
	CLAM::AudioInPort _vyIn;
	CLAM::AudioOutPort _vyOut;
	CLAM::FloatInControl _betaIn;
	CLAM::FloatOutControl _betaOut;
public:
	const char* GetClassName() const { return "VelocityAdapter"; }
	VelocityAdapter(const Config& config = Config()) 
		: _vxIn("Vx input", this)
		, _vxOut("Vx output", this) 
		, _vyIn("Vy input", this) 
		, _vyOut("Vy output", this) 
		, _betaIn("beta input", this)
		, _betaOut("beta output", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		const CLAM::DataArray& vxin =_vxIn.GetAudio().GetBuffer();
		const CLAM::DataArray& vyin =_vyIn.GetAudio().GetBuffer();
		CLAM::DataArray& vxout =_vxOut.GetAudio().GetBuffer();
		CLAM::DataArray& vyout =_vyOut.GetAudio().GetBuffer();

		const int size = vxin.Size();
		for (int i=0; i<size; i++)
		{
			vxout[i] = vxin[i];
			vyout[i] = -vyin[i];

		}
		float beta = _betaIn.GetLastValue();
		_betaOut.SendControl( -beta );
		// Tell the ports this is done
		_vxIn.Consume();
		_vyIn.Consume();
		_vxOut.Produce();
		_vyOut.Produce();
		return true;
	}

};

#endif // VelocityAdapter_hxx

