#ifndef Vbap3D_OneSpeaker_hxx
#define Vbap3D_OneSpeaker_hxx
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

class Vbap3D_OneSpeaker : public CLAM::Processing
{
	typedef std::vector<unsigned> Triangle;

	CLAM::AudioInPort _w;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	CLAM::FloatInControl _azimuth;
	CLAM::FloatInControl _elevation;
	unsigned _lower;
	std::vector<double> _elevations;
	std::vector<double> _azimuths;
	std::vector<std::string> _names;
	std::vector<Triangle> _triangles;
	unsigned _closerSpeaker;
public:
	const char* GetClassName() const { return "Vbap3D_OneSpeaker"; }
	Vbap3D_OneSpeaker(const Config& config = Config()) 
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
			int id;
			const char * name;
			float azimuth;
			float elevation;
		} speakers[] =	{
			{1, "Back Horizontal", 180., 0. },
			{2, "Base Back Left", 135., -45.},
			{3, "Base Back Right", -135, -45.},
			{4, "Base Front Left", 45., -45.},
			{5, "Base Front Right", -45., -45.},
			{6, "Front Horizontal", 0., 0.},
			{7, "Front Left Horizontal", 60., 0.},
			{8, "Front Right Horizontal", -60., 0.},
			{9, "Rear Left Horizontal", 120., 0.},
			{10, "Rear Right Horizontal", -120., 0.},
			{11, "Top Back Left", 135., 45.},
			{12, "Top Back Right", -135., 45.},
			{13, "Top Front Left", 45., 45.},
			{14, "Top Front Right", -45., 45.},
			{0, 0, 0., 0.}
		};
		struct Triangles {
			unsigned one;
			unsigned two;
			unsigned three;
		} triangles[] = {
			//front
			{6, 8, 13},
			{6, 8, 5},
			{6, 7, 4},
			{6, 7, 14},
			{6, 13, 14},
			{6, 4, 5},
			//back
			{1, 9, 11},
			{1, 9, 2},
			{1, 10, 12},
			{1, 10, 3},
			{1, 12, 11},
			{1, 2, 3},
			//left
			{9, 7, 14},
			{9, 7, 4},
			{9, 4, 2},
			{9, 14, 11},
			//right
			{10, 8, 13},
			{10, 8, 5},
			{10, 3, 5},
			{10, 12, 13},
			//up
			{13, 14, 11},
			{13, 12, 11},
			//down
			{5, 4, 2},
			{5, 3, 2},

			{0,0,0}
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
		for (unsigned i=0; triangles[i].one; i++)
		{
			Triangle t;
			t.resize(3);
			t[0]=triangles[i].one;
			t[1]=triangles[i].two;
			t[2]=triangles[i].three;
			_triangles.push_back(t);
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

