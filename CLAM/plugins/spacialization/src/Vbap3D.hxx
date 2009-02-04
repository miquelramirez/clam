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
	struct Vector
	{
		float x;
		float y;
		float z;
	};

	typedef std::vector<unsigned> Triangle;

	CLAM::AudioInPort _w;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	CLAM::InControl _azimuth;
	CLAM::InControl _elevation;
	unsigned _lower;
	std::vector<double> _elevations;
	std::vector<double> _azimuths;
	std::vector<std::string> _names;
	std::vector<Triangle> _triangles;
	std::vector<Vector> _normals;
	std::vector< std::vector<Vector> > _speakersPositions;
	std::vector<float> _ortogonalProjection;
	int _currentTriangle;
	float _delta;

	Vector vectorialProduct(const Vector& v1, const Vector& v2) const
	{
		Vector result = {
			v1.y * v2.z - v1.z * v2.y ,
			v1.z * v2.x - v1.x * v2.z ,
			v1.x * v2.y - v1.y * v2.x ,
			};
		return result;
	}
	Vector product(const float& factor, const Vector& v) const
	{
		Vector result = { factor * v.x, factor * v.y, factor * v.z };
		return result;
	}

	float escalarProduct(const Vector& v1, const Vector& v2) const
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z + v2.z;
	}
	Vector substract(const Vector& v1, const Vector& v2) const
	{
		Vector result = {
			v1.x - v2.x ,
			v1.y - v2.y ,
			v1.z - v2.z ,
			};
		return result;
	}
	float mod(const Vector& v) const
	{
		return v.x*v.x + v.y*v.y + v.z*v.z;
	}
	float angle(const Vector& v1, const Vector& v2) const
	{
		return acos( escalarProduct(v1,v2) / mod(v1)*mod(v2) );
	}
	void print(const Vector& v, std::string name="") const
	{
		std::cout << name << " (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
	}

public:
	const char* GetClassName() const { return "Vbap3D"; }
	Vbap3D(const Config& config = Config()) 
		: _w("W", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _currentTriangle(0)
	{
		Configure( config );
		_azimuth.SetBounds(-360, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90);
		_delta = 0.00001; 
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
#if 0	
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
			{15, "Top", 0., 89.},
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
			//up TODO change using the 15th spk
			{13, 14, 11},
			{13, 12, 11},
			//down
			{5, 4, 2},
			{5, 3, 2},

			{0,0,0}
		};
		
#endif 
//testing setup 
		struct SpeakerPositions {
			int id;
			const char * name;
			float azimuth;
			float elevation;
		} speakers[] =	{
			{1, "left", -45., -45. },
			{2, "right", 45., -45.},
			{3, "back", -180., -45.},
			{4, "top", 0., 89.},
			{0, 0, 0., 0.}
		};
		struct Triangles {
			unsigned one;
			unsigned two;
			unsigned three;
		} triangles[] = {
			//floor
			{1, 2, 3},
			//front
			{1, 2, 4},
			//left
			{1,3,4},
			//right
			{2,3,4},
			//end
			{0,0,0}
		};
//end testing setup

		RemovePorts();
		const unsigned buffersize = BackendBufferSize();
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
			
			std::vector<Vector> speakersPos;
			speakersPos.resize(3);
			for (unsigned j=0; j<3; j++)
			{
				Vector r = {
					cos(speakers[t[j]].elevation) * cos(speakers[t[j]].azimuth),
					cos(speakers[t[j]].elevation) * sin(speakers[t[j]].azimuth),
					sin(speakers[t[j]].elevation),
					};
				speakersPos[j]=r;
			}
			_speakersPositions.push_back( speakersPos );
			Vector n = vectorialProduct( 
				substract(speakersPos[0], speakersPos[1]),  
				substract(speakersPos[0], speakersPos[2])
				);
			_normals.push_back(n);
			_ortogonalProjection.push_back( escalarProduct(n,speakersPos[2]) );
		}
		_elevation.DoControl(0.);
		_azimuth.DoControl(0.);
		_w.SetSize(buffersize);
		_w.SetHop(buffersize);
		return true;
	}
	
	/// params: the source azimuth and elevation
	int findTriangle(float azimuth, float elevation) const
	{
std::cout << "\nfind Triangle. triangles " << _triangles.size() << std::endl;

		unsigned triangle = 0;
		// find triangle testing them all
		for (unsigned i=0; i<_triangles.size(); i++)
		{
			Vector r_source = {
					cos(elevation) * cos(azimuth),
					cos(elevation) * sin(azimuth),
					sin(elevation),
				};
std::cout << "checking triangle "<< i << std::endl;
print(r_source, "r_source");
print(_normals[i], "normal");
			const float divisor = escalarProduct(_normals[i], r_source);
			if (fabs(divisor) < _delta) 
			{
std::cout << "divisor is 0 !!" << std::endl;
				continue;
			}
			const float t =  _ortogonalProjection[i] / divisor;
			std::cout << " t " << t << std::endl;
			if (t>1. or t<0.) continue;
			Vector r_I = product(t, r_source);
			Vector v1 = substract( _speakersPositions[i][0], r_I);
			Vector v2 = substract( _speakersPositions[i][1], r_I);
			Vector v3 = substract( _speakersPositions[i][2], r_I);
			if (fabs(angle(v1,v2) + angle(v2,v3) + angle(v3,v1) - 2*M_PI) < _delta)
			{
				// found!
				triangle = i;
				break;
			}	
		}
		return triangle;
	}

	bool Do()
	{
		const float as=fmod(_azimuth.GetLastValue()+360+180,360)-180; // as: azimuth_source
		const float es=_elevation.GetLastValue(); // es: elevation_source
		CLAM_DEBUG_ASSERT(as>=-180 and as<=+180, "azimuth expected in range -180, +180");
		CLAM_DEBUG_ASSERT(es>=-90 and es<=+90, "elevation expected in range -90, +90");
		int newTriangle = findTriangle(as, es);
		if (newTriangle==0) std::cout << "ERROR: no triangle found!" << std::endl;
		// change triangle
		if (_currentTriangle != newTriangle) // changed triangle
		{
			std::cout << _names[newTriangle] << " - ";
			std::cout << "last " << _currentTriangle << " current " << newTriangle << std::endl;
			_currentTriangle = newTriangle;
		}
		// obtain the three gains.
		unsigned speaker1 = _triangles[_currentTriangle][0];
		unsigned speaker2 = _triangles[_currentTriangle][1];
		unsigned speaker3 = _triangles[_currentTriangle][2];
		double& a1 = _azimuths[speaker1];
		double& a2 = _azimuths[speaker2];
		double& a3 = _azimuths[speaker3];
		double& e1 = _elevations[speaker1];
		double& e2 = _elevations[speaker2];
		double& e3 = _elevations[speaker3];
		double g1 = cos(a3)*cos(e3)*cos(es)*sin(as)*sin(e2)
			+ cos(as)*cos(es) * (cos(e2)*sin(a2)*sin(e3) - cos(e3)*sin(a3)*sin(e2)) 
			- cos(e2)*(cos(a2)*cos(es)*sin(as)*sin(e3) + cos(e3)*sin(a2-a3)*sin(es));
		float g2 = cos(e3)*(cos(es)*sin(a3-as)*sin(e1)
			+ cos(e1)*sin(a1-a3)*sin(es)) 
			- cos(e1)*cos(es)*sin(a1-as)*sin(e3);
		float g3 = cos(a2)*cos(e2)*cos(es)*sin(as)*sin(e1)
			+ cos(as)*cos(es)*(cos(e1)*sin(a1)*sin(e2)
			- cos(e2)*sin(a2)*sin(e1)) 
			- cos(e1)*(cos(a1)*cos(es)*sin(as)*sin(e2)
			+ cos(e2)*sin(a1-a2)*sin(es))
		;
		// copy audio in->out
		// TODO extract method
		const CLAM::DataArray& w =_w.GetAudio().GetBuffer();
		for (unsigned i=0; i<_outputs.size(); i++)
		{
			float gainToApply;
			if (i==speaker1) gainToApply = g1;
			if (i==speaker2) gainToApply = g2;
			if (i==speaker3) gainToApply = g3;
			else gainToApply = 0.;
			CLAM::DataArray& out =_outputs[i]->GetAudio().GetBuffer();
			for (int sample=0; sample<w.Size(); sample++)
				out[sample] = gainToApply>_delta ? w[sample]*gainToApply : 0.;
			
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

