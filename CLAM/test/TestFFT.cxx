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

/** WARNING WARNING WARNING
 * Don't take this file as a good example of processing object test yet.
 * It will be re-made using a better design when XML load is available.
 * This test now only works on linux using octave as an external utility.
 */

#include<list>
#include<cstdlib>
#include <iostream>

#include "FFT.hxx"
#include "FFT_rfftw.hxx"
#include "FFT_numrec.hxx"
#include "OctaveVector.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "Audio.hxx"

using namespace CLAM;

using CLAM::FFT;
using CLAM::FFT_base;
using CLAM::FFTConfig;
using CLAM::FFT_rfftw;
using CLAM::FFT_numrec;
using CLAM::Err;

namespace CLAMTest {


	namespace FFT {


		typedef std::list<FFT_base*> FFTList;
		typedef std::list<Array<Complex >*> ArrayList;
		typedef std::list<Spectrum*> SpectrumList;
		typedef std::list<SpecTypeFlags*> STypeList;
		typedef std::list<Audio*> AudioList;

		/** Test pair (will be used in future). */
		struct TestPair {
			Audio *input;
			Spectrum *output;
			TestPair(Audio *a, Spectrum *s) {input=a; output=s;}
		};


		/** Test commands (will be removed in future) */
		char* octave_command = "/usr/bin/octave -q ";
		char* octave_script = "TestFFT.m";

		int DumpOctaveArrays(const ArrayList &l)
		{
			return 0;
		}

		void SpectrumTypeList(STypeList &prototypes)
		{
			SpecTypeFlags flags;
			int i;

			prototypes.clear();

			// Generate a list with all possible flags combinations.
			for (i=1; i < (1<<4); i++) {
				if (i&(1<<(int)SpecTypeFlags::eComplex))
					flags.bComplex=1;
				else 
					flags.bComplex=0;
				if (i&(1<<(int)SpecTypeFlags::ePolar))
					flags.bPolar=1;
				else 
					flags.bPolar=0;
				if (i&(1<<(int)SpecTypeFlags::eMagPhase))
					flags.bMagPhase=1;
				else 
					flags.bMagPhase=0;
				if (i&(1<<(int)SpecTypeFlags::eMagPhaseBPF))
					flags.bMagPhaseBPF=1;
				else 
					flags.bMagPhaseBPF=0;
				prototypes.push_back(new SpecTypeFlags(flags));
			}
		}

		void LoadFFTTestInput(AudioList &inputs)
		{
			int i,n;
			// 1 random tests: Default settings: 1024, 1 channel, TData representation.
			for (n=0;n<1;n++) {
				Audio *a=new Audio();
				a->SetSize(1024);
				inputs.push_back(a);
				for (i=0; i<a->GetSize(); i++)
					a->GetBuffer().GetPtr()[i]=10.0*rand()/(RAND_MAX);
			}
		}


		int ReliableCheck(const Audio &input, Spectrum &output)
		{
			// Generate Octave/Matlab file from input and output
//		std::string filename = tmpnam("FFTTest");
			std::string filename = "FFTTest.data";
			std::ofstream test_file(filename.c_str());

			SpecTypeFlags flags;
			flags.bComplex=1;
			output.SetTypeSynchronize(flags);
		
			WriteOctaveVector(test_file,input.GetBuffer(),"Audio");
			WriteOctaveVector(test_file,output.GetComplexArray(),"Spectrum");
			test_file.close();


		// Run the test.
			std::string command = octave_command;
			command+=+octave_script;
			return system(command.c_str());
//			return 0;
		}
	
		/** Test routine for FFT objects.
		 * @param l: list of pointers to FFT objects;
		 * @return 0 if all the objects pass the tests, non-0 if any of the objects 
		 * fails any of the tests.
		 */
		int TestFFTs(const FFTList &ffts)
		{
			AudioList inputs;
			FFTList::const_iterator f;
			AudioList::iterator a;
			STypeList prototypes;
			STypeList::iterator st;
			int p;

			// Creation of inputs.
			LoadFFTTestInput(inputs);

			// Configuration of output object.
			SpectrumConfig ssets;
			ssets.SetSize(1024/2+1);
			Spectrum output(ssets);

			for (f=ffts.begin(); f!=ffts.end(); f++) { // Implementation loop.
				std::cout << "Running " << (*f)->GetName() << std::endl;

				// Creation of list of prototypes. In a far future it
				// should be possible to extract it from the processing
				// object ports (even with multiple inputs/outputs).
				SpectrumTypeList(prototypes);
				p=1;

				for (st=prototypes.begin(); st!=prototypes.end(); st++) { // Spectrum Prototype loop.

					output.SetType((**st));
					std::cout << " Prototype " << p << " (default state):";

					for (a=inputs.begin(); a!=inputs.end(); a++) { // Input loop with no state change
						(*f)->Start();
						(*f)->Do(**a,output);
						if (ReliableCheck(**a,output))
							return 1;
						(*f)->Stop();
					}

					std::cout << std::endl << " Prototype " << p++ << " (changing state):";

					for (a=inputs.begin(); a!=inputs.end(); a++) { // Input loop with state change.
						(*f)->Start();
						(*f)->SetPrototypes(**a,output);
						(*f)->Do(**a,output);
						if (ReliableCheck(**a,output))
							return 1;
						(*f)->Stop();
						// Revert to default state.
						(*f)->UnsetPrototypes();
					}

					std::cout << std::endl;
				
				}
				std::cout << " passed all. " << std::endl;
			}
			return 0;
		}
	}

}

int main ()
{

	try {
		CLAMTest::FFT::FFTList objs;
		FFTConfig conf;
		FFT_base *f1,*f2,*f3;

		conf.SetAudioSize(1024);
		conf.SetName("Default FFT");
		f1=new FFT(conf);
		conf.SetName("FFT_rfftw");
		f2=new FFT_rfftw(conf);
		conf.SetName("FFT_numrec");
		f3=new FFT_numrec(conf);

		objs.push_back(f1);
		objs.push_back(f2);
		objs.push_back(f3);
		int res =  CLAMTest::FFT::TestFFTs(objs);
		if (res)
			std::cout << std::endl << "Failed!" << std::endl;
		return res;
	}
	catch (Err er) {
		er.Print();
		return 1;
	}
}

