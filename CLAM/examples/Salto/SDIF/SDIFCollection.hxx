#ifndef __SDIFCollection__
#define __SDIFCollection__

#include "DataTypes.hxx"

class SDIFFrame;
class SDIFStream;

#include "SDIFStorage.hxx"

/** A special SDIFStorage to store SDIFFrames in
*	a linked list. 
*
* The SDIFCollection has the possibility to create the 
*	SDIFStreams to which the frames belong, either on the fly, or on command
*
*	Typically, SDIFCollection is passed to SDIFFile::Read, as the place
*	the store the read SDIFFrames. @see SDIFFile::Read
*/
class SDIFCollection:public SDIFStorage
{
public:
	//* The list of SDIFFrames
	SDIFFrame* mpFirst;
	SDIFFrame* mpLast;

	//* The list of SDIFStreams
	SDIFStream* mpFirstStream;
	SDIFStream* mpLastStream;

	//* Whether or not to create the SDIFStreams on the fly
	bool mAutoStreamAdding;
public:
	//* Create a new SDIFCollection.
	SDIFCollection(bool autoStreamAdding = true);

	virtual void Add(SDIFFrame* pFrame);
	void Add(SDIFStream* pStream);
	SDIFStream* FindStream(TInt32 streamId);

	void ParseStreams(void);
};

#endif

