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
#include "clam_ringbuffer.h"
#include "WorkerSemaphore.hxx"
#include "RingBuffer.hxx"

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

	class SndfileWriter : public  Processing
	{
		class Config : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( Config, 5, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, AudioOutFilename, TargetFile);
			DYN_ATTRIBUTE( 1, public, EAudioFileWriteFormat, Format);
			DYN_ATTRIBUTE( 2, public, int, SampleRate);
			DYN_ATTRIBUTE( 3, public, unsigned, NumberChannels);
			DYN_ATTRIBUTE( 4, public, unsigned, BufferSize);

		protected:
			void DefaultInit()
			{
				AddAll();
				UpdateData();
				SetSampleRate(44100);
				SetNumberChannels(0);
				SetBufferSize(16384);
			};
		};

		typedef std::vector<CLAM::AudioInPort*> InPorts;
		InPorts _inports;
		SndfileHandle* _outfile;
		Config _config;
		unsigned _numChannels;
		int _sampleRate;
		int _format;

		pthread_t _threadId;
		const static unsigned _ringBufferSize = 16384;
		volatile int _isStopped;
		/* Synchronization between process thread and disk thread. */
		RingBuffer<TData> * _ringBuffer;
		WorkerSemaphore * _diskSemaphore;

	public:
		const char* GetClassName() const { return "SndfileWriter"; }

		SndfileWriter(const ProcessingConfig& config =  Config())
			: _outfile(0)
			, _numChannels(0)
			, _isStopped(true)
			, _diskSemaphore(0)
		{
			Configure( config );
		}

		~SndfileWriter()
		{
			ResizePorts(0);
		}
		bool Do()
		{
			//all the ports have to have the same buffer size
			const unsigned portSize = _inports[0]->GetAudio().GetBuffer().Size();

			CLAM::TData* channels[_numChannels];
			for (unsigned channel=0; channel<_numChannels; channel++)
				channels[channel] = &_inports[channel]->GetAudio().GetBuffer()[0];

			TData buffer [_numChannels];
			// produce the portion write (normally the whole buffer)
			for (unsigned frameIndex=0;frameIndex<portSize;frameIndex++)
			{
				for (unsigned channel= 0;channel<_numChannels;channel++)
				{
					buffer[channel] = channels[channel][frameIndex];
				}
				_ringBuffer->write(buffer, _numChannels);
			}
			_diskSemaphore->signalWorkToDo();

			for (unsigned channel=0; channel<_numChannels; channel++)
				_inports[channel]->Consume();
			return true;
		}
		const ProcessingConfig& GetConfig() const
		{
			return _config;
		}

	private:
		void DiskThread()
		{
			TData framebuf[_numChannels];
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
			WorkerSemaphore::ScopedLock lock(*_diskSemaphore);

			while (true)
			{
				while (_ringBuffer->read(framebuf, _numChannels))
				{
					unsigned writeSize = _outfile->write(framebuf,_numChannels);
					if (writeSize != _numChannels)
					{
						// TODO: Not an assert!
						CLAM_WARNING(false,"Error writing the file.");
					}
				}
				if (_isStopped) break;
				// wait until process() signals more data
				_diskSemaphore->waitMoreWork();
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
			_ringBuffer = new RingBuffer<TData>(_ringBufferSize*_numChannels);
			_outfile = new SndfileHandle(_config.GetTargetFile().c_str(), SFM_WRITE,_format,_numChannels,_sampleRate);
			_diskSemaphore = new WorkerSemaphore;
			// TODO: the next line is commented to make the generated wavs compatible with octave wavread function, that ATM doesn't like tags in wavs :-/
//			_outfile->setString(SF_STR_SOFTWARE, "CLAM"); 

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
			pthread_create(&_threadId, NULL, DiskThreadCallback, this);
			return true;
		}

		bool ConcreteStop()
		{
			_isStopped = true;
			_diskSemaphore->signalWorkToDo();
			pthread_join (_threadId, NULL);
			if (_outfile) delete _outfile;
			CLAM_WARNING(not _ringBuffer->hadOverRuns(),"SndfileWriter: Overruns detected, try a higher buffer.");			
			delete _ringBuffer;
			delete _diskSemaphore;
			return true;
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
				_inports[i] = port;
			}
			unsigned portSize = BackendBufferSize();
			for (unsigned i=0; i<nPorts; i++)
			{
				_inports[i]->SetSize( portSize );
				_inports[i]->SetHop( portSize );
			}
		}

	};

}// namespace CLAM
#endif // SndfileWriter_hxx

