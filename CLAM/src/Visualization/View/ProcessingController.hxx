#ifndef __PROCESSINGCONTROLLER__
#define __PROCESSINGCONTROLLER__

#include <map>
#include "ModelController.hxx"
#include "ControlAdapter.hxx"
#include "ProcessingInControlSet.hxx"

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	using CLAM::Processing;

	class ProcessingController
		: public ModelController, public ProcessingInControlSet
	{

	protected:
		void DeleteSubAdapters();

	public:
			
		ProcessingController();

		virtual ~ProcessingController();

		virtual const char* GetClassName() const
		{
			return "ProcessingController";
		}

		virtual bool Publish();

		virtual bool Update();

		virtual bool BindTo( Processing& procObj );
				
		virtual bool Unbind();


	};
}

#endif // ProcessingController.hxx
