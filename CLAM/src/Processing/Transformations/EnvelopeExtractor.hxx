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

#ifndef _EnvelopeExtractor_
#define _EnvelopeExtractor_

#include "ControlConfig.hxx"
#include "Processing.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Envelope.hxx"
#include "Audio.hxx"

namespace CLAM
{

	/** Internal class. Container of interval amplitude average points
	 *  for the EnvelopeExtractor class.
	 *  <p>
	 *  This class stores a sequence of amplitude values corresponding
	 *  to average amplitudes in the interpolation intervals of the
	 *  current audio frame, and of previous audio frames.
	 *  <p>
	 */
	class IntervalAmplitudeAverages
	{
		/** Array of interval amplitude average points.
		 *  <p>
		 *  The first mNMemoryPoints hold amplitude average of
		 *  interpolation intervals from the previous audio frames.
		 *  The following mPointsPerFrame values is where the amplitude
		 *  average of interpolation intervals in the current frame is
		 *  stored.
		 */
		Array<TData> mArray;

		/** Index pointing to the begining of the Amplitude average
		 *  intervals for the current frame.
		 */
		int mCurrentPos;

		/** Number of interpolation points in each frame */
		int mPointsPerFrame;

		/** Number of amplitude interval averages to use for each new
		 *  interpolation point */
		int mNMemoryPoints;

		inline TData AcumulationShape(int pos);

		inline TData& Current(int index);

	public:

		inline void  Reconfigure(int PointsPerFrame,int MemoryPoints);

		inline bool  Configured();

		inline void  Clear(void);

		inline void  AdvanceFrame(void);

		inline TData Acumulated(int index);

		inline void  Compute(int interval,
		                     Array<TData> &audio,
		                     int interval_start,
		                     int interval_end);
	};


	class EnvExtractorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (EnvExtractorConfig, 9, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, TData, SampleRate);

		/** Number of samples in each audio input frame */
		DYN_ATTRIBUTE (1, public, int, FrameSize);

		/** Time period between envelope interpolation points, in
		 *  miliseconds. If you prefer to specify the exact amount of
		 *  interpolation points to use in each audio frame, you can
		 *  use the NInterpPointsPerFrame attribute, leaving this one with
		 *  0.0. If both are non-zero, the other one will be used. 
		 *	<p>
		 *  Note that currently the actual interpolation period used
		 *  in the processing object will be rounded so that there are
		 *  always interpolation points exactly at the beginning and
		 *  at the end of each audio frame, for eficiency
		 *  reasons. This may change in future.
		 */
		DYN_ATTRIBUTE (2, public, ControlConfig, InterpolationPeriod);

		/** 
		 *  Number of mIlliseconds of audio to use for the "average"
		 *  of each new envelope interpolation point.
		 *  <p> The bigger this value is, the smoother the envelope
		 *  will be. It will also introduce a delay in the envelope
		 *  from the original audio signal.
		 *  <p> If you would rather like to specify the exact amount
		 * of interpolationn segments to use for the average, you can
		 * use the InterpolationPeriod attribute, leaving this with a
		 * value of 0.0. If both attributes are non-zero, this one
		 * will be used.
		 */
		DYN_ATTRIBUTE (3, public, ControlConfig, IntegrationLength);

		/** Number of envelope interpolation points to use for each
		 *  audio frame. If you prefer to use time units, you can use
		 *  the InterpolationPeriod attribute instead, (leaving this
		 *  one to 0). If both are non-zero, this one will be used. 
		 */
		DYN_ATTRIBUTE (4, public, int, NInterpPointsPerFrame);

		/** Number of interpolation segments (possibly from previous
		 *  audio frames) to add up to the average used to calculate
		 *  each new interpolation point. At least 1 point must be
		 *  used, so that the last segment in the previous frame is used
		 *  in the new one.
		 *  <p> The bigger this value is, the smoother the envelope
		 *  will be. It will also introduce a delay in the envelope
		 *  from the original audio signal.
		 *  <p> If you would rather like to use time units to specify
		 *  the length of the envelope "integration period", you can
		 *  use the IntegrationLength attribute, leaving this one to zero.
		 *  If both are non-zero, this one will be used.
		 */
		DYN_ATTRIBUTE (5, public, int, NMemoryPoints);
		
		DYN_ATTRIBUTE (6, public, ControlConfig, NormalLevel);

		DYN_ATTRIBUTE (7, public, ControlConfig, SilenceLevel);

		DYN_ATTRIBUTE (8, public, EInterpolation, InterpolationType);
	protected:

		void DefaultInit(void);
	};
	

	class EnvelopeExtractor: public Processing
	{
		TTime mInterpolationPeriodControl;
		TTime mIntegrationLengthControl;
		TData mNormalLevelControl;
		TData mSilenceLevelControl;

	public:

		FloatInControl cInterpolationPeriod;
		FloatInControl cIntegrationLength;
		FloatInControl cNormalLevel;
		FloatInControl cSilenceLevel;

		InPort<Audio> Input;
		OutPort<Envelope> Output;

		EnvelopeExtractor(const EnvExtractorConfig& c = EnvExtractorConfig());

		const char * GetClassName() const {return "EnvelopeExtractor";}

		const ProcessingConfig &GetConfig() const { return mConfig;}

		void Attach(Audio& inp, Envelope& env);

		bool Do();

		bool Do(const Audio& inp, Envelope& env);

		// Debugging accessors

		int NPoints() const { return mPointsPerFrame; }

		TData NormalLevel() const {return mNormalLevel;}

		TData SilenceLevel() const {return mSilenceLevel;}

		TTime InterpolationPeriod() const {return mInterpolationPeriod;}

		TTime IntegrationLength() const {return mIntegrationLength;}

		TData NormalLevelControl() const {return mNormalLevelControl;}

		TData SilenceLevelControl() const {return mSilenceLevelControl;}

		TTime InterpolationPeriodControl() const {return mInterpolationPeriodControl;}

		TTime IntegrationLengthControl() const {return mIntegrationLengthControl;}

	private:

		EnvExtractorConfig mConfig;
		/** Configuration attribute copy */
		int mPointsPerFrame;
		/** Configuration attribute copy */
		int mNMemoryPoints;
		/** Configuration attribute copy */
		TData mNormalLevel;
		/** Configuration attribute copy */
		TData mSilenceLevel;
		/** Time increment between interpolation points */
		TTime mDeltaX;
		/** Lenth of each frame in milliseconds.*/
		TTime mFrameTime;
		/** Lenth of each frame samples.*/
		int mFrameSize;
		/** Lenth of each sample.*/
		TTime mSampleDelta;
		/** Time interval between interpolation points in milliseconds */
		TTime mInterpolationPeriod;
		/** Time interval between interpolation points in milliseconds */
		TTime mIntegrationLength;

		/** Interval amplitude average points. */
		IntervalAmplitudeAverages mAmplitudeAverages;

		/** Interpolation points calculated for the current frame 
		 *  <p>
		 *  This will usually hold mPointsPerFrame+1 values (mPointsPerFrame new
		 *  values, plus the last value from the previous frame), for
		 *  each interpolation point in the output bpf.
		 *  <p>
		 *  If InterpolationType is eSpline, two extra points will be
		 *  stored in order to be able to calculate the lateral
		 *  derivatives, and the three last points in each frame will
		 *  be the three first ones in the next frame.
		 */
		Array<TData> mInterpolationPoints;

		/** True if spline interpolation is being done. In such case,
		 *  the output envelope will be delayed for one interpolation
		 *  interval, so that boundary derivatives can always match */
		bool mIsSpline;

		TData mIpMin, mIpFactor,
		      mIlMin, mIlFactor,
		      mNlMin, mNlFactor,
		      mSlMin, mSlFactor;

		bool SetPointsPerFrame(int npoints);
		bool SetInterpolationPeriod(TTime period);
		void SetNMemoryPoints(int mpoints);
		bool SetIntegrationLength(TTime length);
		void SetNormalLevel(TData nlevel);
		void SetSilenceLevel(TData slevel);

		bool ConcreteConfigure(const ProcessingConfig& c);

		bool ConcreteStart();

		void ConfigureEnvelope(BPFTmpl<TTime,TData> &bpf);

		void WriteEnvelope    (BPFTmpl<TTime,TData> &bpf);

		void StoreInterpolationPoints();

		void CleanSilence();

		void InitializeControls();

		void InterpolationPeriodChange(TControlData val);

		void IntegrationLengthChange(TControlData val);

		void NormalLevelChange(TControlData val);

		void SilenceLevelChange(TControlData val);

	};
	
}

#endif


