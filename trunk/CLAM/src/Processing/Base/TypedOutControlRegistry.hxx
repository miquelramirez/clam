#ifndef _TypedOutControlRegistry_hxx_
#define _TypedOutControlRegistry_hxx_

#include <string>
#include <vector>
#include <CLAM/BaseTypedOutControl.hxx>

namespace CLAM{
	
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
			void ProcessingInterface_Unregister(BaseTypedOutControl *);
			void Clear(){ mTypedOutControls.clear(); }
			Iterator Begin();
			Iterator End();	
			ConstIterator Begin() const;
			ConstIterator End() const;
		private:
			std::string AvailableNames() const;
	};
} // namespace CLAM
#endif
