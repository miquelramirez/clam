/*Storing Objects for a single Sinusoidal Track and an array of Sinusoidal Tracks.

  A SinTrack (Sinusoidal Track) is a data storage class that only holds a 
  pointer to the first peak array (so called peak spectral frame) and to the last. 
  It also holds an identifyier. Basic functionality and operators are also included.

  A SinTrackArray (Sinusoidal Track Array) basically holds a Array of
  SinTracks. It also includes an array of active tracks (those that have not
  been assigned a final peak array and some basic functionality.

  See comments on SinTracking for more details on interaction between processing 
  object and these data containers.
*/

#ifndef _SinTracks_
#define _SinTracks_
#include "Array.hxx"
#include "SpectralPeak.hxx"
#include "SpectralPeakArray.hxx"

using namespace CLAM;

class SinTrackArray;
class SinTrack;

#include "SpectralSegment.hxx"

class SpectralSegment;

class SinTrack
{
/*Friend classes*/
  friend class SinTrackArray;/*related processing object is allowed to access
  all members*/

public:

/*Constructors*/
  SinTrack(TIndex trackId,int firstFrameIndex);
  SinTrack(TIndex trackId);
  SinTrack();

/*Destructors*/
  ~SinTrack();

/*Member Functions*/
  void 		GetFirstPeak(SpectralSegment& segment,SpectralPeak& peak) const;
  void 		GetLastPeak(SpectralSegment& segment,SpectralPeak& peak) const;
  TIndex	GetTrackId()											{return mTrackId;}
  void 		SetTrackId(TIndex trackId)				{mTrackId=trackId;}
  
   

/*Operators*/
  bool operator ==(const SinTrack& newTrack) {return (newTrack.mTrackId==mTrackId);}
  bool operator !=(const SinTrack& newTrack) {return (newTrack.mTrackId!=mTrackId);}
  bool operator <(const SinTrack& newTrack) {return (newTrack.mTrackId<mTrackId);}
  bool operator <=(const SinTrack& newTrack) {return (newTrack.mTrackId<=mTrackId);}
  bool operator >(const SinTrack& newTrack) {return (newTrack.mTrackId>mTrackId);}
  bool operator >=(const SinTrack& newTrack) {return (newTrack.mTrackId>=mTrackId);}

private:
  
/*Member variables*/
  int mFirstFrameIndex;//pointer to first 'frame' in track
  int mLastFrameIndex;//pointer to last 'frame' in track
  int mTrackId;//track id
};


class SinTrackArray
{
public:

/*Constructor*/
  SinTrackArray(int nMaxSines=100);

/*Destructor*/
  ~SinTrackArray();

/* Get/Set */
	TIndex				GetNumActiveTracks()										{return	mpActiveTrackArray.Size();}
	TIndex				GetNumTracks()													{return mnTracks;}
	TIndex				GetActiveTrackIdFromPos(TIndex	pos)		{return mpActiveTrackArray[pos]->GetTrackId();}

	void					GetSinTrack(TIndex pos,SinTrack*& );			


/*member functions*/
  void KillTrack(TIndex trackId,int frameIndex);
  void AddNewTrack(TIndex trackId,int firstFrameIndex);
  
  TIndex FindActiveTrack(TIndex TrackId); // moved this to public JO
    
private:

/*private functions*/


  
/*member variables*/
  Array<SinTrack*> mpTrackArray;//Track array
  Array<SinTrack*> mpActiveTrackArray;/*Tracks that still have no end */
  TSize mnTracks;//Total amount of current tracks
  TSize mnMaxSines;//Maximum number of simultaneous tracks
};


inline EDataFormat DataFormat(SinTrack&){return eFmtDefault;}
inline EDataFormat DataFormat(SinTrack* const){return eFmtDefault;}

#endif
