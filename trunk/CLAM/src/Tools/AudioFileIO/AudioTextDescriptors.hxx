#ifndef __AUDIOTEXTDESCRIPTORS__
#define __AUDIOTEXTDESCRIPTORS__

#include "DynamicType.hxx"
#include "Text.hxx"

namespace CLAM
{
	class AudioTextDescriptors
		: public DynamicType
	{
	public:
		DYNAMIC_TYPE( AudioTextDescriptors, 6 );
		DYN_ATTRIBUTE( 0, public, Text, Artist );
		DYN_ATTRIBUTE( 1, public, Text, Title );
		DYN_ATTRIBUTE( 2, public, Text, Album );
		DYN_ATTRIBUTE( 3, public, Text, TrackNumber );
		DYN_ATTRIBUTE( 4, public, Text, Composer );
		DYN_ATTRIBUTE( 5, public, Text, Performer );

	protected:
		void DefaultInit();

	};
}

#endif // AudioTextDescriptors.hxx


