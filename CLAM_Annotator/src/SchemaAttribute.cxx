#include "SchemaAttribute.hxx"
#include "Text.hxx"
#include <algorithm>

namespace CLAM_Annotator{

	/**************** SchemaAttribute **************/

	void SchemaAttribute::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
	}


};
