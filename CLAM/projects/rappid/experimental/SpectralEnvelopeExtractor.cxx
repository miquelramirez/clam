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


#include "SpectralEnvelopeExtractor.hxx"

namespace CLAM {


	void SpectralEnvelopeExtractorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSpectralSize(512);
		SetSpectralRange(44100/2);
		SetnMaxPeaks(50);
		Synchronise();
	}

	void SpectralEnvelopeExtractorConfig::Synchronise()
	{
		GetDetectConfig().SetSpectralSize(GetSpectralSize());
		GetDetectConfig().SetSpectralRange(GetSpectralRange());
		GetDetectConfig().SetMaxPeaks(GetnMaxPeaks());
		GetGeneratorConfig().SetSpectralSize(GetSpectralSize());
		GetGeneratorConfig().SetSpectralRange(GetSpectralRange());
		GetGeneratorConfig().SetnMaxPeaks(GetnMaxPeaks());
	}

	SpectralEnvelopeExtractor::SpectralEnvelopeExtractor(const SpectralEnvelopeExtractorConfig& c)
		: Input("Input",this,1),
		  Output("Output",this,1),
		  MemoryFactor("MemoryFactor",this,&SpectralEnvelopeExtractor::MemoryFactorChange)
	{
		Configure(c);
	}

	int SpectralEnvelopeExtractor::MemoryFactorChange(TControlData value)
	{
		mGenerator.MemoryFactor.DoControl(value);
		return 0;
	}

	bool SpectralEnvelopeExtractor::ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const SpectralEnvelopeExtractorConfig&>(cfg);

		mConfig.Synchronise();

		mDetector.Configure(mConfig.GetDetectConfig());
		mGenerator.Configure(mConfig.GetGeneratorConfig());

		int max_peaks = mConfig.GetDetectConfig().GetMaxPeaks();

		mPeaks.AddIndexArray();
		mPeaks.UpdateData();
		mPeaks.GetMagBuffer().Resize   (max_peaks);
		mPeaks.GetMagBuffer().SetSize  (max_peaks);
		mPeaks.GetFreqBuffer().Resize (max_peaks);
		mPeaks.GetFreqBuffer().SetSize(max_peaks);
		mPeaks.GetIndexArray().Resize  (max_peaks);
		mPeaks.GetIndexArray().SetSize (max_peaks);
		mPeaks.SetnMaxPeaks            (max_peaks);

		return true;
	}

	bool SpectralEnvelopeExtractor::Do(void)
	{
		bool res = Do(Input.GetData(),Output.GetData());
		Input.LeaveData();
		Output.LeaveData();
		return res;
	}

	void SpectralEnvelopeExtractor::CheckData(const Spectrum& inp, Spectrum &out)
	{
						  
	}

	bool SpectralEnvelopeExtractor::Do(const Spectrum& inp, Spectrum &out)
	{
		CheckData(inp,out);
		mDetector.Do(inp,mPeaks);
		mGenerator.Do(mPeaks,out);
		return true;
	}



}
