#ifndef Vbap3D_hxx
#define Vbap3D_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

/**
 This processing implements a 3D 
 Vector Based Array Panning (VBAP)
 @param[in] w [Port] Pressure that the virtual sound emits.
 @todo Control value azimuth, controls a simulated rotation of the receiver
 @todo Control value elevation, controls a simulated rotation of the receiver
 @todo Reuse angle normalization methods
 @ingroup SpatialAudio
*/

class Vbap3D : public CLAM::Processing
{ 
	CLAM::AudioInPort _w;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	CLAM::InControl _azimuth;
	CLAM::InControl _elevation;
	unsigned _lower;
	std::vector<double> _elevations;
	std::vector<double> _azimuths;
	std::vector<std::string> _names;
	unsigned _closerSpeaker;
public:
	const char* GetClassName() const { return "Vbap3D"; }
	Vbap3D(const Config& config = Config()) 
		: _w("W", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _closerSpeaker(0)
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90);
	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		RemovePorts();

		unsigned buffersize = BackendBufferSize();
		struct SpeakerPositions {
			const char * name;
			float azimuth;
			float elevation;
		} speakers[] =
		{
			{"Base Front Left", 45., -45.},
			{"Base Front Right", -45., -45.},
			{"Base Back Left", 135., -45.},
			{"Base Back Right", -135, -45.},
			{"Top Front Left", 45., 45.},
			{"Top Front Right", -45., 45.},
			{"Top Back Left", 135., 45.},
			{"Top Back Right", -135., 45.},
			{"Front Horizontal", 0., 0.},
			{"Front Left Horizontal", 60., 0.},
			{"Rear Left Horizontal", 120., 0.},
			{"Back Horizontal", 180., 0.},
			{"Rear Right Horizontal", -120., 0.},
			{"Front Right Horizontal", -60., 0.},
			{0, 0., 0.}
		};
		for (unsigned i=0; speakers[i].name; i++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( speakers[i].name, this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
			_azimuths.push_back( speakers[i].azimuth );
			_elevations.push_back( speakers[i].elevation );
			_names.push_back( speakers[i].name );


		}
		_elevation.DoControl(0.);
		_azimuth.DoControl(0.);
		_w.SetSize(buffersize);
		_w.SetHop(buffersize);
		return true;
	}

	bool Do()
	{
		const double delta = 0.001;
		const double azimuth=fmod(_azimuth.GetLastValue()+360+180,360)-180;
		const double elevation=_elevation.GetLastValue();
		CLAM_DEBUG_ASSERT(azimuth>=-180 and azimuth<=+180, "azimuth expected in range -180, +180");
		CLAM_DEBUG_ASSERT(elevation>=-90 and elevation<=+90, "elevation expected in range -90, +90");
		
		double elevationError = 180.;
		double azimuthError = 360.;
		unsigned newSpeaker = 0;
		// find the min error for elevations
		for (unsigned i=0; i<_names.size(); i++)
		{
			const double err = abs(_elevations[i]-elevation);
			if (err < elevationError) elevationError = err;
		}
		// find the min error for azimuths among those with min elevation error
		for (unsigned i=0; i<_names.size(); i++)
		{
			if ( abs(_elevations[i]-elevation) > elevationError+delta )
				continue;
			const double err = abs(_azimuths[i]-azimuth);
			if (err < azimuthError) 
			{
				azimuthError = err;
				newSpeaker = i;
			}
		}
		if (_closerSpeaker != newSpeaker) // changed speaker
		{
			std::cout << _names[newSpeaker] << " - ";
			std::cout << "last " << _closerSpeaker << " current " << newSpeaker << std::endl;
			_closerSpeaker = newSpeaker;
		}
		// copy audio in->out
		const CLAM::DataArray& w =_w.GetAudio().GetBuffer();
		for (unsigned i=0; i<_outputs.size(); i++)
		{
			CLAM::DataArray& out =_outputs[i]->GetAudio().GetBuffer();
			for (int sample=0; sample<w.Size(); sample++)
				out[sample] = (i==_closerSpeaker)? w[sample] : 0.;
			_outputs[i]->Produce();
			
		}
		_w.Consume();
		return true;
	}
private:
	void RemovePorts()
	{
		OutPorts::iterator it;
		for(it=_outputs.begin(); it!=_outputs.end(); it++)
			delete *it;
		_outputs.clear();
		// Delete ports from Processing (base class) register
		GetOutPorts().Clear();
	}
	

};
#endif

