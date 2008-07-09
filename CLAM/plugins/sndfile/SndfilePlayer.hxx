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
		AudioOutPort _out;
		SndfileHandle* _infile;
		SndfilePlayerConfig _config;
		int _channels;
		
	public:
		typedef SndfilePlayerConfig Config;

		const char* GetClassName() const { return "SndfilePlayer"; }

		SndfilePlayer(const ProcessingConfig& config =  Config()) 
			: _out("s Output", this) 
			, _infile(0)
			,_channels(0)
		{ 
			Configure( config );
		}
	 
		bool Do()
		{
			Do(_out.GetAudio().GetBuffer());		
		
			// Tell the ports this is done
			_out.Produce();
			return true;
		}

		bool Do(DataArray& out)
		{
			const int portSize = out.Size();
			if (not _infile)
			{
				for(int i=0; i< portSize; i++) out[i] = 0; // Do it for all the channels
				return true;
			}

			const int bufferSize =  portSize * _channels;
			float buffer[bufferSize];
			int readSize = _infile->read( buffer,bufferSize);
			
			// produce the portion read (normally the whole buffer)
			int frameIndex=0;
			for (int i=0; i<bufferSize; i+= _channels) 
			{      				
					//Now only catch the first channel.		
					out[frameIndex] = buffer[i];
					frameIndex++;
			}

			if (readSize == bufferSize) return true;	
		
			// the last buffer is not complete fill it up with zeros and close file
			for(int i=readSize; i< bufferSize; i++)
			{	if(i<portSize)
					out[i] = 0; // Do it for all the channels
			} 
	 
			delete _infile;
			_infile = 0;

			if(_config.GetLoop())
				_infile = new SndfileHandle(_config.GetSourceFile().c_str(), SFM_READ);			
				
			return true;		
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

			_channels = _infile->channels();
			std::cout << " Channel: " << _channels <<std::endl;
			return true;
		}
	};

}// namespace CLAM
#endif // SndfilePlayer_hxx


