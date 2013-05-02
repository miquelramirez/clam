#include "StdOutProgress.hxx"
#include <iostream>

namespace CLAMGUI
{
	StdOutProgress::StdOutProgress(const char* title,float from,float to)
	:Progress(title,from,to)
	{
		mLastVal=0;
		std::cout << mTitle << "\n";
	}

	StdOutProgress::~StdOutProgress()
	{
		std::cout << "Done\n";
	}

	void StdOutProgress::Update(float val)
	{
		int ival = int(100.*(val-mFrom)/(mTo-mFrom));
		if (ival<mLastVal+10) return;
		mLastVal = ival;
		std::cout << mLastVal << "% ";
	}

}

