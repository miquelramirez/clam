

#ifndef __OUTPORTADAPTER_HXX__
#define __OUTPORTADAPTER_HXX__

#include "ModelAdapter.hxx"
#include "PortModel.hxx"

namespace CLAM
{
	class OutPort;
}
namespace CLAMVM
{

class OutPortAdapter : public ModelAdapter, public PortModel
{
private:
	const CLAM::OutPort* mObserved;
public:
	OutPortAdapter();
	virtual ~OutPortAdapter();
	virtual const char* GetClassName() const
	{
		return "OutPortAdapter";
	}
	virtual bool Publish();
	virtual bool BindTo( const CLAM::OutPort& obj );	
};

} // namespace CLAMVM

#endif // __INPORTMODEL_HXX__
