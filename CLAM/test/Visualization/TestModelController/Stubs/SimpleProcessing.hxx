#ifndef __SIMPLEPROCESSING__
#define __SIMPLEPROCESSING__

#include <Processing.hxx>

namespace CLAM
{
namespace Test
{

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

				FloatInControl& GetControl()
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
				FloatInControl   mInControl;
				
		};


}
}

#endif // SimpleProcessing.hxx

