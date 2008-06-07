#ifndef _TypedInControlRegistry_hxx_
#define _TypedInControlRegistry_hxx_

#include <string>
#include <vector>

namespace CLAM{
	//forward declarations:
	class BaseTypedInControl;
	
	class TypedInControlRegistry {
		std::vector<BaseTypedInControl*> mTypedInControls;
		
		public:
			typedef std::vector<BaseTypedInControl*>::iterator Iterator;
			typedef std::vector<BaseTypedInControl*>::const_iterator ConstIterator;
			
			BaseTypedInControl& Get( const std::string & name ) const;
			BaseTypedInControl& GetByNumber(int index) const;
			bool Has(const std::string& name) const;
			
			int Size() const;
			void ProcessingInterface_Register(BaseTypedInControl *);
		private:
			std::string AvailableNames() const;
	};
} // namespace CLAM
#endif
