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

#include "EnvelopeExtractor.hxx"


namespace CLAM {

	void IntervalAmplitudeAverages::Reconfigure(int points_per_frame,int memory_points)
	{
		int new_size = points_per_frame + memory_points;

		if (mArray.Size() < new_size)
			mArray.Resize(new_size);

		if (mArray.Size() != new_size)
			mArray.SetSize(new_size);

		mCurrentPos     = memory_points-1;
		mNMemoryPoints  = memory_points;
		mPointsPerFrame = points_per_frame;
	}

	bool IntervalAmplitudeAverages::Configured()
	{
		return mArray.Size() != 0;
	}

	TData &IntervalAmplitudeAverages::Current(int index)
	{
		return mArray[mCurrentPos+index];
	}

	void IntervalAmplitudeAverages::Clear()
	{
		int i, npoints = mArray.Size();
		for (i=0; i<npoints; i++)
			mArray[i]=0.0;
	}

	void IntervalAmplitudeAverages::AdvanceFrame()
	{
		for (int i = -mCurrentPos; i<=0; i++)
			Current(i) = Current(i+mPointsPerFrame);
	}

	void IntervalAmplitudeAverages::Compute(int interval,
						Array<TData> &audio,
						int interval_start,
						int interval_end)
	{
		TData interval_mean = 0.0;
		for (int i=interval_start; i<interval_end; i++)
			interval_mean += fabs(audio[i]);
		interval_mean /= interval_end - interval_start;
		Current(interval) = interval_mean;
	}

	TData IntervalAmplitudeAverages::AcumulationShape(int i)
	{
		return TData(mNMemoryPoints-i)/TData(mNMemoryPoints);
	}

	TData IntervalAmplitudeAverages::Acumulated(int ipoint)
	{
		TData res =0.0;
		for (unsigned i=0; i< mNMemoryPoints; i++)
			res += AcumulationShape(i) * Current(ipoint-i);
		return res;
	}


	void EnvExtractorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSampleRate(44100);
		SetFrameSize(512);

		GetInterpolationPeriod().SetInitValue(5.0);
		GetInterpolationPeriod().SetMinValue(2.0);
		GetInterpolationPeriod().SetMaxValue(10.0);

		GetIntegrationLength().SetInitValue(50.0);
		GetIntegrationLength().SetMinValue(10.0);
		GetIntegrationLength().SetMaxValue(2000.0);

		GetNormalLevel().SetInitValue(0.25);
		GetNormalLevel().SetMinValue(0.01);
		GetNormalLevel().SetMaxValue(10.0);

		GetSilenceLevel().SetInitValue(0.0);
		GetSilenceLevel().SetMinValue(0.0);
		GetSilenceLevel().SetMaxValue(0.2);

		SetNInterpPointsPerFrame(0);
		SetNMemoryPoints(0);
		SetInterpolationType(EInterpolation::eLinear);
	}

#define CONTROL(name) c##name(#name,this,&EnvelopeExtractor::name##Change)

	EnvelopeExtractor::EnvelopeExtractor(const EnvExtractorConfig& c)
		: CONTROL(InterpolationPeriod)
		, CONTROL(IntegrationLength)
		, CONTROL(NormalLevel)
		, CONTROL(SilenceLevel)
		, Input("Input",this)
		, Output("Output",this)
		, mPointsPerFrame(0)
		, mNMemoryPoints(0)
		, mNormalLevel(0.0)
		, mSilenceLevel(0.0)
		, mDeltaX(0.0)
		, mFrameTime(0.0)
		, mFrameSize(0)
		, mInterpolationPeriod(0.0)
		, mIntegrationLength(0.0)
		, mIsSpline(false)
	{
		Configure(c);
	}

#undef CONTROL

	bool EnvelopeExtractor::ConcreteStart()
	{
		if (!mAmplitudeAverages.Configured()) 
			return false;

		mAmplitudeAverages.Clear();

		int i,n_interp_points;

		if (mIsSpline) n_interp_points = mPointsPerFrame + 3;
		else           n_interp_points = mPointsPerFrame + 1;
		for (i=0; i<n_interp_points; i++)
			mInterpolationPoints[i]=0.0;

		return true;
	}

	void EnvelopeExtractor::ConfigureEnvelope(BPFTmpl<TTime,TData> & bpf)
	{
		if (bpf.Size() != mPointsPerFrame+1) {
 			bpf.Resize   (mPointsPerFrame+1);
 			bpf.SetSize  (mPointsPerFrame+1);
 		}

		if (bpf.GetInterpolation() != mConfig.GetInterpolationType())
			bpf.SetIntpType(mConfig.GetInterpolationType());

		double pos=0.0;
		int i;
		for (i=0; i<=mPointsPerFrame; i++) {
			bpf.SetXValue(i,pos);
			pos+=mDeltaX;
		}
	}

	bool EnvelopeExtractor::SetPointsPerFrame(int npoints)
	{
		if ( ( npoints < 1              ) ||
			 ( npoints < 2 && mIsSpline ) )
		{
			return false;
		}

		mPointsPerFrame = npoints;

		int n_interp_points = mPointsPerFrame + 1;

		if (mIsSpline)
			n_interp_points +=2; // Needed for boundary derivatives

		if (mInterpolationPoints.AllocatedSize() < n_interp_points)
			mInterpolationPoints.Resize(n_interp_points);

		mInterpolationPoints.SetSize(n_interp_points);

		mAmplitudeAverages.Reconfigure(mPointsPerFrame,mNMemoryPoints);

		mDeltaX = (mSampleDelta * TTime(mFrameSize)) / TTime(mPointsPerFrame);

		return true;
	}

	bool EnvelopeExtractor::SetInterpolationPeriod(TTime period)
	{
		int points_per_frame  = int(mFrameTime / period);

		if (!SetPointsPerFrame(points_per_frame))
  				return false;

		mInterpolationPeriod = period;

		return true;
	}

	void EnvelopeExtractor::SetNMemoryPoints(int mpoints)
	{
		mNMemoryPoints = mpoints;

		mAmplitudeAverages.Reconfigure(mPointsPerFrame,mNMemoryPoints);
	}

	bool EnvelopeExtractor::SetIntegrationLength(TTime length)
	{
		int memory_points = int(length / mInterpolationPeriod);

		if (memory_points <= 0)
			return false;

		mIntegrationLength = length;

		SetNMemoryPoints(memory_points);

		return true;
	}

	void EnvelopeExtractor::SetNormalLevel(TData nlevel)
	{
		mNormalLevel = nlevel;
	}

	void EnvelopeExtractor::SetSilenceLevel(TData slevel)
	{
		mSilenceLevel = slevel;
	}

	bool EnvelopeExtractor::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mIsSpline = (mConfig.GetInterpolationType() == EInterpolation::eSpline);

		mFrameSize = mConfig.GetFrameSize();

		if (!mFrameSize)
		{
			AddConfigErrorMessage("FrameSize must be non-zero");
			return false;
		}

		mFrameTime = 1000.0 * double(mFrameSize) / double(mConfig.GetSampleRate());

		if (mConfig.GetNInterpPointsPerFrame() > 0)
		{
			SetPointsPerFrame(mConfig.GetNInterpPointsPerFrame());
		}
		else if (mConfig.GetInterpolationPeriod().GetInitValue() > 0.0)
		{
			if (!SetInterpolationPeriod(mConfig.GetInterpolationPeriod().GetInitValue()))
			{
				AddConfigErrorMessage("The interpolation period requested in config would require\n"
				           "less than one interpolation point per frame");
				return false;
			}
		}
		else {
			AddConfigErrorMessage("Neither the number of interpolation points per frame nor the \n"
			           "interpolation period requested in configuration are valid.");
			return false;
		}

		if (mConfig.GetNMemoryPoints() > 0 )
		{
			SetNMemoryPoints(mConfig.GetNMemoryPoints());
		}
		else if ( mConfig.GetIntegrationLength().GetInitValue() > 0.0  )
		{
			if (!SetIntegrationLength(mConfig.GetIntegrationLength().GetInitValue()))
			{
				AddConfigErrorMessage("The integration length requested leads"
				           "to a non-positive number of memory points.");
				return false;
			}
		}
		else
		{
			AddConfigErrorMessage("Neither the integration length nor the number of memory points"
			           "requested in configuration are valid.");
			return false;
		}

		SetNormalLevel(mConfig.GetNormalLevel().GetInitValue());

		SetSilenceLevel(mConfig.GetSilenceLevel().GetInitValue());

		mSampleDelta = 1000.0 / (TTime)mConfig.GetSampleRate();

		mDeltaX = (mSampleDelta * TTime(mFrameSize)) / TTime(mPointsPerFrame);

		InitializeControls();

		Input.SetSize(mFrameSize);
		return true;
	}

	void EnvelopeExtractor::CleanSilence()
	{
		int i,first,end;

		if (mIsSpline) {
			first = 3;
			end = mPointsPerFrame+2;
		}
		else {
			first = 1;
			end = mPointsPerFrame;
		}

		for (i=first; i<=end; i++)
			if (mInterpolationPoints[i] < mSilenceLevel)
				mInterpolationPoints[i] = 0.0;
			else 
				mInterpolationPoints[i] -= mSilenceLevel;
	}

	void EnvelopeExtractor::WriteEnvelope(BPFTmpl<TTime,TData> &bpf)
	{
		int i,ipos;

		if (mIsSpline) ipos=1;
		else           ipos=0;

		for (i=0; i<=mPointsPerFrame; i++)
			bpf.SetValue(i,mInterpolationPoints[ipos++]);

		if (mIsSpline) {
			bpf.SetLeftDerivative(
			      (mInterpolationPoints[2]-mInterpolationPoints[0]) / mDeltaX);
			bpf.SetRightDerivative(	
				(mInterpolationPoints[mPointsPerFrame+2] - 
				 mInterpolationPoints[mPointsPerFrame]     ) / mDeltaX);
			bpf.UpdateSplineTable();
		}
	}

	void EnvelopeExtractor::StoreInterpolationPoints()
	{
		mInterpolationPoints[0] = mInterpolationPoints[mPointsPerFrame];
		if (mIsSpline) {
			mInterpolationPoints[1] = mInterpolationPoints[mPointsPerFrame+1];
			mInterpolationPoints[2] = mInterpolationPoints[mPointsPerFrame+2];
		}
	}




	void EnvelopeExtractor::InterpolationPeriodChange(TControlData val)
	{
		SetInterpolationPeriod(mIpMin + val * mIpFactor);
		mInterpolationPeriodControl = val;
	}

	void EnvelopeExtractor::IntegrationLengthChange(TControlData val)
	{
		SetIntegrationLength(mIlMin + val * mIlFactor);
		mIntegrationLengthControl = val;
	}

	void EnvelopeExtractor::NormalLevelChange(TControlData val)
	{
		SetNormalLevel(mNlMin + val * mNlFactor);
		mNormalLevelControl = val;
	}

	void EnvelopeExtractor::SilenceLevelChange(TControlData val)
	{
		SetSilenceLevel(mSlMin + val * mSlFactor);
		mSilenceLevelControl = val;
	}

	void EnvelopeExtractor::InitializeControls(void)
	{
		mIpMin    = mConfig.GetInterpolationPeriod().GetMinValue();
		mIpFactor = mConfig.GetInterpolationPeriod().GetMaxValue() - mIpMin;
		mInterpolationPeriodControl = 
			(mConfig.GetInterpolationPeriod().GetInitValue() -mIpMin) / mIpFactor;

		mIlMin    = mConfig.GetIntegrationLength().GetMinValue();
		mIlFactor = mConfig.GetIntegrationLength().GetMaxValue() - mIlMin;
		mIntegrationLengthControl = 
			(mConfig.GetIntegrationLength().GetInitValue() - mIlMin) / mIlFactor;

		mNlMin    = mConfig.GetNormalLevel().GetMinValue();
		mNlFactor = mConfig.GetNormalLevel().GetMaxValue() - mNlMin;
		mNormalLevelControl = 
			(mConfig.GetNormalLevel().GetInitValue() - mNlMin) / mNlFactor;

		mSlMin    = mConfig.GetSilenceLevel().GetMinValue();
		mSlFactor = mConfig.GetSilenceLevel().GetMaxValue() - mSlMin;
		mSilenceLevelControl = 
			(mConfig.GetSilenceLevel().GetInitValue() - mSlMin) / mSlFactor;

	}

	bool EnvelopeExtractor::Do()
	{ 
		bool res = Do(Input.GetData(),Output.GetData()); 
		Input.Consume();
		Output.Produce();
		return res;
	}

	bool EnvelopeExtractor::Do(const Audio& inp, Envelope& env)
	{
		CLAM_ASSERT(inp.GetSize() == mFrameSize,
					"EnvelopeExtractor::Do(): Wrong audio size.");

		Array<TData> &audio   = inp.GetBuffer();
		BPFTmpl<TTime,TData> &bpf = env.GetAmplitudeBPF();
		int i,ipos,interval_start=0, interval_end;

		ConfigureEnvelope(bpf);

		if (mIsSpline) ipos=3;
		else           ipos=1;

		for (i=1; i<=mPointsPerFrame; i++) {
			interval_end  = (mFrameSize*i)/mPointsPerFrame;
			mAmplitudeAverages.Compute(i,audio,interval_start,interval_end);
			interval_start = interval_end;
			mInterpolationPoints[ipos++] = mAmplitudeAverages.Acumulated(i) / 
				(mNMemoryPoints*mNormalLevel);
		}
		if (mSilenceLevel > 0.0)
			CleanSilence();
		WriteEnvelope(bpf);
		StoreInterpolationPoints();
		mAmplitudeAverages.AdvanceFrame();
		return true;
	}
}

