#ifndef _OutPortArrayTmpl_hxx_
#define _OutPortArrayTmpl_hxx_

#include "OutPortTmpl.hxx"

#include <sstream>
#include "Array.hxx" // todo: ->std::vector

namespace CLAM
{

class Processing;


template<class T>
class OutPortArrayTmpl
{
	Array<OutPortTmpl<T>*> mArray;
	Array<T*> mDataPointers;
public:
	inline OutPortArrayTmpl(int size,
	                        const std::string &n,
	                        Processing *o,
	                        int length,
	                        int hop,
	                        bool inplace=false);

	inline ~OutPortArrayTmpl();

	Array<T*> &GetDataArray();
	void LeaveDataArray();

	void SetParams(unsigned int length,
	               unsigned int hop = 0);

	OutPortTmpl<T>       &operator[](int i)        { return *mArray[i]; }
	const OutPortTmpl<T> &operator[](int i) const  { return *mArray[i]; }
};



// Implementation
//---------------------------------------------------------

template<class T>
OutPortArrayTmpl<T>::OutPortArrayTmpl(int size,
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
		mArray[i] = new OutPortTmpl<T>(sstr.str(),o,length,hop,inplace);
	}
}

template<class T>
OutPortArrayTmpl<T>::~OutPortArrayTmpl()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		delete mArray[i];
}

template<class T>
void OutPortArrayTmpl<T>::SetParams(unsigned int length,
                                    unsigned int hop )
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		mArray[i]->SetParams(length,hop);
}

template<class T>
Array<T*> &OutPortArrayTmpl<T>::GetDataArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		mDataPointers[i] = &mArray[i]->GetData();
	return mDataPointers;
}

template<class T>
void OutPortArrayTmpl<T>::LeaveDataArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		mArray[i]->LeaveData();
}


} // namespace CLAM


#endif
