#ifndef _InPortTmpl_hxx_
#define _InPortTmpl_hxx_

template <typename T>
class InPortTmpl
{
	T* mData;
public:
	InPortTmpl(const char*, void*, int=0)
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
