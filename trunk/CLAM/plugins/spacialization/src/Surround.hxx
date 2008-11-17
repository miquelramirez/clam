#ifndef Surround_hxx
#define Surround_hxx

#include "NSurround.hxx"

/**
 Converts ambisonics B-Format (W,X,Y,Z) input to an
 ITU 5.1 Surround speaker system with channels
 L(30), R(-30), C(0), SL(110), SR(-110).

 @param[in] beta [Control] Angle in degrees for a simulated rotation of the receiver in the xy plane to the right.
 @param[in] p [Port] Pressure to be reproduced at the sweet-spot
 @param[in] vx [Port] X component of the velocity to be reproduced at the sweet-spot
 @param[in] vy [Port] Y component of the velocity to be reproduced at the sweet-spot
 @param[out] center [Port] Output for channel  placed at 0 degrees.
 @param[out] right [Port] Output for channel  placed at -30 degrees.
 @param[out] surroundRight [Port] Output for channel  placed at -110 degrees.
 @param[out] surroundLeft [Port] Output for channel  placed at 110 degrees.
 @param[out] left [Port] Output for channel  placed at 30 degrees.
 @todo Is that what is called ITU 5.1?
 @todo Control value beta, controls a simulated rotation of the receiver
 @todo Review whether the input should be ambisonics or pressure/velocity
 @todo Document ports and controls
 @ingroup SpatialAudio
*/
class Surround : public NSurround
{
	
public:
	const char* GetClassName() const { return "Surround"; }
	Surround(const Config& config = Config())
		: NSurround()
	{
		Configure( config );
		_nChannels = 5;
		_beta.SetBounds(-360, 360); //a complete spin on each slider direction

		struct SpeakerPositions {
			const char * name;
			int angle;
		} speaker[] =
		{
			{"center", 0},
			{"right", -30},
			{"surroundRight", -110},
			{"surroundLeft", 110},
			{"left", 30},
			{0,0}
		};
		
		unsigned buffersize = BackendBufferSize();
		for (unsigned i=0; speaker[i].name; i++)
		{
			double angle = speaker[i].angle;
			double rad = M_PI*angle/180;
			_sinAlphas.push_back( std::sin(rad) );
			_cosAlphas.push_back( std::cos(rad) );
			std::cout << "sin "<< angle << " "<< std::sin(rad) << std::endl;

			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( speaker[i].name, this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		_nChannels = _outputs.size();
		
		_p.SetSize(buffersize);
		_p.SetHop(buffersize);
		_vx.SetSize(buffersize);
		_vx.SetHop(buffersize);
		_vy.SetSize(buffersize);
		_vy.SetHop(buffersize);

	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		std::cout << "Surround::ConcreteConfigure()"<<std::endl;
		return true;
	}

};
#endif

