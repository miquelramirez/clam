/*
 * Copyright (c) 2009 Fundació Barcelona Media Universitat Pompeu Fabra
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef MultiSampler_hxx
#define MultiSampler_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/AudioOutPortPublisher.hxx>
#include <CLAM/TypedInControl.hxx>
#include <CLAM/ProcessingConfig.hxx>


#include <CLAM/AudioMixer.hxx>
#include "../sndfile/SndfilePlayer.hxx"

#include <vector>

namespace CLAM
{

	class MultiSamplerConfig : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( MultiSamplerConfig,3, ProcessingConfig );
		DYN_ATTRIBUTE(0,public,bool,LoopDefaultState);
		DYN_ATTRIBUTE(1,public,std::string,BasePath);
		DYN_ATTRIBUTE(2,public,std::string,Samples);

		protected:	
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetLoopDefaultState(false);
			SetBasePath("./");
			SetSamples("");
		};	
	};
class MultiSampler : public Processing
{
//protected:
	MultiSamplerConfig _config;

		CLAM::AudioOutPortPublisher _proxyOutPublisher;
		CLAM::AudioMixer _mixer;
		CLAM::TypedInControl<int> _activeSample;
		CLAM::TypedInControl<std::string> _triggerControl;
		CLAM::TypedInControl<int> _loop;
		CLAM::TypedInControl<std::string> _sampleName;
		std::vector<CLAM::SndfilePlayer *> _samples; 

public:
	const char* GetClassName () const { return "MultiSampler"; }
	MultiSampler ( const ProcessingConfig & config=MultiSamplerConfig())
		: _proxyOutPublisher("audio out port",this)
		, _activeSample("set active sample number",this)
		, _triggerControl("trigger control input (play/stop)",this)
		, _loop("loop value (1,0)",this)
		, _sampleName("sample name",this)
	{
		Configure (config);
	}
	

	void ClearSamplers()
	{
		std::vector<CLAM::SndfilePlayer *>::iterator it;
		for (it=_samples.begin();it!=_samples.end();it++)
		{
			delete *it;
		}
		_samples.clear();
	}

	~MultiSampler()	{ ; }

	bool Do()
	{	
//std::cout<<"MultiSampler::Do()"<<std::endl;
		if (_samples.size()==0)
			return true;
		unsigned actualSample=_activeSample.GetLastValue();
		if (actualSample>=_samples.size())
			actualSample=0;
#if 1
		CLAM::SndfilePlayerConfig configClone = dynamic_cast<const CLAM::SndfilePlayerConfig &> (_samples[actualSample]->GetConfig());
		std::cout<<_sampleName.GetLastValue()<<std::endl;
#endif
		std::vector<CLAM::SndfilePlayer *>::iterator it;
		for (it=_samples.begin();it!=_samples.end();it++)
		{
			if ((*it)->IsRunning() and (*it)->CanConsumeAndProduce())
				(*it)->Do();
		}
		if (_mixer.IsRunning() and _mixer.CanConsumeAndProduce())
			_mixer.Do();
		return true;
	}

	bool ConcreteStart()
	{
//std::cout<<"MultiSampler::ConcreteStart()"<<std::endl;
		_mixer.Start();
		std::vector<CLAM::SndfilePlayer *>::iterator it;
		for (it=_samples.begin();it!=_samples.end();it++)
		{
			(*it)->Start();
		}
		return true;
	}

	bool ConcreteStop()
	{
//std::cout<<"MultiSampler::ConcreteStop()"<<std::endl;
		_mixer.Stop();
		std::vector<CLAM::SndfilePlayer *>::iterator it;
		for (it=_samples.begin();it!=_samples.end();it++)
		{
			(*it)->Stop();
		}
		return true;
	}

	const ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	std::vector<std::string> GetDefinedSamples()
	{
		std::vector<std::string> samplesVector;
		if (not _config.HasSamples())
			return samplesVector;
		std::string samplesString = _config.GetSamples();
		std::string basePath="./";
		if (_config.HasBasePath())
			basePath = _config.GetBasePath();

		size_t i=0;
		while (i<samplesString.size() and i!=-1)
		{
			size_t foundPos=samplesString.find(">",i); // (", ",i);
			if (foundPos==-1) // last coincidence
			{
				samplesVector.push_back(basePath+samplesString.substr(i,samplesString.size()-i));
				break;
			}
			samplesVector.push_back(basePath+samplesString.substr(i,foundPos-i));
			i=foundPos+1;
		}

/*		std::vector<std::string>::iterator it;
		std::cout<<"vector de strings: "<<std::endl;
		for (it=samplesVector.begin();it!=samplesVector.end();it++)
		{
			std::cout<<"\t"<<(*it)<<std::endl;
		}
*/
		return samplesVector;
	}


	bool ConcreteConfigure(const ProcessingConfig & config)
	{

		_activeSample.DoControl(0);
		_triggerControl.DoControl("");
		_loop.DoControl(0);
//		_sampleName.DoControl("InLanguageAnecoic-cut_48000.wav");


		CopyAsConcreteConfig(_config,config);

		ClearSamplers();
		std::vector<std::string> fileNames=GetDefinedSamples();


		if (_network)
			_mixer.SetNetworkBackLink(_network);

		unsigned numberOfSamples=fileNames.size();
		if (numberOfSamples==0)
		{
			AddConfigErrorMessage("No samples defined");
			return false;
		}
		CLAM::AudioMixerConfig mixerConfig=CLAM::AudioMixerConfig();
		mixerConfig.SetNumberOfInPorts(numberOfSamples);
		_mixer.Configure(mixerConfig);


		std::vector<std::string>::iterator it;
		unsigned counter=0;
		for (it=fileNames.begin();it!=fileNames.end();it++)
		{
			CLAM::SndfilePlayer * samplerInstance = new CLAM::SndfilePlayer();

			if (_network)
				samplerInstance->SetNetworkBackLink(_network);

			CLAM::SndfilePlayerConfig playerConfig=CLAM::SndfilePlayerConfig();
			playerConfig.SetLoop(_config.GetLoopDefaultState());
			playerConfig.SetSourceFile(*it);
			if (not samplerInstance->Configure(playerConfig))
			{
				AddConfigErrorMessage(samplerInstance->GetConfigErrorMessage());
				return false;
			}
			std::cout<< playerConfig.GetSourceFile()<<std::endl;

			_samples.push_back(samplerInstance);

			CLAM::ConnectPorts(samplerInstance->GetOutPort(0), _mixer, counter);
			counter++;
		}


		_proxyOutPublisher.PublishOutPort(_mixer.GetOutPort(0)); 

		return true;
	}
};
}
#endif
