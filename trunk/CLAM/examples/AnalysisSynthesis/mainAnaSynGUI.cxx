#include "AnalysisSynthesisExampleGUI.hxx"
/******* TRANSFORMATION *******/
#include "SMSFreqShift.hxx"
#include "Err.hxx"
#include <exception>
#include <iostream>
#include <cstdlib>

using CLAM::SMSFreqShift;
using CLAM::Err;

using CLAMGUI::AnalysisSynthesisExampleGUI;

int main(void)
{
	try
		{
			AnalysisSynthesisExampleGUI example;
			example.Run();
		}
	catch(Err& error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
	}
	catch (std::bad_cast& e)
	{
		std::cout << e.what() << std::endl; 
	}
	catch (std::exception& e)
	{
		std::cout<<typeid(e).name();
		std::cout << e.what() << std::endl; 
	}
	catch ( ... )
		{
			std::cerr << "If you are under windows this can be a major memory corruption... DEBUG!" << std::endl;
			abort();
		}
	
	std::clog << "Finished successfully!"<<std::endl;
	return 0;
}
