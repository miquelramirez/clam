#ifndef VelocityAdapter_hxx
#define VelocityAdapter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>

class VelocityAdapter : public CLAM::Processing
{ 
	CLAM::AudioInPort _vxIn;
	CLAM::AudioOutPort _vxOut;
	CLAM::AudioInPort _vyIn;
	CLAM::AudioOutPort _vyOut;
	CLAM::InControl _betaIn;
	CLAM::OutControl _betaOut;
public:
	const char* GetClassName() const { return "VelocityAdapter"; }
	VelocityAdapter(const Config& config = Config()) 
		: _vxIn("Vx input", this)
		, _vxOut("Vy input", this) 
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
			vxout[i] = vyin[i];
			vyout[i] = vxin[i];

		}
		float beta = _betaIn.GetLastValue();
		_betaOut.SendControl( beta+90. );
		// Tell the ports this is done
		_vxIn.Consume();
		_vyIn.Consume();
		_vxOut.Produce();
		_vyOut.Produce();
		return true;
	}

};

#endif // VelocityAdapter_hxx

