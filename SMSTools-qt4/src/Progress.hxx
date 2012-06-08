#ifndef __PROGRESS__
#define __PROGRESS__

namespace CLAMGUI
{
	class Progress
	{
	public:
		char* mTitle;
		float mFrom;
		float mTo;

		Progress(const char* title,float from,float to);
		virtual ~Progress();
		virtual void Update(float val) = 0;
	};


}

#endif // Progress.hxx

