/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _SpectralFilterGen_
#define _SpectralFilterGen_

#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Audio.hxx"
#include "OutPort.hxx"
#include "Spectrum.hxx"
#include "Enum.hxx"

namespace CLAM {

class EFDFilterGenControls : public Enum
{
public:
	
	EFDFilterGenControls() : Enum(ValueTable(), gain) { }
	EFDFilterGenControls(tValue v) : Enum(ValueTable(), v) { }
	EFDFilterGenControls(std::string s) : Enum(ValueTable(), s) { }
	~EFDFilterGenControls() { };

	Component * Species() const 
	{ 
		return new EFDFilterGenControls;
	}

	enum 
	{
		gain=0,
		highcutoff,
		lowcutoff,
		passbandslope,
		stopbandslope
	} tEnum;

	static tEnumValue * ValueTable()
	{
		static tEnumValue sEnumValues[] =
		{
			{ gain, "gain" },
			{ highcutoff, "highcutoff" },
			{ lowcutoff, "lowcutoff" },
			{ passbandslope, "passbandslope" },
			{ stopbandslope, "stopbandslope" },
			{ 0, NULL }
		};
		return sEnumValues;
	}
};

/** Simple filter types, more to be added*/
class EFDFilterType : public Enum
{
public:
	
	EFDFilterType() : Enum(ValueTable(), eLowPass) {}
	EFDFilterType(tValue v) : Enum(ValueTable(), v) {};
	EFDFilterType(std::string s) : Enum(ValueTable(), s) {};

	enum {
		eLowPass,
		eHighPass,
		eBandPass,
		eStopBand
	};

	virtual Component* Species() const
	{
		// TODO: This is a xapusa. I want a default constructor!
		return (Component*) new EFDFilterType(eLowPass);
	};
	static tEnumValue * ValueTable()
	{
		static tEnumValue sEnumValues[] = {
			{eLowPass,"Low-pass"},
			{eHighPass,"High-pass"},
			{eBandPass,"Band-pass"},
			{eStopBand,"Stop-Band"},
			{0,NULL}
		};
		return sEnumValues;
	}
};


/** Configuration class for SpectralFilterGen class */
class FDFilterGenConfig: public ProcessingConfig 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FDFilterGenConfig, 7, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, EFDFilterType, Type);
	DYN_ATTRIBUTE (1, public, TData, SpectralRange);
	DYN_ATTRIBUTE (2, public, TData, Gain);
	DYN_ATTRIBUTE (3, public, TData, HighCutOff);
	DYN_ATTRIBUTE (4, public, TData, LowCutOff);
	DYN_ATTRIBUTE (5, public, TData, PassBandSlope);
	DYN_ATTRIBUTE (6, public, TData, StopBandSlope);
protected:
	void DefaultInit(void);
};

/** Frequency Domain Filter Generator */
class FDFilterGen: public Processing
{
private:
	typedef FDFilterGenConfig Config;

public:
	OutPort<Spectrum> Output;
	
	FloatInControl Gain;
	FloatInControl HighCutOff;
	FloatInControl LowCutOff;
	FloatInControl PassBandSlope;
	FloatInControl StopBandSlope;
	
private:
	TData SpectralRange;
	EFDFilterType Type;
	
	Config mConfig;
	
	bool mControlChanged;
protected:
	
	// Control change callback function
	void UpdateControlChangedFlag(TControlData);

		
public:
	
	/** Configuration constructor.
	 * @param Configuration object. 
	 */
	FDFilterGen(const Config &c = Config());
	

	virtual ~FDFilterGen()
	{
	}
	/** Supervised-mode Do function.
	 * UNINPLEMENTED
	 */
	bool Do();
	
	/** Non-supervised mode Do function.
	 * @todo: decide ConstantOutput mode.
	 * In ConstantOutput mode it will actually generate the filter, only if
	 * some in control has been received. In Non-ConstantOutput mode, the 
	 * output will allways be calculated.
	 *
	 * @return true if the out spectrum has been calculated.
	 */
	bool Do(Spectrum &out);
	
	/** Definition of the Processing virtual method */
	const ProcessingConfig &GetConfig() const { return mConfig; };
		
private:
	
	const char *GetClassName() const {return "FDFilterGen";}
	
	/** Private method called by the public Configure(cfg) method on the base class*/
	bool ConcreteConfigure(const ProcessingConfig& c);

	/** Auxiliary Function:*/
	void SetFilterPoint(
			Spectrum& out,
			TIndex pos,
			TData freq,
			TData mag,
			TData phase=0
		);
	
};

} //namespace CLAM


#endif // _FFT_

