/*
 * Copyright (c) 2008 Fundació Barcelona Media Universitat Pompeu Fabra
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
#ifndef LockFreeSndfilePlayer_hxx
#define LockFreeSndfilePlayer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioOutPort.hxx>
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

	class LockFreeSndfilePlayer : public  Processing
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
		SndfileHandle* _infile;
		SndfilePlayerConfig _config;
		unsigned _sampleSize;
		unsigned _numChannels;
		std::vector<float> _buffer; 

		pthread_t _threadId;
		long _overruns;
		const static unsigned _ringBufferSize = 16384;
		volatile int _canCapture;
		volatile int _canProcess;
		volatile int _isStopped;
		/* Synchronization between process thread and disk thread. */
		pthread_mutex_t _diskThreadLock;
		pthread_cond_t  _dataReadyCondition;
		jack_ringbuffer_t* _rb;		

	public:
		const char* GetClassName() const { return "LockFreeSndfilePlayer"; }

		LockFreeSndfilePlayer(const ProcessingConfig& config =  Config()) 
			: _infile(0)
			,_numChannels(0)
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
			if ((!_canProcess) || (!_canCapture))
				return false;
			ReadBufferAndWriteToPorts();
			
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
			if (not _infile)
			{
				for(int i=0; i< portSize; i++) 
					for(unsigned channel = 0; channel<_numChannels; channel++)
						channels[channel][i] = 0;
				
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
				frameIndex ++;			
			}

			if(frameIndex == portSize)
			{	return;
			}

			while(frameIndex < portSize)
			{	for(unsigned channel = 0; channel<_numChannels; channel++)
					channels[channel][frameIndex] = 0;											
				frameIndex++;
			}

			delete _infile;	
			_infile = 0;
		
			if(_config.GetLoop())
				_infile = new SndfileHandle(_config.GetSourceFile().c_str(), SFM_READ);	
			return;					
		}
		
		void DiskThread()
		{
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
			Lock lock(_diskThreadLock);			
					
			while (1) 
			{	
				ReadFileAndWriteRingBuffer();
				// wait until process() signals more data 
				if(_isStopped)
				{	return;
				}
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
				if (readSize != _numChannels) 
				{ 	return;
				}
				unsigned writeSize = jack_ringbuffer_write(_rb, (char*)frameBuffer,_sampleSize);
				if(writeSize != _sampleSize)
					_overruns++;	
			}	
			return;			
		}

		static void* DiskThreadCallback (void *arg)
		{
			LockFreeSndfilePlayer * player = static_cast<LockFreeSndfilePlayer*> (arg);
			player->DiskThread();
			return 0;
		}
		bool ConcreteStart()	
		{	
			CLAM_ASSERT(_infile, "LockFreeSndfilePlayer::ConcreteStart() should have _infile with value.");

			_isStopped = false;
			_canProcess = 0;
			_rb = jack_ringbuffer_create (_sampleSize*_ringBufferSize*_numChannels);
			memset(_rb->buf, 0, _rb->size);
			_infile->seek(0,SEEK_SET);
			_canCapture = 0;
			ReadFileAndWriteRingBuffer();			
			pthread_create(&_threadId, NULL,DiskThreadCallback,this);
			_canProcess = 1;
			_canCapture = 1;
			
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
			std::ostringstream nameStream;
			std::string nameOutPort = "out";
			if(_outports.empty())
			{
				//initialization port
				nameStream<<nameOutPort<<1;
				AudioOutPort * port = new AudioOutPort( nameStream.str(), this);
				port->SetSize(portSize);
				port->SetHop( portSize );
				_outports.push_back( port );
				_numChannels = _outports.size();
			}

			if ( !_config.HasSourceFile() )
			{
				AddConfigErrorMessage("The provided config object lacked the field 'SourceFile'");
				return false;
			}

			const std::string & location = _config.GetSourceFile();
			if ( location == "")
			{
				AddConfigErrorMessage("No file selected");
				return false;
			}

			_infile = new SndfileHandle(location.c_str(), SFM_READ);
			// check if the file is open
			if(!*_infile)
			{
				AddConfigErrorMessage("The audio file '" + location + "' could not be opened");
				return false;
			}
			
			//report sndfile library errors
			if(_infile->error() != 0)
			{
				AddConfigErrorMessage(_infile->strError());
				return false;
			}			

			if(_config.GetSavedNumberOfChannels() != 0)
			{
				if( _config.GetSavedNumberOfChannels() !=(unsigned)_infile->channels() )
				{				
					AddConfigErrorMessage("The configuration have a number of channels saved which \ndoes not correspond to the channels in the provided audio \nfile. If you want to just open a file, first choose '0' in the\n SavedNumberOfChannels config parameter");

					return false;
				}
			}
			_numChannels = _infile->channels();
			_sampleSize = _numChannels*sizeof(TData);
			_config.SetSavedNumberOfChannels(_infile->channels() );
			_buffer.resize(portSize*_numChannels);
			// the processing have _outports.size() and the target number of ports is _infile->channels()
	
			// case 1: maintain the same ports
			if ( (unsigned)_infile->channels() == _outports.size() )
			{
				return true;
			}

			// case 2: increase number of same ports
			if ( (unsigned)_infile->channels() > _outports.size() )
			{
				for (int i=_outports.size()+1; i<= _infile->channels(); i++)
				{
					std::ostringstream nameStream;
					nameStream << nameOutPort << i;
					AudioOutPort * port = new AudioOutPort( nameStream.str(), this);
					port->SetSize( portSize );
					port->SetHop( portSize );
					_outports.push_back( port );
				}
				return true;
			}
			// case 3: decrease number of same ports
			if ( (unsigned)_infile->channels() < _outports.size() )
			{
				for (unsigned i=_infile->channels(); i<_outports.size(); i++)
				{
					delete _outports[i];
				}
				_outports.resize( _infile->channels() );
				
				return true;
			}			
			CLAM_ASSERT(false, "Should not reach here");
			return false;			
		}

		void RemovePorts()
		{
			if(!_outports.empty())
			{
				for(OutPorts::iterator itOutPorts=_outports.begin();itOutPorts!=_outports.end();itOutPorts++)
				{	
					delete *itOutPorts;
				}				
			}		
			_outports.clear();

			// Delete ports from Processing (base class) register
			GetOutPorts().Clear();
		}

		~LockFreeSndfilePlayer()
		{
			RemovePorts();
		}
	};

}// namespace CLAM
#endif // LockFreeSndfilePlayer_hxx

