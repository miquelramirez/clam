#ifndef ProcessingFactory_hxx
#define ProcessingFactory_hxx

#include "Processing.hxx"
#include "Factory.hxx"

namespace CLAM
{

class ProcessingFactory : public Factory<Processing>
{
	bool _ladspasFullyLoaded;
public:
	ProcessingFactory() : _ladspasFullyLoaded(false) {}

	static ProcessingFactory& GetInstance();

	bool areLadspasFullyLoaded() const { return _ladspasFullyLoaded; }
	void setLadspasFullyLoaded() { _ladspasFullyLoaded = true; }
};

} //namespace CLAM

#endif

