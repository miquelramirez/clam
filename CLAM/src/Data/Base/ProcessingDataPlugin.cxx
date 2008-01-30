#include "ProcessingDataPlugin.hxx"
#include "DataTypes.hxx"
#include <vector>

namespace CLAM
{
namespace Hidden 
{
static ProcessingDataPlugin::Registrator<CLAM::TData> dataRegistratorTData("lightblue", "CLAM::Audio (Stream)");
static ProcessingDataPlugin::Registrator<std::vector<CLAM::TData> > dataRegistratorFloatArray("silver", "Floats vector");
static ProcessingDataPlugin::Registrator<std::vector<std::pair<CLAM::TData,CLAM::TData> > > dataRegistratorPeakArray("thistle", "Float pairs vector");
}

ProcessingDataPlugin::TypeMap & ProcessingDataPlugin::getTypeMap()
{
	static TypeMap map;
	return map;
}

}


