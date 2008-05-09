/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

///////////////////////////////////////////////////////////////////////////////
// Class SpectralPeak: Processing Data class to store spectral peak data
///////////////////////////////////////////////////////////////////////////////
// Description: 
//  This class holds a basic data structure used for spectral processing: the
//  the Spectral Peak. A Spectral Peak holds information about a particular
//  spectral sample extracted from a Peak Detection Algorithm. Some information
//  such as Frequency and Amplitude is mandatory, other as Phase and BinWidth
//  may be needed for some particular applications and BinPos may be redundant
//  with Frequency but is kept for backwards compatibility.
///////////////////////////////////////////////////////////////////////////////


#ifndef _SpectralPeak_
#define _SpectralPeak_

#include <typeinfo>

#include "DynamicType.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"
#include "GlobalEnums.hxx"
#include "CLAM_Math.hxx"

namespace CLAM{



/**
* Class SpectralPeak is a basic spectral domain Processing Data class. 
* Contains frequency, magnitude, phase, bin position and bin width of a given
* spectral peak. An array of Spectral Peaks is created by means of the 
* SpectralPeakArray class
* @see ProcessingData
* @see SpectralPeakArray
* @author Xavier Amatriain
*/
class SpectralPeak: public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SpectralPeak, 6, ProcessingData);
	DYN_ATTRIBUTE (0, public, EScale, Scale);
	DYN_ATTRIBUTE (1, public, TData, Freq);
	DYN_ATTRIBUTE (2, public, TData, Mag);
	DYN_ATTRIBUTE (3, public, TData, BinPos);
	DYN_ATTRIBUTE (4, public, TData, Phase);
	DYN_ATTRIBUTE (5, public, int, BinWidth);


protected:
/** Configuration constructor
*/
	void DefaultInit();

public:
//operators

/** Operator for computing distance between spectral peaks only taking into
* account their frequency
* @return the frequency distance in Hz (TData)
*/
	TData operator|(const SpectralPeak& a) const
	{
		return (Abs(GetFreq()-a.GetFreq()));
	}
/** Operator for multiplying a spectral peak by a given factor, 
 * just influences
* its magnitude
* TODO: does not take into account if the scale is logarithmic or not, should it?
* @param peak A reference to the peak
* @param factor the multiplying factor
*/
	friend  SpectralPeak operator * (const SpectralPeak& peak,int factor)
	{
		SpectralPeak ret;
		ret.SetFreq(peak.GetFreq());
		ret.SetMag(peak.GetMag()*factor);
		ret.SetPhase(peak.GetPhase());
		ret.SetBinPos(peak.GetBinPos());
		ret.SetBinWidth(peak.GetBinWidth());
		ret.SetScale(peak.GetScale());
		return ret;
	}


};

/** Function for changing the scale of a peak to linear, 
* conserving data consistency
* @param inPeak  incoming peak
* @param scalingFactor factor to apply in the logarithmic conversion 
* (default 20)
*/
inline SpectralPeak Lin(SpectralPeak &inPeak,int scalingFactor)
{
	if (inPeak.GetScale()!=EScale::eLinear){
		TData currMag = inPeak.GetMag();
		inPeak.SetMag(CLAM_pow(TData(10),TData(currMag/scalingFactor)));
		inPeak.SetScale(EScale::eLinear);
	}
	return inPeak;
}

inline SpectralPeak Lin(SpectralPeak &inPeak)
{
	if (inPeak.GetScale()!=EScale::eLinear){
		TData currMag = inPeak.GetMag();
		inPeak.SetMag(log2lin(currMag));
		inPeak.SetScale(EScale::eLinear);
	}
	return inPeak;
}


/** Function for changing the scale of a peak to log, conserving data consistency
* @param inPeak incoming peak
* @param scalingFactor factor to apply in the logarithmic 
* conversion (default 20)
*/
inline SpectralPeak DB(SpectralPeak& inPeak,int scalingFactor=20)
{
	if (inPeak.GetScale()!=EScale::eLog){
		TData currMag = inPeak.GetMag();
		inPeak.SetMag(scalingFactor*CLAM_log10(currMag));
		inPeak.SetScale(EScale::eLog);
	}
	return inPeak;
}

};//namespace

#endif

