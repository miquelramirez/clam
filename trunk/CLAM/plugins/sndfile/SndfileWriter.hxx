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
#ifndef SndfileWriter_hxx
#define SndfileWriter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutFilename.hxx>
#include <CLAM/Audio.hxx>
#include <sndfile.hh>
#include <CLAM/Enum.hxx>
#include <pthread.h>
#include <jack/ringbuffer.h>

namespace CLAM
{
	class EAudioFileWriteFormat : public Enum
	{
	public:
		EAudioFileWriteFormat() : Enum(EnumValues(), DefaultValue()) {}
		EAudioFileWriteFormat( tValue val ) : Enum(EnumValues(), val) {}
		EAudioFileWriteFormat( std::string s ) : Enum(EnumValues(), s) {}
		virtual Component* Species() const { return new EAudioFileWriteFormat; }

		enum
		{
			eDefault = 0,
			ePCM_16 = SF_FORMAT_WAV | SF_FORMAT_PCM_16,      /* Signed 16 bit data */
			ePCM_24 = SF_FORMAT_WAV | SF_FORMAT_PCM_24,      /* Signed 24 bit data */
			ePCM_32 = SF_FORMAT_WAV | SF_FORMAT_PCM_32,      /* Signed 32 bit data */
			ePCMFLOAT = SF_FORMAT_WAV | SF_FORMAT_FLOAT,     /* 32 bit float data */
			ePCMDOUBLE = SF_FORMAT_WAV | SF_FORMAT_DOUBLE,   /* 64 bit float data */
			eFLAC_16 = SF_FORMAT_FLAC | SF_FORMAT_PCM_16,    /* FLAC lossless file format 16 bit data */
		};

		static tValue     DefaultValue() { return eDefault; }
		static tEnumValue * EnumValues()
		{
			static tEnumValue sEnumValues[] = {
				{ eDefault, "Choose a format"},
				{ ePCM_16,    "Wav Integer 16 bit" },
				{ ePCM_24,    "Wav Integer 24 bit" },
				{ ePCM_32,    "Wav Integer 32 bit" },
				{ ePCMFLOAT,  "Wav Float 32 bit"},
				{ ePCMDOUBLE, "Wav Float 64 bit"},
				{ eFLAC_16,   "FLAC 16 bit"},
				{ 0, 0 }
			};

			return sEnumValues;
		}
	public:
		static  EAudioFileWriteFormat FormatFromFilename( std::string filename );
	};

	class SndfileWriterConfig : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( SndfileWriterConfig, 4, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, AudioOutFilename, TargetFile);
		DYN_ATTRIBUTE( 1, public, EAudioFileWriteFormat, Format);
		DYN_ATTRIBUTE( 2, public, int, SampleRate);
		DYN_ATTRIBUTE( 3, public, unsigned, NumberChannels);

		protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetSampleRate(44100);
			SetNumberChannels(0);
		};
	};

	class SndfileWriter : public  Processing
	{
		class ScopedLock
		{
			pthread_mutex_t & _mutex;
			public:
				ScopedLock(pthread_mutex_t & mutex)
					: _mutex(mutex)
				{
					pthread_mutex_lock(&_mutex);
				}
				~ScopedLock()
				{
					pthread_mutex_unlock(&_mutex);
				}
		};

		typedef SndfileWriterConfig Config;
		typedef std::vector<CLAM::AudioInPort*> InPorts;
		InPorts _inports;
		SndfileHandle* _outfile;
		SndfileWriterConfig _config;
		unsigned _numChannels;
		unsigned _sampleSize;
		int _sampleRate;
		int _format;

		pthread_t _threadId;
		long _overruns;
		const static unsigned _ringBufferSize = 16384;
		volatile int _canCapture;
		volatile int _canProcess;
		volatile int _isStopped;
		/* Synchronization between process thread and disk thread. */
		pthread_mutex_t _diskThreadLock;
		pthread_cond_t  _dataReadyCondition;
		jack_ringbuffer_t* _ringBuffer;

	public:
		const char* GetClassName() const { return "SndfileWriter"; }

		SndfileWriter(const ProcessingConfig& config =  Config())
			: _outfile(0)
			, _numChannels(0)
			, _overruns(0)
			, _isStopped(true)
		{
			pthread_cond_t pthreadCondInitializer = PTHREAD_COND_INITIALIZER;
			_dataReadyCondition = pthreadCondInitializer;

			pthread_mutex_t pthreadMutexInitializer = PTHREAD_MUTEX_INITIALIZER;
			_diskThreadLock = pthreadMutexInitializer;

			Configure( config );
		}

		bool Do()
		{
			/* Do nothing until we're ready to begin. */
			if (not _canProcess) return false;
			if (not _canCapture) return false;

			//all the ports have to have the same buffer size
			const unsigned portSize = _inports[0]->GetAudio().GetBuffer().Size();

			CLAM::TData* channels[_numChannels];
			for (unsigned channel=0; channel<_numChannels; channel++)
				channels[channel] = &_inports[channel]->GetAudio().GetBuffer()[0];

			TData buffer [_numChannels];
			// produce the portion write (normally the whole buffer)
			for(unsigned frameIndex=0;frameIndex<portSize;frameIndex++)
			{
				for(unsigned channel= 0;channel<_numChannels;channel++)
				{
					buffer[channel] = channels[channel][frameIndex];
				}

				if(jack_ringbuffer_write(_ringBuffer, (char*)buffer, _sampleSize) < _sampleSize)
					_overruns++;
			}
			/* Tell the disk thread there is work to do.  If it is already
			 * running, the lock will not be available.  We can't wait
			 * here in the process() thread, but we don't need to signal
			 * in that case, because the disk thread will read all the
			 * data queued before waiting again. */

			if (pthread_mutex_trylock(&_diskThreadLock) == 0)
			{
				pthread_cond_signal (&_dataReadyCondition);
				pthread_mutex_unlock (&_diskThreadLock);
			}
			for (unsigned channel=0; channel<_numChannels; channel++)
				_inports[channel]->Consume();
			return true;
		}

		void DiskThread()
		{
			static unsigned total_captured = 0;
			TData framebuf[_numChannels];
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
			ScopedLock lock(_diskThreadLock);

			while (true)
			{
				while ((jack_ringbuffer_read_space (_ringBuffer) >=_sampleSize)) {
					jack_ringbuffer_read (_ringBuffer,(char*) framebuf,_sampleSize);
					unsigned writeSize = _outfile->write(framebuf,_numChannels);
					if (writeSize != _numChannels)
					{
						CLAM_ASSERT(_outfile,"Error writing the file.");
						return;
					}
					total_captured++;
				}

				if(_isStopped) return;
				// wait until process() signals more data
				pthread_cond_wait (&_dataReadyCondition, &_diskThreadLock);
			}
		}

		static void* DiskThreadCallback (void *arg)
		{
			SndfileWriter * writer = static_cast<SndfileWriter*> (arg);
			writer->DiskThread();
			return 0;
		}

		bool ConcreteStart()
		{
			_isStopped = false;
			_canProcess = 0;
			_ringBuffer = jack_ringbuffer_create (_sampleSize*_ringBufferSize*_numChannels);
			memset(_ringBuffer->buf, 0, _ringBuffer->size);

			_outfile = new SndfileHandle(_config.GetTargetFile().c_str(), SFM_WRITE,_format,_numChannels,_sampleRate);

			// check if the file is open
			if(!*_outfile)
			{
				CLAM_ASSERT(_outfile, "The file is not writeable");
				return false;
			}
			//report sndfile library errors
			if(_outfile->error() != 0)
			{
				CLAM_ASSERT(_outfile, _outfile->strError());
				return false;
			}
			_canCapture = 0;
			pthread_create(&_threadId, NULL,DiskThreadCallback,this);
			_canProcess = 1;
			_canCapture = 1;
			return true;
		}

		bool ConcreteStop()
		{
			_isStopped = true;
			if (pthread_mutex_trylock(&_diskThreadLock) == 0)
			{
				pthread_cond_signal (&_dataReadyCondition);
				pthread_mutex_unlock (&_diskThreadLock);
			}
			pthread_join (_threadId, NULL);
			
			if (_outfile) delete _outfile;
			// TODO: This will halt the app!!!! Can not be an assert!
			CLAM_ASSERT(_overruns,"Overruns is greater than 0. Try a bigger buffer");			
			jack_ringbuffer_free (_ringBuffer);
			return true;
		}

		const ProcessingConfig& GetConfig() const
		{
			return _config;
		}

		EAudioFileWriteFormat ChooseFormat(std::string location)
		{
			EAudioFileWriteFormat fileNameFormat = EAudioFileWriteFormat::FormatFromFilename(location);
			EAudioFileWriteFormat configuredFormat = _config.GetFormat();

			if (configuredFormat == 0)
			{
				_config.SetFormat(fileNameFormat);
				return fileNameFormat;
			}
			// .wav extension
			if (fileNameFormat == EAudioFileWriteFormat::ePCM_16)
			{
				switch (configuredFormat)
				{
					// Valid formats for .wav
					case EAudioFileWriteFormat::ePCM_16:
					case EAudioFileWriteFormat::ePCM_24:
					case EAudioFileWriteFormat::ePCM_32:
					case EAudioFileWriteFormat::ePCMFLOAT:
					case EAudioFileWriteFormat::ePCMDOUBLE:
						return configuredFormat;
					default:
						return 0;
				}
			}
			// .flac extension
			if (fileNameFormat == EAudioFileWriteFormat::eFLAC_16)
			{
				switch (configuredFormat)
				{
					// Valid formats for .flac
					case EAudioFileWriteFormat::eFLAC_16:
						return configuredFormat;
					default:
						return 0;
				}
			}
			return configuredFormat;
		}

		bool ConcreteConfigure(const ProcessingConfig & config)
		{
			CopyAsConcreteConfig(_config, config);
			if(_inports.empty()) ResizePorts(1);

			const std::string & location = _config.GetTargetFile();
			if ( location == "")
				return AddConfigErrorMessage("No file selected");

			_format=ChooseFormat(location);
			_numChannels = _config.GetNumberChannels();
			_sampleSize = _numChannels*sizeof(TData);
			_sampleRate = _config.GetSampleRate();

			if (_numChannels == 0)
				return AddConfigErrorMessage("The number of channels has to be greater than 0");

			ResizePorts(_numChannels);
			return true;
		}
		
		void ResizePorts(unsigned nPorts)
		{
			unsigned oldNPorts = _inports.size();
			for (unsigned i=nPorts; i<oldNPorts; i++)
				delete _inports[i];
			_inports.resize(nPorts);
			for (unsigned i=oldNPorts; i<nPorts; i++)
			{
				static const char * nameInPort = "in";
				std::ostringstream nameStream;
				nameStream << nameInPort << i+1;
				AudioInPort * port = new AudioInPort( nameStream.str(), this);
				_inports.push_back( port );
			}
			unsigned portSize = BackendBufferSize();
			for (unsigned i=0; i<nPorts; i++)
			{
				_inports[i]->SetSize( portSize );
				_inports[i]->SetHop( portSize );
			}
		}

		~SndfileWriter()
		{
			ResizePorts(0);
		}
	};

}// namespace CLAM
#endif // SndfileWriter_hxx

