#include "Factory.hxx"
#include "SMSAnalysisCore.hxx"
#include "SMSSynthesis.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

static ProcessingFactory::Registrator<CLAM::SMSAnalysisCore> regtSMSAnalysisCore( "SMSAnalysisCore" );
static ProcessingFactory::Registrator<CLAM::SMSSynthesis> regtSMSSynthesis( "SMSSynthesis" );
