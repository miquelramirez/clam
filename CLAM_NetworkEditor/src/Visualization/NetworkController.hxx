
#ifndef _NETWORKCONTROLLER_HXX_
#define _NETWORKCONTROLLER_HXX_

#include "ModelController.hxx"
#include "NetworkModel.hxx"
#include "ProcessingAdapter.hxx"
#include "ConnectionAdapter.hxx"

#include <list>
#include <string>

namespace CLAM
{
	class Network;
}

namespace CLAMVM
{
	class NetworkController : public ModelController, public NetworkModel
	{
	private:		
		CLAM::Network* mObserved;
		std::list<ProcessingAdapter*> mProcessingAdapters;
		typedef std::list<ProcessingAdapter*>::iterator ProcessingAdapterIterator;
		std::list<ConnectionAdapter*> mConnectionAdapters;
		typedef std::list<ConnectionAdapter*>::iterator ConnectionAdapterIterator;

		void OnNewConnectionFromGUI(const std::string &, const std::string &);
		void OnNewChangeState( bool);
		void OnRemoveConnectionFromGUI(const std::string &, const std::string &);
	public:
		NetworkController();
		virtual ~NetworkController();
		virtual const char* GetClassName() const
		{
			return "NetworkController";
		}
		void AddProcessing( const std::string & , CLAM::Processing *  );
		void NewProcessingFromGUI( const std::string & , CLAM::Processing * );
	
		virtual bool Publish();
		virtual bool Update();
		virtual bool BindTo( CLAM::Network&  );
	};
} // namespace CLAMVM

#endif //_NETWORKCONTROLLER_HXX_
