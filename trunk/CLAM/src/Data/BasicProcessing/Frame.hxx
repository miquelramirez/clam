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

#ifndef _Frame_
#define _Frame_

#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Array.hxx"
#include "IndexArray.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"
#include "Fundamental.hxx"
#include "SpectralPeakArray.hxx"
#include "Audio.hxx"

namespace CLAM {


/** Processing Data class to encapsulate a frame and all its related information. A Frame has an associated
 *	time tag that points to the center of the audio chunk being used in the analysis and a duration.
 *	It also holds audio data in 4 attributes: AudioFrame is the input audio used for analysis;
 *	SynthAudioFrame is the synthesized audio after analysis and possible transformation; 
 *	SinusoidalAudioFrame is the synthesized only sinusoidal component; ResidualAudioFrame is
 *	the synthesized only resiual component.
 *	A Frame also has three associated spectrums: Spectrum is the result of the STFT applied
 *	to the input audio; SinusoidalSpec is the spectrum of the sinusoidal component; ResidualSpec
 *	is the spectrum of the residual.
 *	Finally, a frame has SpectralPeakArray that holds the output of the Spectral Peak Detection
 *	algorithm and possibly a peak continuation.The Fundamental attribute holds the data of the
 *	output of the Fundamental detection algorithm.
 *	Note: There are some Getters and Setters, but for efficiency you need to work directly on the dynamic attributes
 *	@see Spectrum, SpectralPeakArray, Fundamental, Audio
 */
class Frame : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Frame, 14, ProcessingData);
	/** Value of time in the center of the frame in miliseconds */
	DYN_ATTRIBUTE (0, public, TData, CenterTime);
	/** Duration of the Frame in miliseconds */
	DYN_ATTRIBUTE (1, public, TData, Duration);
	/** Spectrum analyzed from the Audio input  (in SMS with Residual Window)*/
	DYN_ATTRIBUTE (2, public, Spectrum, Spectrum);
	/** Spectrum analyzed from the Audio input  (in SMS with Sinusoidal Window)*/
	DYN_ATTRIBUTE (3, public, Spectrum, SinusoidalAnalSpectrum);
	/** Vector of peaks in spectral analysis  */
	DYN_ATTRIBUTE (4, public, SpectralPeakArray, SpectralPeakArray);
	/** Fundamental frequency of the information being analyzed  */
	DYN_ATTRIBUTE (5, public, Fundamental, Fundamental);
	/** Output global spectrum*/
	DYN_ATTRIBUTE (6, public, Spectrum, OutSpec);
	/** Residual spectrum of the Audio  */
	DYN_ATTRIBUTE (7, public, Spectrum, ResidualSpec);
	/** Sinusoidal spectrum of the Audio  */
	DYN_ATTRIBUTE (8, public, Spectrum, SinusoidalSpec);
	/*Audio chunk that has been used for generating spectral data, will usually be
	a previously windowed audio chunk*/
	/** Audio chunk we want to analyze */
	DYN_ATTRIBUTE (9, public, Audio, AudioFrame);
	/** Sinusoidal synthesized Audio */
	DYN_ATTRIBUTE (10, public, Audio, SinusoidalAudioFrame);
	/** Residual synthesized Audio */
	DYN_ATTRIBUTE (11, public, Audio, ResidualAudioFrame);
	/** Global synthesized Audio */
	DYN_ATTRIBUTE (12, public, Audio, SynthAudioFrame);
	/** Is a harmonic frame or not */
	DYN_ATTRIBUTE (13, public, bool, IsHarmonic);

protected:
	void DefaultInit();

public:
	//Accessory getters and setters. Note: for efficiency you need to work directly on the dynamic attributes

	//For Spectrum
	/** Returns the Magnitude of a given frequency
	 *  @param freq The Frequency we want to know the Magnitude
	 *  @return The Magnitude of spectrum in the frequency indicated
	 */
	TData GetMag(TData freq) const;

	/** Returns the Magnitude given a position in the Spectrum array
	 *  @param pos The position we want to know the Magnitude
	 *  @return The Magnitude of spectrum in the index indicated
	 */
	TData GetMag(TIndex pos) const ;

	/** Returns the Phase of a given frequency
	 *  @param freq The Frequency we want to know the Phase
	 *  @return The Phase of spectrum in the frequency indicated
	 */
	TData GetPhase(TData freq) const ;

	/** Returns the Phase of the spectrum related to a position in the Spectrum array
	 *  @param pos The position we want to know the Phase
	 *  @return The Phase of spectrum in the position indicated
	 */
	TData GetPhase(TIndex pos) const ;



	/** Replaces the Magnitude of the specified Frequency in the Spectrum with the value indicated
	 *   @param freq The Frequency which we want to change Magnitude
	 *   @param newMag The new value of Magnitude
	 */
	void SetMag(TData freq, TData newMag);

	/** Replaces the Magnitude of the specified position in the Spectrum array with the value indicated
	 *   @param pos The position where we want to change Magnitude
	 *   @param newMag The new value of Magnitude
	 */
	void SetMag(TIndex pos, TData newMag);

	 /** Replaces the Phase of the specified Frequency in the Spectrum with the value indicated
	 *   @param freq The Frequency which we want to change Phase
	 *   @param newPhase The new value of Phase
	 */
	void SetPhase(TData freq, TData newPhase);

	 /** Replaces the Phase of the specified position in the Spectrum array with the value indicated
	 *   @param pos The position where we want to change Phase
	 *   @param newPhase The new value of Phase
	 */
	void SetPhase(TIndex pos, TData newPhase);

	//For Peak Array

	/** Returns the Spectral peak located in the given position
	 *  @param pos The position we want to obtain the Spectral peak
	 *  @return The Spectral peak in the position indicated
	 */
	SpectralPeak GetSpectralPeak(TIndex pos) const;

	/** Returns the Spectral peak located in the given position of the Spectral Array
	 *  @param pos The position of Peak Array we want to obtain the Spectral peak
	 *  @return The Spectral peak in the position indicated
	 */
	SpectralPeak GetThruIndexSpectralPeak(TIndex pos) const ;

	/** This method allows to change values of a Spectral Peak given its position in the array
	 *  @param pos Position of Peak Array to modify
	 *  @param spectralPeak New spectralPeak
	 *  @param inde Index value of the given SpectralPeak, set by default to -1
	 */
	void SetSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,TIndex index=-1);

	/** Inserts a peak at the given position
	 *  @param pos Position in the array where we want to insert the new peak
	 *  @param spectralPeak New spectralPeak to insert
	 *  @param insertIndex Value indicating if the index array should be modified, set by default to false
	 *  @param index Value of the given SpectralPeak, set by default to -1
	 */   
	void InsertSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,
	bool insertIndex=false,TIndex index=-1);

	/** Inserts a peak at the end of Spectral Array
	 *  @param spectralPeak New spectralPeak to insert
	 *  @param addIndex Value indicating if the index array should be modified, set by default to false
	 *  @param index Value of the given SpectralPeak, set by default to -1
	 */   
	void AddSpectralPeak(const SpectralPeak& spectralPeak,bool addIndex=false,TIndex index=-1);

	/** Deletes a peak of Spectral Array
	 *  @param pos Position of the Peak Array we want to delete
	 *  @param deleteIndex Value indicating if the index array should be modified, set by default to false
	 */   
	void DeleteSpectralPeak(TIndex pos,bool deleteIndex=false);

	//For Residual

	/** Returns the Magnitude of a given frequency in residual Spectrum
	 *  @param freq The Frequency we want to know the Magnitude
	 *  @return The Magnitude of spectrum in the frequency indicated
	 */
	TData GetResMag(TData freq) const;

	/** Returns the Magnitude given a position in the Residual Spectrum array
	 *  @param pos The position we want to know the Magnitude
	 *  @return The Magnitude of spectrum in the index indicated
	 */
	TData GetResMag(TIndex pos) const ;

	/** Returns the Phase of a given frequency in residual Spectrum
	 *  @param freq The Frequency we want to know the Phase
	 *  @return The Phase of spectrum in the frequency indicated
	 */
	TData GetResPhase(TData freq) const ;

	/** Returns the Phase of the spectrum related to a position in the residual Spectrum array
	 *  @param pos The position we want to know the Phase
	 *  @return The Phase of spectrum in the position indicated
	 */
	TData GetResPhase(TIndex pos) const ;


	/** Replaces the Magnitude of the specified Frequency in the residual Spectrum with the value indicated
	 *   @param freq The Frequency which we want to change Magnitude
	 *   @param newMag The new value of Magnitude
	 */
	void SetResMag(TData freq, TData newMag);

	/** Replaces the Magnitude of the specified position in the residual Spectrum array with the value indicated
	 *   @param pos The position where we want to change Magnitude
	 *   @param newMag The new value of Magnitude
	 */
	void SetResMag(TIndex pos, TData newMag);

	/** Replaces the Phase of the specified Frequency in the residual Spectrum with the value indicated
	 *   @param freq The Frequency which we want to change Phase
	 *   @param newPhase The new value of Phase
	 */
	void SetResPhase(TData freq, TData newPhase);

	/** Replaces the Phase of the specified position in the residual Spectrum array with the value indicated
	 *   @param pos The position where we want to change Phase
	 *   @param newPhase The new value of Phase
	 */
	void SetResPhase(TIndex pos, TData newPhase);

	//For Fundamental

	/** Returns the fundamental frequency at a given index
	 *  @param pos Position of candidate we want to get frequency value
	 *  @return Value of Frequency in the position given
	 */
	TData GetFundamentalFreq(TIndex pos=0)const;

	/** Returns the error of the candidate of a given index
	 *  @param pos Position of candidate we want to get error value
	 *  @return Value of Error in the position
	 */
	TData GetFundamentalErr(TIndex pos=0);

	/** Modifies the frequency of a candidate at a given index
	 *  @param pos Position to change the frequency
	 *  @param newFreq New value of this position
	 */
	void SetFundamentalFreq(TIndex pos,TData newFreq)const;

	/** Modifies the error of a candidate at a given index
	 *  @param pos Position to change the error
	 *  @param newFreq New value of this position
	 */
	void SetFundamentalErr(TIndex pos,TData newErr)const;

	/*Comparison Operators*/

	/** If both frames have the same center time (value of time in the center of the frame this method will return true; false otherwise
	 * @param newFrame Frame to be compared
	 * @return A boolean value, true is the condition is validated
	 */
	bool operator ==(const Frame& newFrame) const ;

	/**  If center time of two frames is different will return true.
	 * @param newFrame Frame to be compared
	 * @return A boolean value, true is the condition is validated
	 */
	bool operator !=(const Frame& newFrame) const ;

	/** If this frame is centered in a time inferior relative to the frame passed by parameter will return true; false otherwise
	 * @param newFrame Frame to be compared
	 * @return A boolean value, true is the condition is validated
	 */
	bool operator <(const Frame& newFrame) const ;

	/** If this frame is centered in a time inferior or equal in relation with the frame passed by parameter will return true; false otherwise 
	 * @param newFrame Frame to be compared
	 * @return A boolean value, true is the condition is validated
	 */
	bool operator <=(const Frame& newFrame) const ;

	 /** If this frame is centered in a time superior relative to the frame passed by parameter will return true; false otherwise
	 * @param newFrame Frame to be compared
	 * @return A boolean value, true is the condition is validated
	 */
	bool operator >(const Frame& newFrame) const ;

	/** If this frame is centered in a time superior or equal in relation with the frame passed by parameter will return true; false otherwise
	 * @param newFrame Frame to be compared
	 * @return A boolean value, true is the condition is validated
	 */
	bool operator >=(const Frame& newFrame) const ;

};

};//namespace

#endif

