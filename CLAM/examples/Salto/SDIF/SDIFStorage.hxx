#ifndef __SDIFStorage__
#define __SDIFStorage__

#include "SDIFFrame.hxx"

/** SDIFStorage is an abstract class, that is passed to SDIFFile,
* used to handle every read frame.
* 
*       The most common construction is to use a SDIFCollection that is derived
*       from SDIFStorage, and pass it to SDIFFile::Read() but it is possible
*       to implement other ways of handling read frames.
* @see SDIFCollection
*/
class SDIFStorage
{
public:	
	/** virtual function that where derived classes get the frames read by the
	* SDIFFile. Note that the derived class is responsible for handling
	* the memory deallocation of the frames it gets passed.
	*/
	virtual void Add(SDIFFrame* pFrame) { delete pFrame; }
};

#endif
