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
 */

#include<list>
#include<cstdlib>

#include "IFFT.hxx"
#include "IFFT_rfftw.hxx"
//#include "FFT_numrec.hxx"
#include "OctaveVector.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"

using CLAM::IFFT;
using CLAM::IFFTConfig;
using CLAM::IFFT_rfftw;
using CLAM::IFFT_base;
using CLAM::Err;

namespace CLAMTest {


	namespace IFFT {

		using namespace CLAM;

		typedef std::list<IFFT_base*> IFFTList;
		typedef std::list<Array<Complex >*> ArrayList;
		typedef std::list<Spectrum*> SpectrumList;
		typedef std::list<SpecTypeFlags*> STypeList;
		typedef std::list<Spectrum*> SpectrumList;

		/** Test pair (will be used in future, loading from XML vectors).*/
		struct TestPair {
			Spectrum *input;
			Audio *output;
			TestPair(Spectrum *s,Audio *a) {input=s; output=a;}
		};


		/** Test commands (will be removed in future) */
		char* octave_command = "/usr/bin/octave -q ";
		char* octave_script = "TestIFFT.m";

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

		void SetIFFTTestInput(Spectrum &input,SpecTypeFlags *flags)
		{
			int i;
			bool needsmagphase = false;
			// 1 random tests: Default settings: 1024, 1 channel, TData representation.
			if (flags->bMagPhase)
				needsmagphase = true;
			else
				flags->bMagPhase=1;

			input.SetType(*flags);

			for (i=0; i<input.GetSize(); i++) {
				input.GetMagBuffer().GetPtr()[i]=10.0*rand()/(RAND_MAX);
				input.GetPhaseBuffer().GetPtr()[i]=10.0*rand()/(RAND_MAX);
//				input.GetMagBuffer().GetPtr()[i]=0;
//				input.GetPhaseBuffer().GetPtr()[i]=0;
			}
//			input.GetMagBuffer().GetPtr()[100]=999;

			
			input.SynchronizeTo(SpecTypeFlags());
			
			if (!needsmagphase) {
				flags->bMagPhase=0;
				input.SetType(*flags);
			}
		}

		int ReliableCheck(Spectrum &input, Audio &output)
		{
			// Generate Octave/Matlab file from input and output
//		std::string filename = tmpnam("IFFTTest");
			std::string filename = "IFFTTest.data";
			std::ofstream test_file(filename.c_str());

			SpecTypeFlags flags;
			flags.bComplex=1;
			input.SetTypeSynchronize(flags);
		
			WriteOctaveVector(test_file,output.GetBuffer(),"Audio");
			WriteOctaveVector(test_file,input.GetComplexArray(),"Spectrum");
			test_file.close();


		// Run the test.
			std::string command = octave_command;
			command+=+octave_script;
			return system(command.c_str());
//			return 0;
		}
	
		int CompareSpectrums(const Spectrum& a, const Spectrum &b)
		{
			return 1;
		}


		/** Test routine for IFFT objects.
	 * @param l: list of pointers to IFFT objects;
	 * @return 0 if all the objects pass the tests, non-0 if any of the objects 
	 * fails any of the tests.
	 */
		int TestIFFTs(const IFFTList &iffts)
		{
			IFFTList::const_iterator ifft;
			STypeList prototypes;
			STypeList::iterator st;
			int p;

			// Configuration of input object.
			SpectrumConfig ssets;
			ssets.SetSize(1024/2+1);
			Spectrum input(ssets);

			// Configuration of output object
			Audio output;
			output.SetSize(1024);

			for (ifft=iffts.begin(); ifft!=iffts.end(); ifft++) { // Implementation loop.
				std::cout << "Running " << (*ifft)->GetName() << std::endl;

				// Creation of list of prototypes. In a far future it
				// should be possible to extract it from the processing
				// object ports (even with multiple inputs/outputs).
				SpectrumTypeList(prototypes);
				p=1;

				for (st=prototypes.begin(); st!=prototypes.end(); st++) { // Spectrum Prototype loop.

					SetIFFTTestInput(input,*st);
					std::cout << " Prototype " << p << " (default state):";

					(*ifft)->Start();
					(*ifft)->Do(input,output);
					if (ReliableCheck(input,output))
						return 1;
					(*ifft)->Stop();

					std::cout << std::endl << " Prototype " << p++ << " (changing state):";

					(*ifft)->Start();
					(*ifft)->SetPrototypes(input,output);
					(*ifft)->Do(input,output);
					if (ReliableCheck(input,output))
						return 1;
					(*ifft)->Stop();
					// Revert to default state.
					(*ifft)->UnsetPrototypes();

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
	using namespace CLAM;

	try {
		CLAMTest::IFFT::IFFTList objs;
		IFFTConfig conf;
		IFFT_base *f1,*f2,*f3;

		conf.SetAudioSize(1024);
		conf.SetName("Default IFFT");
		f1=new IFFT(conf);
		conf.SetName("IFFT_rfftw");
		f2=new IFFT_rfftw(conf);
//		conf.SetName("IFFT_numrec");
//		f3=new IFFT_numrec(conf);

		objs.push_back(f1);
		objs.push_back(f2);
//		objs.push_back(f3);
		int res =  CLAMTest::IFFT::TestIFFTs(objs);
		if (res)
			std::cout << std::endl << "Failed!" << std::endl;
		return res;
	}
	catch (Err er) {
		er.Print();
		return 1;
	}
}

