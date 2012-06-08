#ifndef __QTWAITMESSAGEDLG__
#define __QTWAITMESSAGEDLG__

#include <string>

namespace CLAM
{
	namespace VM
	{
		class QtWaitMessageImpl;

		class QtWaitMessageDlg 
		{
		public:
			QtWaitMessageDlg();
			~QtWaitMessageDlg();

			void Caption(const std::string& caption);
			void LabelText(const std::string& label);
			void Show();

		private:
			QtWaitMessageImpl* mWaitMessage;

		};
	}
}

#endif

