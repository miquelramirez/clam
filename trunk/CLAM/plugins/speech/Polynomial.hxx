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

/*
  \class Polynomial
  \brief Represents a polynomial as a vector of coefficients and provides a method to solve its roots
*/



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
		CLAM::Array<Complex> PolyRoots(DataArray coefficients); //TODO: Move to it's own processing
		void  BuildCompanion(DataArray coefficients);
		CLAM::Array <Complex> EigenHessenberg();
    
	protected:
		void DefaultInit();
	private:
		std::vector <std::vector <double> > M;
    
		static const double epsilon = DBL_EPSILON;
	};
}


#endif // Polynomial.hxx
