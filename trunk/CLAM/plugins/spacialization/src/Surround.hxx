#ifndef Surround_hxx
#define Surround_hxx

#include "NSurround.hxx"

/**
 Converts ambisonics B-Format (W,X,Y,Z) input to an
 ITU 5.1 Surround speaker system with channels
 L(30), R(-30), C(0), SL(110), SR(-110).

 @param[in] W [Port] W ambisonics component.
 @param[in] X [Port] X ambisonics component.
 @param[in] Y [Port] Y ambisonics component.
 @param[out] center [Port] Output for channel  placed at 0 degrees.
 @param[out] right [Port] Output for channel  placed at -45 degrees.
 @param[out] surroundRight [Port] Output for channel  placed at -135 degrees.
 @param[out] surroundLeft [Port] Output for channel  placed at 135 degrees.
 @param[out] left [Port] Output for channel  placed at 45 degrees.

 The encoding angles are not the ITU angles but -45, 45, 135, -135.
 @see AmbisonicsConventions

 @todo Is that what is called ITU 5.1?
 @todo Control value beta, controls a simulated rotation of the receiver
 @todo Review whether the input should be ambisonics or pressure/velocity
 @todo Document ports and controls
 @ingroup SpatialAudio
*/
class Surround : public NSurround
{
	typedef Processing::Config Config;
public:
	const char* GetClassName() const { return "Surround"; }
	Surround(const Config& config = Config())
		: NSurround()
	{
		Configure( config );
		_nChannels = 5;

		struct SpeakerPositions {
			const char * name;
			int angle;
		} speaker[] =
		{
		// The following angles are "encoding" angles not ITU exhibiton setup angles.
			{"center", 0},
			{"right", -45},
			{"surroundRight", -135},
			{"surroundLeft", 135},
			{"left", 45},
			{0,0}
		};
		
		unsigned buffersize = BackendBufferSize();
		for (unsigned i=0; speaker[i].name; i++)
		{
			double angle = speaker[i].angle;
			double rad = M_PI*angle/180;
			_sinAlphas.push_back( std::sin(rad) );
			_cosAlphas.push_back( std::cos(rad) );

			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( speaker[i].name, this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		_nChannels = _outputs.size();
		
		_w.SetSize(buffersize);
		_w.SetHop(buffersize);
		_x.SetSize(buffersize);
		_x.SetHop(buffersize);
		_y.SetSize(buffersize);
		_y.SetHop(buffersize);

	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		static Config config;
		return config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		// Just avoid base class configuration
		return true;
	}

};
#endif

