#ifndef __CMTGMIDITarget__
#define __CMTGMIDITarget__

#include "MTGDataTypes.hxx"

/**
* This is an abstract class that is used in combination with CMTGMIDIIn,
* and is used to implement a target for incoming midi packages.
* @see CMTGMIDIIn
*/
class CMTGMIDITarget
{
public:
	/**
	* Virtual callback function that is called for each midi package that is to 
	* be handled. Derived classes should implement this function.
	* @param timetag Starttime of the package
	* @param bytes Data
	* @param size Size in bytes of the data
	*/
	virtual void Handle(TMIDITime timetag, const TMIDIByte bytes[], TInt32 size);
};

#endif
