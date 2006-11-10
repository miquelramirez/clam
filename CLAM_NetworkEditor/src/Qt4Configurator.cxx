#include "Qt4Configurator.hxx"
namespace CLAM
{

	Qt4Configurator::~Qt4Configurator()
	{
		if (mSetter) delete mSetter;
		if (mGetter) delete mGetter;
	}

}


