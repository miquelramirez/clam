#ifndef __SDIFCollectionDisplay__
#define __SDIFCollectionDisplay__

#include "SDIFCollection.hxx"

class SDIFCollectionDisplay:public SDIFCollection
{
public:
	SDIFCollectionDisplay(bool autoStreamAdding = true)
	:SDIFCollection(autoStreamAdding)
	{
	}
	void SDIFCollectionDisplay::Display(void);
	void SDIFCollectionDisplay::DisplayFrame(const SDIFFrame* pFrame);
};

#endif
