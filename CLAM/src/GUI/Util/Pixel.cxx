#include "Pixel.hxx"
#include <iostream>

namespace CLAMGUI
{
	std::istream& operator >> (std::istream& myStream, const Pixel& p)
	{

		return myStream;
	}

	std::ostream& operator << (std::ostream& myStream, const Pixel& p)
	{

		// Deberia traducir los valores de las componentes RGB en hexadecimal: color={FF0AC1} o algo por el estilo...
		myStream << "color=";
		myStream << "{";
		//	myStream << "FFFFFF";
		myStream << "}";
		
		return myStream;
	}
	


}
