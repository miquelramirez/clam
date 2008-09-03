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
		typedef SndfilePlayerConfig Config;
		typedef std::vector<CLAM::AudioOutPort*> OutPorts;
		OutPorts _outports;
		CLAM::OutControl _outControlSeek;
		CLAM::InControl _inControlSeek;
		CLAM::InControl _inControlPause;
		SndfileHandle* _infile;
		SndfilePlayerConfig _config;
		unsigned _numChannels;
		std::vector<float> _buffer; 
		unsigned _numReadFrames;
		unsigned _numTotalFrames;
		
	public:
		const char* GetClassName() const { return "SndfilePlayer"; }

		SndfilePlayer(const ProcessingConfig& config =  Config()) 
			: _outControlSeek("Position out-Control",this) 
			, _inControlSeek("Seek in-Control",this) 
			, _inControlPause("Pause in-Control",this)
			, _infile(0)
			,_numChannels(0)
			, _numReadFrames(0)
			, _numTotalFrames(0)
		{ 
			Configure( config );
		}
	 
		bool Do()
		{
			// PAUSE CONTROL
			//User has moved the slider and we have to change the position
			//TODO potentially dangerous since a different thread is reading!
			if(_infile)
			{	
				if(_inControlPause.GetLastValue()<0.5)
					WriteSilenceToPorts();
				else
					ReadBufferAndWriteToPorts();
			}		

			static CLAM::TControlData controlSliderValue = 0;			
			//User has moved the slider and we have to change the position
			//TODO potentially dangerous since a different thread is reading!
			if(controlSliderValue != _inControlSeek.GetLastValue() and _infile)
			{	controlSliderValue = _inControlSeek.GetLastValue();
				_numReadFrames = controlSliderValue*_numTotalFrames;
				_infile->seek(_numReadFrames ,SEEK_SET);
			}		

			//Calculate the seek position between 0 and 1
			float seekPosition = ((float)_numReadFrames/(float)_numTotalFrames);
			if(seekPosition >1)
			{	_numReadFrames = _numReadFrames -_numTotalFrames;
				seekPosition = ((float)_numReadFrames/(float)_numTotalFrames);							
			}

			_outControlSeek.SendControl(seekPosition);
	
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
			{	_numReadFrames = 0;
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
				_numReadFrames++;
				frameIndex++;
			}

			if (readSize == (int)bufferSize) return;	
		
			// the last buffer is not complete fill it up with zeros and close file
			frameIndex=0;
			for(int i=readSize; i<(int) bufferSize; i += _numChannels)
			{	for(unsigned channel=0;channel<_numChannels;channel++)
				{
					channels[channel][frameIndex] = 0; 
				}
				_numReadFrames++;
				frameIndex++;
			}

			delete _infile;
			_infile = 0;

			if(_config.GetLoop())
				_infile = new SndfileHandle(_config.GetSourceFile().c_str(), SFM_READ);			
				
			return;		
		}
		
		void WriteSilenceToPorts()
		{
			//all the ports have to have the same buffer size
			const int portSize = _outports[0]->GetAudio().GetBuffer().Size();
			CLAM::TData* channels[_numChannels];

			for (unsigned channel=0; channel<_numChannels; channel++)
				channels[channel] = &_outports[channel]->GetAudio().GetBuffer()[0];

			for(int i=0; i< portSize; i++) 
				for(unsigned channel = 0; channel<_numChannels; channel++)
					channels[channel][i] = 0;					
			return;	
					
		}

		bool ConcreteStart()	
		{	
			CLAM_ASSERT(_infile, "SndfilePlayer::ConcreteStart() should have _infile with value.");
			_infile->seek(0,SEEK_SET);
			_numReadFrames = 0;
			//initial configuration for the controls.
			_inControlSeek.SetBounds(0,1);
			_inControlSeek.DoControl(0.);
			_inControlPause.SetBounds(0,1);
			_inControlPause.DoControl(0.5);
			_outControlSeek.SendControl(0.);			
			return true; 
		}
		bool ConcreteStop()
		{
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
			_config.SetSavedNumberOfChannels(_infile->channels() );
			_buffer.resize(portSize*_numChannels);
			_numTotalFrames = _infile->frames();
			// The file has not size, perhaps that's because the file is empty			
			if(_numTotalFrames == 0)_numTotalFrames = 1;
	
			// case 1: maintain the same ports
			if ( (unsigned)_infile->channels() == _outports.size() )
				return true;
			
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

		~SndfilePlayer()
		{
			RemovePorts();
		}
	};

}// namespace CLAM
#endif // SndfilePlayer_hxx


