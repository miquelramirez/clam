
#ifndef _CSaltoTimbreVektor_
#define _CSaltoTimbreVektor_

#include "DataTypes.hxx"
#include "CSaltoDefines.hxx"

namespace CLAM
{

class CSaltoTimbreVektor
{
public:
CSaltoTimbreVektor();
~CSaltoTimbreVektor();


inline TSize		GetPitch()													{return mPitch;}            // database 
inline TSize		GetAttack()													{return mAttack;}           // database
inline TSize		GetLevel()												  {return mLevel;}            // attribute
inline TSize		GetSmoothness()											{return mSmoothness;}       // morph
inline TSize		GetTransitionFrames()								{return mTransitionFrames;} // attribute
inline TSize		GetNextPitch()								      {return mNextPitch;}        // attribute

inline TSize    GetPosition()                       // implematation for now, we dont have the whole database yet

{
    return (MAX_PITCH_VALUE*mAttack)+mPitch;    
}





inline TSize    GetNextPosition()                   {return mNextPitch * MAX_ATTACKS_VALUE;}

inline void			SetPitch(TSize pitch)								{mPitch 			= pitch;}
inline void			SetAttack(TSize attack)							{mAttack 			= attack;}
inline void			SetLevel(TSize level)					      {mLevel 		= level;}
inline void			SetSmoothness(TSize smoothness)			{mSmoothness	= smoothness;}
inline void			SetTransitionFrames(TSize frames)		{mTransitionFrames	= frames;}
inline void			SetNextPitch(TSize pitch)	      	  {mNextPitch	= pitch;}




private:

	TSize mPitch;
  TSize mAttack;
	TSize mLevel;
	TSize mSmoothness;
	TSize mTransitionFrames;
	TSize mNextPitch;

};

} // end of namespace CLAM

#endif
