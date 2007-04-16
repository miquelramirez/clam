#include "ConcreteModelController.hxx"
#include <Assert.hxx>

namespace CLAMVM
{
		ConcreteModelController::ConcreteModelController()
		{
				Prepare();
		}

		ConcreteModelController::~ConcreteModelController()
		{
		}


		void ConcreteModelController::RegisterOutControls( )
		{
				RegisterAs(  &mOutCtl_1, "Control#1"  );
				RegisterAs(  &mOutCtl_2, "Control#2"  );
				RegisterAs(  &mOutCtl_3, "Control#3"  );
				RegisterAs(  &mOutCtl_4, "Control#4"  );
				RegisterAs(  &mOutCtl_5, "Control#5"  );
				RegisterAs(  &mOutCtl_6, "Control#6"  );
		}
		


}

