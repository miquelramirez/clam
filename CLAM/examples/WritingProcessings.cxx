#include <iostream>
#include "Processing.hxx"

// TODOS
// Do() is void
// GetConfig not mandatory
// ConcreteConfig not mandatoru

class SimpleProcessing : public CLAM::Processing
{
	bool Do() 
	{
		return true;
	}
	const char* GetClassName() const
	{
		return "--";
	}
	const CLAM::ProcessingConfig& GetConfig() const
	{
		CLAM_ASSERT(false, "");
	}
	bool ConcreteConfigure (const CLAM::ProcessingConfig& )
	{
		return true;
	}

};

int main()
{
	SimpleProcessing simple;

	std::cout << "Done." << std::endl;
	return 0;
}

