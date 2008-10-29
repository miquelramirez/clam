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

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include "FDFilterGen.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include <iostream>

using namespace CLAM;


class FDControlsGenerator : public Processing {

	/** Private method called by the public Configure(cfg) method */
	bool ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig(mConfig, c);
			return true;
		}

public:
	FDControlsGenerator() :
		Gain("Gain",this),
		HighCutOff("High Cutoff Frecuency",this),
		LowCutOff("Low Cutoff Frecuency",this),
		PassBandSlope("Pass Band Slope",this),
		StopBandSlope("Stop Band Slope",this)
		{}
	const char * GetClassName() const {return "FDControlsGenerator";}

	bool Do(void)
		{
			Gain.SendControl(mConfig.GetGain());
			HighCutOff.SendControl(mConfig.GetHighCutOff());
			LowCutOff.SendControl(mConfig.GetLowCutOff());
			PassBandSlope.SendControl(mConfig.GetPassBandSlope());
			StopBandSlope.SendControl(mConfig.GetStopBandSlope());
			return true;
		}

	const ProcessingConfig &GetConfig() const {return mConfig;}

// public controls:
	FloatOutControl Gain, HighCutOff, LowCutOff, PassBandSlope, StopBandSlope;

private:
	FDFilterGenConfig mConfig;


};

void Set(Enum::tValue type, FDFilterGenConfig &cfg);

void Set(Enum::tValue type, FDControlsGenerator &fil)
{
	static FDFilterGenConfig cfg;

	Set(type,cfg);
	fil.Configure(cfg);

}		
		
void Set(Enum::tValue type, FDFilterGenConfig &cfg)
{
	cfg.SetSpectralRange(22050);
	switch(type) {
	case EFDFilterType::eLowPass:
		cfg.SetGain(1);
		cfg.SetLowCutOff(1000);
		cfg.SetStopBandSlope(3);
		break;
	case EFDFilterType::eHighPass:
		cfg.SetGain(1);
		cfg.SetHighCutOff(1000);
		cfg.SetPassBandSlope(3);
		break;
	case EFDFilterType::eBandPass:
		cfg.SetGain(1);
		cfg.SetHighCutOff(1000);
		cfg.SetPassBandSlope(3);
		cfg.SetLowCutOff(10000);
		cfg.SetStopBandSlope(3);
		break;
	case EFDFilterType::eStopBand:
		cfg.SetGain(1);
		cfg.SetLowCutOff(1000);
		cfg.SetStopBandSlope(3);
		cfg.SetHighCutOff(5000);
		cfg.SetPassBandSlope(3);
		break;
	}
}

void PrintPoints(Spectrum &spec)
{
	BPF &bpf = spec.GetMagBPF();
	std::cout << "  " << bpf.Size() << " points BPF:" << std::endl;
	for (int i=0; i<bpf.Size(); i++)
		std::cout << "     " << bpf.GetXValue(i) << ", " << bpf.GetValueFromIndex(i) << std::endl;
}

int Test(FDFilterGen &gen)
{
	SpectrumConfig sets;
	SpecTypeFlags f;
	f.bMagPhaseBPF=true;
	f.bMagPhase=false;
	sets.SetType(f);
	sets.SetScale(EScale::eLinear);
	Spectrum spec(sets);
	FDFilterGenConfig cfg,cfg2;
	Enum::tValue i;

	std::cout << "Trying Configure changes" << std::endl;

	for (i=0; i<4; i++) {
		std::cout << " " << EFDFilterType::ValueTable()[i].name << std::endl;
		Set(i,cfg);
		cfg.SetType(EFDFilterType(i));
		gen.Configure(cfg);
		gen.Start();
		gen.Do(spec);
		gen.Stop();
		PrintPoints(spec);
	}

	std::cout << "Trying Control changes" << std::endl;
	
	FDControlsGenerator controls;

	controls.GetOutControl(0).AddLink(gen.GetInControl(0));
	controls.GetOutControl(1).AddLink(gen.GetInControl(1));
	controls.GetOutControl(2).AddLink(gen.GetInControl(2));
	controls.GetOutControl(3).AddLink(gen.GetInControl(3));
	controls.GetOutControl(4).AddLink(gen.GetInControl(4));

	for (i=0; i<4; i++) {
		std::cout << " " << EFDFilterType::ValueTable()[i].name << std::endl;
		cfg.SetType(EFDFilterType(i));
		cfg2=dynamic_cast<const FDFilterGenConfig&>(gen.GetConfig());
		cfg2.SetType(EFDFilterType(i));
		gen.Configure(cfg2);
		Set(i,cfg);
		controls.Configure(cfg);
		gen.Configure(cfg2);
		gen.Start();
		controls.Start();
		controls.Do();
		gen.Do(spec);
		controls.Stop();
		gen.Stop();
		PrintPoints(spec);
	}


	return 0;
}

int TestContrChanges(FDFilterGen &gen)
{

	SpectrumConfig sets;
	SpecTypeFlags f;
	f.bMagPhaseBPF=true;
	f.bMagPhase=false;
	sets.SetType(f);
	Spectrum spec(sets);
	FDControlsGenerator controls;

	controls.GetOutControl(0).AddLink(gen.GetInControl(0));
	controls.GetOutControl(1).AddLink(gen.GetInControl(1));
	controls.GetOutControl(2).AddLink(gen.GetInControl(2));
	controls.GetOutControl(3).AddLink(gen.GetInControl(3));
	controls.GetOutControl(4).AddLink(gen.GetInControl(4));



	// Sequence of control-changes and Do's of the generator
	// Available controls: Gain, HighCutOff, LowCutOff, PassBandSlope, StopBandSlope

	gen.Do(spec);
	if (gen.Do(spec))
		throw Err("Do constant stream out error (should return false)");
	controls.Gain.SendControl(0.8f);
	if (!gen.Do(spec))
		throw Err("Do constant stream out error (should be true)");
	if (gen.Do(spec))
		throw Err("Do constant stream out error (should return false)");


	return 0;
}

int main()
{
	using namespace CLAM;

	try {
		int res=0;

		FDFilterGen gen;

		res = Test(gen);
		if (!res) res = TestContrChanges(gen);

		if (res)
			std::cerr << "Failed." << std::endl;
		else
			std::cerr << "Passed." << std::endl;

		return res;

	}
	catch(Err e)
	{
		e.Print();
		return 1;
	}
}

