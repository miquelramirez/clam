

#ifndef __INCONTROLADAPTER_HXX__
#define __INCONTROLADAPTER_HXX__

#include "ModelAdapter.hxx"
#include "ControlModel.hxx"

namespace CLAM
{
	class InControl;
}
namespace CLAMVM
{

class InControlAdapter : public ModelAdapter, public ControlModel
{
private:
	const CLAM::InControl* mObserved;
public:
	InControlAdapter();
	virtual ~InControlAdapter();
	virtual const char* GetClassName() const
	{
		return "InControlAdapter";
	}
	virtual bool Publish();
	virtual bool BindTo( const CLAM::InControl& obj );	
};

} // namespace CLAMVM

#endif // __INCONTROLMODEL_HXX__
