#ifndef __PROCFEATSEVERALCONTROLS__
#define __PROCFEATSEVERALCONTROLS__

#include <Processing.hxx>

namespace CLAMDraft
{
		using CLAM::Processing;
		using CLAM::ProcessingConfig;
		using CLAM::InControlTmpl;
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

				InControlTmpl<ProcFeatSeveralControls>    mInCtl_0;
				InControlTmpl<ProcFeatSeveralControls>    mInCtl_1;
				InControlTmpl<ProcFeatSeveralControls>    mInCtl_2;
				InControlTmpl<ProcFeatSeveralControls>    mInCtl_3;
				InControlTmpl<ProcFeatSeveralControls>    mInCtl_4;
				InControlTmpl<ProcFeatSeveralControls>    mInCtl_5;

				int   mTimesCalled0;
				int   mTimesCalled1;
				int   mTimesCalled2;
				int   mTimesCalled3;
				int   mTimesCalled4;
				int   mTimesCalled5;
		};

}

#endif // ProcFeatSeveralControls.hxx

