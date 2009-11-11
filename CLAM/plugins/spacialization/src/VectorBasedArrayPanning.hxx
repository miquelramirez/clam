#ifndef VectorBasedArrayPanning_hxx
#define VectorBasedArrayPanning_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

/**
 This processing implements a 2D 
 Vector Based Array Panning (VBAP)
 algorithm to virtually map a sound source 
 in a direction along the horizontal plane
 by using the two nearer speakers of an
 ITU 5.1 Surround speaker system with channels 
 L(30), R(-30), C(0), SL(110), SR(-110).
 @param[in] w [Port] Ambisonics B-format W channel (omni directionsl pressure) that the virtual sound emits.
 @param[out] center [Port] Output for channel  placed at 0 degrees.
 @param[out] right [Port] Output for channel  placed at -30 degrees.
 @param[out] surroundRight [Port] Output for channel  placed at -110 degrees.
 @param[out] surroundLeft [Port] Output for channel  placed at 110 degrees.
 @param[out] left [Port] Output for channel  placed at 30 degrees.
 @todo Is that what is called ITU 5.1?
 @todo Control value beta, controls a simulated rotation of the receiver
 @ingroup SpatialAudio
*/

class VectorBasedArrayPanning : public CLAM::Processing
{ 
	CLAM::AudioInPort _w;
	CLAM::AudioOutPort _center;
	CLAM::AudioOutPort _left;
	CLAM::AudioOutPort _right;
	CLAM::AudioOutPort _surroundLeft;
	CLAM::AudioOutPort _surroundRight;
	CLAM::FloatInControl _beta;
	unsigned _lower;
public:
	const char* GetClassName() const { return "VectorBasedArrayPanning"; }
	VectorBasedArrayPanning(const Config& config = Config()) 
		: _w("W", this)
		, _center("center", this)
		, _left("left", this)
		, _right("right", this)
		, _surroundLeft("surroundLeft", this)
		, _surroundRight("surroundRight", this)
		, _beta("beta", this) // angle in degrees
	{
		Configure( config );
		_beta.SetBounds(-360, 360); //a complete spin on each slider direction
	}

	unsigned getUpperChannel(double beta, const double * alphas, unsigned nChannels)
	{
		for (unsigned channel=0; channel<nChannels; channel++)
			if (alphas[channel] > beta)
				return channel;
		return 0;
	}
 
	bool Do()
	{
		const double beta=fmod(_beta.GetLastValue()+180,360)-180;
		CLAM_DEBUG_ASSERT(beta>=-180 and beta<=+180, "beta expected in range -180, +180");
		const CLAM::DataArray& w =_w.GetAudio().GetBuffer();
		CLAM::DataArray& center =_center.GetAudio().GetBuffer();
		CLAM::DataArray& left =_left.GetAudio().GetBuffer();
		CLAM::DataArray& right =_right.GetAudio().GetBuffer();
		CLAM::DataArray& surroundLeft =_surroundLeft.GetAudio().GetBuffer();
		CLAM::DataArray& surroundRight =_surroundRight.GetAudio().GetBuffer();

		const double alphas[]= { -110, -30, 0, 30, 110 };
		const char * names[]= { "SR", "R", "C", "L", "RL" };
		const unsigned nChannels=5;
		unsigned upper = getUpperChannel(beta, alphas, nChannels);
		unsigned lower = (upper+nChannels-1)%nChannels;
		if (lower != _lower)
		{
			std::cout << " Between " << names[lower] << " and " << names[upper] << std::endl;
			_lower = lower;
		}
		const double sinToLower = std::sin( (beta-alphas[lower])*M_PI/180 );
		const double sinToUpper = std::sin( (alphas[upper]-beta)*M_PI/180 );
		const double gLower = sinToUpper / (sinToLower*sinToLower + sinToUpper*sinToUpper);
		const double gUpper = sinToLower / (sinToLower*sinToLower + sinToUpper*sinToUpper);
		CLAM::TData * channels[nChannels];
		channels[0]=&surroundRight[0];
		channels[1]=&right[0];
		channels[2]=&center[0];
		channels[3]=&left[0];
		channels[4]=&surroundLeft[0];
		for (unsigned channel=0; channel<nChannels; channel++)
		{
			if (channel==lower)
			{
				for (int i=0; i<w.Size(); i++)
					channels[channel][i]=w[i]*gLower;
			}
			else if (channel==upper)
			{
				for (int i=0; i<w.Size(); i++)
					channels[channel][i]=w[i]*gUpper;
			}
			else
			{
				for (int i=0; i<w.Size(); i++)
					channels[channel][i]=0.;
			}
		}
		// Tell the ports this is done
		_w.Consume();
		_center.Produce();
		_left.Produce();
		_right.Produce();
		_surroundLeft.Produce();
		_surroundRight.Produce();
		return true;
	}

};
#endif

