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


#ifndef _SpectralFilterGen_
#define _SpectralFilterGen_

#include"Processing.hxx"
#include"ProcessingData.hxx"
#include"Port.hxx"
#include"DataTypes.hxx"
#include"Audio.hxx"
#include"Spectrum.hxx"
#include "Enum.hxx"
// #include"Defines.hxx"

namespace CLAM {

class EFDFilterGenControls
	: public Enum
{
public:

	static tEnumValue sEnumValues[];
	static tValue sDefault;
	
	EFDFilterGenControls() 
		: Enum(sEnumValues, sDefault) 
	{
	}
	
	EFDFilterGenControls(tValue v) 
		: Enum(sEnumValues, v) 
	{
	}
	
	EFDFilterGenControls(std::string s) 
		: Enum(sEnumValues, s) 
	{
	}
	
	~EFDFilterGenControls() 
	{
	};
	
	Component * Species() const 
	{ 
		return new EFDFilterGenControls;
	}

	typedef enum 
		{
			gain=0,
			highcutoff,
			lowcutoff,
			passbandslope,
			stopbandslope
		} tEnum;

};

/** Simple filter types, more to be added*/
class EFDFilterType : public Enum
{
public:
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EFDFilterType() : Enum(sEnumValues, sDefault) {}
	EFDFilterType(tValue v) : Enum(sEnumValues, v) {};
	EFDFilterType(std::string s) : Enum(sEnumValues, s) {};

	typedef enum {
		eLowPass,
		eHighPass,
		eBandPass,
		eStopBand,
	};

	virtual Component* Species() const
	{
		// TODO: This is a xapusa. I want a default constructor!
		return (Component*) new EFDFilterType(eLowPass);
	};
};


/** Configuration class for SpectralFilterGen class */
class FDFilterGenConfig: public ProcessingConfig 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FDFilterGenConfig, 8, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, EFDFilterType, Type);
	DYN_ATTRIBUTE (2, public, TData, SpectralRange);
	DYN_ATTRIBUTE (3, public, TData, Gain);
	DYN_ATTRIBUTE (4, public, TData, HighCutOff);
	DYN_ATTRIBUTE (5, public, TData, LowCutOff);
	DYN_ATTRIBUTE (6, public, TData, PassBandSlope);
	DYN_ATTRIBUTE (7, public, TData, StopBandSlope);
protected:
	void DefaultInit(void);
};

/** Frequency Domain Filter Generator */
class FDFilterGen: public Processing
{
private:
	typedef InControlTmpl<FDFilterGen> FDFilterGenCtrl;

public:
	OutPortTmpl<Spectrum> Output;
	
	FDFilterGenCtrl Gain;
	FDFilterGenCtrl HighCutOff;
	FDFilterGenCtrl LowCutOff;
	FDFilterGenCtrl PassBandSlope;
	FDFilterGenCtrl StopBandSlope;
	
private:
	TData SpectralRange;
	EFDFilterType Type;
	
	FDFilterGenConfig mConfig;
	
	bool mControlChanged;
protected:
	
	// Control change callback function
	int UpdateControlChangedFlag(TControlData);

		
public:
	
	FDFilterGen();
	
	/** Configuration constructor.
	 * @param Configuration object. 
	 */
	FDFilterGen(const FDFilterGenConfig &c);
	

	virtual ~FDFilterGen()
	{
	}
	/** Supervised-mode Do function.
	 * UNINPLEMENTED
	 */
	bool Do(void);
	
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
		
	bool Start(){return 0;};
	bool Stop(){return 0;};
	
private:
	
	const char *GetClassName() {return "FDFilterGen";}
	
	/** Private method called by the public Configure(cfg) method on the base class*/
	bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast);

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
