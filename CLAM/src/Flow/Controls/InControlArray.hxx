#ifndef _InControlArray_hxx_
#define _InControlArray_hxx_

#include "InControl.hxx"
#include <vector>

namespace CLAM
{
/**
 * This class wrapps a vector (array) of InControls and gives its InControl automatic
 * names at construction
 */ 
class InControlArray
{
	typedef std::vector<InControl*> Controls;
	Controls mControls;
public:

	inline InControlArray(
		int size, 
		const std::string &name, 
		Processing* parent = 0);

	inline ~InControlArray();

	inline InControl       &operator[](int i)        { return *mControls[i]; }
	inline const InControl &operator[](int i) const  { return *mControls[i]; }
};


InControlArray::InControlArray(int size,
                               const std::string &name,
                               Processing *parent /*0 by default. See decl above*/)
{
	mControls.resize(size);
	for (int i=0; i<size; i++) {
		std::stringstream str;
		str << name << "_" << i;
		if (parent)
			mControls[i] = new InControl(str.str(),parent);
		else
			mControls[i] = new InControl(str.str());
	}
}

InControlArray::~InControlArray()
{
	for (std::size_t i=0; i<mControls.size(); i++)
		delete mControls[i];
}

} //namespace CLAM

#endif
