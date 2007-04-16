#ifndef __WAITMESSAGE__
#define __WAITMESSAGE__

namespace CLAMGUI
{
	class WaitMessage
	{
		public:
		char* mTitle;
		
		WaitMessage(const char* title);
		virtual ~WaitMessage();
	};

}

#endif // WaitMessage.hxx

