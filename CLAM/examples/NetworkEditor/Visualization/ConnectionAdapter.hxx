
#ifndef _CONNECTIONADAPTER_HXX_
#define _CONNECTIONADAPTER_HXX_

#include "ModelAdapter.hxx"
#include "ConnectionModel.hxx"

#include <list>

namespace CLAM
{
	class OutPort;
	class InPort;
}

namespace CLAMVM
{
	class ConnectionAdapter : public ModelAdapter, public ConnectionModel
	{
	private:		
		const CLAM::OutPort* mOutObserved;
		const CLAM::InPort* mInObserved;
		
	public:
		ConnectionAdapter();
		virtual ~ConnectionAdapter();
		virtual const char* GetClassName() const
		{
			return "ConnectionAdapter";
		}

		virtual bool ConnectsInPort( CLAM::InPort &);
		virtual bool Publish();
		virtual bool BindTo( const CLAM::OutPort&, const CLAM::InPort& );
		
	};
} // namespace CLAMVM

#endif //_CONNECTIONADAPTER_HXX_
