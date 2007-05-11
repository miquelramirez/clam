/*
 * FundPlayer.hxx
 * Interface for FundPlayer class
 * Used to play the fundamental frequency extracted through the analysis.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _FUNDPLAYER_
#define _FUNDPLAYER_

#include <pthread.h>
#undef GetClassName
#include <CLAM/Array.hxx>
#include <CLAM/Segment.hxx>
#include <CLAM/Signalv0.hxx>
#include <CLAM/Slotv0.hxx>
#include <CLAM/Signalv1.hxx>
#include <CLAM/Slotv1.hxx>

using CLAM::DataArray;
using CLAM::Segment;
using SigSlot::Signalv0;
using SigSlot::Slotv0;
using SigSlot::Signalv1;
using SigSlot::Slotv1;

namespace V2M
{

class FundPlayer
{
public:
    FundPlayer(const Segment& originalSegment,
	       Slotv0& slot0,
	       Slotv1<DataArray>& slot1);

    ~FundPlayer();

    void Stop();

private:
    pthread_t mThread;
    Segment mSegment;

    Signalv0 mRequestStop;
    Signalv1<DataArray> mSendData;

    bool active;

    void PlayFundThreadSafe();
    static void* sPlayFundThreadSafe(void* thisObject);

};

} // namespace V2M

#endif


