#include "MiniCppUnit.hxx"

int main()
{
	if (FactoryDeTests::laInstancia().testejaTots() )
		return 0;
	else 
		return -1;
}	
