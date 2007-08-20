#include "SampleMetadata.hxx"
#include <sstream>

namespace CLAM
{

void SampleMetadata::DefaultInit()
{
	AddAll();
	UpdateData();

	// any default values will be destroyed if you restore the objects from XML
	SetFilename( "nofile" );
	SetSortIndex(SAMPLE_METADATA_PITCH_INDEX);
}

void SampleMetadata::ConfigureAfterRestore()
{
	AddSortIndex();
	UpdateData();
}

bool SampleMetadata::operator<(const SampleMetadata& argSampleMetadata) const
{

	if (GetSortIndex() == SAMPLE_METADATA_PITCH_INDEX)
	{
		return GetPitch() < argSampleMetadata.GetPitch();
	}
	else if (GetSortIndex() == SAMPLE_METADATA_AMPLITUDE_INDEX)
	{
		return GetAmplitude() > argSampleMetadata.GetAmplitude();
	}
	else
	{
		std::ostringstream outStream;
		outStream << std::string("SampleMetadata: unsupported argument for GetSortIndex(): ") << GetSortIndex();
		//CLAM_DEBUG_ASSERT( false, outStream.str() );
		return false;
	}
}

bool SampleMetadata::operator>(const SampleMetadata& argSampleMetadata) const
{
	if (GetSortIndex() == SAMPLE_METADATA_PITCH_INDEX)
	{
		return GetPitch() > argSampleMetadata.GetPitch();
	}
	else if (GetSortIndex() == SAMPLE_METADATA_AMPLITUDE_INDEX)
	{
		return GetAmplitude() > argSampleMetadata.GetAmplitude();
	}
	else
	{
		std::ostringstream outStream;
		outStream << std::string("SampleMetadata: unsupported argument for GetSortIndex(): ") << GetSortIndex();
		//CLAM_DEBUG_ASSERT( false, outStream.str() );
		return false;
	}
}

bool SampleMetadata::operator==(const SampleMetadata& argSampleMetadata) const
{
	if (GetSortIndex() == SAMPLE_METADATA_PITCH_INDEX)
	{
		return GetPitch() == argSampleMetadata.GetPitch();
	}
	else if (GetSortIndex() == SAMPLE_METADATA_AMPLITUDE_INDEX)
	{
		return GetAmplitude() == argSampleMetadata.GetAmplitude();
	}
	else
	{
		std::ostringstream outStream;
		outStream << std::string("SampleMetadata: unsupported argument for GetSortIndex(): ") << GetSortIndex();
		//CLAM_DEBUG_ASSERT( false, outStream.str() );
		return false;
	}
}

}


