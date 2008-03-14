#ifndef Surround_hxx
#define Surround_hxx

#include "NSurround.hxx"

class Surround : public NSurround
{
	
public:
	const char* GetClassName() const { return "Surround"; }
	Surround(const Config& config = Config())
		: NSurround(0)
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
			{"right", 30},
			{"surroundRight", 110},
			{"surroundLeft", -110},
			{"left", -30},
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

