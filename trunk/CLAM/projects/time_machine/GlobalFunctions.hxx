/**--------------------------------------------------------------------------

	Modulename: Global Template Functions

	----

	Filename:	  GlobalFunctions.cpp

	Author:     Lars Fabig

	Date:       25.10.2001

	Compiler:	  Microsoft Visual C++ 6.0

	Revision:   0.1

  Test:       

	---------------------------------
	Description:  
    
      

	---------------------------------
	Classes:

	---------------------------------
  Config Classes:

	---------------------------------
  Inherited Classes:

	---------------------------------
	Functions:

      T   max(T tX1, T tX2) : returns the maximum of tX1 and tX2
        
      T   min(T tX1, T tX2) : return the minimum of tX1 and tX2

	---------------------------------
	Comments:

	---------------------------------
  Test Results:

	---------------------------------
	To Do:

---------------------------------------------------------------------------*/

#ifndef _Max_
template <class T>
T Max(T tX1, T tX2)
{
  T tX = tX1;
  if (tX < tX2)
    tX = tX2;
  return tX;
}
#define _Max_
#endif

#ifndef _Min_
template <class T>
T Min(T tX1, T tX2)
{
  T tX = tX1;
  if (tX > tX2)
    tX = tX2;
  return tX;
}
#define _Min_
#endif

#ifndef _dabs_
inline double dabs(double value) { if (value >= 0) return value; else return -value; };
#define _dabs_
#endif


/*
double dabs(double value) 
{ 
  if (value >= 0) 
    return value; 
  else return -value; 
};*/

