
#ifndef __INCONTROLPRESENTATION_HXX__
#define __INCONTROLPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv1.hxx"
#include <string>

namespace CLAMVM
{
	class ControlModel;
}

namespace NetworkGUI
{

class InControlPresentation : public CLAMVM::Presentation
{
protected:
	std::string mName;
public:
	InControlPresentation();
	virtual ~InControlPresentation();
	virtual void AttachTo(CLAMVM::ControlModel & );
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual const std::string & GetName(){return mName;}

protected:
	virtual void OnNewName(const std::string& name) = 0;

public: //slots
	SigSlot::Slotv1<const std::string &> SetName;
};

} // namespace NetworkGUI

#endif // __INCONTROLPRESENTATION_HXX__
