#include "ByteOrderConvert.hxx"


ByteOrderConvert::ByteOrderConvert()
{
}


ByteOrderConvert::~ByteOrderConvert()
{
}


TUInt16
ByteOrderConvert::Swap(const TUInt16& val)
{
#if defined linux 
	return bswap_16(val);
#else
	return (val>>8)|(val<<8);
#endif
}

TUInt32
ByteOrderConvert::Swap(const TUInt32& val)
{
#if defined linux 
	return bswap_32(val);
#else
  TUInt32 cp = val;
	TByte* ptr=(TByte*) &cp;
	static TByte tmp;
	tmp=ptr[0]; ptr[0]=ptr[3]; ptr[3]=tmp;
	tmp=ptr[1]; ptr[1]=ptr[2]; ptr[2]=tmp;
  return cp;
#endif
}

TUInt64 
ByteOrderConvert::Swap(const TUInt64& val)
{
#if defined linux 
	return bswap_64(val);
#else
  TUInt64 cp = val;
	TByte* ptr=(TByte*) &cp;
	static TByte tmp;
	tmp=ptr[0]; ptr[0]=ptr[7]; ptr[7]=tmp;
	tmp=ptr[1]; ptr[1]=ptr[6]; ptr[6]=tmp;
	tmp=ptr[2]; ptr[2]=ptr[5]; ptr[5]=tmp;
	tmp=ptr[3]; ptr[3]=ptr[4]; ptr[4]=tmp;
  return cp;
#endif
}


void ByteOrderConvert::_FixByteOrder(
	TByte* ptr,TUInt32 nElems,TUInt32 elemSize)
{
	switch (elemSize)
	{
		case 1: return;
		case 2:
		{
			TUInt16* fptr = (TUInt16*) ptr;
			for (TUInt32 i=0;i<nElems;i++)
			{
				*fptr = Swap(*fptr);
				fptr++;
			}
			return;
		}
		case 4:
		{
			TUInt32* fptr = (TUInt32*) ptr;
			for (TUInt32 i=0;i<nElems;i++)
			{
				*fptr = Swap(*fptr);
				fptr++;
			}
			return;
		}
		case 8:
		{
			TUInt64* fptr = (TUInt64*) ptr;
			for (TUInt32 i=0;i<nElems;i++)
			{
				*fptr = Swap(*fptr);
				fptr++;
			}
			return;
		}
		default: CLAM_ASSERT(false, "Bad element size");
	}
}

