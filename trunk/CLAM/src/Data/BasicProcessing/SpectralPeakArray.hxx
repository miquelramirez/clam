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

/////////////////////////////////////////////////////////////////////////////////////
// Class SpectralPeakArray: Processing Data class to store an array of spectral peaks
////////////////////////////////////////////////////////////////////////////////////
// Description:
//  This processing data class offers an interface for storing an array of spectral
//  peaks while maintaining efficient data access. This is accomplished by actually
//  storing in memory an independent buffer for each of the attributes of a spectral
//  peak. The interface to use SpectralPeak class is simulated adding an interface
//  that constructs these objects on the fly, whenever a Peak is accessed.
//  Furthermore, this class holds an array of indices that can be used for different
//  purposes such as sorting, assigning tracks...Any peak can be accessed throug its
//  actual position in the array or its associated index
///////////////////////////////////////////////////////////////////////////////



#ifndef _SpectralPeakArray_
#define _SpectralPeakArray_

#include "DynamicType.hxx"
#include "Array.hxx"
#include "IndexArray.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"
#include "SpectralPeak.hxx"



namespace CLAM{


/**
* This processing data class offers an interface for storing an array of spectral
* peaks while maintaining efficient data access. This is accomplished by actually
* storing in memory an independent buffer for each of the attributes of a spectral
* peak. The interface to use SpectralPeak class is simulated adding an interface
* that constructs these objects on the fly, whenever a Peak is accessed.
* Furthermore, this class holds an array of indices that can be used for different
* purposes such as sorting, assigning tracks...Any peak can be accessed throug its
* actual position in the array or its associated index
* @see ProcessingData
* @see SpectralPeak
* @author Xavier Amatriain
*/
class SpectralPeakArray : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SpectralPeakArray, 8, ProcessingData);
	DYN_ATTRIBUTE (0, public, EScale, Scale);
	DYN_ATTRIBUTE (1, public, DataArray, MagBuffer);
	DYN_ATTRIBUTE (2, public, DataArray, FreqBuffer);
	DYN_ATTRIBUTE (3, public, DataArray, PhaseBuffer);
	DYN_ATTRIBUTE (4, public, DataArray, BinPosBuffer);
	DYN_ATTRIBUTE (5, public, DataArray, BinWidthBuffer);
	DYN_ATTRIBUTE (6, public, IndexArray, IndexArray);
	DYN_ATTRIBUTE (7, public, bool, MinimizeResizes);

protected:
	void DefaultInit();
public:

/** Accessory Constructor method
* Constructs the spectral peak array given a peak structure used as prototype and instantiating
* the needed dynamic attributes
* @param SpectralPeak used as prototype
*/
	SpectralPeakArray(const SpectralPeak& prototype);

/** Accessory Initialitation method
* Initializes the spectral peak array given a peak structure used as prototype and instantiating
* the needed dynamic attributes
* @param SpectralPeak used as prototype
*/
	void InitFromPrototype(const SpectralPeak& spectralPeak);

/** Accessory Initialitation method
* Initializes the spectral peak according to peak array prototype and instantiating
* the needed dynamic attributes
* @param SpectralPeak to initialize
*/
	void InitPeak(SpectralPeak& spectralPeak) const;


/** Accessory getter . Note: for efficiency you need to work directly on the buffer
* @return Magnitude of the peak in the given position
* @param Position in the array
*/
	TData GetMag(TIndex pos) const{return GetMagBuffer()[pos];}
/** Accessory getter . Note: for efficiency you need to work directly on the buffer
* @return Frequency of the peak in the given position
* @param Position in the array
*/
	TData GetFreq(TIndex pos)const{return GetFreqBuffer()[pos];}
/** Accessory getter . Note: for efficiency you need to work directly on the buffer
* @return Phase of the peak in the given position
* @param Position in the array
*/
	TData GetPhase(TIndex pos)const{return GetPhaseBuffer()[pos];}
/** Accessory getter . Note: for efficiency you need to work directly on the buffer
* @return Bin Position of the peak in the given position
* @param Position in the array
*/
	TData GetBinPos(TIndex pos)const{return GetBinPosBuffer()[pos];}
/** Accessory getter . Note: for efficiency you need to work directly on the buffer
* @return Bin Width of the peak in the given position
* @param Position in the array
*/
	int GetBinWidth(TIndex pos)const{return int(GetBinWidthBuffer()[pos]);}
/** Accessory setter . Note: for efficiency you need to work directly on the buffer
* @param Position in the array
* @param new value for magnitude of the peak in the given position
*/
	void SetMag(TIndex pos,TData newMag){GetMagBuffer()[pos]=newMag;}
/** Accessory setter . Note: for efficiency you need to work directly on the buffer
* @param Position in the array
* @param new value for frequency of the peak in the given position
*/
	void SetFreq(TIndex pos,TData newFreq){GetFreqBuffer()[pos]=newFreq;}
/** Accessory setter . Note: for efficiency you need to work directly on the buffer
* @param Position in the array
* @param new value for phase of the peak in the given position
*/
	void SetPhase(TIndex pos,TData newPhase){GetPhaseBuffer()[pos]=newPhase;}
/** Accessory setter . Note: for efficiency you need to work directly on the buffer
* @param Position in the array
* @param new value for bin position of the peak in the given position
*/
	void SetBinPos(TIndex pos,TData newBinPos){GetBinPosBuffer()[pos]=newBinPos;}
/** Accessory setter . Note: for efficiency you need to work directly on the buffer
* @param Position in the array
* @param new value for bin width of the peak in the given position
*/
	void SetBinWidth(TIndex pos,int newBinWidth){GetBinWidthBuffer()[pos]=TData(newBinWidth);}

/** Additional interface
* @return the position of the peak array where the maximum amplitude is found
*/
	TIndex GetMaxMagPos() const;

/** Additional interface
* @return the position of the index array containing the index of the peak ewhere the maximum amplitude is found
*/
	TIndex GetMaxMagIndex() const;


/** Accessory method offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* Initializes a given peak structure according to structure of array instantiating
* the needed dynamic attributes
* @param SpectralPeak to be initialized
*/
	void InitSpectralPeak(SpectralPeak& spectralPeak) const;
/** Accessory method offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* Checks that spectral peak prototype is consistent with spectral array structure
* @param SpectralPeak to be checked
*/
	bool IsCorrectPrototype(const SpectralPeak& spectralPeak) const;
/** Accessory getter offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* @return SpectralPeak in the given position
* @param position in the array
*/
	SpectralPeak GetSpectralPeak(TIndex pos) const;
/** Accessory getter offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* @return SpectralPeak in the position given by the value of the index in the given position
* @param index position in the index array
*/
	SpectralPeak GetThruIndexSpectralPeak(TIndex pos) const;
/** Accessory setter offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* @param position in the array
* @param SpectralPeak to set in the given position
* @param Index value of the given SpectralPeak, set by default to -1
*/
	void SetSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,TIndex index=-1);
/** Accessory method offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* Inserts a peak (or rather the values of its attributes) at the given position
* moving the ones in next positions
* @param position in the array where to insert
* @param SpectralPeak to set in the given position
* @param boolean value indicating if the index array should be modified or not
* @param Index value of the given SpectralPeak, set by default to -1
*/
	void InsertSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,
							bool insertIndex=false,TIndex index=-1);
/** Accessory method offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* Adds a peak (or rather the values of its attributes) at the end of the array
* @param SpectralPeak to add
* @param boolean value indicating if the index array should be modified or not
* @param Index value of the given SpectralPeak, set by default to -1
*/
	void AddSpectralPeak(const SpectralPeak& spectralPeak,bool addIndex=false,TIndex index=-1);
/** Accessory method offering interface for working with SpectralPeak class.
* Note: for efficiency you need to work directly on the buffer
* Deletes a peak (or rather the values of its attributes) at a given position
* @param position of the peak to delete
* @param boolean value indicating if the index array should be modified or not
*/
	void DeleteSpectralPeak(TIndex pos,bool deleteIndex=false);



/* index handling */

/** Additional interface for working with indices
* @return the number of non assigned peaks in the array
*/
	int GetnIndexedPeaks() const;

/** Additional interface for working with indices
* @return first Position in the index array that holds a given index
* @param index to find in index array
*/
	TIndex GetPositionFromIndex(TIndex index) const;

/** Additional interface for working with indices
* @param position where to set the given index
* @param index to set
*/
	void SetIndex(TIndex pos,TIndex index){GetIndexArray()[pos]=index;};
/** Additional interface for working with indices
* @return index found at a given position of the index array
* @param position in the index array
*/
	TIndex GetIndex(TIndex pos) const {return GetIndexArray()[pos];}
/** Additional interface for working with indices
* @param position where to delete an index
*/
	void DeleteIndex (TIndex index);
/** Additional interface for working with indices
* resizes index array according to the current number of peaks and assigns
* subsecuent indices starting with 0 and finishing with nPeaks-1
*/
	void ResetIndices();
/** Additional interface for working with indices
* resizes index array according to the current number of peaks and sets each ç
* index to -1 (which is usually interpreted as "no valid index"
*/
	void InitIndices();
/** Additional interface for working with indices
* sets all indices to a given value
* @param value to set all indices to
*/
	void SetIndicesTo(TIndex val);
/** Additional interface for working with indices
* @param index to add at the end of the index array
*/
	void AddIndex(TIndex index);

/** Additional interface for working with indices
* returns first position in index array to have a value of -1
* @return position where the index was found, -1 if all indices are valid
* @param position to begin search
*/
	TIndex GetFirstNonValidIndexPosition(TIndex beginAt) const;
/** Additional interface for working with indices
* @return true if index at the given position is greater than 0
* @param position of the index to validate
*/
	bool IsValidIndex(TIndex position) const;
/** Additional interface for working with indices
* @return true if index is found in the arry
* @param index to find
*/
	TIndex IsIndexExisting(TIndex index) const;
/** Additional interface for working with indices
* @return the value of the index with a higher value
*/
	TIndex GetMaxIndex() const;

/* Peak Array size handling*/

/** Returns the actual size of the different instantiated buffers
* which should be consistent in size (this is guaranteed if this
* interface is used but not if the user sets the buffer size directly
* accessing the arrays.
*/
	TSize GetnPeaks() const
	{
		if(HasMagBuffer()) return GetMagBuffer().Size();
		if(HasFreqBuffer()) return GetFreqBuffer().Size();
		if(HasPhaseBuffer()) return GetPhaseBuffer().Size();
		if(HasBinPosBuffer()) return GetBinPosBuffer().Size();
		if(HasBinWidthBuffer()) return GetBinWidthBuffer().Size();
		else return 0;
	}

/** Sets the actual size of the different instantiated buffers keeping them
* consistent. If allocated size is less than npeaks, a resize is automatically
* performed on all buffers.
*/
	void SetnPeaks(TSize npeaks)
	{
		if (npeaks>GetnMaxPeaks()) SetnMaxPeaks(npeaks);
		if(HasMagBuffer()) {
		//	GetMagBuffer().Resize(npeaks);
			GetMagBuffer().SetSize(npeaks);}
		if(HasFreqBuffer()) {
		//	GetFreqBuffer().Resize(npeaks);
			GetFreqBuffer().SetSize(npeaks);}
		if(HasPhaseBuffer()){
		//	GetPhaseBuffer().Resize(npeaks);
			GetPhaseBuffer().SetSize(npeaks);}
		if(HasBinPosBuffer()) {
		//	GetBinPosBuffer().Resize(npeaks);
			GetBinPosBuffer().SetSize(npeaks);}
		if(HasBinWidthBuffer()) {
		//	GetBinWidthBuffer().Resize(npeaks);
			GetBinWidthBuffer().SetSize(npeaks);}
		if(HasIndexArray()) {
		//	GetIndexArray().Resize(npeaks);
			GetIndexArray().SetSize(npeaks);}



	}

/** Sets the actual allocated size of the different instantiated buffers keeping them
* consistent. Useful for reducing the number of allocations in run time.
*/
	void SetnMaxPeaks(TSize npeaks)
	{
		if(HasMagBuffer()){
			GetMagBuffer().Resize(npeaks);
			if(GetMinimizeResizes())
				GetMagBuffer().SetStep(npeaks);}
		if(HasFreqBuffer()){
			GetFreqBuffer().Resize(npeaks);
			if(GetMinimizeResizes())
				GetFreqBuffer().SetStep(npeaks);}
		if(HasPhaseBuffer()){
			GetPhaseBuffer().Resize(npeaks);
			if(GetMinimizeResizes())
				GetPhaseBuffer().SetStep(npeaks);}
		if(HasBinPosBuffer()){
			GetBinPosBuffer().Resize(npeaks);
			if(GetMinimizeResizes())
				GetBinPosBuffer().SetStep(npeaks);}
		if(HasBinWidthBuffer()){
			GetBinWidthBuffer().Resize(npeaks);
			if(GetMinimizeResizes())
				GetBinWidthBuffer().SetStep(npeaks);}
		if(HasIndexArray()){
			GetIndexArray().Resize(npeaks);
			if(GetMinimizeResizes())
				GetIndexArray().SetStep(npeaks);}
	}

/** Returns the actual allocated size of the different instantiated buffers
* which should be consistent in size (this is guaranteed if this
* interface is used but not if the user resizes buffers directly
* accessing the arrays.
*/
	TSize GetnMaxPeaks() const
	{
		if(HasMagBuffer()) return GetMagBuffer().AllocatedSize();
		if(HasFreqBuffer()) return GetFreqBuffer().AllocatedSize();
		if(HasPhaseBuffer()) return GetPhaseBuffer().AllocatedSize();
		if(HasBinPosBuffer()) return GetBinPosBuffer().AllocatedSize();
		if(HasBinWidthBuffer()) return GetBinWidthBuffer().AllocatedSize();
		else return 0;
	}

	//index getters and setters
	double GetThruIndexFreq(TIndex pos) const;
	double GetThruIndexMag(TIndex pos) const;
	double GetThruIndexPhase(TIndex pos) const;
	double GetThruIndexBinPos(TIndex pos) const;
	TSize  GetThruIndexBinWidth(TIndex pos) const;
	void   SetThruIndexFreq(TIndex pos,double freq);
	void   SetThruIndexMag(TIndex pos,double mag) ;
	void   SetThruIndexPhase(TIndex pos,double phase) ;
	void   SetThruIndexBinPos(TIndex pos,double binPos);
	void   SetThruIndexBinWidth(TIndex pos,TSize binWidth);
	void   SetThruIndexSpectralPeak(TIndex pos,SpectralPeak& peak);

	void SetIsIndexUpToDate(bool isUpToDate)
	{
		mIsIndexUpToDate = isUpToDate;
	}

	/** Converts linear magnitude data to dB*/
	void TodB();
	/** Converts dB magnitude data to linear*/
	void ToLinear();

	/** Copies all of the spectral peak data from the given objects
		member arrays --
		FreqBuffer, MagBuffer, PhaseBuffer, BinPosBuffer, BinWidthBuffer,
		and IndexArray -- to the member arrays of this object.
		Copies the properties Scale and MinimizeResizes too. */
	void CopyMembers(SpectralPeakArray& sourceSpectralPeakArray);

	SpectralPeakArray operator+(const SpectralPeakArray& in);
private:
	bool mIsIndexUpToDate;

};

}; // namespace CLAM

#endif

