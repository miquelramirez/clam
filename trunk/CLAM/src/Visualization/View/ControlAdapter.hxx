#ifndef __CONTROLADAPTER__
#define __CONTROLADAPTER__

#include "InControl.hxx"
#include "OutControl.hxx"
#include "ModelController.hxx"
#include "InputControlModel.hxx"

namespace CLAMVM
{
		using CLAM::InControl;
		using CLAM::OutControl;

		class ControlAdapter
				: public  ModelController, public  InputControlModel
		{
		private:

				struct tManagedEmitter
				{

						OutControl              mControl;
						CLAM::TControlData            mValueToEmit;
						bool                    mIsDirty;

						tManagedEmitter();

						void Emit(  );

				};

				tManagedEmitter         mEmitter;
				bool                    mIsBound;
				bool                    mValueRangeChanged;
				CLAM::TControlData            mMinValue;
				CLAM::TControlData            mMaxValue;
		protected:

				void DispatchControlValue( CLAM::TControlData value );

				void DeleteSubAdapters();

		public:
				ControlAdapter();

				virtual ~ControlAdapter();

				virtual const char* GetClassName() const
				{
						return "ControlAdapter";
				}

				virtual bool Publish();

				virtual bool Update();

				void SetValueRange( CLAM::TControlData min, CLAM::TControlData max )
				{
						mMinValue = min;
						mMaxValue = max;
						mValueRangeChanged = true;
				}

				virtual bool BindTo( InControl&  adaptee );

				bool IsBound() const
				{
						return mIsBound;
				}


		};

}

#endif // ControlAdapter.hxx
