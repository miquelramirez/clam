#ifndef _InPortArrayTmpl_hxx_
#define _InPortArrayTmpl_hxx_

#include "InPortTmpl.hxx"

#include "mtgsstream.h"
#include "Array.hxx" // todo: ->std::vector

namespace CLAM
{

class Processing;

template<class T>
class InPortArrayTmpl
{
	Array<InPortTmpl<T>*> mArray;
	Array<T*> mDataPointers;
public:
	inline InPortArrayTmpl(int size,
	                       const std::string &n,
	                       Processing *o,
	                       int length,
	                       int hop = 0,
	                       bool inplace=false);

	inline ~InPortArrayTmpl();

	Array<T*> &GetDataArray();
	void LeaveDataArray();

	void SetParams(unsigned int length,
	               unsigned int hop = 0);

	InPortTmpl<T>       &operator[](int i)        { return *mArray[i]; }
	const InPortTmpl<T> &operator[](int i) const  { return *mArray[i]; }
};


// Implementation
//----------------------------------------------------------
	
template<class T>
InPortArrayTmpl<T>::InPortArrayTmpl(int size,
                                    const std::string &n,
                                    Processing *o,
                                    int length,
                                    int hop,
                                    bool inplace)
{
	mArray.Resize(size);
	mArray.SetSize(size);
	mDataPointers.Resize(size);
	mDataPointers.SetSize(size);

	for (int i=0; i<size; i++) {
		std::stringstream sstr;
		sstr.str("");
		sstr << n << "_" << i;
		mArray[i] = new InPortTmpl<T>(sstr.str(),o,length,hop,inplace);
	}
}

template<class T>
InPortArrayTmpl<T>::~InPortArrayTmpl()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		delete mArray[i];
}

template<class T>
void InPortArrayTmpl<T>::SetParams(unsigned int length,
                                   unsigned int hop)
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		mArray[i]->SetParams(length,hop);
}

template<class T>
Array<T*> &InPortArrayTmpl<T>::GetDataArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		mDataPointers[i] = &mArray[i]->GetData();
	return mDataPointers;
}

template<class T>
void InPortArrayTmpl<T>::LeaveDataArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		mArray[i]->LeaveData();
}



} // namespace CLAM

#endif
