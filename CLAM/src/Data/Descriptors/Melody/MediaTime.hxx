
#ifndef _MediaTime_
#define _MediaTime_

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"

#include "Enum.hxx"

#ifdef WIN32
	//XA: this should not be here but else does not compile
	#undef GetClassName
#endif

namespace CLAM {
	
//////////////////////////////////////////////////////
// Class MediaTime:
//
/** Media Time type definition                   *
 * This type propose a flexible way of defining  *
 * the temporal location of an audio segment     *
 * Types: {begin, end}, {begin, duration}        *
 * This type is thought to be used to define     *
 * some kind of probability when defining segment*
 * boundaries                                    *
 *                                               */

class MediaTime: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MediaTime, 3, ProcessingData);
		DYN_ATTRIBUTE (0, public, TData, Begin);   
		DYN_ATTRIBUTE (1, public, TData, End);
		DYN_ATTRIBUTE (2, public, TData, Duration);
		
	void DefaultInit();	
};

}; // namespace CLAM

#endif


