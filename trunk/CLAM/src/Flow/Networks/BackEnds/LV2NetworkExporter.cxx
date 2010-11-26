#include "LV2NetworkExporter.hxx"
#include "LV2Library.hxx"
#include "LV2NetworkPlayer.hxx"

LV2NetworkExporter::LV2NetworkExporter(
	LV2Library & library, 
	const std::string & networkXmlContent,
	const std::string & uri
	)
{
	LV2_Descriptor * descriptor = CLAM::LV2NetworkPlayer::CreateLV2Descriptor(networkXmlContent, uri);
	if (not descriptor) return;
	library.AddPluginType(descriptor,networkXmlContent);
}

