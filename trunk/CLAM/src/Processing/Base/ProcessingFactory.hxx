#ifndef _ProcessingFactory_hxx_
#define _ProcessingFactory_hxx_

#include "Factory.hxx"
#include "Processing.hxx"

namespace CLAM
{
	
	class ProcessingFactory : public Factory<Processing>
	{
		public:

			static Factory<Processing>& GetInstance();
			
			virtual ~ProcessingFactory();
	};

} //namespace CLAM
#endif //_ProcessingFactory_hxx_
