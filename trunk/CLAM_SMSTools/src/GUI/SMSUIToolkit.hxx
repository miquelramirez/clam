#ifndef __SMSUITOOLKIT__
#define __SMSUITOOLKIT__

namespace SMS
{

namespace ToolsUI
{
	// Widget toolkit used by SMS Tools UI abstraction
	class Toolkit
	{
	public:
		/**
		 *  Adjusts toolkit widgets so they look matches
		 *  that of the user's desktop
		 */
		static void AdjustLookAndFeel();

		static void QueryDesktopMetrics( int& desktopW,
						 int& dekstopH );
	};
}

}

#endif //SMSUIToolkit.hxx
