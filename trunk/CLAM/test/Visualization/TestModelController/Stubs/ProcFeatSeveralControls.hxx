#ifndef __PROCFEATSEVERALCONTROLS__
#define __PROCFEATSEVERALCONTROLS__

#include <Processing.hxx>

namespace CLAM
{
namespace Test
{
		using CLAM::Processing;
		using CLAM::ProcessingConfig;
		using CLAM::TControlData;

		class ProcFeatSeveralControlsConfig : public ProcessingConfig
		{
				DYNAMIC_TYPE_USING_INTERFACE( ProcFeatSeveralControlsConfig, 1, ProcessingConfig );
				DYN_ATTRIBUTE( 0, public, std::string, Name );

		protected:

				void DefaultInit();
		};

		class ProcFeatSeveralControls : public Processing
		{
		public:
				ProcFeatSeveralControls();				
				ProcFeatSeveralControls( const ProcFeatSeveralControlsConfig& cfg );

				~ProcFeatSeveralControls();

				virtual bool Do();

				inline const ProcessingConfig& GetConfig() const
				{
						return mConfig;
				}

				virtual const char* GetClassName() const
				{
						return "CLAMDraft::ProcFeatSeveralControls";
				}				

				int GetControlTimesCalled( int id );

				float GetControlLastValue( int id );

		protected:

				bool ConcreteConfigure( const ProcessingConfig& cfg  );
				bool ConcreteStart();
				bool ConcreteStop();								

				int  InCtl0_cb( TControlData value );
				int  InCtl1_cb( TControlData value );
				int  InCtl2_cb( TControlData value );
				int  InCtl3_cb( TControlData value );
				int  InCtl4_cb( TControlData value );
				int  InCtl5_cb( TControlData value );
				

		private:
				ProcFeatSeveralControlsConfig             mConfig;

				FloatInControl    mInCtl_0;
				FloatInControl    mInCtl_1;
				FloatInControl    mInCtl_2;
				FloatInControl    mInCtl_3;
				FloatInControl    mInCtl_4;
				FloatInControl    mInCtl_5;

				int   mTimesCalled0;
				int   mTimesCalled1;
				int   mTimesCalled2;
				int   mTimesCalled3;
				int   mTimesCalled4;
				int   mTimesCalled5;
		};

}
}

#endif // ProcFeatSeveralControls.hxx

