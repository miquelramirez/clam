#ifndef __SDIFStorage__
#define __SDIFStorage__

#include "SDIFFrame.hxx"

namespace SDIF
{

	/** Storage is an abstract class, that is passed to File,
	* used to handle every read frame.
	* 
	*       The most common construction is to use a Collection that is derived
	*       from Storage, and pass it to File::Read() but it is possible
	*       to implement other ways of handling read frames.
	* @see Collection
	*/
	class Storage
	{
	public:
		typedef std::list<Frame*>::const_iterator FrameIterator;

		/** virtual function that where derived classes get the frames read by the
		* File. Note that the derived class is responsible for handling
		* the memory deallocation of the frames it gets passed.
		*/
		virtual void Add(Frame* pFrame) = 0;
		virtual FrameIterator Begin(void) const = 0;
		virtual FrameIterator End(void) const = 0;
	};

}
#endif

