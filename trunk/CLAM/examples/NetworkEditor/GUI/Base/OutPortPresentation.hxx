
#ifndef __OUTPORTPRESENTATION_HXX__
#define __OUTPORTPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv1.hxx"
#include <string>

namespace CLAMVM
{
	class PortModel;
}

namespace NetworkGUI
{

class OutPortPresentation : public CLAMVM::Presentation
{
protected:
	std::string mName;
public:
	OutPortPresentation();
	virtual ~OutPortPresentation();
	virtual void AttachTo(CLAMVM::PortModel & );
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual const std::string & GetName(){return mName;}

protected:
	virtual void OnNewName(const std::string& name) = 0;

public: //slots
	SigSlot::Slotv1<const std::string &> SetName;	

};

} // namespace NetworkGUI

#endif // __OUTPORTPRESENTATION_HXX__
