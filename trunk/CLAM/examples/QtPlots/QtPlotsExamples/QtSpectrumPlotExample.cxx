/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "QtAppWrapper.hxx"
#include "QtSpectrumPlot.hxx"
#include "wave_utils.hxx"
#include "analysis_utils.hxx"

int main()
{
    CLAM::Audio audio;
    CLAM::Segment seg;

    TData frequency = TData(1000.0);
    TData amplitude = TData(0.7);
    TData duration = TData(0.5);
    TData sampleRate = TData(11025.0);

    printf("Buidding sine wave: freq=%.0f Hz amp=%.1f duration=%.1f sec. sample rate=%.0f Hz\n", 
		   frequency, amplitude, duration, sampleRate);
    qtvm_examples_utils::sine(frequency, amplitude, duration, sampleRate, audio);

    printf("Analysing");
    qtvm_examples_utils::analyze(audio,seg);
    printf("\ndone!\n");
	
    int index = seg.GetnFrames()/2;
    
    CLAM::VM::QtAppWrapper::Init();
	
    CLAM::VM::QtSpectrumPlot specPlot;
    specPlot.Label("Spectrum");
    specPlot.Geometry(100,100,500,225);
    specPlot.SetData(seg.GetFrame(index).GetSinusoidalAnalSpectrum());
    specPlot.SetForegroundColor(CLAM::VM::VMColor::Blue());
    specPlot.Show();
	
    return CLAM::VM::QtAppWrapper::Run();
}

// END



