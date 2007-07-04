
#include <CLAM/AudioFileIn.hxx>
#include <CLAM/SDIFOut.hxx>
#include <CLAM/SDIFIn.hxx>
#include <CLAM/Segment.hxx>

namespace CLAM
{
/**********
 *
 * Load a Wav file into a segment.
 *
 ***/
	TSize WAVELoad(const std::string& filename, Segment& segment);

/**********
 *
 * Store a segment into a SDIF file.
 *
 ***/
	bool SDIFStore(const std::string&  fileName, Segment& segment);

	bool SDIFLoad(const std::string& filename, Segment& segment);

}

