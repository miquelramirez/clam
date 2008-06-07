#ifndef _TypedOutControlRegistry_hxx_
#define _TypedOutControlRegistry_hxx_

#include <string>
#include <vector>

namespace CLAM{
	//forward declarations:
	class BaseTypedOutControl;
	
	class TypedOutControlRegistry {
		std::vector<BaseTypedOutControl*> mTypedOutControls;
		
		public:
			typedef std::vector<BaseTypedOutControl*>::iterator Iterator;
			typedef std::vector<BaseTypedOutControl*>::const_iterator ConstIterator;
			
			BaseTypedOutControl& Get( const std::string & name ) const;
			BaseTypedOutControl& GetByNumber(int index) const;
			bool Has(const std::string& name) const;
			
			int Size() const;
			void ProcessingInterface_Register(BaseTypedOutControl *);
		private:
			std::string AvailableNames() const;
	};
} // namespace CLAM
#endif
