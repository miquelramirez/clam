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
		
		std::vector<int> angles;
		angles.push_back(0);
		angles.push_back(30);
		angles.push_back(110);
		angles.push_back(-110);
		angles.push_back(-300);
		std::vector<std::string> names;
		names.push_back("center");
		names.push_back("left");
		names.push_back("right");
		names.push_back("surroundLeft");
		names.push_back("surroundRight");
		for (unsigned i=0; i<_nChannels; i++)
		{
			double angle = 360.*i/_nChannels;
			angles.push_back(int(angle));
			double rad = M_PI*angle/180;
			_sinAlphas.push_back( std::sin(rad) );
			_cosAlphas.push_back( std::cos(rad) );
			std::cout << "sin "<< angle << " "<< std::sin(rad) << std::endl;
		}
		unsigned buffersize = BackendBufferSize();
		for (unsigned i=0; i<_nChannels; i++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( names[i], this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		
		_p.SetSize(buffersize);
		_p.SetHop(buffersize);
		_vx.SetSize(buffersize);
		_vx.SetHop(buffersize);
		_vy.SetSize(buffersize);
		_vy.SetHop(buffersize);

	}

};
#endif

