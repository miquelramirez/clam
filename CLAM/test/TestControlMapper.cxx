#include "ControlMapper.hxx"

using namespace CLAM;
	
int main(void)
{
	ControlMapperConfig mapperCfg;
	
	mapperCfg.SetMapping("linear");
	TData ptr[]={0.0, 127.0, 0.0, 1.0};
	mapperCfg.SetArguments(DataArray(ptr,4));
	
	ControlMapper mapper(mapperCfg);
	
	return 0;
}
