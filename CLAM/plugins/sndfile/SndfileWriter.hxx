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

namespace CLAM
{
	class EAudioFileWriter : public Enum
	{
	public:
		EAudioFileWriter() : Enum(EnumValues(), DefaultValue()) {}
		EAudioFileWriter( tValue val ) : Enum(EnumValues(), val) {}
		EAudioFileWriter( std::string s ) : Enum(EnumValues(), s) {}
		virtual Component* Species() const { return new EAudioFileWriter; }

		typedef enum
		{
			eDefault = 0,
			ePCM_16 = SF_FORMAT_WAV | SF_FORMAT_PCM_16,      /* Signed 16 bit data */
			ePCM_24 = SF_FORMAT_WAV | SF_FORMAT_PCM_24,	/* Signed 24 bit data */
			ePCM_32 = SF_FORMAT_WAV | SF_FORMAT_PCM_32,	/* Signed 32 bit data */
			ePCMFLOAT = SF_FORMAT_WAV | SF_FORMAT_FLOAT,	/* 32 bit float data */
			ePCMDOUBLE = SF_FORMAT_WAV | SF_FORMAT_DOUBLE,	/* 64 bit float data */
			eFLAC_16 = SF_FORMAT_FLAC | SF_FORMAT_PCM_16,	/* FLAC lossless file format 16 bit data */
		} tEnum;

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
				{ eFLAC_16,      "FLAC 16 bit"},
				{ 0, 0 }
			};

			return sEnumValues;
		}
	public:
		static  EAudioFileWriter FormatFromFilename( std::string filename );

	};

	class SndfileWriterConfig : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( SndfileWriterConfig, 4, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, AudioOutFilename, TargetFile);
  	    DYN_ATTRIBUTE( 1, public, EAudioFileWriter, Format);
	    DYN_ATTRIBUTE( 2, public, int, SampleRate);
	    DYN_ATTRIBUTE( 3, public, unsigned, NumberChannels);

		protected:	
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetSampleRate(44100);
			SetNumberChannels(1);
		};	
	};

	class SndfileWriter : public  Processing
	{ 
		typedef SndfileWriterConfig Config;
		typedef std::vector<CLAM::AudioInPort*> InPorts;
		
		InPorts _inports;
		SndfileHandle* _outfile;
		SndfileWriterConfig _config;
		unsigned _numChannels;
		int _sampleRate;
		int _format;
		
	public:
		const char* GetClassName() const { return "SndfileWriter"; }

		SndfileWriter(const ProcessingConfig& config =  Config()) 
			: _outfile(0)
			,_numChannels(0)
		{ 
			Configure( config );
		}
	 
		bool Do()
		{	bool isDone;			
			isDone = ReadBufferAndWriteToPorts();
			for (unsigned channel=0; channel<_numChannels; channel++)
				_inports[channel]->Consume();		
			return isDone;
		}

		bool ReadBufferAndWriteToPorts()
		{
			//all the ports have to have the same buffer size
			const int portSize = _inports[0]->GetAudio().GetBuffer().Size();

			CLAM::TData* channels[_numChannels];
			for (unsigned channel=0; channel<_numChannels; channel++)
				channels[channel] = &_inports[channel]->GetAudio().GetBuffer()[0];

			const int bufferSize =  portSize * _numChannels;
			float buffer[bufferSize];

			// produce the portion write (normally the whole buffer)
			int frameIndex = 0;
			for(int i=0;i<bufferSize;i+=_numChannels)
			{
				for(unsigned channel= 0;channel<_numChannels;channel++)
				{
					buffer[i+channel] = channels[channel][frameIndex];

				}
				frameIndex ++;
			}			

			int writeSize = _outfile->write( buffer,bufferSize);

			if (writeSize == bufferSize) return true;	

			return false;		
		}		


		bool ConcreteStart()	
		{	
			CLAM_ASSERT(_outfile, "SndfileWriter::ConcreteStart() should have _outfile with value.");

			_outfile = new SndfileHandle(_config.GetTargetFile().c_str(), SFM_WRITE, _format,_numChannels,_sampleRate);

			// check if the file is open
			if(!*_outfile)
			{	
				std::cout << "The file is not writeable"<< std::endl;
				return false;
			}
			
			return true; // Configuration ok		
		}

		bool ConcreteStop()	
		{
			if(_outfile)
			{	
				delete _outfile;
			}	
			return true; // Configuration ok
		}
	
		const ProcessingConfig& GetConfig() const
		{
			return _config;
		}

		EAudioFileWriter ChooseFormat(std::string location)
		{
			EAudioFileWriter formatFileName = EAudioFileWriter::FormatFromFilename(location);
			EAudioFileWriter formatConfigure = _config.GetFormat();

			//case0: if formatFileName has a value and formatConfigure hasn't, then return formatFileName
			if(formatFileName !=0 && formatConfigure == 0)
			{	_config.SetFormat(formatFileName);
				return formatFileName;
			}

			//case1: formatfileName is .wav
			if(formatFileName == EAudioFileWriter::ePCM_16)
			{	//if formatConfigure is also wav integer type, return formatConfigure because it has priority
				if(formatConfigure==EAudioFileWriter::ePCM_16 || formatConfigure==EAudioFileWriter::ePCM_24 || formatConfigure==EAudioFileWriter::ePCM_32)
					return formatConfigure;		

				//if formatConfigure is also wav Float type, return formatConfigure because it has priority
				if(formatConfigure==EAudioFileWriter::ePCMFLOAT || formatConfigure==EAudioFileWriter::ePCMDOUBLE)
					return formatConfigure;

				//if formatConfigure is not another type, return 0 because It's a contradiction.
				return 0;
			}
			//case2: formatfileName is .flac
			if(formatFileName == EAudioFileWriter::eFLAC_16)
			{	//if formatConfigure is also wav integer type, return formatConfigure because it has priority
				if(formatConfigure==EAudioFileWriter::eFLAC_16)
					return formatConfigure;
				//if formatConfigure is not another type, return 0 because It's a contradiction.
				return 0;			
			}

			return formatConfigure;

		}

		bool ConcreteConfigure(const ProcessingConfig & config)
		{
			CopyAsConcreteConfig(_config, config);
			unsigned portSize = BackendBufferSize();
			std::ostringstream nameStream;
			std::string nameInPort = "in";
			if(_inports.empty())
			{
				//initialization port
				nameStream<<nameInPort<<1;
				AudioInPort * port = new AudioInPort( nameStream.str(), this);
				port->SetSize(portSize);
				port->SetHop( portSize );
				_inports.push_back( port );
			}

			if ( !_config.HasTargetFile() )
			{
				AddConfigErrorMessage("The provided config object lacked the field 'TargetFile'");
				return false;
			}

			const std::string & location = _config.GetTargetFile();
			if ( location == "")
			{
				AddConfigErrorMessage("No file selected");
				return false;
			}

			//Choose the format from the filename
			_format=ChooseFormat(location); 
			_numChannels = _config.GetNumberChannels();
 			_sampleRate = _config.GetSampleRate();
			_outfile = new SndfileHandle(location.c_str(), SFM_WRITE,_format,_numChannels,_sampleRate);
			
			// check if the file is open
			if(!*_outfile)
			{
				AddConfigErrorMessage("The file is not writeable");
				return false;
			}
			
			//report sndfile library errors
			if(_outfile->error() != 0)
			{
				AddConfigErrorMessage(_outfile->strError());
				return false;
			}

			// case 1: maintain the same ports
			if ( (unsigned)_outfile->channels() == _inports.size() )
			{
				return true;
			}

			// case 2: increase number of same ports
			if ( (unsigned)_outfile->channels() > _inports.size() )
			{
				for (int i=_inports.size()+1; i<= _outfile->channels(); i++)
				{
					std::ostringstream nameStream;
					nameStream << nameInPort << i;
					AudioInPort * port = new AudioInPort( nameStream.str(), this);
					port->SetSize( portSize );
					port->SetHop( portSize );
					_inports.push_back( port );
				}
				return true;
			}

			// case 3: decrease number of same ports
			if ( (unsigned)_outfile->channels() < _inports.size() )
			{
				for (unsigned i=_outfile->channels(); i<_inports.size(); i++)
				{
					delete _inports[i];
				}
				_inports.resize( _outfile->channels() );
				
				return true;
			}
			
			CLAM_ASSERT(false, "Should not reach here");
			return false;	
		}
		
		void RemovePortsAndControls()
		{
			if(!_inports.empty())
			{
				for(InPorts::iterator itInPorts=_inports.begin();itInPorts!=_inports.end();itInPorts++)
				{	
					delete *itInPorts;
				}				
			}		
			_inports.clear();

			// Delete ports from Processing (base class) register
			GetInPorts().Clear();
		}

		~SndfileWriter()
		{
			RemovePortsAndControls();
		}
	};

}// namespace CLAM
#endif // SndfileWriter_hxx


