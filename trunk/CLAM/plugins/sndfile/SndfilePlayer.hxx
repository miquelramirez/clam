/*
 *
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
#ifndef SndfilePlayer_hxx
#define SndfilePlayer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/AudioInFilename.hxx> 
#include <CLAM/Audio.hxx>
#include <sndfile.hh>  
#include <pthread.h>
#include <jack/ringbuffer.h>

namespace CLAM
{
	class SndfilePlayerConfig : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( SndfilePlayerConfig,3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, AudioInFilename, SourceFile );
		DYN_ATTRIBUTE( 1, public, bool, Loop );
		DYN_ATTRIBUTE( 2, public, unsigned, SavedNumberOfChannels );

		protected:	
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetSavedNumberOfChannels(0);
		};	
	};

	class SndfilePlayer : public  Processing
	{ 
		class Lock
		{
			pthread_mutex_t & _diskThreadLock;
			public:
				Lock(pthread_mutex_t & diskThreadLock)
					: _diskThreadLock(diskThreadLock)
				{	
					pthread_mutex_lock(&diskThreadLock);					
				}
				~Lock()
				{
					pthread_mutex_unlock(&_diskThreadLock);
				}
		};

		typedef SndfilePlayerConfig Config;
		typedef std::vector<CLAM::AudioOutPort*> OutPorts;
		OutPorts _outports;		
		CLAM::FloatOutControl _outControlSeek;
		CLAM::FloatInControl _inControlSeek;
		CLAM::FloatInControl _inControlPause;
		CLAM::FloatInControl _inControlLoop;
		SndfileHandle* _infile;
		SndfilePlayerConfig _config;
		unsigned _sampleSize;
		unsigned _numChannels;
		unsigned _numReadFrames;
		unsigned _numTotalFrames;
		float  _lastControlSeek;
		std::vector<float> _buffer; 

		pthread_t _threadId;
		long _overruns;
		const static unsigned _ringBufferSize = 16384;
		volatile int _canPlay;
		volatile int _canProcess;
		volatile int _isStopped;
		/* Synchronization between process thread and disk thread. */
		pthread_mutex_t _diskThreadLock;
		pthread_cond_t  _dataReadyCondition;
		jack_ringbuffer_t* _rb;		

	public:
		const char* GetClassName() const { return "SndfilePlayer"; }

		SndfilePlayer(const ProcessingConfig& config =  Config()) 
			: _outControlSeek("Position out-Control",this) 
			, _inControlSeek("Seek in-Control",this) 
			, _inControlPause("Pause in-Control",this)
			, _inControlLoop("Loop in-Control",this)
			, _infile(0)				
			, _numChannels(0)
			, _numReadFrames(0)
			, _numTotalFrames(0)
			, _lastControlSeek(0)
			, _overruns(0)
		{ 
			static pthread_cond_t sPthreadCondInitializer = PTHREAD_COND_INITIALIZER;
			static pthread_mutex_t sPthreadMutexInitializer = PTHREAD_MUTEX_INITIALIZER;
			_dataReadyCondition = sPthreadCondInitializer;
			_diskThreadLock = sPthreadMutexInitializer;
			_isStopped = true;
			Configure( config );
		}
	 
		bool Do()
		{
			/* Do nothing until we're ready to begin. */
			if ((!_canProcess) || (!_canPlay))
				return false;

			// PAUSE CONTROL
			//User has moved the slider and we have to change the position
			//TODO potentially dangerous since a different thread is reading!
			if(_inControlPause.GetLastValue()<0.5)
				ReadBufferAndWriteToPorts();
			else
				WriteSilenceToPorts();
			// SEEK CONTROL
			//User has moved the slider and we have to change the position
			if(not _inControlSeek.HasBeenRead() and _infile)
			{
				_lastControlSeek = _inControlSeek.GetLastValue();
				_numReadFrames = _lastControlSeek*_numTotalFrames;
				_infile->seek(_numReadFrames ,SEEK_SET);
			}		
			//Calculate the seek position between 0 and 1
			float seekPosition = ((float)_numReadFrames/(float)_numTotalFrames);
			if(seekPosition <=1)
				_outControlSeek.SendControl(seekPosition);

			/* Tell the disk thread there is work to do.  If it is already
			 * running, the lock will not be available.  We can't wait
			 * here in the process() thread, but we don't need to signal
			 * in that case, because the disk thread will read all the
			 * data queued before waiting again. */

			if (pthread_mutex_trylock(&_diskThreadLock) == 0) 
			{	    pthread_cond_signal (&_dataReadyCondition);
				    pthread_mutex_unlock (&_diskThreadLock);
			}
			// Tell the ports this is done
			for (unsigned channel=0; channel<_numChannels; channel++)
				_outports[channel]->Produce();
	
			return true;
		}

		void ReadBufferAndWriteToPorts()
		{
			//all the ports have to have the same buffer size
			const int portSize = _outports[0]->GetAudio().GetBuffer().Size();
			CLAM::TData* channels[_numChannels];
			float _frameBuffer[_numChannels];

			for (unsigned channel=0; channel<_numChannels; channel++)
				channels[channel] = &_outports[channel]->GetAudio().GetBuffer()[0];

			//case 1 ringbuffer is empty. Fill the ports with zeros
			if (_numReadFrames>=_numTotalFrames and _inControlLoop.GetLastValue()<=0.5)
			{
				WriteSilenceToPorts();
				return;
			}
			
			//case 2 Reading the ringbuffer information to the Ports
			int frameIndex = 0;
			while ((jack_ringbuffer_read_space (_rb) >=_sampleSize)) 
			{												
				jack_ringbuffer_read(_rb,(char*) _frameBuffer,_sampleSize);
				for(unsigned channel = 0; channel<_numChannels; channel++)
				{				
					channels[channel][frameIndex] = _frameBuffer[channel];
				}
				_numReadFrames++;
				frameIndex ++;			
			}

			if(frameIndex == portSize) return;

			while(frameIndex < portSize)
			{	for(unsigned channel = 0; channel<_numChannels; channel++)
					channels[channel][frameIndex] = 0;											
				_numReadFrames++;
				frameIndex++;
			}

			if(_inControlLoop.GetLastValue()>0.5)
			{
				_numReadFrames = _numReadFrames -_numTotalFrames;
				_infile->seek(_numReadFrames,SEEK_SET);
			}
		}
		void WriteSilenceToPorts()
		{
			//all the ports have to have the same buffer size
			const unsigned portSize = _outports[0]->GetAudio().GetBuffer().Size();
			CLAM::TData* channels[_numChannels];

			for (unsigned channel=0; channel<_numChannels; channel++)
				channels[channel] = &_outports[channel]->GetAudio().GetBuffer()[0];

			for (unsigned i=0; i<portSize; i++) 
				for(unsigned channel = 0; channel<_numChannels; channel++)
					channels[channel][i] = 0;					
			return;	
					
		}
		
		void DiskThread()
		{
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
			Lock lock(_diskThreadLock);			
					
			while (true) 
			{	
				ReadFileAndWriteRingBuffer();
				// wait until process() signals more data 
				if(_isStopped) return;
				pthread_cond_wait(&_dataReadyCondition, &_diskThreadLock);				
			}		
		}

		void ReadFileAndWriteRingBuffer()
		{	
			const unsigned portSize = _outports[0]->GetAudio().GetBuffer().Size()*sizeof(TData);			
			TData frameBuffer[_numChannels];
			
			while((jack_ringbuffer_read_space(_rb)< (portSize)*_numChannels) and _infile)
			{	
				unsigned readSize = _infile->read(frameBuffer,_numChannels);
				if (readSize != _numChannels) return;
				unsigned writeSize = jack_ringbuffer_write(_rb, (char*)frameBuffer,_sampleSize);
				if (writeSize != _sampleSize)
					_overruns++;	
			}
		}

		static void* DiskThreadCallback (void *arg)
		{
			SndfilePlayer * player = static_cast<SndfilePlayer*> (arg);
			player->DiskThread();
			return 0;
		}

		bool ConcreteStart()	
		{	
			CLAM_ASSERT(_infile, "SndfilePlayer::ConcreteStart() should have _infile with value.");

			//initial configuration for the controls.
			_inControlSeek.SetBounds(0,1);
			_inControlSeek.DoControl(0.);
			_inControlPause.SetBounds(0,1);
			_inControlPause.DoControl(0.0);
			_outControlSeek.SendControl(0.);
			_numReadFrames = 0;
			_isStopped = false;
			_canProcess = 0;
			_rb = jack_ringbuffer_create (_sampleSize*_ringBufferSize*_numChannels);
			memset(_rb->buf, 0, _rb->size);
			_infile->seek(0,SEEK_SET);
			_canPlay = 0;
			ReadFileAndWriteRingBuffer();			
			pthread_create(&_threadId, NULL,DiskThreadCallback,this);
			_canProcess = 1;
			_canPlay = 1;						
			return true; 
		}
		bool ConcreteStop()	
		{	
			_isStopped = true;
			if (pthread_mutex_trylock(&_diskThreadLock) == 0) 
			{	    pthread_cond_signal (&_dataReadyCondition);
				    pthread_mutex_unlock (&_diskThreadLock);
			}
			pthread_join (_threadId, NULL);	
			if(_infile)
				delete _infile;
				
			_infile = new SndfileHandle(_config.GetSourceFile().c_str(), SFM_READ);

			if (_overruns > 0) 
			{
				CLAM_ASSERT(_overruns,"Overruns is greater than 0. Try a bigger buffer");			
			}
			jack_ringbuffer_free (_rb);
			_numReadFrames = 0;
			return true; 
		}
	
		const ProcessingConfig& GetConfig() const
		{
			return _config;
		}

		bool ConcreteConfigure(const ProcessingConfig & config)
		{
			CopyAsConcreteConfig(_config, config);
			unsigned portSize = BackendBufferSize();
			std::string nameOutPort = "out";
			if(_outports.empty())
			{
				ResizePorts(1);
				_numChannels = _outports.size();
			}

			if ( !_config.HasSourceFile() )
			{
				return AddConfigErrorMessage("The provided config object lacked the field 'SourceFile'");
			}

			const std::string & location = _config.GetSourceFile();
			if ( location == "")
			{
				return AddConfigErrorMessage("No file selected");
			}

			if(_infile) delete _infile;
			_infile = new SndfileHandle(location.c_str(), SFM_READ);
			// check if the file is open
			if(!*_infile)
			{
				return AddConfigErrorMessage("The audio file '" + location + "' could not be opened");
			}

			//report sndfile library errors
			if(_infile->error() != 0)
			{
				return AddConfigErrorMessage(_infile->strError());
			}
			unsigned savedChannels = _config.GetSavedNumberOfChannels();
			if (savedChannels and savedChannels != _infile->channels() )
			{
				ResizePorts(savedChannels);
				return AddConfigErrorMessage(
					"The configuration have a number of channels saved which \n"
					"does not correspond to the channels in the provided audio \n"
					"file. If you want to just open a file, first choose '0' in the\n"
					"SavedNumberOfChannels config parameter");
			}

			_inControlLoop.DoControl(_config.GetLoop() ? 1 : 0);

			_numChannels = _infile->channels();
			_sampleSize = _numChannels*sizeof(TData);
			_config.SetSavedNumberOfChannels(_infile->channels() );
			_buffer.resize(portSize*_numChannels);
			_numTotalFrames = _infile->frames();
			// The file has not size, perhaps that's because the file is empty			
			if(_numTotalFrames == 0) _numTotalFrames = 1;
			ResizePorts(_infile->channels());
			return true;			
		}

		~SndfilePlayer()
		{
			if (_infile) delete _infile;
			ResizePorts(0);
		}
	private:
		void ResizePorts(unsigned nPorts)
		{
			const std::string nameBase = "out";
			const unsigned portSize = BackendBufferSize();
			for (unsigned i=_outports.size(); i<nPorts; i++)
			{
				std::ostringstream nameStream;
				nameStream << nameBase << i;
				AudioOutPort * port = new AudioOutPort( nameStream.str(), this);
				port->SetSize( portSize );
				port->SetHop( portSize );
				_outports.push_back( port );
			}
			for (unsigned i=nPorts; i<_outports.size(); i++)
				delete _outports[i];
			_outports.resize(nPorts);
		}
	};

}// namespace CLAM
#endif // SndfilePlayer_hxx

