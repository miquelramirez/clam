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

/** @file StreamBufferExample.cxx
 *
 *  This file ilustrates the usage of StreamBuffers. Note that you
 *  will not usually want to work with StreamBuffers directly. See
 *  NodeExample.cxx for a simpler way to achieve the same thing,
 *  completely avoiding StreamRegions management.
 *
 *  The application just reads audio frames of a certain size, makes a
 *  transform with a small hop size and some overlap, makes the
 *  inverse transform and writes the output (i.e, just some delay...)
 *
 *  The only task of the StreamBuffer in this example is hidding the
 *  circular buffer management, and making sure that the buffer is
 *  accessed in a consistent way.
 *
 *  Of course, scheduling is not yet automatic, it has to be
 *  implemented by hand...
 */

#include "AudioIO.hxx"

#include "SpectralAnalysisLite.hxx"
#include "SpectralSynthesisLite.hxx"
#include "StreamBuffer.hxx"
#include "CircularStreamImpl.hxx"

#include <iostream>

using namespace CLAM;

int main()
{
	int i;
	int source_size      = 512;  // Size of audio frames read from the input
	int hop_size         = 256;  // Hop size for analysis and synthesis
	int analysis_size    = 1001; // Size of the analysis window size
	int padded_size      = 2047; // Size of the audio fed to the transform.
	int destination_size = 512;  // Size of audio frames fed to the output.
	int spect_size       = padded_size/2+1;

	try {

		// Data and processing configuration. Nothing special here...

		AudioManager manager(22000);
		AudioIOConfig iocfg;
		iocfg.SetFrameSize(source_size);
		AudioIn  source(iocfg);
		iocfg.SetFrameSize(destination_size);
		AudioOut destination(iocfg);

		SpectralAnalysisConfig acfg;
		acfg.SetAnalysisWindowSize(analysis_size);
		acfg.SetHopSize(hop_size);
		acfg.SetTransformAudioSize(padded_size);

		SpectralAnalysis  analysis(acfg);

		SpectralSynthesisConfig scfg;
		scfg.SetTransformAudioSize(padded_size);
		scfg.SetAnalysisHopSize(hop_size);
		
		SpectralSynthesis synthesis(scfg);

		Audio    input_audio;
		Audio    analysis_audio;
		Audio    synthesis_audio;
		Audio    output_audio;

		Spectrum spectrum;
		spectrum.AddComplexArray();
		spectrum.UpdateData();
		spectrum.SetSize(spect_size);

		// Here comes the interesting part: buffer configuration.

		StreamBuffer<Audio,CircularStreamImpl<TData> > input_buffer;
		WriteStreamRegion *input_writer = input_buffer.NewWriter(source_size, source_size);
		ReadStreamRegion  *input_reader = input_buffer.NewReader(hop_size, analysis_size);
		input_buffer.Configure(analysis_size*5);

		StreamBuffer<Audio,CircularStreamImpl<TData> > output_buffer;
		WriteStreamRegion *output_writer = output_buffer.NewWriter(hop_size, padded_size);
		ReadStreamRegion  *output_reader = output_buffer.NewReader(destination_size,destination_size);
		output_buffer.Configure(padded_size*5);

		source.Start();
		destination.Start();
		analysis.Start();
		synthesis.Start();

		// We need to get some extra buffers for the first analysis execution,
		// because the analysis window size is bigger than the input frames.
		for (i=0; i<3; i++)
		{
			input_buffer.GetAndActivate(input_writer,input_audio);
			source.Do(input_audio);
			input_buffer.LeaveAndAdvance(input_writer);
		}

		while (1)
		{

			input_buffer.GetAndActivate(input_writer,input_audio);
			source.Do(input_audio);
			input_buffer.LeaveAndAdvance(input_writer);


			// The hop size is half the input/output frame size, so we
			// need to execute the analysis and synthesis twice for
			// each input/output frame.
			for (i=0; i<2; i++) {

				input_buffer.GetAndActivate(input_reader,analysis_audio);
				analysis.Do(analysis_audio,spectrum);
				input_buffer.LeaveAndAdvance(input_reader);

				// Your spectral processing would go here...

				output_buffer.GetAndActivate(output_writer,synthesis_audio);
				synthesis.Do(spectrum,synthesis_audio);
				output_buffer.LeaveAndAdvance(output_writer);

			}

			output_buffer.GetAndActivate(output_reader,output_audio);
			destination.Do(output_audio);
			output_buffer.LeaveAndAdvance(output_reader);
		}

		source.Stop();
		destination.Stop();
		analysis.Stop();
		synthesis.Stop();

	}
	catch(std::exception &e)
	{
		std::cout << e.what();
		return 1;
	}

	return 0;
}
