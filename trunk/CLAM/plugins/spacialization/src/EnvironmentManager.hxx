#ifndef EnvironmentManager_hxx
#define EnvironmentManager_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioMixer.hxx>
#include <CLAM/Filename.hxx>
#include <string>
#include <cmath>
#include <fstream>
#include "LoadImpulseResponse.hxx"
#include "scene.hxx"

namespace CLAM
{
/**
  Read an environment configuration file, which have the name and limits of a geometry and an impulse response file for each environment, and outputs the proper one according to the input control position
*/

class EnvironmentManager : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 4, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, EnvironmentImpulseResponsesFile);
		DYN_ATTRIBUTE( 1, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 2, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 3, public, InFilename, EnvironmentsGeometryFile);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetEnvironmentImpulseResponsesFile("");
			SetFrameSize(512);
		};
	};

	typedef std::vector<std::string> Row;
	struct bformatIR
	{
		ImpulseResponse irW;
		ImpulseResponse irX;
		ImpulseResponse irY;
		ImpulseResponse irZ;
	};
	struct Environment
	{
		std::string name;
		float x0,y0,z0;
		float x1,y1,z1;
		std::string irWavfile;
		bformatIR ir;
	};
	std::vector<Environment> _environments;
	ImpulseResponse _silenceIR;

	Config _config;

	FloatOutControl _actualEnvironment;
	FloatInControl _xInControl;
	FloatInControl _yInControl;
	FloatInControl _zInControl;
	OutPort<ImpulseResponse*> _responseSpectrumsW;
	OutPort<ImpulseResponse*> _responseSpectrumsX;
	OutPort<ImpulseResponse*> _responseSpectrumsY;
	OutPort<ImpulseResponse*> _responseSpectrumsZ;
	TypedOutControl<std::string> _environmentName;
	AudioInPort _inSync;
	Scene * _scene;

public:
	EnvironmentManager(const Config& config = Config()) 
		: _actualEnvironment ("Actual Environment",this)
		, _xInControl("X in",this)
		, _yInControl("Y in",this)
		, _zInControl("Z in",this)
		, _responseSpectrumsW("ImpulseResponseW",this)
		, _responseSpectrumsX("ImpulseResponseX",this)
		, _responseSpectrumsY("ImpulseResponseY",this)
		, _responseSpectrumsZ("ImpulseResponseZ",this)
		, _environmentName("Environment name",this)
		, _inSync("Sync in",this)
		, _scene(0)
		
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
		_inSync.Consume();
		float x,y,z;
		x=_xInControl.GetLastValue();
		y=_yInControl.GetLastValue();
		z=_zInControl.GetLastValue();

		Vertex position=Vertex(x,y,z);
		const std::string environmentName=_scene->getCurrentEnvironment(position,true);
		_environmentName.SendControl(environmentName);

		if (environmentName=="")
		{
			_responseSpectrumsW.GetData()= &_silenceIR;
			_responseSpectrumsW.Produce();
			_responseSpectrumsX.GetData()= &_silenceIR;
			_responseSpectrumsX.Produce();
			_responseSpectrumsY.GetData()= &_silenceIR;
			_responseSpectrumsY.Produce();
			_responseSpectrumsZ.GetData()= &_silenceIR;
			_responseSpectrumsZ.Produce();
			_actualEnvironment.SendControl(0);
			return true;
		}

		unsigned environment=1;
		std::vector<Environment>::iterator it;
		for (it=_environments.begin();it!=_environments.end();it++)
		{
			if (it->name!=environmentName) 
			{
				environment++;
				continue;
			}
			_responseSpectrumsW.GetData()= &(it->ir.irW);
			_responseSpectrumsW.Produce();
			_responseSpectrumsX.GetData()= &(it->ir.irX);
			_responseSpectrumsX.Produce();
			_responseSpectrumsY.GetData()= &(it->ir.irY);
			_responseSpectrumsY.Produce();
			_responseSpectrumsZ.GetData()= &(it->ir.irZ);
			_responseSpectrumsZ.Produce();
			_actualEnvironment.SendControl(environment);
			return true;
		}
		CLAM_ASSERT(true,"Received an environment mesh without proper defined IR");
	}
	const char* GetClassName() const
	{
		return "EnvironmentManager";
	}

	~EnvironmentManager()
	{
		if (_scene) delete _scene;
	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		std::ifstream modelcheck( _config.GetEnvironmentsGeometryFile().c_str() );
		if ( not modelcheck.is_open() )
		{
			std::string err("Could not open the environments geometry definitions: "); 
			err += _config.GetEnvironmentsGeometryFile();
			AddConfigErrorMessage(err);
			return false;
		}

		Settings settings;
                settings.repeatable = true; // TODO: It should be configurable
                settings.extra_bound = 0.5;
                settings.human_height = 1.70;
                settings.model_filename = _config.GetEnvironmentsGeometryFile();
                settings.num_rays = 0;
                settings.num_rebounds = 0;
                settings.physical_output = 0;
                settings.ir_length = 0;
                settings.ignore_sources_models = true;
                settings.use_osc = false;
                settings.use_dat = false;
                settings.hoa_order = 0;

		if (_scene) delete _scene;
		_scene= new Scene(settings);

		std::vector<std::string> meshes=_scene->getMeshesNames();


		_environments.clear();
		//Load the sequence
		if (_config.GetEnvironmentImpulseResponsesFile()=="") 
			return AddConfigErrorMessage("Not filename defined for IRs definitions");

		// Load table from file
		std::ifstream file( _config.GetEnvironmentImpulseResponsesFile().c_str() );
		if (not file)
			return AddConfigErrorMessage("Unable to open the file "+_config.GetEnvironmentImpulseResponsesFile());
		unsigned lineCount = 0;
		while (file)
		{
			lineCount++;
			std::string line;
			std::getline(file, line);
			line.erase(0,line.find_first_not_of("\t\r\n "));
			if (line=="" or line[0]=='#') continue;
			std::istringstream is(line);
			Row row;
			while (is and not is.eof())
			{
				std::string data;
				is >> data;
				if (not is) break;
				row.push_back(data);
			}
			if (not is.eof())
			{
				std::ostringstream os;
				os
					<< "Bad sequence file, at line " << lineCount 
					<< ", not a float number at columns " << row.size() << ".";
				return AddConfigErrorMessage(os.str());
			}
			Environment environment;
			environment.name=row[0];
			environment.x0=atoi(row[1].c_str());
			environment.y0=atoi(row[2].c_str());
			environment.z0=atoi(row[3].c_str());
			environment.x1=atoi(row[4].c_str());
			environment.y1=atoi(row[5].c_str());
			environment.z1=atoi(row[6].c_str());
			environment.irWavfile=row[7];
			_environments.push_back(environment);
		}
		std::string errorMsg;
		const unsigned sampleRate=_config.GetSampleRate();

		std::vector<std::string>::const_iterator itMeshes;
		for (itMeshes=meshes.begin();itMeshes!=meshes.end();itMeshes++)
		{
			std::vector<Environment>::iterator itIRs;
			bool definedEnvironment=false;
			for (itIRs=_environments.begin(); itIRs!=_environments.end(); itIRs++)
			{
				if (itIRs->name!=(*itMeshes)) continue;
				if (!computeResponseSpectrums( itIRs->irWavfile, itIRs->ir.irW, _config.GetFrameSize(), errorMsg, 0, sampleRate) 
					or !computeResponseSpectrums( itIRs->irWavfile, itIRs->ir.irX, _config.GetFrameSize(), errorMsg, 1, sampleRate)
					or !computeResponseSpectrums( itIRs->irWavfile, itIRs->ir.irY, _config.GetFrameSize(), errorMsg, 2, sampleRate)
					or !computeResponseSpectrums( itIRs->irWavfile, itIRs->ir.irZ, _config.GetFrameSize(), errorMsg, 3, sampleRate)	)
				{
					AddConfigErrorMessage(errorMsg + " while using environment "+itIRs->name);
					return false;
				}
				definedEnvironment=true;
			}
			if (not definedEnvironment)
				return AddConfigErrorMessage("Cannot found IR definition for mesh " + (*itMeshes));
		}
		// create a silence IR
		std::vector<double> zerosBuffer;
		for (unsigned i=0;i<_config.GetFrameSize();i++)
		{
			zerosBuffer.push_back(0);
		}
		if (!computeResponseSpectrums( zerosBuffer, _silenceIR, _config.GetFrameSize(), errorMsg, sampleRate ))
		{
			AddConfigErrorMessage(errorMsg + ". Cannot compute the silence IR");
			return false;
		}

		return true;

	}
};

} //namespace

#endif
