#ifndef _FUNCTION_DISPLAY_H
#define _FUNCTION_DISPLAY_H

#include "MTPresentation.hxx"
#include "DataTypes.hxx"
#include <list>

namespace CLAMGUI {

	class FunctionDisplay
	{
	protected:
		bool mModified;
		std::list<MTPresentation*> mPresentations;
	public:
		FunctionDisplay()
			: mModified(false)
			{}

		virtual ~FunctionDisplay() {}

		void SetModified() {mModified = true;}

		void AddMe(MTPresentation&);

		virtual void SetFunctionArea(CLAM::TData x_min, CLAM::TData x_width, CLAM::TData x_delta,
									 CLAM::TData y_min, CLAM::TData y_width, CLAM::TData y_delta) = 0;

	};

}


#endif
