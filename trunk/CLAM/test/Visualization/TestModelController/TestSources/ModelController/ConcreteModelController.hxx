#ifndef __CONCRETEMODELCONTROLLER__
#define __CONCRETEMODELCONTROLLER__

#include <map>
#include "ProcessingController.hxx"
#include "ProcFeatSeveralControls.hxx"

namespace CLAMVM
{
		using CLAM::Processing;
		using CLAMDraft::ProcFeatSeveralControls;

		class ConcreteModelController
				: public ProcessingController
		{

				// attributes
		private:
				ControlAdapter                  mOutCtl_1;
				ControlAdapter                  mOutCtl_2;
				ControlAdapter                  mOutCtl_3;
				ControlAdapter                  mOutCtl_4;
				ControlAdapter                  mOutCtl_5;
				ControlAdapter                  mOutCtl_6;

		public:

				ConcreteModelController();

				virtual ~ConcreteModelController();

				virtual const char* GetClassName() const
				{
						return "ConcreteModelController";
				}

		protected:

				virtual void RegisterOutControls();
		};


}

#endif // ConcreteModelController.hxx

