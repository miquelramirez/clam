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

#include "Spectrum.hxx"
#include "SpectrumComp.hxx"
#include "XMLTestHelper.hxx"
#include "Point.hxx"
#include "GlobalEnums.hxx"
#include "Array.hxx"
#include "Complex.hxx"
#include "Point.hxx"
#include "BPF.hxx"
#include "Polar.hxx"
#include "DataTypes.hxx"
#include <list>

using CLAM::TData;
using CLAM::Polar;
using CLAM::Point;
using CLAM::Complex;
using CLAM::BPF;
using CLAM::EScale;
using CLAM::SpectrumConfig;
using CLAM::Spectrum;
using CLAM::SpecTypeFlags;

namespace CLAMTest {


	class SpectrumTest {
		std::string reason;
		int confidx;
		int protoidx;
		bool SpecConfCmp(const SpectrumConfig &c1,const SpectrumConfig &c2);
		void ResetConfGen();
		void SetFlagsLowLevel(Spectrum &s);
		SpectrumConfig GenConfig();
		void ResetFlagsGen();
		SpecTypeFlags GenFlags();
		void DumpConfig(const SpectrumConfig &cfg);
		bool TestSetGet(Spectrum &s);
		const std::string &ProtoName(int proto);
		const std::string &ProtoName(const SpecTypeFlags &flags);
	public:
		SpectrumTest() : confidx(0) {};
		bool Do();
		bool TestConstruction();
		bool TestConfiguration();
		bool TestInstantiationAndGetSet();
		bool TestSynchronization();
		bool TestScales();
		bool TestCopies();
		const std::string &Why() {return reason;}
		bool TestXML();
	};

	void SpectrumTest::ResetConfGen() {confidx = protoidx = 0;}
	void SpectrumTest::ResetFlagsGen() {protoidx = 0;}

	const std::string &SpectrumTest::ProtoName(int proto)
	{
		static std::string str;
		str="";
		for (int i=0; i<4; i++)
			if (proto&(1<<i)) {
				str+=SpecTypeFlags::sFlagValues[i].name;
				str+=' ';
			}
		return str;
	}

	const std::string &SpectrumTest::ProtoName(const SpecTypeFlags &flags)
	{
		static std::string str;
		str="";
		for (int i=0; i<4; i++)
			if (flags[i]) {
				str+=SpecTypeFlags::sFlagValues[i].name;
				str+=' ';
			}
		return str;
	}


	// This method generates a different spectrum config object each
	// time it is invoqued. The secuence can be restarted using the
	// ResetConfGen method.
	SpectrumConfig SpectrumTest::GenConfig()
	{
		static SpectrumConfig cfg;
		static int bpfsize=1;

		// Every two calls we change the scale
		if (!(confidx & 1)) { 
			if (cfg.GetScale() == EScale::eLinear) 
				cfg.SetScale(EScale::eLog);
			else
				cfg.SetScale(EScale::eLinear);
		}

		// Every four calls we change the flags.
		else if (!(confidx & 2)) { 
			cfg.SetType(GenFlags());
		}

		// Every eight calls we change the Size;
		else if (!(confidx & 4)) {
			cfg.SetSize(cfg.GetSize()/2 +
			            int(float(cfg.GetSize()) * float(rand()) / float(RAND_MAX)) );
		}
		
		// Every sixteen calls we change the spectral range;
		else if (!(confidx & 8)) {
			cfg.SetSpectralRange(cfg.GetSpectralRange()+10);
		}

		// Every other sixteen calls we change the BPFSize;
		else if (!cfg.HasBPFSize()) {
			cfg.AddBPFSize();
			cfg.UpdateData();
			cfg.SetBPFSize(bpfsize);
			bpfsize++;
		}
		else {
			cfg.RemoveBPFSize();
			cfg.UpdateData();
		}

		confidx++;
		return cfg;
	}
	
	// This method generates a different flags object each
	// time it is invoqued. The secuence can be restarted using the
	// ResetConfGen method.
	SpecTypeFlags SpectrumTest::GenFlags()
	{
		static SpecTypeFlags f;
		f.bComplex=protoidx&(1<<(int)SpecTypeFlags::eComplex);
		f.bPolar=protoidx&(1<<(int)SpecTypeFlags::ePolar);
		f.bMagPhase=protoidx&(1<<(int)SpecTypeFlags::eMagPhase);
		f.bMagPhaseBPF=protoidx&(1<<(int)SpecTypeFlags::eMagPhaseBPF);
		protoidx++;
		return f;
	}

	void SpectrumTest::SetFlagsLowLevel(Spectrum &s)
	{
		if (protoidx&(1<<(int)SpecTypeFlags::eComplex))
			s.AddComplexArray();
		else
			s.RemoveComplexArray();

		if (protoidx&(1<<(int)SpecTypeFlags::ePolar))
			s.AddPolarArray();
		else
			s.RemovePolarArray();

		if (protoidx&(1<<(int)SpecTypeFlags::eMagPhase)) {
			s.AddMagBuffer();
			s.AddPhaseBuffer();
		}
		else {
			s.RemoveMagBuffer();
			s.RemovePhaseBuffer();
		}

		if (protoidx&(1<<(int)SpecTypeFlags::eMagPhaseBPF)) {
			s.AddMagBPF();
			s.AddPhaseBPF();
		}
		else {
			s.RemoveMagBPF();
			s.RemovePhaseBPF();
		}

		s.UpdateData();
		s.SetSize(s.GetSize());
		s.SetBPFSize(5);
		protoidx++;
	}

	void SpectrumTest::DumpConfig(const SpectrumConfig &cfg)
	{
		std::cout << "  Configuration:" << std::endl;
		std::cout << "    Scale:         ";
		if (cfg.HasScale())
			std::cout << cfg.GetScale();
		else
			std::cout << " ---";
		std::cout << std::endl << "    SpectralRange: ";
		if (cfg.HasSpectralRange())
			std::cout << cfg.GetSpectralRange();
		else
			std::cout << " ---";
		std::cout << std::endl <<  "    Size:          ";
		if (cfg.HasSize())
			std::cout << cfg.GetSize();
		else
			std::cout << " ---";
		std::cout << std::endl <<  "    BPFSize:          ";
		if (cfg.HasBPFSize())
			std::cout << cfg.GetBPFSize();
		else
			std::cout << " ---";
		std::cout << std::endl <<  "    Type:          ";
		if (cfg.HasType())
			std::cout << ProtoName(cfg.GetType());
		else
			std::cout << " ---";
		std::cout << std::endl;
	}


	bool SpectrumTest::SpecConfCmp(const SpectrumConfig &c1,const SpectrumConfig &c2)
	{
		if (c1.HasScale() != c2.HasScale())
			return false;
		if (c1.HasSpectralRange() != c2.HasSpectralRange())
			return false;
		if (c1.HasSize() != c2.HasSize())
			return false;
		if (c1.HasType() != c2.HasType())
			return false;
		if (c1.HasBPFSize() != c2.HasBPFSize())
			return false;

		if (c1.HasScale() && (c1.GetScale() != c2.GetScale()))
			return false;
		if (c1.HasSpectralRange() && (c1.GetSpectralRange() != c2.GetSpectralRange()))
			return false;
		if (c1.HasSize() && (c1.GetSize() != c2.GetSize()))
			return false;
		if (c1.HasType() && (c1.GetType() != c2.GetType()))
			return false;
		if (c1.HasBPFSize() && (c1.GetBPFSize() != c2.GetBPFSize()))
			return false;

		return true;
	}


	bool SpectrumTest::TestSetGet(Spectrum &s)
	{
		int size=s.GetSize();
		int bpfsize=s.GetBPFSize();

		int i;
		if (s.HasComplexArray()) {
			CLAM::Array<Complex> array(size);
			array.SetSize(size);
			for (i=0; i<size; i++)
				array[i]=Complex(i,i);
			s.SetComplexArray(array);
			if (!(array == s.GetComplexArray())) {
				reason="GetComplex returns an array different to the set one";
				return false;
			}
		}
		if (s.HasPolarArray()) {
			CLAM::Array<Polar> array(size);
			array.SetSize(size);
			for (i=0; i<size; i++)
				array[i]=Polar(i,i);
			s.SetPolarArray(array);
			if (!(array == s.GetPolarArray())) {
				reason="GetPolar returns an array different to the set one";
				return false;
			}
		}
		if (s.HasMagBuffer()) {
			CLAM::Array<TData> array;
			array.Resize(size);
			array.SetSize(size);
			for (i=0; i<size; i++)
				array[i]=i;
			s.SetMagBuffer(array);
			for (i=0; i<size;i++)
				if (array[i] != s.GetMagBuffer()[i]) {
					reason="GetMagBuffer returns an array different to the set one";
					return false;
				}
		}
		if (s.HasPhaseBuffer()) {
			CLAM::Array<TData> array;
			array.Resize(size);
			array.SetSize(size);
			for (i=0; i<size; i++)
				array[i]=i;
			s.SetPhaseBuffer(array);
			for (i=0; i<size; i++)
				if (array[i] != s.GetPhaseBuffer()[i]) {
					reason="GetPhaseBuffer returns an array different to the set one";
					return false;
				}
		}
		if (s.HasMagBPF()) {
			if (bpfsize <= 0) {
				reason="MagBPF is added with BPFSize set, but GetBPFSize returns <=0";
				return false;
			}
			BPF array;
			array.Resize(bpfsize);
			array.SetSize(bpfsize);
			for (i=0; i<bpfsize; i++)
				array.SetValue(i,i);
			s.SetMagBPF(array);
			for (i=0; i<bpfsize;i++)
				if (array.GetValueFromIndex(i) != s.GetMagBPF().GetValueFromIndex(i)) {
					reason="GetMagBPF returns an array different to the set one";
					return false;
				}
		}
		if (s.HasPhaseBPF()) {
			if (bpfsize <= 0) {
				reason="PhaseBPF is added with BPFSize set, but GetBPFSize returns <=0";
				return false;
			}
			BPF array;
			array.Resize(bpfsize);
			array.SetSize(bpfsize);
			for (i=0; i<bpfsize; i++)
				array.SetValue(i,i);
			s.SetPhaseBPF(array);
			for (i=0; i<bpfsize; i++)
				if (array.GetValueFromIndex(i) != s.GetPhaseBPF().GetValueFromIndex(i)) {
					reason="GetPhaseBPF returns an array different to the set one";
					return false;
				}
		}
		return true;
	}

	bool SpectrumTest::TestConstruction()
	{
		// Try with default constructors
		Spectrum spec1;

		SpectrumConfig cfg;
		Spectrum spec2(cfg);
		
		SpectrumConfig cfg1;
		spec1.GetConfig(cfg1);
		SpectrumConfig cfg2;
		spec2.GetConfig(cfg2);

		if (!SpecConfCmp(cfg1,cfg)) {
			reason="Configuration differs after construction!";
// DISABLED:
//			return false;
		}
		if (!SpecConfCmp(cfg1,cfg2)) {
			reason="Default construction differs from default configuration construction";
			return false;
		}
		
		for (int i =0; i<10000; i++) {
			std::cout << "Iteration: " << i << std::endl;
			cfg = GenConfig();
			DumpConfig(cfg);
			Spectrum spec3(cfg);
		}
		return true;
	}

	bool SpectrumTest::TestConfiguration()
	{
		SpectrumConfig cfg,cfg2;
		Spectrum spec;
		ResetConfGen();
		for (int i = 0; i<10000; i++) {
			cfg.SetType(GenFlags());
			cfg.SetSize(int(2048.0*float(rand())/float(RAND_MAX)));
			std::cout << "Iteration: " << i << std::endl;
			DumpConfig(cfg);
			spec.Configure(cfg);
			spec.GetConfig(cfg2);
			if (!SpecConfCmp(cfg,cfg2)) {
				reason="Config argument differs from getconfig result.";
				return false;
			}
		}
		return true;
	}

	bool SpectrumTest::TestInstantiationAndGetSet()
	{
		SpectrumConfig cfg;
		cfg.AddBPFSize();
		cfg.UpdateData();
		cfg.SetBPFSize(5);
		cfg.SetSize(20);
		Spectrum spec(cfg);
		ResetConfGen();
		for (int i = 0; i<10000; i++) {
			std::cout << "Iteration: " << i << std::endl;
			SetFlagsLowLevel(spec);
			spec.GetConfig(cfg);
			DumpConfig(cfg);
			if (!TestSetGet(spec))
				return false;
		}
		return true;
	}
	bool SpectrumTest::TestSynchronization()
	{
		std::cout << "(not implemented)";
		return false;
	}
	bool SpectrumTest::TestScales()
	{
		std::cout << "(not implemented)";
		return false;
	}
	bool SpectrumTest::TestCopies()
	{
		std::cout << "(not implemented)";
		return false;
	}
	bool SpectrumTest::TestXML() {
		bool ok=true;
		Spectrum s;
		if (!XMLInputOutputMatches(s, "TestSpectrum1.xml")) {
			reason+="XML output for a default constructed Spectrum is not recoverable\n";
			ok=false;
		}
		SpectrumConfig cfg;
		cfg.AddBPFSize();
		cfg.UpdateData();
		cfg.SetBPFSize(5);
		cfg.SetSize(20);
		cfg.GetType().bMagPhaseBPF=true;
		Spectrum spec(cfg);
		spec.GetMagBPF().Insert(Point(2.4,5.0));
		if (!XMLInputOutputMatches(spec, "TestSpectrum2.xml")) {
			reason+="XML output for modified Spectrum is not recoverable\n";
			ok=false;
		}

		return ok;
	}

	bool SpectrumTest::Do()
	{

		std::cout << "Doing XML test..." << std::endl;
		if (!TestXML()) {
			std::cout << "XML test failed!" << std::endl;
			return false;
		}
		std::cout << " passed!" << std::endl;


		std::cout << "Doing construction test..." << std::endl;
		if (!TestConstruction()) {
			std::cout << "Construction test failed!" << std::endl;
			return false;
		}
		std::cout << " passed!" << std::endl;

		std::cout << "Doing configuration test..." << std::endl;
		if (!TestConfiguration()) {
			std::cout << "Configuration test failed!" << std::endl;
			return false;
		}
		std::cout << " passed!" << std::endl;

		std::cout << "Doing instantiation test..." << std::endl;
		if (!TestInstantiationAndGetSet()) {
			std::cout << "Instantiation test failed!" << std::endl;
			return false;
		}
		std::cout << " passed!" << std::endl;


		return true;


	}



}


int main() {
	CLAMTest::SpectrumTest test;
	try {
		bool res = test.Do();
		if (!res) {
			std::cout << test.Why() << std::endl;
			return -1;
		}
		std::cout << "Passed." << std::endl;
		return 0;
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl
		          << "Failed." << std::endl;
		return -1;
	}
}

