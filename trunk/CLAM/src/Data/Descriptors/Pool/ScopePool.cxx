#include "ScopePool.hxx"

namespace CLAM
{
	ScopePool::~ScopePool()
	{
		AttributesData::iterator it = _attributePools.begin();
		AttributesData::iterator end = _attributePools.end();
		for (; it!=end; it++) it->Deallocate();
		_size=0;
	}
}

