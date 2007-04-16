#ifndef __STDOUTPROGRESS__
#define __STDOUTPROGRESS__

#include "Progress.hxx"

namespace CLAMGUI
{
	class StdOutProgress
		: public Progress
	{
		public:
		int mLastVal;

		StdOutProgress( const char* title, float from, float to );

		~StdOutProgress();

		void Update( float val );
	};
}

#endif // StdOutProgress.hxx

