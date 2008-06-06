#include <iostream>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/Pool.hxx>
#include <CLAM/Array.hxx>
#include "../Schema.hxx"

void Segment2BoundMarks(CLAM::DataArray & boundMarks);

int main(int argc, char* argv[])			// access command line arguments
{
	CLAM_Annotator::Schema schema;
	CLAM::DescriptionScheme descriptionScheme;

	schema.AddSegmentation("Song","Silences", CLAM_Annotator::SegmentationPolicy::eDiscontinuous, "");
	// The Uri is used to identify the kind of document. 
	schema.SetUri("descriptionScheme:www.iua.upf.edu:clam:JunJunSilenceDetector"); 
	//The documentation is the text you can see on the schema browser in Annotator
	schema.AttributeDocumentation("Song", "Silences",
		"<p>The <em>Silences</em> is a discontinuous segmentation attribute"
		" that indicates which parts of the audio are silences</p>\n"
		"<p><b>Data for this data is not the one in Simac but randomly generated.</b></p>\n"
		);

	schema.FillDescriptionScheme(descriptionScheme); // This step is due to the scheme classes duplication 

	CLAM::XMLStorage::Dump(schema, "Schema", std::cout);

	const CLAM::DescriptionScope & descriptionScope = descriptionScheme.GetScope("Song"); //temp

	std::cout <<"Get the first attribute name:"<< descriptionScope.GetAttributeName(0) <<std::endl;  //temp
		
	 //Now we create a Pool
	CLAM::DescriptionDataPool pool(descriptionScheme);
	//Creat the descriptor pool, and populate the 'Song' with 1 element 
	pool.SetNumberOfContexts("Song", 1);

	//Silence Segmentation
	CLAM::DataArray & segmentation = pool.GetWritePool<CLAM::DataArray>("Song","Silences")[0];

	//insert the segmentation boundMarks	
	Segment2BoundMarks(segmentation);

	std::cout << "the size of segmentation="<< segmentation.Size() <<std::endl;

	CLAM::XMLStorage::Dump(pool, "DescriptorsPool", std::cout);
	return 0;
}


void Segment2BoundMarks(CLAM::DataArray & boundMarks)
{
	CLAM::TData boundArray[3]={0.1, 0.2, 0.3};
	//we temporarily set nsegments to be 3
	for (int i=0; i<3; i++)
	{
		 // insert boundMarks
		boundMarks.AddElem(boundArray[i]); 
	}
}

