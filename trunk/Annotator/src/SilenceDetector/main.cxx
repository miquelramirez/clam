#include <iostream>
#include <CLAM/XMLStorage.hxx>
#include "../Schema.hxx"


int main(int argc, char* argv[])			// access command line arguments
{

    CLAM_Annotator::Schema schema;
    
    schema.AddSegmentation("Song","Silences", CLAM_Annotator::SegmentationPolicy::eDiscontinuous, "");

	schema.SetUri("descriptionScheme:www.iua.upf.edu:clam:JunJunSilenceDetector"); // the Uri is used to identify the kind of document.

	//The documentation is the text you can see on the schema browser in Annotator
 	schema.AttributeDocumentation("Song", "Silences",
		"<p>The <em>Silences</em> is a discontinuous segmentation attribute"
		" that indicates which part of the audio is the silence</p>\n"
		"<p><b>Data for this data is not the one in Simac but randomly generated.</b></p>\n"
		);

	CLAM::XMLStorage::Dump(schema, "Schema", std::cout);

	return 0;
}
