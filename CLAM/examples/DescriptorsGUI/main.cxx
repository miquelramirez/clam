/****** MELODIA: Melody  display utility *****/
/****** @author=emilia.gomez@iua.upf.es  *****/

#include "Melodia.hxx"
#include "Err.hxx"

using namespace CLAM;

int main(int argc, char **argv) {
	try
	{
		MelodiaParam settings;
		Melodia display(&settings);
	} 
	catch(Err error)
	{
		std::cerr << "It has not been possible to finish the description" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	
	std::clog << "You are leaving Melodi@" << std::endl;
	return 0;
}

