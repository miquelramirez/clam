#ifndef _OUTCONTROLARRAY_HXX_
#define _OUTCONTROLARRAY_HXX_

#include "OutControl.hxx"
#include "Processing.hxx"

/////////////////////////////////////////////////////////////////////////////////////////
//  Control Arrays
//

namespace CLAM{

class OutControlArray
{
	std::vector<OutControl*> mControls;
public:
//Constructors    
    /** Default constructor, control array is initialized to size 0*/
	OutControlArray();
	/** Control array is initialized to "size" and the name is used as a
	 * prefix, adding a number afterwards */
	OutControlArray(int size, const std::string &name, Processing* whereToPublish=0);
	/** Control array is initialized to "size" and the list of names is used to label
	 * each of the controls */
	OutControlArray(int size, const std::list<std::string>& names, Processing *parent);
	
	~OutControlArray();

	OutControl       &operator[](int i)        { return *mControls[i]; }
	const OutControl &operator[](int i) const  { return *mControls[i]; }
	
	void Resize(int size, const std::string &name, Processing* parent);
	void Resize(int size, const std::list<std::string>& names, Processing* parent);
				

	int Size() const {return mControls.size();}
protected:
	void Shrink(int size);
	
};


};

#endif /*_OUTCONTROLARRAY_HXX_*/
