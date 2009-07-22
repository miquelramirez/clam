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
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, EnvironmentImpulseResponsesFile);
		DYN_ATTRIBUTE( 1, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 2, public, unsigned, SampleRate);
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
	AudioInPort _inSync;

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
			environment++;
		}
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
		const unsigned sampleRate=_config.GetSampleRate();
		for (it=_environments.begin(); it!=_environments.end(); it++)
		{
			if (!computeResponseSpectrums( it->irWavfile, it->ir.irW, _config.GetFrameSize(), errorMsg, 0, sampleRate) 
				or !computeResponseSpectrums( it->irWavfile, it->ir.irX, _config.GetFrameSize(), errorMsg, 1, sampleRate)
				or !computeResponseSpectrums( it->irWavfile, it->ir.irY, _config.GetFrameSize(), errorMsg, 2, sampleRate)
				or !computeResponseSpectrums( it->irWavfile, it->ir.irZ, _config.GetFrameSize(), errorMsg, 3, sampleRate)	)
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
