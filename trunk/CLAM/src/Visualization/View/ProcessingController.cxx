#include "ProcessingController.hxx"
#include "Processing.hxx"
#include "Assert.hxx"

namespace CLAMVM
{
	using CLAM::Processing;

	ProcessingController::ProcessingController()
	{
	}

	ProcessingController::~ProcessingController()
	{
		DeleteSubAdapters();
	}

	bool ProcessingController::Publish()
	{
		iterator i = mChildren.begin();

		while ( i != mChildren.end() )
		{
			ControlAdapter* adap = static_cast<ControlAdapter* >( i->second );
			CLAM_ASSERT( adap!=NULL, "The static cast failed: invariant does not hold!" );
			adap->Publish();

			i++;
		}

		return true;
	}

	bool ProcessingController::Update()
	{
		iterator i = mChildren.begin();

		while ( i != mChildren.end() )
		{
			ControlAdapter* adap = static_cast<ControlAdapter* >( i->second );
			CLAM_ASSERT( adap!=NULL, "The static cast failed: invariant does not hold!" );
			adap->Update();

			i++;
		}
				
		return true;
	}


	bool ProcessingController::BindTo( Processing& procObj )
	{

		CLAM::PusblishedInControls::Iterator i = procObj.GetInControls().Begin();

		while ( i!=procObj.GetInControls().End() )
		{
			ControlAdapter* adap = new ControlAdapter();
						
			Insert( (*i)->GetName(), *adap   );
			adap->BindTo( *(*i) );
						
			i++;
		}

		return true;
	}

	bool ProcessingController::Unbind()
	{
		DeleteSubAdapters();
		mChildren.clear();
				
		return true;
	}

	void ProcessingController::DeleteSubAdapters()
	{
		iterator i = mChildren.begin();

		while ( i != mChildren.end() )
		{
			delete i->second;
				
			i++;
		}				

	}
}
