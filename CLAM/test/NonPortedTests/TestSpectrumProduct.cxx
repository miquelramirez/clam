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

/* WARNING WARNING WARNING: This test will only pass if CLAM_DOUBLE is defined */

#include <fstream>
#include <cstdlib> //rand()
#include "SpectrumProduct.hxx"
#include "TestUtils.hxx"
#include "OctaveVector.hxx"
//TODO: To be unquoted when we use gcc 3.0
//#include <limits> 



namespace CLAMTest {
	static const unsigned int ArraySize=40;

	// Loads the vectors from a file, if they have not been previously loaded,
	// and sets the arguments to the loaded values.
	bool SetTestVectors(Spectrum &s1, Spectrum &s2, Spectrum &s3)
	{
		bool remove1=false,remove2=false,remove3=false;

		static bool loaded=false;
		static Spectrum orig1,orig2,orig3;

		if (!loaded) {
			std::ifstream file("SpectrumProductTest.data");
			SpecTypeFlags f;
			f.bMagPhase=false;
			f.bComplex=true;
			orig1.SetSize(ArraySize);
			orig2.SetSize(ArraySize);
			orig3.SetSize(ArraySize);
			orig1.SetType(f);
			orig2.SetType(f);
			orig3.SetType(f);
			if (file) {
				std::cerr << "Loading Input1..." << std::endl;
				if (!CLAMTest::LoadOctaveComplexVector(file,"Input1",orig1.GetComplexArray())) {
					std::cerr <<  "Could not find Input1 vector in input!" << std::endl;
					return false;
				}
				std::cerr << "Loading Input2..." << std::endl;
				if (!CLAMTest::LoadOctaveComplexVector(file,"Input2",orig2.GetComplexArray())) {
					std::cerr <<  "Could not find Audio vector in input!" << std::endl;
					return false;
				}
				std::cerr << "Loading Output..." << std::endl;
				if (!CLAMTest::LoadOctaveComplexVector(file,"Output",orig3.GetComplexArray())) {
					std::cerr <<  "Could not find Audio vector in input!" << std::endl;
					return false;
				}
				file.close();
			}
			else {
				std::cerr << "Generating Inputs and output..." << std::endl;
				for (int i = 0; i < ArraySize; i++) {
					orig1.GetComplexArray().GetPtr()[i] = 
						Complex(10.0*(TData)rand()/(TData)RAND_MAX,
								 10.0*(TData)rand()/(TData)RAND_MAX);
					orig2.GetComplexArray().GetPtr()[i] = 
						Complex(10.0*(TData)rand()/(TData)RAND_MAX,
								 10.0*(TData)rand()/(TData)RAND_MAX);
					orig3.GetComplexArray().GetPtr()[i] = 
						orig1.GetComplexArray().GetPtr()[i] *
						orig2.GetComplexArray().GetPtr()[i];
				}
			}
			loaded=true;
		}

		SpecTypeFlags spec;

		s1.GetType(spec);
		if (!spec.bComplex) {
			spec.bComplex=1;
			s1.SetType(spec);
			remove1=true;
		}
		s2.GetType(spec);
		if (!spec.bComplex) {
			spec.bComplex=1;
			s2.SetType(spec);
			remove2=true;
		}
		s3.GetType(spec);
		if (!spec.bComplex) {
			spec.bComplex=1;
			s3.SetType(spec);
			remove3=true;
		}

		s1.SetComplexArray(orig1.GetComplexArray());
		s2.SetComplexArray(orig2.GetComplexArray());
		s3.SetComplexArray(orig3.GetComplexArray());

		spec.bMagPhase=spec.bPolar=spec.bMagPhaseBPF=false;
		s1.SynchronizeTo(spec);
		s2.SynchronizeTo(spec);
		s3.SynchronizeTo(spec);

		if (remove1) {
			s1.GetType(spec);
			spec.bComplex=false;
			s1.SetType(spec);
		}
		if (remove2) {
			s2.GetType(spec);
			spec.bComplex=false;
			s2.SetType(spec);
		}
		if (remove3) {
			s3.GetType(spec);
			spec.bComplex=false;
			s3.SetType(spec);
		}
		return true;
	}

	void SetPrototypes(int proto, Spectrum &s)
	{
		SpecTypeFlags f;
		f.bComplex=proto&(1<<(int)SpecTypeFlags::eComplex);
		f.bPolar=proto&(1<<(int)SpecTypeFlags::ePolar);
		f.bMagPhase=proto&(1<<(int)SpecTypeFlags::eMagPhase);
		f.bMagPhaseBPF=proto&(1<<(int)SpecTypeFlags::eMagPhaseBPF);
		s.SetType(f);
	}

	std::string ProtoName(int proto)
	{
		std::string str;
		for (int i=0; i<4; i++)
			if (proto&(1<<i)) {
				str+=SpecTypeFlags::sFlagValues[i].name;
				str+=' ';
			}
		return str;
	}

	bool TestProduct(SpectrumProduct &product)
	{
		SpectrumConfig cfg;
		int proto1,proto2,proto3;
		static int count=0;
#ifdef CLAM_DOUBLE
		TData max_err = 0.00005;
#else
		TData max_err = 0.00039f;
#endif
		//TODO: To be unquoted when we use gcc 3.0
		//TData max_err = std::numeric_limits<TData>::epsilon()*TData(1e3);
		product.Start();

		cfg.AddBPFSize();
		cfg.UpdateData();
		cfg.SetSize(ArraySize);
		cfg.SetBPFSize(ArraySize);
		Spectrum in1(cfg),in2(cfg),out(cfg),out_good(cfg);

		
		for (proto1=1; proto1<16; proto1++)
			for (proto2=1; proto2<16; proto2++)
				for (proto3=1; proto3<16; proto3++) {
					std::cerr << "Test " << count++ << '(' 
							  << proto1 << ',' << proto2 << ',' << proto3 << ')' <<std::endl;
					std::cerr << " in1 attributes: " << ProtoName(proto1) << std::endl
							  << " in2 attributes: " << ProtoName(proto2) << std::endl
							  << " out attributes: " << ProtoName(proto3) << std::endl;
					if (proto3 & 8) { // Skip BPF outputs for now...
						std::cerr << "skipped " << std::endl;
						continue;
					}
					SetPrototypes(proto1,in1);
					SetPrototypes(proto2,in2);
					SetPrototypes(proto3,out);
					SetTestVectors(in1,in2,out_good);
					product.SetPrototypes(in1,in2,out);
					product.Do(in1,in2,out);
					TData diff = CLAMTest::TestUtils::MaxDiff(out,out_good);
					std::cerr << "Tolerance: " << max_err << std::endl;
					std::cerr << "Difference: " << diff << std::endl;;
					if (diff > max_err)
						return false;
					std::cerr.flush();
				}
		return true;
	}

}


int main()
{
	try {
		CLAM::SpectrumProduct prod;
		bool res = CLAMTest::TestProduct(prod);
		if (res)
			std::cerr << std::endl << "Passed." << std::endl;
		else
			std::cerr << std::endl << "Failed." << std::endl;
		return !res;
	}
	catch (CLAM::Err e) {
		e.Print();
	}
}

