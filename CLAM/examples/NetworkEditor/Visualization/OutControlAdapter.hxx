

#ifndef __OUTCONTROLADAPTER_HXX__
#define __OUTCONTROLADAPTER_HXX__

#include "ModelAdapter.hxx"
#include "ControlModel.hxx"

namespace CLAM
{
	class OutControl;
}
namespace CLAMVM
{

class OutControlAdapter : public ModelAdapter, public ControlModel
{
private:
	const CLAM::OutControl* mObserved;
public:
	OutControlAdapter();
	virtual ~OutControlAdapter();
	virtual const char* GetClassName() const
	{
		return "OutControlAdapter";
	}
	virtual bool Publish();
	virtual bool BindTo( const CLAM::OutControl& obj );	
};

} // namespace CLAMVM

#endif // __INCONTROLMODEL_HXX__
