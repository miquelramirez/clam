#ifndef __WIN32CONSOLEAPP__
#define __WIN32CONSOLEAPP__

#ifndef WIN32
#error "Hey you, Linux User! You shouldn't be including this thing"
#endif

#include "BaseAudioApplication.hxx"
#include <windows.h>
#undef GetClassName
#undef CreateWindow

namespace CLAM
{

#ifdef WIN32

	// :MRJ:TODO: I don't know why DirectXAudioDevice class hasn't the static pointer already defined
	// instead of relying that responsability to such a concrete class as DXFullDuplex. While I haven't
	// got some time to rework the thing we will need the following ugly kludge...

	class DXFullDuplex;

	template <typename DXDeviceType>
	void DXHook( DXDeviceType* homemade_tmpl_func_overload, void* wndhndler )
	{
		DXDeviceType::shMainWnd = (HWND)wndhndler;
	}

#define DXFullDuplexHook( handler ) DXHook( (DXFullDuplex*)NULL, handler ) 

#endif


class Win32ConsoleApp 
: public BaseAudioApplication
{
public:

	Win32ConsoleApp()
	{
	}

	virtual ~Win32ConsoleApp()
	{
	}


	void Run( int argc = 0, char** argv = 0 );
	virtual void UserMain(void);
	virtual void AudioMain(void) = 0;
	static HWND GetHandle( void );	

private:

	virtual void AppCleanup(void)
	{
	}
	
};



	
}

#endif // Win32ConsoleApp.hxx
