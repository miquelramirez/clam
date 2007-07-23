#include "ProcessingDataPlugin.hxx"
#include "DataTypes.hxx"
#include <vector>

namespace CLAM
{
namespace Hidden 
{
static ProcessingDataPlugin::Registrator<CLAM::TData> dataRegistratorTData("lightblue");
static ProcessingDataPlugin::Registrator<std::vector<CLAM::TData> > dataRegistratorFloatArray("silver");
static ProcessingDataPlugin::Registrator<std::vector<std::pair<CLAM::TData,CLAM::TData> > > dataRegistratorPeakArray("thistle");
}

ProcessingDataPlugin::TypeMap & ProcessingDataPlugin::getTypeMap()
{
	static TypeMap map;
	return map;
}

}


