#ifndef Surround_hxx
#define Surround_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>

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
	{
		Configure( config );
	}
 
	bool Do()
	{
		float beta=0.f;
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
			std::cout << "." << std::flush;
		}
		std::cout << "|" << std::flush;

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

