
#include "Segment.hxx"
#include "XMLStorage.hxx"
#include "SegmentDescriptors.hxx"
#include "DescriptorComputation.hxx"
#include "FileChooser.hxx"

int main( int argc, char** argv )
{
	CLAM::Segment segment;

	CLAMVM::FileChooserDialog dlg;

	dlg.SetTitle( "Please select an XML analysis file" );
	dlg.AddFileType( "*.xml" );

	if ( !dlg.Show() )
	{
		std::cerr << "No analuysis file was selected!" << std::endl;
		exit(0);
	}


	std::cout<<"Loading Analysis File. Please Wait."<<"\n";
	CLAM::XMLStorage::Restore(segment, dlg.GetSelectedFilename() );
	std::cout<<"Analysis File Loaded Successfully"<<"\n";

	std::cout<<"Computing Descriptors. Please Wait."<<"\n";

	CLAM::SegmentDescriptors segmentDescriptors;
	segmentDescriptors.AddAll();
	segmentDescriptors.UpdateData();

	CLAM::SpectralDescriptors specProto;
	specProto.AddAll();
	specProto.UpdateData();

	CLAM::SpectralPeakDescriptors peakProto;
	peakProto.AddAll();
	peakProto.UpdateData();

	CLAM::FrameDescriptors frameProto;
	frameProto.AddAll();
	frameProto.UpdateData();
		
	frameProto.GetSpectralPeakD().SetPrototype(peakProto);
	frameProto.GetResidualSpecD().SetPrototype(specProto);
		
	segmentDescriptors.SetFramePrototype(frameProto,segment.GetnFrames());
	segmentDescriptors.SetpSegment(&segment);
		
	//we can directly call the compute operation on the descriptor
	//segmentDescriptors.Compute();
		
	//or use an intermediate DescriptorComputation processing
	CLAM::DescriptorComputation processing;
	processing.Do(segmentDescriptors);
		
	std::cout<<"Descriptors Computed Successfully"<<"\n";
		
	dlg.SetTitle( "Please enter name of where you want your output descriptors to be stored" );
	std::string outFilename;
	
	if ( !dlg.Show() )
	{
		std::cerr << "No file was specified defaulting to: 'Descriptors.xml'" << std::endl;
		outFilename = "Descriptors.xml";
	}
	else
		outFilename = dlg.GetSelectedFilename();
	

	std::cout<<"Storing Results into xml file. Please Wait."<<"\n";
	CLAM::XMLStorage::Dump(segmentDescriptors,"segmentDescriptors",outFilename);

	std::cout<<"Program finished Successfully"<<"\n";
	return 0;
}


