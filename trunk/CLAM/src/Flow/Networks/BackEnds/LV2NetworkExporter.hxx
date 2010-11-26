#ifndef LV2NetworkExporter_hxx
#define LV2NetworkExporter_hxx

#include <string>

class LV2Library;
class LV2NetworkExporter
{
public:
	LV2NetworkExporter(LV2Library & library, 
		const std::string & networkXmlContent,
		const std::string & uri
		);
};


#endif//LV2NetworkExporter_hxx

