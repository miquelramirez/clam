#ifndef __QTPROGRESSDLG__
#define __QTPROGRESSDLG__

#include <string>

class Q3ProgressDialog;

namespace CLAM
{
	namespace VM
	{
		class QtProgressDlg
		{
		public:
			QtProgressDlg();
			~QtProgressDlg();

			void Caption(const std::string& caption);
			void LabelText(const std::string& label);
			void TotalSteps(int steps);
			void Update(int val);
			void Show();
	
		private:
			Q3ProgressDialog* mProgressDlg;
	};
	}
}

#endif

