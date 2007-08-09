#ifndef ProcessingFactory_hxx
#define ProcessingFactory_hxx

#include "Processing.hxx"
#include "Factory.hxx"

namespace CLAM
{

class ProcessingFactory : public Factory<Processing>
{
public:
	static ProcessingFactory& GetInstance();
};

} //namespace CLAM

#endif

