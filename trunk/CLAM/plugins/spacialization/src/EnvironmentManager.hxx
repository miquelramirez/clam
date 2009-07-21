#ifndef EnvironmentManager_hxx
#define EnvironmentManager_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioMixer.hxx>
#include <CLAM/Filename.hxx>
#include <string>
#include <cmath>
#include <fstream>
#include "LoadImpulseResponse.hxx"

namespace CLAM
{
/**
  Read an environment configuration file, which have the name and limits of a geometry and an impulse response file for each environment, and outputs the proper one according to the input control position
*/

class EnvironmentManager : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, EnvironmentImpulseResponsesFile);
		DYN_ATTRIBUTE( 1, public, int, FrameSize );
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetEnvironmentImpulseResponsesFile("");
			SetFrameSize(512);
		};
	};

	Config _config;
	FloatOutControl _actualEnvironment;
	FloatInControl _xInControl;
	FloatInControl _yInControl;
	FloatInControl _zInControl;

	typedef std::vector<std::string> Row;

	struct Environment
	{
		std::string name;
		float x0,y0,z0;
		float x1,y1,z1;
		std::string irWavfile;
		ImpulseResponse ir;
	};
	std::vector<Environment> _environments;
	OutPort<ImpulseResponse*> _responseSpectrums;
	AudioInPort _inSync;

	ImpulseResponse _silenceIR;

public:
	EnvironmentManager(const Config& config = Config()) 
		: _actualEnvironment ("Actual Environment",this)
		, _xInControl("X in",this)
		, _yInControl("Y in",this)
		, _zInControl("Z in",this)
		, _responseSpectrums("ImpulseResponse",this)
		, _inSync("Sync in",this)
		
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
		unsigned environment=1;
		std::vector<Environment>::iterator it;
		for (it=_environments.begin();it!=_environments.end();it++)
		{
			if (x>it->x0 and x<it->x1 and y>it->y0 and y<it->y1 and z>it->z0 and z<it->z1)
			{
				_responseSpectrums.GetData()= &(it->ir);
				_responseSpectrums.Produce();
				_actualEnvironment.SendControl(environment);
				return true;
			}
			environment++;
		}
		_responseSpectrums.GetData()= &_silenceIR;
		_responseSpectrums.Produce();
		_actualEnvironment.SendControl(0);
		return true;
	}
	const char* GetClassName() const
	{
		return "EnvironmentManager";
	}

	~EnvironmentManager() {	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_environments.clear();
		//Load the sequence
		if (_config.GetEnvironmentImpulseResponsesFile()=="") 
			return AddConfigErrorMessage("Not filename defined");

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
		std::vector<Environment>::iterator it;
		std::string errorMsg;
		for (it=_environments.begin(); it!=_environments.end(); it++)
		{
			if (!computeResponseSpectrums( it->irWavfile, it->ir , _config.GetFrameSize(), errorMsg ))
			{
				AddConfigErrorMessage(errorMsg + " while using environment "+it->name);
				return false;
			}
		}
		// create a silence IR
		std::vector<double> zerosBuffer;
		for (unsigned i=0;i<_config.GetFrameSize();i++)
		{
			zerosBuffer.push_back(0);
		}
		if (!computeResponseSpectrums( zerosBuffer, _silenceIR, _config.GetFrameSize(), errorMsg ))
		{
			AddConfigErrorMessage(errorMsg + ". Cannot compute the silence IR");
			return false;
		}

		return true;

	}
};

} //namespace

#endif
