/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MaskingPeakDetect.hxx"

namespace CLAM {


	Enum::tEnumValue EFrequencyWeight::sEnumValues[] = {
		{EFrequencyWeight::eNone,       "None"},
		{EFrequencyWeight::eLogarithmic,"Logarithmic"},
		{EFrequencyWeight::eInverse,    "Inverse"},
		{0,NULL}
	};

	Enum::tValue EFrequencyWeight::sDefault = EFrequencyWeight::eNone;

	Enum::tEnumValue EMaskingShape::sEnumValues[] = {
		{EMaskingShape::eNone,    "None"},
		{EMaskingShape::eLinear,  "Linear"},
		{EMaskingShape::eGaussian,"Gaussian"},
		{0,NULL}
	};

	Enum::tValue EMaskingShape::sDefault = EMaskingShape::eNone;




	void MaskingPeakDetectConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetMaxPeaks(10);
		SetMagThreshold(-60);
		SetSpectralSize(512);
		SetFrequencyWeight(EFrequencyWeight::eNone);
		SetMaskingShape(EMaskingShape::eNone);
		SetMaskingBandwidth(200.0);
	}

	MaskingPeakDetect::MaskingPeakDetect(const MaskingPeakDetectConfig& c)
		: Input("Input",this,1),
		  Output("Output",this,1)
	{
		Configure(c);
	}

	bool MaskingPeakDetect::ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const MaskingPeakDetectConfig&>(cfg);

		mMaxPeaks      = mConfig.GetMaxPeaks();
		mSpectralSize  = mConfig.GetSpectralSize();
		mSpectralRange = mConfig.GetSpectralRange();

		mInterpolatedMags.Resize(mSpectralSize);
		mInterpolatedMags.SetSize(mSpectralSize);
		mInterpolatedFreqs.Resize(mSpectralSize);
		mInterpolatedFreqs.SetSize(mSpectralSize);
		mMasked.Resize(mSpectralSize);
		mMasked.SetSize(mSpectralSize);

		mPeakMagHeap.reserve(mSpectralSize);
		mPeakFreqHeap.reserve(mMaxPeaks);

		mPeakFinder.Configure(mSpectralSize,
							  mConfig.GetFrequencyWeight(),
							  mSpectralRange,
							  mConfig.GetMagThreshold());

		mMaskingBandwidth = mConfig.GetMaskingBandwidth();
		mMaskingShape     = mConfig.GetMaskingShape();

		return true;
	}

	bool MaskingPeakDetect::Do(void)
	{
		bool res = Do(Input.GetData(),Output.GetData());
		Input.LeaveData();
		Output.LeaveData();
		return res;
	}

	void MaskingPeakDetect::CheckData(const Spectrum& inp, SpectralPeakArray& out)
	{
		CLAM_ASSERT(inp.GetSize() == mSpectralSize,
					"MaskingPeakDetect::Do(): Wrong input size");
		CLAM_ASSERT(out.HasMagBuffer()  &&
					out.HasFreqBuffer() &&
					out.HasIndexArray(),
					"MaskingPeakDetect::Do(): Missing attributes in output");
		CLAM_ASSERT(out.GetMagBuffer ().Size() >= mMaxPeaks &&
					out.GetFreqBuffer().Size() >= mMaxPeaks &&
					out.GetIndexArray().Size() >= mMaxPeaks,
					"MaskingPeakDetect::Do(): Too small attributes in output");
	}

	void MaskingPeakDetect::FindAndSortLocalPeaks(const Array<TData> &magnitude)
	{
		int peak_number = 0;
		TData weighted_mag;

		mPeakFinder.Start(magnitude);
		mPeakMagHeap.clear();

		while ( mPeakFinder.FindNextPeak() )
		{
			mInterpolatedFreqs[peak_number] =
				mPeakFinder.InterpolatedPeakFreq();

			mInterpolatedMags[peak_number] =
				mPeakFinder.InterpolatedPeakMag();

			weighted_mag = mPeakFinder.WeightedPeakMag();

			mPeakMagHeap.push(ValuedIndex(weighted_mag,peak_number));

			peak_number++;
		}

		mNumLocalPeaks = peak_number;

		CLAM_ASSERT(mNumLocalPeaks == int(mPeakMagHeap.size()),
					"Post condition failed.");
		CLAM_ASSERT(mNumLocalPeaks < mSpectralSize,
					"Post condition failed.");

	}


	TData MaskingPeakDetect::MaskingShape(TData distance)
	{
		switch (mMaskingShape) {
		case EMaskingShape::eGaussian:                                          // TODO
		case EMaskingShape::eLinear:
			return 1.0 - distance/mMaskingBandwidth;
		default:
			return 0.0;
		}
	}

	/** @todo
	 * Possible optimisation: use integer spectral bin positions instead 
	 * of floating point frequencies for the distance between the peak
	 * and its neighbours, and as index for a mask shape table. This would
	 * be a grosser aproximation.
	 */ 
	void MaskingPeakDetect::TagMaskedNeighbourgs(int peak_pos,
												 EDirection direction)
	{
		int neighbour_pos, limit;
		TData neighbour_mag, neighbour_freq;
		TData mag_threshold;
		TData freq_distance;
		TData peak_mag  = mInterpolatedMags[peak_pos];
		TData peak_freq = mInterpolatedFreqs[peak_pos];

		if (direction==eRight)
			limit=mNumLocalPeaks;
		else
			limit=-1;

		for (neighbour_pos =  peak_pos + direction; 
			 neighbour_pos != limit; 
			 neighbour_pos += direction  )
		{
			neighbour_freq = mInterpolatedFreqs[neighbour_pos];

			freq_distance = fabs( neighbour_freq - peak_freq);

			if ( freq_distance > mMaskingBandwidth)
				return;

			neighbour_mag = mInterpolatedMags[neighbour_pos];

			mag_threshold = peak_mag * MaskingShape(freq_distance);

			if ( neighbour_mag < mag_threshold )
			{
				mMasked[neighbour_pos] = true;
			}
		}
	}

	void MaskingPeakDetect::WriteUnmaskedPeaks(SpectralPeakArray &output)
	{
		int peak_pos;
		int npeaks = 0, peak_index;

		Array<TData> &mags  = output.GetMagBuffer();
		Array<TData> &freqs = output.GetFreqBuffer();
		Array<TIndex> &idx  = output.GetIndexArray();

		for (int i=0; i<mNumLocalPeaks; i++)
			mMasked[i] = false;

		mPeakFreqHeap.clear();

		while (mPeakMagHeap.size())
		{
			peak_pos = mPeakMagHeap.top().index;

			if ( ! mMasked[peak_pos] )
			{
				TagMaskedNeighbourgs(peak_pos,eLeft);

				TagMaskedNeighbourgs(peak_pos,eRight);

				mPeakFreqHeap.push(ValuedIndex(mInterpolatedFreqs[peak_pos],peak_pos));

				if (++npeaks == mMaxPeaks)
					break;
			}
			mPeakMagHeap.pop();
		}

		peak_index = npeaks;

		while (mPeakFreqHeap.size())
		{
			peak_pos = mPeakFreqHeap.top().index;

			peak_index--;

			mags[peak_index]  = mInterpolatedMags[peak_pos];
			freqs[peak_index] = mInterpolatedFreqs[peak_pos];
			idx[peak_index]   = peak_pos;

			mPeakFreqHeap.pop();
		}

		output.SetnPeaks(npeaks);
	}

	bool MaskingPeakDetect::Do(const Spectrum& inp, SpectralPeakArray& out)
	{
		CheckData(inp,out);

		FindAndSortLocalPeaks(inp.GetMagBuffer());

		WriteUnmaskedPeaks(out);

		return true;
	}


	// Peak finder now...

	MaskingPeakDetect::PeakFinder::PeakFinder()
	{
	}

	void MaskingPeakDetect::PeakFinder::Configure(int spectrum_size,
							   EFrequencyWeight weight,
							   TData spectral_range,
							   TData mag_relative_threshold)
	{
		mWeightedMags.Resize(spectrum_size);
		mWeightTable. Resize(spectrum_size);
		mWeightedMags.SetSize(spectrum_size);
		mWeightTable. SetSize(spectrum_size);

		mBinDelta     = spectral_range / (spectrum_size-1);
		mRelativeThreshold = mag_relative_threshold;


		for (int i=0; i<spectrum_size; i++)        // TODO: Actually fill the table with proper values.
			mWeightTable[i] = i*mBinDelta;
	}

	void MaskingPeakDetect::PeakFinder::Start(const Array<TData> &magnitude)
	{
		int size = magnitude.Size();
		TData val;
		TData max = 0.0;

		CLAM_ASSERT(size == mWeightedMags.Size(),
					"MaskingPeakDetect::PeakFinder::Start(): size mismach");

		for (int i=0; i<size; i++) {
			val = magnitude[i] * mWeightTable[i];
			mWeightedMags[i] = val;
			if (val > max)
				max = val;
		}

		mAbsoluteThreshold = max * pow(10,mRelativeThreshold);

		magnitude.GiveChunk(0,magnitude.Size(),mMag);

		mCurrent = 0;
		mState = eGoingDownhill;
	}

	bool MaskingPeakDetect::PeakFinder::CheckAndComputePeak()
	{
		if (mWeightedMags[mCurrent] < mAbsoluteThreshold)
			return false;

		TData midMag   = mMag[mCurrent];
		TData leftMag  = mMag[mCurrent-1];
		TData rightMag = mMag[mCurrent+1];

		TData midWeightedMag   = mWeightedMags[mCurrent];
		TData leftWeightedMag  = mWeightedMags[mCurrent-1];
		TData rightWeightedMag = mWeightedMags[mCurrent+1];

		TData diff_from_max_pos = 0.5 * ((leftMag-rightMag) / (leftMag- 2*midMag + rightMag));

		TData exact_pos = TData(mCurrent) + diff_from_max_pos;

		mPeakFreq = exact_pos * mBinDelta;
		mPeakMag  = midMag - (leftMag - rightMag) * diff_from_max_pos / 4.0 ;

		mPeakWeightedMag  = midWeightedMag - (leftWeightedMag - rightWeightedMag) / 
			( 8.0 * diff_from_max_pos) ;

//		mPeakMag  = mMag[mCurrent];
//		mPeakFreq = mCurrent * mBinDelta;
		return true;
	}

	bool MaskingPeakDetect::PeakFinder::CheckAndComputePlainPeak()
	{
		if (mWeightedMags[mCurrent] < mAbsoluteThreshold)
			return false;

		mPeakMag  = mMag[mCurrent];
		mPeakFreq = (mCurrent - mUphillPlainFirst) * mBinDelta / 2.0;
		return true;
	}

	// Peak search state machine.
	bool MaskingPeakDetect::PeakFinder::FindNextPeak()
	{
		EDirection direction;
		TData mag_delta;

		int size = mMag.Size();

		while (mCurrent < size - 1) {

			mag_delta =  mMag[mCurrent+1] - mMag[mCurrent];

			if      ( mag_delta < 0.0 ) direction = eDown;
			else if ( mag_delta > 0.0 ) direction = eUp;
			else                        direction = eHorizontal;

			switch (mState) {

			case eGoingUphill:
				switch (direction) {
				case eDown:
					mState = eGoingDownhill;
					if (CheckAndComputePeak()) {
						mCurrent++;
						return true; // Peak found.
					}
					break;
				case eHorizontal:
					mState = eInPlainGoingUphill;
					mUphillPlainFirst = mCurrent;
					break;
				case eUp:
					break;
				}
				break;

			case eGoingDownhill:
				if (direction == eUp)
					mState = eGoingUphill;
				break;

			case eInPlainGoingUphill:
				switch (direction) {
				case eDown:
					mState = eGoingDownhill;
					if (CheckAndComputePlainPeak()) {
						mCurrent++;
						return true; // Peak found
					}
					break;
				case eUp:
					mState = eGoingUphill;
					break;
				case eHorizontal:
					break;
				}
			}
			mCurrent++;
		}
		return false;
	}

	TData MaskingPeakDetect::PeakFinder::InterpolatedPeakMag()
	{
		return mPeakMag;
	}

	TData MaskingPeakDetect::PeakFinder::WeightedPeakMag()
	{
		return mPeakWeightedMag;
	}

	TData MaskingPeakDetect::PeakFinder::InterpolatedPeakFreq()
	{
		return mPeakFreq;
	}
	

}
