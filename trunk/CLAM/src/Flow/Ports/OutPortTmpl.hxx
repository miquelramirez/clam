#ifndef _OutPortTmpl_hxx_
#define _OutPortTmpl_hxx_

template<typename T>
class OutPortTmpl
{
	T* mData;
public:
	OutPortTmpl(const char*, void*, int=0)
	{
	}
	void Attach( T& toAttach)
	{
		mData = &toAttach;
	}
	T& GetData()
	{
		return *mData;
	}
};

#endif

