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

/** @file NodeExample.cxx
 *
 *  This file ilustrates the usage of Audio Nodes, in the context of
 *  an spectral analysis/synthesis application.
 * 
 *  The application just reads audio frames of a certain size, makes a
 *  transform with a small hop size and some overlap, makes the
 *  inverse transform and writes the output (i.e, just some delay...)
 *  
 *  The interesting stuff is that all the audio sizes and hops are
 *  specified only once, when configuring the processing
 *  objects. Buffer pointer book-keeping, circular buffer management,
 *  and reading/writting region consistency checks are all hidden by
 *  ports/nodes.
 *
 *  If you want to see how things look if you don't want to use nodes,
 *  and use the non-supervised-mode execution API instead, have a look
 *  to the StreamBufferExample.cxx file.
 *
 *  Of course, scheduling is not yet automatic, it has to be
 *  implemented by hand...
 */


#include "AudioIO.hxx"

#include "SpectralAnalysisLite.hxx"
#include "SpectralSynthesisLite.hxx"
#include "NodeTmpl.hxx"
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

		// NODE STUFF

		NodeTmpl<Audio,CircularStreamImpl<TData> > input_node;
		NodeTmpl<Audio,CircularStreamImpl<TData> > output_node;

		    source.Output.Attach(input_node);
		   analysis.Input.Attach(input_node);
		  analysis.Output.Attach(spectrum);
		  synthesis.Input.Attach(spectrum);
		 synthesis.Output.Attach(output_node);
		destination.Input.Attach(output_node);

		input_node.Configure(analysis_size*5);
		output_node.Configure(padded_size*5);

		// There we go...

		     source.Start();
		destination.Start();
		   analysis.Start();
		  synthesis.Start();

		for (i=0; i<3; i++) // analysis window size is bigger than the input frames.
			source.Do();

		while (1)
		{
			source.Do();
			for (i=0; i<2; i++)  // hop_size is half the i/o frame size
			{
				analysis.Do();
				// Your spectral processing would go here...
				synthesis.Do();
			}
			destination.Do();
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
