/*
 * AudioFileIO.hxx
 * Interface for AudioFileIO class.
 *
 * Used to load and store audio files.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
 */
#ifndef _AUDIOFILEIO_
#define _AUDIOFILEIO_

#include <CLAM/Audio.hxx>

namespace V2M
{

class AudioFileIO
{
public:
    AudioFileIO();
    ~AudioFileIO();

    /*
     * Load audio file method
     * param1: file name
     * param2: audio object to hold the data
     */
    int Load(const char* fileName,CLAM::Audio& out);

    /*
     * Save audio file method
     * param1: file name
     * param2: audio object with the data that will be saved
     */
    int Save(const char* fileName,CLAM::Audio& in);

};

} // namespace V2M

#endif


