#include <iostream>
#include <CLAM/XMLStorage.hxx>
#include "../Schema.hxx"


int main(int argc, char* argv[])			// access command line arguments
{

    CLAM_Annotator::Schema schema;
    
    schema.AddSegmentation("Song","Silences", CLAM_Annotator::SegmentationPolicy::eDiscontinuous, "");

	CLAM::XMLStorage::Dump(schema, "Schema", std::cout);

	return 0;
}
