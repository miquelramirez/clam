#ifndef EnvironmentManager_hxx
#define EnvironmentManager_hxx

// TODO: make them configuration parameters
#define DefaultOcclusionFadeTime 300
#define DefaultOcclusionMinCutoffFrequency 350
#define DefaultOcclusionGainFactor 0.2
#define DefaultGeneralAmbienceGain 0.5

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
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
		DYNAMIC_TYPE_USING_INTERFACE( Config, 5, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, EnvironmentImpulseResponsesFile);
		DYN_ATTRIBUTE( 1, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 2, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 3, public, InFilename, EnvironmentsGeometryFile);
		DYN_ATTRIBUTE( 4, public, InFilename, DefaultIR);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetEnvironmentImpulseResponsesFile("");
			SetFrameSize(512);
			SetSampleRate(44100);
			SetEnvironmentsGeometryFile("");
			SetDefaultIR("");
		};
	};

	typedef std::vector<std::string> Row;
	struct Environment
	{
		std::string name;
		std::string irWavfile;
		std::vector <ImpulseResponse> ir;
		float occlusionFadeInMs;
		float occlusionMinCutoffFrequency;
		float occlusionGainFactor;
		float generalAmbienceGain;
		Environment() { }
		Environment(const std::string envName, const std::string envIrFile, const unsigned nChannels, const float fadeInMs, const float minCutoffFrequency, const float gainFactor, const float ambienceGain)
		: 	name(envName)
		,	irWavfile(envIrFile)
		,	occlusionFadeInMs(fadeInMs)
		,	occlusionMinCutoffFrequency(minCutoffFrequency)
		,	occlusionGainFactor(gainFactor)
		,	generalAmbienceGain(ambienceGain)
		{
			for (unsigned i=0;i<nChannels;i++)
				ir.push_back(ImpulseResponse());
		}
	};
	std::vector<Environment> _environments;

	Config _config;

	FloatOutControl _currentEnvironment;
	FloatInControl _xInControl;
	FloatInControl _yInControl;
	FloatInControl _zInControl;
	OutControl<std::string> _environmentName;
	
	FloatOutControl _occlusionFadeInMsOutControl;
	FloatOutControl _occlusionMinCutoffFrequencyOutControl;
	FloatOutControl _occlusionGainFactorOutControl;
	FloatOutControl _generalAmbienceGainOutControl;
	

	AudioInPort _inSync;
	Scene * _scene;
	std::vector< OutPort<ImpulseResponse*>* > _outputPorts;

public:
	EnvironmentManager(const Config& config = Config()) 
		: _currentEnvironment ("Current Environment Index",this)
		, _xInControl("X in",this)
		, _yInControl("Y in",this)
		, _zInControl("Z in",this)
		, _environmentName("Current Environment Name",this)
		, _occlusionFadeInMsOutControl("Fade in miliseconds for occlusions",this)
		, _occlusionMinCutoffFrequencyOutControl("Minimum cutoff frequency for occlusions LP filter",this)
		, _occlusionGainFactorOutControl("Gain factor for occlusions",this)
		, _generalAmbienceGainOutControl("Gain for general ambience",this)
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

		unsigned environment=0;
		std::vector<Environment>::iterator it;
		for (it=_environments.begin(); it!=_environments.end();it++)
		{
			if (it->name!=environmentName) 
			{
				environment++;
				continue;
			}
			for (unsigned channelNr=0;channelNr<_outputPorts.size();channelNr++)
			{
				_outputPorts[channelNr]->GetData() = & ( it->ir[channelNr] );
				_outputPorts[channelNr]->Produce();
			}
			_currentEnvironment.SendControl(environment);
			_occlusionFadeInMsOutControl.SendControl(it->occlusionFadeInMs);
			_occlusionMinCutoffFrequencyOutControl.SendControl(it->occlusionMinCutoffFrequency);
			_occlusionGainFactorOutControl.SendControl(it->occlusionGainFactor);
			_generalAmbienceGainOutControl.SendControl(it->generalAmbienceGain);
			return true;
		}
		CLAM_ASSERT(true,"Received an environment mesh without proper defined IR");
		return false; //just to avoid compiler warning
	}
	const char* GetClassName() const
	{
		return "EnvironmentManager";
	}

	~EnvironmentManager()
	{
		for (unsigned port = 0; port < _outputPorts.size(); ++port)
			delete _outputPorts[port];
		if (_scene) delete _scene;
	}

protected:
	Scene * createScene()
	{
		std::ifstream modelcheck( _config.GetEnvironmentsGeometryFile().c_str() );
		if ( not modelcheck.is_open() )
		{
			std::string err("Could not open the environments geometry definitions: "); 
			err += _config.GetEnvironmentsGeometryFile();
			AddConfigErrorMessage(err);
			return 0;
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
		return new Scene(settings);
	}

	void createOutputPorts()
	{
		if (_outputPorts.size()!=0) return; //already created
		_outputPorts.push_back(new OutPort<ImpulseResponse*>("W",this)); 
		_outputPorts.push_back(new OutPort<ImpulseResponse*>("X",this)); 
		_outputPorts.push_back(new OutPort<ImpulseResponse*>("Y",this)); 
		_outputPorts.push_back(new OutPort<ImpulseResponse*>("Z",this)); 
		for (unsigned channelNr=0;channelNr<_outputPorts.size();channelNr++)
		{
			_outputPorts[channelNr]->SetSize(1);
			_outputPorts[channelNr]->SetHop(1);
		}
	}

	const bool initEnvironments()
	{
		// create a default environment at the begining
		std::string errorMsg;
		const unsigned sampleRate=_config.GetSampleRate();

		std::vector<double> zerosBuffer;
		for (unsigned i=0;i<_config.GetFrameSize();i++)
			zerosBuffer.push_back(0);

		bool OK;


		Environment defaultEnvironment("","",_outputPorts.size(),DefaultOcclusionFadeTime,DefaultOcclusionMinCutoffFrequency,DefaultOcclusionGainFactor,DefaultGeneralAmbienceGain);
		for (unsigned channelNr=0; channelNr<_outputPorts.size(); channelNr++)
		{
			if (_config.GetDefaultIR()=="") // use silence IR if there is no selected default IR wav file
				OK = computeResponseSpectrums( zerosBuffer, defaultEnvironment.ir[channelNr], _config.GetFrameSize(), errorMsg, sampleRate );
			else	// use IR wav file
				OK = computeResponseSpectrums(_config.GetDefaultIR(), defaultEnvironment.ir[channelNr], _config.GetFrameSize(), errorMsg, channelNr, sampleRate);
			if (!OK) return AddConfigErrorMessage(errorMsg + ". Cannot compute the default IR");
		}

		_environments.clear();
		_environments.push_back(defaultEnvironment);
		return true;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		_inSync.SetSize(_config.GetFrameSize());
		_inSync.SetHop(_config.GetFrameSize());
		createOutputPorts();

		if (_scene) delete _scene;
		_scene = createScene();
		if (not _scene)	return false;

		std::vector<std::string> meshes=_scene->getMeshesNames();

		//Load the sequence
		if (_config.GetEnvironmentImpulseResponsesFile()=="") 
			return AddConfigErrorMessage("Not filename defined for IRs definitions");

		// Load table from file
		std::ifstream file( _config.GetEnvironmentImpulseResponsesFile().c_str() );
		if (not file)
			return AddConfigErrorMessage("Unable to open the file "+_config.GetEnvironmentImpulseResponsesFile());

		if (!initEnvironments()) return false;

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
			const std::string name=row[0];
			const std::string irWavfile=row[1];
			const float occlusionFadeInMs=atof(row[2].c_str());
			const float occlusionMinCutoffFrequency=atof(row[3].c_str());
			const float occlusionGainFactor=atof(row[4].c_str());
			const float ambienceGain=atof(row[5].c_str());
			const unsigned nChannels=_outputPorts.size();
			_environments.push_back(Environment(name,irWavfile,nChannels,occlusionFadeInMs,occlusionMinCutoffFrequency,occlusionGainFactor,ambienceGain));
		}

		std::string errorMsg;
		unsigned sampleRate=_config.GetSampleRate();

		std::vector<std::string>::const_iterator itMeshes;
		for (itMeshes=meshes.begin();itMeshes!=meshes.end();itMeshes++)
		{
			std::vector<Environment>::iterator itIRs=_environments.begin();
			bool definedEnvironment=false;
			for (++itIRs; itIRs!=_environments.end(); itIRs++)
			{
				if (itIRs->name!=(*itMeshes)) continue;
				for (unsigned channelNr = 0; channelNr < itIRs->ir.size(); channelNr++)
				{
					const bool OK = computeResponseSpectrums(itIRs->irWavfile, itIRs->ir[channelNr], _config.GetFrameSize(), errorMsg, channelNr, sampleRate);
					if (!OK) return AddConfigErrorMessage(errorMsg + " while using environment "+itIRs->name);
				}
				definedEnvironment=true;
			}
			if (not definedEnvironment)
				return AddConfigErrorMessage("Cannot found IR definition for mesh " + (*itMeshes));
		}

		return true;
	}


};

} //namespace

#endif
