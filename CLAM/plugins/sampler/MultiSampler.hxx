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
#include <CLAM/OutControl.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Text.hxx>

#include <CLAM/AudioMixer.hxx>
#include "../sndfile/SndfilePlayer.hxx"

#include <vector>

namespace CLAM
{

	class MultiSamplerConfig : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( MultiSamplerConfig,3, ProcessingConfig );
		DYN_ATTRIBUTE(0,public,bool,LoopDefaultState);
		DYN_ATTRIBUTE(1,public,CLAM::Text,BasePath);
		DYN_ATTRIBUTE(2,public,CLAM::Text,Samples);

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
		MultiSamplerConfig _config;

		CLAM::AudioOutPortPublisher _proxyOutPublisher;
		CLAM::AudioMixer _mixer;
		CLAM::TypedInControl<int> _activeSample;
		CLAM::TypedInControl<std::string> _triggerControl;
		CLAM::TypedInControl<int> _loop;
//		CLAM::TypedInControl<std::string> _sampleName;
		std::vector<CLAM::SndfilePlayer *> _samples; 

		std::vector<CLAM::FloatOutControl *> _seekDummyControls;
		std::vector<CLAM::FloatOutControl *> _pauseDummyControls;

public:
	const char* GetClassName () const { return "MultiSampler"; }
	MultiSampler ( const ProcessingConfig & config=MultiSamplerConfig())
		: _proxyOutPublisher("audio out port",this)
		, _activeSample("set active sample number",this)
		, _triggerControl("trigger control input (play/stop)",this)
		, _loop("loop value (1,0)",this)
//		, _sampleName("sample name",this)
	{
		_proxyOutPublisher.PublishOutPort(_mixer.GetOutPort(0)); 
		Configure (config);
	}
	

	~MultiSampler()	{ ; }

	bool Do()
	{	
		if (_samples.size()==0)
			return true;
		unsigned int actualSample=_activeSample.GetLastValue();
		if (actualSample>=_samples.size())
			actualSample=0;

		if (not _triggerControl.HasBeenRead())
		{
			const std::string triggerControl = _triggerControl.GetLastValue();

			if (triggerControl=="play" or triggerControl=="PLAY")
			{
//				std::cout<<"multisampler play! of sample: "<<actualSample<<std::endl;
				_seekDummyControls[actualSample]->SendControl(0.);
				_pauseDummyControls[actualSample]->SendControl(0.);
			}


			if (triggerControl=="stop" or triggerControl=="STOP")
			{
//				std::cout<<"multisampler stop! of sample: "<<actualSample<<std::endl;
				_pauseDummyControls[actualSample]->SendControl(1.);
			}
		}

		CLAM::SndfilePlayerConfig configClone = dynamic_cast<const CLAM::SndfilePlayerConfig &> (_samples[actualSample]->GetConfig());
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

	const std::vector<std::string> GetDefinedSamples() const 
	{
		std::vector<std::string> samplesVector;
		if (not _config.HasSamples())
			return samplesVector;
		std::string samplesString = _config.GetSamples();
		std::string basePath="./";
		if (_config.HasBasePath())
		{
			basePath = _config.GetBasePath();
			if (basePath.substr(basePath.size()-1,1)!="/")
				basePath+="/";
		}

		size_t i=0;
		while (i<samplesString.size() and i!=-1)
		{
			size_t foundPos=samplesString.find(",",i); // (", ",i);
			while (samplesString.substr(i,1)==" ")
				i++;
			if (foundPos==-1) // last coincidence
			{
				samplesVector.push_back(basePath+samplesString.substr(i,samplesString.size()-i));
				break;
			}
			samplesVector.push_back(basePath+samplesString.substr(i,foundPos-i));
			i=foundPos+1;
		}

		std::vector<std::string>::iterator it;
		std::cout<<"vector de strings: "<<std::endl;
		for (it=samplesVector.begin();it!=samplesVector.end();it++)
		{
			std::cout<<"\t"<<(*it)<<std::endl;
		}

		return samplesVector;
	}



	bool ConcreteConfigure(const ProcessingConfig & config)
	{

		_activeSample.DoControl(0);
		_triggerControl.DoControl("");
		_loop.DoControl(0);

		CopyAsConcreteConfig(_config,config);

		ClearPlayers();
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


		if (not createPlayers(fileNames))
			return false;



		return true;
	}

private:
	bool createPlayers(const std::vector<std::string> fileNames)
	{
		std::vector<std::string>::const_iterator it;
		unsigned counter=0;
		for (it=fileNames.begin();it!=fileNames.end();it++)
		{
			CLAM::SndfilePlayer * fileplayerInstance = new CLAM::SndfilePlayer();

			if (_network)
				fileplayerInstance->SetNetworkBackLink(_network);

			CLAM::SndfilePlayerConfig playerConfig=CLAM::SndfilePlayerConfig();
			playerConfig.SetLoop(_config.GetLoopDefaultState());
			playerConfig.SetSourceFile(*it);
			if (not fileplayerInstance->Configure(playerConfig))
			{
				AddConfigErrorMessage(fileplayerInstance->GetConfigErrorMessage());
				return false;
			}

			_samples.push_back(fileplayerInstance);
			CLAM::ConnectPorts(fileplayerInstance->GetOutPort(0), _mixer, counter);

			CLAM::FloatOutControl * seekControl=new CLAM::FloatOutControl("dummy",0);
			seekControl->AddLink(fileplayerInstance->GetInControl("Seek in-Control"));
			_seekDummyControls.push_back(seekControl);

			CLAM::FloatOutControl * pauseControl=new CLAM::FloatOutControl("dummy",0);
			pauseControl->AddLink(fileplayerInstance->GetInControl("Pause in-Control"));
			_pauseDummyControls.push_back(pauseControl);

			counter++;
		}
		return true;
	}

	void ClearPlayers()
	{

		// clear players

		std::vector<CLAM::SndfilePlayer *>::iterator itPlayers;
		for (itPlayers=_samples.begin();itPlayers!=_samples.end();itPlayers++)
			delete *itPlayers;

		_samples.clear();
		// clear dummy controls:
		std::vector<CLAM::FloatOutControl *>::iterator itControls;

		for (itControls=_seekDummyControls.begin();itControls!=_seekDummyControls.end();itControls++)
			delete *itControls;
		for (itControls=_pauseDummyControls.begin();itControls!=_pauseDummyControls.end();itControls++)
			delete *itControls;

		_seekDummyControls.clear();
		_pauseDummyControls.clear();

	}


};
}
#endif
