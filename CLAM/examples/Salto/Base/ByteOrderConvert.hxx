#ifndef _ByteOrderConvert_
#define _ByteOrderConvert_


#include "DataTypes.hxx"
#include "ByteOrder.hxx"

#ifdef linux
  #include <byteswap.h>
#endif

using namespace CLAM;

class ByteOrderConvert
{

public:
     ByteOrderConvert();
    ~ByteOrderConvert();
  
  
inline void FixByteOrder(TByte* ptr, TUInt32 nElems,TUInt32 elemSize)
{
#ifdef MTG_LITTLE_ENDIAN
	_FixByteOrder(ptr,nElems,elemSize);
#else
#ifndef MTG_BIG_ENDIAN
  #pragma message ("BYTE ORDER NOT DEFINED!")
#endif
#endif
}

private:

void    _FixByteOrder(TByte* ptr,TUInt32 nElems,TUInt32 elemSize);
TUInt16 Swap(const TUInt16& val);
TUInt32 Swap(const TUInt32& val);
TUInt64 Swap(const TUInt64& val);  
  
};
#endif

