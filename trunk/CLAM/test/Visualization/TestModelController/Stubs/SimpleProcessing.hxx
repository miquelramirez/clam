#ifndef __SIMPLEPROCESSING__
#define __SIMPLEPROCESSING__

#include <Processing.hxx>

namespace CLAMDraft
{

		using CLAM::Processing;
		using CLAM::ProcessingConfig;
		using CLAM::InControlTmpl;
		using CLAM::TControlData;

		class SimpleProcessingConfig : public ProcessingConfig
		{
				DYNAMIC_TYPE_USING_INTERFACE( SimpleProcessingConfig, 1, ProcessingConfig );
				DYN_ATTRIBUTE( 0, public, std::string, Name  );

		protected:

				void DefaultInit();
		};

		class SimpleProcessing : public Processing
		{
		public:

				SimpleProcessing();
				SimpleProcessing( const SimpleProcessingConfig& cfg );
				
				~SimpleProcessing();

				virtual bool Do();

				inline const ProcessingConfig& GetConfig() const
				{
						return mConfig;
				}
				
				virtual const char* GetClassName() const
				{
						return "CLAMDraft::SimpleProcessing";
				}

				InControlTmpl<SimpleProcessing>& GetControl()
				{
						return mInControl;
				}

		protected:

				bool ConcreteConfigure( const ProcessingConfig& cfg );
				bool ConcreteStart();
				bool ConcreteStop();
				
				int  InControl_cb( TControlData value );

		private:

				SimpleProcessingConfig            mConfig;
				InControlTmpl<SimpleProcessing>   mInControl;
				
		};


}

#endif // SimpleProcessing.hxx

