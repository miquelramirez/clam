#ifndef _InControlTmplArray_hxx_
#define _InControlTmplArray_hxx_

#include "InControl.hxx"
#include <vector>

namespace CLAM
{
/**
 * \todo document this class
 */ 
template <class TProcessing>
class InControlTmplArray
{
	typedef InControlTmpl<TProcessing> TInControl;
	typedef typename TInControl::TPtrMemberFuncId TPtrMemberFuncId;
	typedef std::vector<TInControl*> Controls;
	Controls mControls;

public:
	InControlTmplArray(int size, const std::string &name, TProcessing* parent,
		TPtrMemberFuncId f, const bool publish=true);
	~InControlTmplArray();

	inline TInControl& operator[](int i) { return *mControls[i]; }
	inline const TInControl& operator[](int i) const { return *mControls[i]; }

};

//------------------------------------------------------------------------------
// Implementation
template <class TProcessing>
InControlTmplArray<TProcessing>::InControlTmplArray(
		int size, 
		const std::string &name,
		TProcessing *parent, 
		TPtrMemberFuncId f,
		const bool publish )
{
	mControls.resize(size);
	for (int i=0; i<size; i++) {
		std::stringstream str;
		str << name << "_" << i;
		CLAM_ASSERT(parent, "InControlTmplArray must be published. Check ctr processing* parameter");
		mControls[i] = new TInControl(i, str.str(), parent, f);	
	}
}
template <class TProcessing>
InControlTmplArray<TProcessing>::~InControlTmplArray()
{
	for (std::size_t i=0; i<mControls.size(); i++)
		delete mControls[i];
}

} //namespace CLAM

#endif
