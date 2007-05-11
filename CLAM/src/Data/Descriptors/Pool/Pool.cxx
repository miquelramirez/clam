#include "Pool.hxx"

namespace CLAM
{
	DescriptionDataPool::~DescriptionDataPool()
	{
		ScopePools::iterator it = _scopePools.begin();
		ScopePools::iterator end = _scopePools.end();
		for (; it != end; it++)
			if (*it) delete *it;
	}
}
