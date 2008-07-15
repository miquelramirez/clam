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
#ifndef SndfilePlayer_hxx
#define SndfilePlayer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/AudioInFilename.hxx> 
#include <CLAM/Audio.hxx>
#include <sndfile.hh>  

namespace CLAM
{
	class SndfilePlayerConfig : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( SndfilePlayerConfig, 2, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, AudioInFilename, SourceFile );
	    DYN_ATTRIBUTE( 1, public, bool, Loop );

		protected:	
		void DefaultInit()
		{
			AddAll();
			UpdateData();
		};	
	};

	class SndfilePlayer : public  Processing
	{ 
		typedef SndfilePlayerConfig Config;
		typedef std::vector<CLAM::AudioOutPort*> OutPorts;
		OutPorts _outports;

		AudioOutPort _out;
		SndfileHandle* _infile;
		SndfilePlayerConfig _config;
		unsigned _numChannels;
		std::vector<float> _buffer; 
		
	public:
		const char* GetClassName() const { return "SndfilePlayer"; }

		SndfilePlayer(const ProcessingConfig& config =  Config()) 
			: _out("s Output", this) 
			, _infile(0)
			,_numChannels(0)
		{ 
			Configure( config );
		}
	 
		bool Do()
		{
			ReadBufferAndWriteToPorts();		
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
			for (unsigned channel=0; channel<_numChannels; channel++)
				channels[channel] = &_outports[channel]->GetAudio().GetBuffer()[0];

			if (not _infile)
			{
				for(int i=0; i< portSize; i++) 
					for(unsigned channel = 0; channel<_numChannels; channel++)
						channels[channel][i] = 0; 
				return;
			}
			unsigned bufferSize = portSize*_numChannels;
			int readSize = _infile->read( &_buffer[0], bufferSize);
			
			// produce the portion read (normally the whole buffer)
			int frameIndex=0;
			for (int i=0; i<readSize; i+= _numChannels) 
			{	for(unsigned channel = 0; channel<_numChannels; channel++)
				{				
					channels[channel][frameIndex] = _buffer[i+channel];
				}
				frameIndex++;
			}

			if (readSize == bufferSize) return;	
		
			// the last buffer is not complete fill it up with zeros and close file
			frameIndex=0;
			for(int i=readSize; i< bufferSize; i += _numChannels)
			{	for(unsigned channel=0;channel<_numChannels;channel++)
				{
					channels[channel][frameIndex] = 0; 
				}
				frameIndex++;
			}

			delete _infile;
			_infile = 0;

			if(_config.GetLoop())
				_infile = new SndfileHandle(_config.GetSourceFile().c_str(), SFM_READ);			
				
			return;		
		}

		bool ConcreteStart()	
		{
			if(_infile)
			{	
				delete _infile;
			}	

			_infile = new SndfileHandle(_config.GetSourceFile().c_str(), SFM_READ);	

			// check if the file is open
			if(!*_infile)
			{
				std::cout << "Error. Can not open the file"<< std::endl;
				return false;
			}
			
			return true; // Configuration ok
		}
	
		const ProcessingConfig& GetConfig() const
		{
			return _config;
		}

		bool ConcreteConfigure(const ProcessingConfig & config)
		{
			CopyAsConcreteConfig(_config, config);

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
				AddConfigErrorMessage("Error with the file type. Only PCM WAV files are supported");
				return false;
			}

			RemovePortsAndControls();
			_numChannels = _infile->channels();
			unsigned portSize = BackendBufferSize();
			for(unsigned i=0;i<_numChannels;i++)
			{		
				std::ostringstream nameStream;
				nameStream << "out " << (i+1);
				AudioOutPort * port = new AudioOutPort( nameStream.str(), this);
				port->SetSize( portSize );
				port->SetHop( portSize );
				_outports.push_back( port );
			}
			_buffer.resize(portSize*_numChannels);
			return true;
		}
		void RemovePortsAndControls()
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

		~SndfilePlayer()
		{
			RemovePortsAndControls();
		}
	};

}// namespace CLAM
#endif // SndfilePlayer_hxx


