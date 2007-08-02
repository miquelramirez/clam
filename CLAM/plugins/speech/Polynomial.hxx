/*   Clam Copyright (C)  */

#ifndef __POLYNOMIAL__
#define __POLYNOMIAL__

#include <CLAM/ProcessingData.hxx>
#include <CLAM/Array.hxx>
#include <CLAM/CLAM_Math.hxx> 
#include <CLAM/Spectrum.hxx>  
#include <CLAM/Matrix.hxx>    
#include <CLAM/MatrixTmpl.hxx>
#include <vector>        
#include <float.h>

namespace CLAM
{
	/*just an array of coefficients and a method to solve them*/
	class Polynomial 
		: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( Polynomial, 0, ProcessingData );  //I'm not sure if this is needed
		//DYN_ATTRIBUTE( 0, public, DataArray,   Coefficients );
    
	public:
		CLAM::Array<Complex> PolyRoots(DataArray coefficients); //Abe 29-6-2007: Move to it's own processing
		void  BuildCompanion(DataArray coefficients);  //Abe 24-6-2007
		CLAM::Array <Complex> EigenHessenberg(); //Abe 30-6-2007
    
	protected:
		void DefaultInit();
	private:
		std::vector <std::vector <double> > M;
    
		static const double epsilon = DBL_EPSILON;//epsilon();
	};
}


#endif // Polynomial.hxx
