#ifndef __FDFILTERCONTROLLER__
#define __FDFILTERCONTROLLER__

#include "ProcessingController.hxx"

namespace CLAM
{
	class Processing;
	class FDFilterGen;
}


namespace CLAMVM
{
	using CLAM::FDFilterGen;

	class FDFilterController : public ProcessingController
	{
	protected:
		const FDFilterGen* mObserved;

	public:

		FDFilterController();
		virtual ~FDFilterController();

		virtual const char* GetClassName() const {
			return "FDFilterController";
		}

		virtual bool BindTo( Processing& proc );

		virtual bool Publish();	
	};
}


#endif // FDFilterController.hxx
