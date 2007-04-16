#include "ControlMapper.hxx"
#include "Err.hxx"
#include <iostream>

using namespace CLAM;
	
int main(void)
{
	try
	{

		ControlMapperConfig mapperCfg;
		
		mapperCfg.SetMapping("linear");
		TData ptr[]={0.0, 127.0, 0.0, 1.0};
		mapperCfg.SetArguments(DataArray(ptr,4));
		
		ControlMapper mapper(mapperCfg);
	}
	catch ( Err& e )
	{
		std::cout << "Test Failed." << std::endl;
		std::cout << "CLAM recognized error: " << std::endl;
		e.Print();
		return -1;
	}
	catch ( std::exception& e )
	{
		std::cout << "Test Failed." << std::endl;
		std::cout << "STL recognized error: " << std::endl;
		e.what();
		return -1;
	}
	catch( ... )
	{
		std::cout << "Test Failed." << std::endl;
		std::cout << "Unknown error" << std::endl;
		return -1;
	}

		std::cout << "Test Passed." << std::endl;
	return 0;
}

