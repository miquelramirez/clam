#ifndef Surround_hxx
#define Surround_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

class Surround : public CLAM::Processing
{ 
	CLAM::AudioInPort _p;
	CLAM::AudioInPort _vx;
	CLAM::AudioInPort _vy;
	CLAM::AudioOutPort _center;
	CLAM::AudioOutPort _left;
	CLAM::AudioOutPort _right;
	CLAM::AudioOutPort _surroundLeft;
	CLAM::AudioOutPort _surroundRight;
	CLAM::InControl _beta;
public:
	const char* GetClassName() const { return "Surround"; }
	Surround(const Config& config = Config()) 
		: _p("p", this)
		, _vx("vx", this)
		, _vy("vy", this)
		, _center("center", this)
		, _left("left", this)
		, _right("right", this)
		, _surroundLeft("surroundLeft", this)
		, _surroundRight("surroundRight", this)
		, _beta("beta", this)
	{
		Configure( config );
		_beta.SetBounds(-2*M_PI,2*M_PI); //a complete spin on each slider direction
	}
 
	bool Do()
	{
		const double beta=_beta.GetLastValue();
		const double cosBeta=std::cos(beta);
		const double sinBeta=std::sin(beta);
		const double sin30 = .5;
		const double cos30 = 0.8660254037844387076;
		const double sin110 = 0.9396926207859084279;
		const double cos110 = -0.3420201433256687129;
		const CLAM::DataArray& p =_p.GetAudio().GetBuffer();
		const CLAM::DataArray& vx =_vx.GetAudio().GetBuffer();
		const CLAM::DataArray& vy =_vy.GetAudio().GetBuffer();
		CLAM::DataArray& center =_center.GetAudio().GetBuffer();
		CLAM::DataArray& left =_left.GetAudio().GetBuffer();
		CLAM::DataArray& right =_right.GetAudio().GetBuffer();
		CLAM::DataArray& surroundLeft =_surroundLeft.GetAudio().GetBuffer();
		CLAM::DataArray& surroundRight =_surroundRight.GetAudio().GetBuffer();
		for (int i=0; i<p.Size(); i++)
		{
			double ux = vx[i] * cosBeta - vy[i] * sinBeta;
			double uy = vx[i] * sinBeta + vy[i] * cosBeta;
			// General C_i = p - ux * cos(alpha_i) - uy * sin(alpha_i)
			center[i] = p[i] - ux; // alpha_center = 0 
			left[i] = p[i] - ux * cos30 - uy * sin30;
			right[i] = p[i] - ux * cos30 + uy * sin30;
			surroundLeft[i] = p[i] - ux * cos110 - uy * sin110;
			surroundRight[i] = p[i] - ux * cos110 + uy * sin110;
		}

		// Tell the ports this is done
		_p.Consume();
		_vx.Consume();
		_vy.Consume();
		_center.Produce();
		_left.Produce();
		_right.Produce();
		_surroundLeft.Produce();
		_surroundRight.Produce();
		return true;
	}

};
#endif

