#include "DescriptionScope.hxx"

namespace CLAM
{

		DescriptionScope::~DescriptionScope()
		{
			Attributes::iterator it = _attributes.begin();
			Attributes::iterator end = _attributes.end();
			for (; it!=end; it++)
				delete *it;
		}
}
