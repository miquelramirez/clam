#ifndef Vbap3D_hxx
#define Vbap3D_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Filename.hxx>
#include "Orientation.hxx"
#include "SpeakerLayout.hxx"
#include <cmath>

/**
 This processing implements a 3D Vector Based Array Panning (VBAP)
 @param SpeakerLayout [Config] A file containing the target speaker layout.
 @param Triangulation [Config] A file containing a triangulation for the layout.
 @param[in] w [Port] Pressure that the virtual sound emits.
 @todo Control value azimuth, controls a simulated rotation of the receiver
 @todo Control value elevation, controls a simulated rotation of the receiver
 @todo Reuse angle normalization methods
 @ingroup SpatialAudio
*/

class Vbap3D : public CLAM::Processing
{
public:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, CLAM::InFilename, SpeakerLayout);
		DYN_ATTRIBUTE( 1, public, CLAM::InFilename, Triangulation);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
		};
	};

private:

	struct Vector
	{
		float x;
		float y;
		float z;
	};

	float _lastAzimuth;
	float _lastElevation;

	typedef std::vector<unsigned> Triangle;

	SpeakerLayout _layout;
	CLAM::AudioInPort _w;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	CLAM::InControl _azimuth;
	CLAM::InControl _elevation;
	Config _config;
	std::vector<Triangle> _triangles;
	std::vector<Vector> _normals;
	std::vector<Vector> _speakersPositions;
	std::vector<float> _ortogonalProjection;
	int _currentTriangle;
	float _deltaAngle;
	float _deltaNumeric;

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
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
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
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}
	float angle(const Vector& v1, const Vector& v2) const
	{
		float divisor = mod(v1)*mod(v2);
		CLAM_ASSERT( divisor > _deltaNumeric, "Cannot compute an angle of a zero vector"); 
		float arg =  escalarProduct(v1,v2) / (mod(v1)*mod(v2));
		if (arg <-1 or arg >1) return arg < 0 ? M_PI : 0;
		return acos( arg );
	}
	void print(const Vector& v, std::string name="") const
	{
		std::cout << name << " (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
	}
	float rad( float deg ) const
	{
		return deg / 180 * M_PI;
	}

public:
	const char* GetClassName() const { return "Vbap3D"; }
	Vbap3D(const Config& config = Config()) 
		: _lastAzimuth(0)
		, _lastElevation(0)
		, _w("W", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _currentTriangle(0)
	{
		Configure( config );
		_azimuth.SetBounds(0, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90);
		_deltaAngle = 0.001; 
		_deltaNumeric = 0.00001; 
	}
	~Vbap3D()
	{
		for (unsigned speaker=0; speaker<_outputs.size(); speaker++)
			delete _outputs[speaker];
	}

	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{

		CopyAsConcreteConfig(_config, config);
#if 1	
		struct SpeakerPositions {
			int id;
			const char * name;
			float azimuth;
			float elevation;
		} speakers[] =	{
			{0, "Back Horizontal", 180., 0. },
			{1, "Base Back Left", 135., -45.},
			{2, "Base Back Right", -135, -45.},
			{3, "Base Front Left", 45., -45.},
			{4, "Base Front Right", -45., -45.},
			{5, "Front Horizontal", 0., 0.},
			{6, "Front Left Horizontal", 60., 0.},
			{7, "Front Right Horizontal", -60., 0.},
			{8, "Rear Left Horizontal", 120., 0.},
			{9, "Rear Right Horizontal", -120., 0.},
			{10, "Top Back Left", 135., 45.},
			{11, "Top Back Right", -135., 45.},
			{12, "Top Front Right", -45., 45.},
			{13, "Top Front Left", 45., 45.},
			{14, "Top", 0., 89.},
			{0, 0, 0., 0.}
		};
		struct Triangles {
			unsigned one;
			unsigned two;
			unsigned three;
		} triangles[] = {
			//front
			{5, 7, 12},
			{5, 7, 4},
			{5, 6, 3},
			{5, 6, 13},
			{5, 12, 13},
			{5, 3, 4},
			//back
			{0, 8, 10},
			{0, 8, 1},
			{0, 9, 11},
			{0, 9, 2},
			{0, 11, 10},
			{0, 1, 2},
			//left
			{8, 6, 13},
			{8, 6, 3},
			{8, 3, 1},
			{8, 13, 10},
			//right
			{9, 7, 12},
			{9, 7, 4},
			{9, 2, 4},
			{9, 11, 12},
			//up 
			{14, 10, 11},
			{14, 11, 12},
			{14, 12, 13},
			{14, 13, 10},
			//down
			{4, 3, 1},
			{4, 2, 1},

			{0,0,0}
		};
		
#else
//testing setup 
		struct SpeakerPositions {
			int id;
			const char * name;
			float azimuth;
			float elevation;
		} speakers[] =	{
			{0, "left", -45., -45. },
			{1, "right", 45., -45.},
			{2, "back", -180., -45.},
			{3, "top", 0., 89.},
			{0, 0, 0., 0.}
		};
		struct Triangles {
			unsigned one;
			unsigned two;
			unsigned three;
		} triangles[] = {
			//floor
			{0, 1, 2},
			//front
			{0, 1, 3},
			//left
			{0,2,3},
			//right
			{1,2,3},
			//end
			{0,0,0}
		};
//end testing setup
#endif

		_layout.clear();
		_speakersPositions.clear();
		for (unsigned i=0; speakers[i].name; i++)
		{
			_layout.add(speakers[i].azimuth, speakers[i].elevation, speakers[i].name);
			const CLAM::Orientation & speaker=_layout.orientation(i);
			Vector r = {
				speaker.ce * speaker.ca,
				speaker.ce * speaker.sa,
				speaker.se,
				};
			_speakersPositions.push_back(r);
		}
		const unsigned buffersize = BackendBufferSize();
		ResizePortsToLayout(buffersize);
		_triangles.clear();
		_normals.clear();
		_ortogonalProjection.clear();
		for (unsigned i=0; triangles[i].one!=triangles[i].two; i++)
		{
			Triangle t;
			t.resize(3);
			t[0]=triangles[i].one;
			t[1]=triangles[i].two;
			t[2]=triangles[i].three;
			_triangles.push_back(t);
			
			Vector normal = vectorialProduct( 
				substract(_speakersPositions[t[0]], _speakersPositions[t[1]]),  
				substract(_speakersPositions[t[0]], _speakersPositions[t[2]])
				);
			_normals.push_back(normal);
			_ortogonalProjection.push_back( escalarProduct(normal,_speakersPositions[t[2]]) );
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

//std::cout << "\nfind Triangle. triangles " << _triangles.size() << std::endl;
		// find triangle testing them all
		Vector r_source = {
			cos(elevation) * cos(azimuth),
			cos(elevation) * sin(azimuth),
			sin(elevation),
		};
		for (unsigned i=0; i<_triangles.size(); i++)
		{
//std::cout << "\n\nchecking triangle "<< i << std::endl;
//print(r_source, "r_source");
//print(_normals[i], "normal");
//print(_speakersPositions[i][0], "speak 0");
//print(_speakersPositions[i][1], "speak 1");
//print(_speakersPositions[i][2], "speak 2");

			const Triangle & triangle = _triangles[i];
			const float divisor = escalarProduct(_normals[i], r_source);
			// If source direction and the plane are almost ortogonal, continue
			// (also avoids a divide by zero)
			if (fabs(divisor) < _deltaNumeric) continue;

			const float t =  _ortogonalProjection[i] / divisor;
//std::cout << "--> t " << t  << " t-1 " << t-1. << std::endl;
			if (t < 0.) continue; // opposite direction
			if (t > 1.+_deltaNumeric) continue; // TODOC: what does it means
//std::cout << "--> Ok intersection line < 1" << std::endl;
			Vector intersection = product(t, r_source);
			Vector v1 = substract( _speakersPositions[triangle[0]], intersection);
			Vector v2 = substract( _speakersPositions[triangle[1]], intersection);
			Vector v3 = substract( _speakersPositions[triangle[2]], intersection);
			// If intersection is too close to one of the vertex, consider this triangle
			if (mod(v1)*mod(v2)*mod(v3) < _deltaNumeric)
			{
				return i; // Matches one of the speakers
			}
			// if the sum of the relative angles is 
			if (fabs(angle(v1,v2) + angle(v2,v3) + angle(v3,v1) - 2*M_PI) < _deltaAngle)
			{
				return i; // Inside a triangle
			}
		}
		return -1; // None found
	}

	bool Do()
	{
		CLAM::Orientation ori = CLAM::Orientation(_azimuth.GetLastValue(), _elevation.GetLastValue());
		ori.normalize();
		const float azimuthDegrees = ori.azimuth;
		const float elevationDegrees = ori.elevation;
		if (_lastAzimuth!=azimuthDegrees or _lastElevation!=elevationDegrees)
		{
			_lastAzimuth=azimuthDegrees;
			_lastElevation=elevationDegrees;
//std::cout << "\nazimuth, elevation: " << azimuthDegrees << "\t" << elevationDegrees << std::endl;

		}

		const float as = rad( azimuthDegrees );
		const float es = rad( elevationDegrees );
		int newTriangle = findTriangle(as, es);
		CLAM_ASSERT(newTriangle > -1, "Vbap3D: findTriangle() found no triangle for the given angle!");
		if (newTriangle==-1) newTriangle = 0;
		
		// change triangle
		if (_currentTriangle != newTriangle) // changed triangle
		{
			std::cout << " changing triangle: " << _currentTriangle << "->" << newTriangle << " " << std::flush;
			_currentTriangle = newTriangle;

//std::cout << " speakers: "<< _triangles[_currentTriangle][0] << " " << _triangles[_currentTriangle][1] << " " << _triangles[_currentTriangle][2] << std::endl;
		}
		// obtain the three gains.
		Triangle & triangle = _triangles[_currentTriangle];
		unsigned speaker1 = triangle[0];
		unsigned speaker2 = triangle[1];
		unsigned speaker3 = triangle[2];
		const CLAM::Orientation & o1 = _layout.orientation(speaker1);
		const CLAM::Orientation & o2 = _layout.orientation(speaker2);
		const CLAM::Orientation & o3 = _layout.orientation(speaker3);
		double a1 = o1.aradians;
		double a2 = o2.aradians;
		double a3 = o3.aradians;
		double se1=o1.se, ce1=o1.ce, sa1=o1.sa, ca1=o1.ca;
		double se2=o2.se, ce2=o2.ce, sa2=o2.sa, ca2=o2.ca;
		double se3=o3.se, ce3=o3.ce, sa3=o3.sa, ca3=o3.ca;
		double xs = cos(as)*cos(es);
		double ys = sin(as)*cos(es);
		double zs = sin(es);
		double project1s = -xs*sa1 +ys*ca1;
		double project2s = -xs*sa2 +ys*ca2;
		double project3s = -xs*sa3 +ys*ca3;
		double g1 = 
			+ ce3*se2*project3s
			- ce2*(
				+ se3*project2s
				+ zs*ce3*sin(a2-a3)
				)
			;
		float g2 = 
			+ ce1*se3*project1s
			- ce3*(
				+ se1*project3s
				+ zs*ce1*sin(a3-a1)
				) 
			;
		float g3 =
			+ ce2*se1*project2s
			- ce1*(
				+ se2*project1s
				+ zs*ce2*sin(a1-a2)
				)
			;
		float normalization = 1. / sqrt(g1*g1 + g2*g2 + g3*g3);
		g1 = fabs(g1) * normalization;
		g2 = fabs(g2) * normalization;
		g3 = fabs(g3) * normalization;
		// copy audio in->out
		// TODO extract method
		const CLAM::DataArray& w =_w.GetAudio().GetBuffer();
		for (unsigned i=0; i<_outputs.size(); i++)
		{
			float gainToApply = 0.;
			if (i==speaker1) gainToApply = g1;
			if (i==speaker2) gainToApply = g2;
			if (i==speaker3) gainToApply = g3;
			CLAM::DataArray& out =_outputs[i]->GetAudio().GetBuffer();
			for (int sample=0; sample<w.Size(); sample++)
				out[sample] = gainToApply>_deltaNumeric ? w[sample]*gainToApply : 0.;
			
			_outputs[i]->Produce();
		}
		_w.Consume();
		return true;
	}
private:
	void ResizePortsToLayout(unsigned buffersize)
	{
		// Set up the outputs according to the layout
		unsigned speakerToUpdate = firstDirtySpeaker();
		// delete existing speakers from the first one with different name
		for (unsigned speaker=0; speaker<speakerToUpdate; speaker++)
		{
			// Update the size and hop just in case
			_outputs[speaker]->SetSize( buffersize );
			_outputs[speaker]->SetHop( buffersize );
		}
		for (unsigned speaker=speakerToUpdate ; speaker<_outputs.size(); speaker++)
			delete _outputs[speaker];
		_outputs.resize(speakerToUpdate);
		// adding new speakers
		for (unsigned speaker=speakerToUpdate; speaker<_layout.size(); speaker++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( _layout.name(speaker), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
	}
	unsigned firstDirtySpeaker() const
	{
		for (unsigned speaker = 0; speaker<_layout.size(); speaker++)
		{
			if (speaker>=_outputs.size()) return speaker; 
			if (_outputs[speaker]->GetName() != _layout.name(speaker)) return speaker;
		}
		return _layout.size();
	}
	

};
#endif

