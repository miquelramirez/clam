

#ifndef __INPORTADAPTER_HXX__
#define __INPORTADAPTER_HXX__

#include "ModelAdapter.hxx"
#include "PortModel.hxx"

namespace CLAM
{
	class InPort;
}
namespace CLAMVM
{

class InPortAdapter : public ModelAdapter, public PortModel
{
private:
	const CLAM::InPort* mObserved;
public:
	InPortAdapter();
	virtual ~InPortAdapter();
	virtual const char* GetClassName() const
	{
		return "InPortAdapter";
	}
	virtual bool Publish();
	virtual bool BindTo( const CLAM::InPort& obj );	
};

} // namespace CLAMVM

#endif // __INPORTMODEL_HXX__
