#ifndef __AUDIOFILEHEADER__
#define __AUDIOFILEHEADER__

#include "AudioFileFormats.hxx"
#include "DynamicType.hxx"

namespace CLAM
{
	class AudioFileHeader
		: public DynamicType
	{
	public:
		DYNAMIC_TYPE( AudioFileHeader, 6 );
		DYN_ATTRIBUTE( 0, public, TData,               SampleRate );
		DYN_ATTRIBUTE( 1, public, TSize,               Channels   );
		DYN_ATTRIBUTE( 2, public, TTime,               Length     );
		DYN_ATTRIBUTE( 3, public, EAudioFileFormat,    Format     );
		DYN_ATTRIBUTE( 4, public, EAudioFileEncoding,  Encoding   );
		DYN_ATTRIBUTE( 5, public, EAudioFileEndianess, Endianess  );

	protected:
		void DefaultInit();

	public:

		void SetValues( TData rate, TSize numberOfChannels,
				EAudioFileFormat fmt );
		
		void SetValues( TData rate, TSize numberOfChannels,
				const char* fmtString );

	};

}

#endif // AudioFileHeader.hxx
