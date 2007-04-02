#include "QTConfigurator.hxx"
namespace CLAM
{

	QTConfigurator::~QTConfigurator()
	{
		if (mSetter) delete mSetter;
		if (mGetter) delete mGetter;
	}

}


