#include "Array.hxx"

namespace CLAM
{
#define CLAM_NUMERIC_ARRAY_INITIALIZATION(Type)          \
template<>                                              \
void Array<Type>::InitializeElement(int i)              \
{                                                       \
    mpData[i]=0;                                        \
}                                                       \


CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned long)
CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned int)
CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned short)
CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned char)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed long)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed int)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed short)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed char)
CLAM_NUMERIC_ARRAY_INITIALIZATION(double)
CLAM_NUMERIC_ARRAY_INITIALIZATION(float)


/** And fast spetialization for basic types */

#define CLAM_FAST_ARRAY_SPECIALIZATIONS(TYPE)                           \
template<>                                                             \
void Array<TYPE >::CopyDataBlock(int first, int last,                  \
                                 const TYPE *src)                      \
{                                                                      \
    if (last>first)                                                    \
        memcpy(&mpData[first],&src[first],                             \
               sizeof(TYPE)*(last-first));                             \
}                                                                      \
template<>                                                             \
void Array<TYPE >::InitializeCopyDataBlock(int first, int last,        \
                                           const TYPE *src)            \
{                                                                      \
    if (last>first)                                                    \
        memcpy(&mpData[first],&src[first],                             \
               sizeof(TYPE)*(last-first));                             \
}                                                                      \
template<>                                                             \
void Array<TYPE >::InitializeCopyDataBlock(int first, int last,        \
                                           int src_first,              \
                                           const TYPE *src)            \
{                                                                      \
    if (last>first)                                                    \
        memcpy(&mpData[first],&src[src_first],                         \
               sizeof(TYPE)*(last-first));                             \
}                                                                      \
template<>                                                             \
void Array<TYPE >::ResizeDataBuffer(int new_size)                      \
{                                                                      \
    mpData = (TYPE*) realloc(mpData,new_size*sizeof(TYPE));            \
}                                                                      \
template<>                                                             \
void Array<TYPE >::InsertElemInDataBuffer(int where)                   \
{                                                                      \
    memmove(&mpData[where+1],&mpData[where],                           \
            (mSize-where)*sizeof(TYPE));                               \
}                                                                      \
template<>                                                             \
void Array<TYPE >::DeleteElemInDataBuffer(int where)                   \
{                                                                      \
    memmove(&mpData[where],&mpData[where+1],                           \
            (mSize-where-1)*sizeof(TYPE));                             \
}                                                                      \

CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned long)
CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned int)
CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned short)
CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned char)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed long)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed int)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed short)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed char)
CLAM_FAST_ARRAY_SPECIALIZATIONS(double)
CLAM_FAST_ARRAY_SPECIALIZATIONS(float)


}
