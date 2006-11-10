/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "SMSAnalysis.hxx"
#include "analysis_utils.hxx"

using namespace CLAM;

namespace qtvm_examples_utils
{
    void analyze(const Audio& in, Segment& out, bool harmonic)
    {
		out.AddAudio();
		out.UpdateData();

		out.SetAudio(in);
		out.SetBeginTime(0);
		out.SetSamplingRate(out.GetAudio().GetSampleRate());
		out.mCurrentFrameIndex=0;

		SMSAnalysisConfig cfg;
		cfg.SetSinWindowType(EWindowType::eBlackmanHarris92);
		cfg.SetResWindowType(EWindowType::eBlackmanHarris92);
		cfg.SetSinWindowSize(2049);
		cfg.SetResWindowSize(1025);
		cfg.SetHopSize(256);
		cfg.SetSinZeroPadding(2);
		cfg.SetResZeroPadding(0);
		cfg.SetSamplingRate(out.GetSamplingRate());
		cfg.GetPeakDetect().SetMagThreshold(-120.0);
		if(harmonic) cfg.GetSinTracking().SetIsHarmonic(true);

		SMSAnalysis analyzer;
		analyzer.Configure(cfg);
	
		analyzer.Start();
		while(analyzer.Do(out))
		{
			printf(".");
		}         
		analyzer.Stop();
		out.SetEndTime(out.GetAudio().GetSize()/out.GetAudio().GetSampleRate());
    }   
}

// END

