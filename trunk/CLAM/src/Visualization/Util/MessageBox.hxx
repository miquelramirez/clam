#ifndef __MESSAGEBOX__
#define __MESSAGEBOX__

#include <string>

namespace CLAMVM
{
	class MessageBox 
	{
	public:

		MessageBox();
		~MessageBox();

		void SetText( const std::string& msgTxt );
		void Show();

	protected:

		std::string mMessageText;
	};
}


#endif // MessageBox.hxx
