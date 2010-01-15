#ifndef MTCReceiver_hxx
#define MTCReceiver_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/MIDIMessage.hxx>

#include "../RtMidi.hxx"

namespace CLAM {

	class MTCReceiver : public CLAM::Processing
	{ 
		/** Controls **/

		//TODO: OutControl<long>
		FloatOutControl _millisecondsOut;
		
		RtMidiIn *_MIDIin;
		int _mtcFrameIndex;
		short _frames;
		short _seconds;
		short _minutes;
		short _hours;
		long _lastReceivedTime;

		static void RtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData );

	public:
		const char* GetClassName() const { return "MTCReceiver"; }

		MTCReceiver( const Config& config = Config() );

		~MTCReceiver();

		bool Do() 
		{ 
			return true; 
		}

		bool Do(std::vector< unsigned char > *message)
		{
			float fps = 25;
			float frame_length = 1000./fps;
			float tick_length = 1000./fps/8;

			// TODO: deal with many messages in the vector/queue and take timestamp into account

			unsigned int nBytes = message->size();
			if (not nBytes) return true;

			if (nBytes==2 and (*message)[0]==0xF1)
			{
				unsigned nibble = (*message)[1];
				unsigned value = (0x0F & nibble);
				unsigned index = (0xF0 & nibble) >> 4;
// std::cout << "index " << std::hex << index << " value " << value <<std::endl;
				// wait till the beginning of the next time chunk.
				if (_mtcFrameIndex==-1 and index != 0)
					return true; 

				_mtcFrameIndex=index;
				if (_mtcFrameIndex != int(index)) 
					std::cout << "WARNING: Bad MTC index. Expected " << _mtcFrameIndex << " " << std::flush;
				
				_mtcFrameIndex++;
				if (_mtcFrameIndex==8) _mtcFrameIndex=0;

				switch (index)
				{
					case 0:
						_frames = value;
						break;
					case 1:
						_frames += (value << 4);
						break;
					case 2:
						_seconds = value;
						break;
					case 3:
						_seconds += (value << 4);
						break;
					case 4:
						_minutes = value;
						break;
					case 5:
						_minutes += (value << 4);
						break;
					case 6:
						_hours = value;
						break;
					case 7:
						_hours += (value & 0x1) << 4;
						unsigned fps_type =  ((value & 0x6) >> 2); 
					//	std::cout << fps_type << " h:m:s:f "<< std::dec << _hours << ":" << _minutes << ":" << _seconds << ":" << _frames << std::endl;
						_lastReceivedTime = (_hours*60*60 + _minutes*60 + _seconds)*1000 + _frames*frame_length;
						
						break;
				}
				long milliseconds = _lastReceivedTime + index*tick_length;
				// Send Message
				_millisecondsOut.SendControl( milliseconds );

			}
			else
			{
				std::cout << "Sysex received"<< std::endl;
				for (unsigned i=0; i<nBytes; i++)
				{
					unsigned char aa=(*message)[i];
					//std::cout<<std::hex << int(aa) <<std::endl;
				}
			}
			return true;
		}
	};
	
} // End namespace
#endif // MTCReceiver_hxx
