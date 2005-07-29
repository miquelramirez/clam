#ifndef __SHOWMESSAGE__
#define __SHOWMESSAGE__

#include <vector>

namespace CLAM
{
	namespace VM
	{
		class ShowMessage
		{
			typedef std::vector<bool> Messages;
		public:
			static void NoMIDISupport();

		private:
			static Messages mMessages;
			enum { MESSAGES=1 };
			enum { NOMIDI=0 };

			static void Init();
			static bool IsInitiated();
		};
	}
}

#endif

