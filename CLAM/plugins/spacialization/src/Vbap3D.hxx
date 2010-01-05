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
#include <iomanip>


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
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, CLAM::InFilename, SpeakerLayout);
		DYN_ATTRIBUTE( 1, public, CLAM::InFilename, Triangulation);
		DYN_ATTRIBUTE( 2, public, bool, IgnoreLabels);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetIgnoreLabels(true);
		};
	};

private:


	float _lastAzimuth;
	float _lastElevation;

	typedef std::vector<unsigned> Triangle;
	typedef CLAM::Vector3D Vector3D;

	SpeakerLayout _layout;
	CLAM::AudioInPort _w;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	CLAM::FloatInControl _azimuth;
	CLAM::FloatInControl _elevation;
	Config _config;
	std::vector<Vector3D> _speakersPositions;
	int _currentTriangle;


//	std::vector <unsigned> _activeSpeakersIndex;
	std::vector <float> _activeSpeakersLastGains;
	std::vector <float> _activeSpeakersActualGains;
	
	static float deltaAngle() { return 0.01; }
	static float deltaNumeric() { return 0.00001; } 


	class Triangulation
	{
		std::vector<Triangle> _triangles;
		std::vector<Vector3D> _normals;
		std::vector<float> _orthogonalProjection;
		const SpeakerLayout & _layout;
		const std::vector<Vector3D> & _speakersPositions;
		bool error(std::string & errorMsg, const std::string & message)
		{
			errorMsg += message;
			errorMsg += "\n";
			return false;
		}
	public:
		Triangulation(const SpeakerLayout & layout, const std::vector<Vector3D> & speakerPositions)
			: _layout(layout)
			, _speakersPositions(speakerPositions)
		{
		}
		void add(unsigned v1, unsigned v2, unsigned v3)
		{
			Triangle t;
			t.resize(3);
			t[0]=v1;
			t[1]=v2;
			t[2]=v3;
			_triangles.push_back(t);
			Vector3D normal = vectorialProduct( 
				substract(_speakersPositions[v1], _speakersPositions[v2]),  
				substract(_speakersPositions[v1], _speakersPositions[v3])
				);
			_normals.push_back(normal);
			_orthogonalProjection.push_back(scalarProduct(normal,_speakersPositions[v3]));
		}
		bool load(const std::string & path, std::string & errorMsg)
		{
			
			if (path.empty()) return error(errorMsg, "No triangulation file specified");
			std::ifstream file(path.c_str());
			if (!file) return error(errorMsg, "Could not open the triangulation file "+path);
			clear();
			unsigned nSpeakers = _layout.size();
			unsigned i=0;
			while (true)
			{
				std::string line;
				getline(file,line);
				if (file.eof()) break;
				if (line[0]=='#') continue;
				std::ostringstream os;
				os << i;
				std::istringstream is(line);
				int v1, v2, v3;
				if (not (is >> v1)) return error(errorMsg, "Bad vertex index 1 on triangle "+os.str());
				if (not (is >> v2)) return error(errorMsg, "Bad vertex index 2 on triangle "+os.str());
				if (not (is >> v3)) return error(errorMsg, "Bad vertex index 3 on triangle "+os.str());
				if (v1>=nSpeakers or v2>=nSpeakers or v3>=nSpeakers)
					return error(errorMsg, "Triangulation uses speakers that are not available");
//				std::cout << v1 << " " << v2 << " " << v3 << std::endl;
				add(v1,v2,v3);
				i++;
			}
			return true;
		}
		void clear()
		{
			_triangles.clear();
			_normals.clear();
			_orthogonalProjection.clear();
		}
		unsigned size() const { return _triangles.size(); }
		const Triangle & triangle(unsigned index) const { return _triangles[index]; }
		const Vector3D & normal(unsigned index) const { return _normals[index]; }
		float orthoProjection(unsigned index) const { return _orthogonalProjection[index]; }
	};

	Triangulation _triangulation;

public:
	const char* GetClassName() const { return "Vbap3D"; }
	Vbap3D(const Config& config = Config()) 
		: _lastAzimuth(0)
		, _lastElevation(0)
		, _w("W", this)
		, _azimuth("azimuth", this) // angle in degrees
		, _elevation("elevation", this) // angle in degrees
		, _currentTriangle(0)
		, _triangulation(_layout, _speakersPositions)
	{
		Configure( config );
		_azimuth.SetBounds(0, 360); //a complete spin on each slider direction
		_elevation.SetBounds(-90, 90);
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

	struct SpeakerPositions
	{
		int id;
		const char * name;
		float azimuth;
		float elevation;
	};
	struct Triangles {
		unsigned one;
		unsigned two;
		unsigned three;
	};
	const SpeakerPositions * layoutFor15Speakers()
	{
		static SpeakerPositions speakers[] =	{
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
		return speakers;
	}
	const Triangles * trianglesFor15Speakers()
	{
		static Triangles triangles[] = {
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
		return triangles;
	}
	const SpeakerPositions * layoutFor4Speakers()
	{
		static SpeakerPositions speakers[] =
		{
			{0, "left", -45., -45. },
			{1, "right", 45., -45.},
			{2, "back", -180., -45.},
			{3, "top", 0., 89.},
			{0, 0, 0., 0.}
		};
		return speakers;
	}
	const Triangles * trianglesFor4Speakers()
	{
		static Triangles triangles[] =
		{
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
		return triangles;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		// Whether to use 15 or 4 speaker hardcoded layout if no layout or triangulation is given
		bool use15Harcoded = true;

		CopyAsConcreteConfig(_config, config);
		std::string errorMessage;
		if (not _config.HasSpeakerLayout() or _config.GetSpeakerLayout() == "")
		{
			const SpeakerPositions * speakers = use15Harcoded? layoutFor15Speakers() : layoutFor4Speakers();
			_layout.clear();
			for (unsigned i=0; speakers[i].name; i++)
				_layout.add(speakers[i].azimuth, speakers[i].elevation, speakers[i].name);
		}
		else if (not _layout.load(_config.GetSpeakerLayout(), errorMessage))
			return AddConfigErrorMessage(errorMessage);

		_speakersPositions.clear();

//		_activeSpeakersIndex.clear();
		_activeSpeakersLastGains.clear();
		_activeSpeakersActualGains.clear();
		_activeSpeakersLastGains.resize(_layout.size(),0);
		_activeSpeakersActualGains.resize(_layout.size(),0);

		for (unsigned i=0; i<_layout.size(); i++)
		{
			Vector3D r = _layout.orientation(i).toCartesian();
			_speakersPositions.push_back(r);
		}

		const unsigned buffersize = BackendBufferSize();
		ResizePortsToLayout(buffersize);

		if (not _config.HasSpeakerLayout() or _config.GetTriangulation() == "")
		{
			const Triangles * triangles = use15Harcoded? trianglesFor15Speakers() : trianglesFor4Speakers();
			_triangulation.clear();
			for (unsigned i=0; triangles[i].one!=triangles[i].two; i++)
				_triangulation.add(triangles[i].one, triangles[i].two, triangles[i].three);
		}
		else if (not _triangulation.load(_config.GetTriangulation(), errorMessage))
			return AddConfigErrorMessage(errorMessage);

		_elevation.DoControl(0.);
		_azimuth.DoControl(0.);
		_w.SetSize(buffersize);
		_w.SetHop(buffersize);
		return true;
	}
	
	/// params: the source azimuth and elevation
	int findTriangle(float azimuth, float elevation) const
	{
		// find triangle testing them all
		Vector3D r_source = {
			cos(elevation) * cos(azimuth),
			cos(elevation) * sin(azimuth),
			sin(elevation),
		};
		for (unsigned i=0; i<_triangulation.size(); i++)
		{
//std::cout << "\n\nchecking triangle "<< i << std::endl;
//print(r_source, "r_source");
//print(_triangulation.normal(i), "normal");

			const Triangle & triangle = _triangulation.triangle(i);
			const float divisor = scalarProduct(_triangulation.normal(i), r_source);
//print(_speakersPositions[triangle[0]], "speak 0");
//print(_speakersPositions[triangle[1]], "speak 1");
//print(_speakersPositions[triangle[2]], "speak 2");
			// If source direction and the plane are almost ortogonal, continue
			// (also avoids a divide by zero)
			if (fabs(divisor) < deltaNumeric()) continue;

			const float t =  _triangulation.orthoProjection(i) / divisor;
//std::cout << "--> t " << t  << " t-1 " << t-1. << std::endl;
			if (t < 0.) continue; // opposite direction
			if (t > 1.+deltaNumeric()) continue; // TODOC: what does it means
//std::cout << "--> Ok intersection line < 1" << std::endl;
			Vector3D intersection = vectorByScalar(t, r_source);
			Vector3D v1 = substract( _speakersPositions[triangle[0]], intersection);
			Vector3D v2 = substract( _speakersPositions[triangle[1]], intersection);
			Vector3D v3 = substract( _speakersPositions[triangle[2]], intersection);
			// If intersection is too close to one of the vertex, consider this triangle
//std::cout << "product of modules " <<  (mod(v1)*mod(v2)*mod(v3) < deltaNumeric()) << std::endl;
			if (mod(v1)*mod(v2)*mod(v3) < deltaNumeric())
			{
				return i; // Matches one of the speakers
			}
			// if the sum of the relative angles is 
//std::cout << "suma de angulos - 2pi (if <0 inside) " << fabs(angle(v1,v2) + angle(v2,v3) + angle(v3,v1) - 2*M_PI) << std::endl;
			if (fabs(angle(v1,v2) + angle(v2,v3) + angle(v3,v1) - 2*M_PI) < deltaAngle())
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


		const float as = CLAM::rad( azimuthDegrees );
		const float es = CLAM::rad( elevationDegrees );
		int newTriangle = findTriangle(as, es);
		CLAM_ASSERT(newTriangle > -1, "Vbap3D: findTriangle() found no triangle for the given angle!");
		if (newTriangle==-1) newTriangle = 0;
		
		// change triangle
		if (_currentTriangle != newTriangle) // changed triangle
		{
			std::cout << " changing triangle: " << _currentTriangle << "->" << newTriangle << " " << std::flush;
			_currentTriangle = newTriangle;
//std::cout << " speakers: "
//	<< _triangulation.triangle(_currentTriangle)[0] << " " 
//	<< _triangulation.triangle(_currentTriangle)[1] << " " 
//	<< _triangulation.triangle(_currentTriangle)[2] << " " << std::endl;
		}
		// obtain the three gains.
		const Triangle & triangle = _triangulation.triangle(_currentTriangle);
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
	
		for (unsigned i=0; i<_outputs.size(); i++)
		{
			_activeSpeakersActualGains[i] = ( i==speaker1 ? g1 : (i==speaker2 ? g2 : (i==speaker3 ? g3 : 0. ) ) );
		}
	
		// copy audio in->out
		// TODO extract method
		const CLAM::DataArray& w = _w.GetAudio().GetBuffer();
		const CLAM::TData * in = w.GetPtr();
		const unsigned size = w.Size();
		const float sizeInverse=1./size;
		for (unsigned i=0; i<_outputs.size(); i++)
		{
			CLAM::TData * out =_outputs[i]->GetAudio().GetBuffer().GetPtr();
			const float gainDifference=_activeSpeakersActualGains[i]-_activeSpeakersLastGains[i]; 

			if (fabs(gainDifference)>deltaNumeric() or _activeSpeakersActualGains[i]>deltaAngle())
			{
				for (int sample=0; sample<size; sample++)
				{
					const float gainToApply = _activeSpeakersLastGains[i] + gainDifference * (sample*sizeInverse);
					out[sample] = in[sample]*gainToApply;
				}
			}
			else
				for (int sample=0; sample<size; sample++)
					out[sample] = 0.;
			_activeSpeakersLastGains[i]=_activeSpeakersActualGains[i];
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
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( portName(speaker), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
	}
	std::string portName(unsigned speaker) const
	{
		if (_config.HasIgnoreLabels() and _config.GetIgnoreLabels())
		{
			std::ostringstream os;
			os << std::setw(2) << std::setfill('0') << (speaker+1);
			return os.str();
		}
		return _layout.name(speaker);
	}
	unsigned firstDirtySpeaker() const
	{
		for (unsigned speaker = 0; speaker<_layout.size(); speaker++)
		{
			if (speaker>=_outputs.size()) return speaker; 
			if (_outputs[speaker]->GetName() != portName(speaker)) return speaker;
		}
		return _layout.size();
	}
	

};
#endif

