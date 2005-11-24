#ifndef __TIMEINDEX_H__
#define __TIMEINDEX_H__

#include <CLAM/ProcessingData.hxx>

namespace CLAM
{

class TimeIndex : public ProcessingData 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE(TimeIndex, 2, ProcessingData);
	DYN_ATTRIBUTE (0,public, TData, Position);
	DYN_ATTRIBUTE (1,public, TData, Weight);
protected:
	void DefaultInit();
};

} // namespace CLAM

#endif // __TIMEINDEX_H__

