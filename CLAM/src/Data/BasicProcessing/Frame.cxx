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

#include "Frame.hxx"

namespace CLAM{



///////////////////////////////////////////////////////
//
// Frame
//
///////////////////////////////////////////////////////

	void Frame::DefaultInit()
	{
		AddCenterTime();
		AddDuration();
		UpdateData();
		SetCenterTime(-1);
		SetDuration(-1);
	};
	


	//For Spectrum
	TData Frame::GetMag(TData freq) const
	{
		return GetSpectrum().GetMag(freq);
	}
	TData Frame::GetMag(TIndex pos) const 
	{
		return GetSpectrum().GetMag(pos);
	}
	TData Frame::GetPhase(TData freq) const 
	{
		return GetSpectrum().GetPhase(freq);
	}
	TData Frame::GetPhase(TIndex pos) const 
	{
		return GetSpectrum().GetPhase(pos);
	}

	void Frame::SetMag(TData freq, TData newMag)
	{
		GetSpectrum().SetMag(freq,newMag);
	}
	void Frame::SetMag(TIndex pos, TData newMag)
	{
		GetSpectrum().SetMag(pos,newMag);
	}
	void Frame::SetPhase(TData freq, TData newPhase)
	{
		GetSpectrum().SetPhase(freq,newPhase);
	}
	void Frame::SetPhase(TIndex pos, TData newPhase)
	{
		GetSpectrum().SetPhase(pos,newPhase);
	}

	//For Peak Array
	SpectralPeak Frame::GetSpectralPeak(TIndex pos) const
	{
		return GetSpectralPeakArray().GetSpectralPeak(pos);
	}
	SpectralPeak Frame::GetThruIndexSpectralPeak(TIndex pos) const 
	{
		return GetSpectralPeakArray().GetThruIndexSpectralPeak(pos);
	}
	void Frame::SetSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,TIndex index)
	{
		GetSpectralPeakArray().SetSpectralPeak(pos,spectralPeak,index);
	}
	void Frame::InsertSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,
		bool insertIndex,TIndex index)
	{
		GetSpectralPeakArray().InsertSpectralPeak(pos,spectralPeak,insertIndex,index);
	}
	void Frame::AddSpectralPeak(const SpectralPeak& spectralPeak,
		bool addIndex,TIndex index)
	{
		GetSpectralPeakArray().AddSpectralPeak(spectralPeak,addIndex,index);
	}
	void Frame::DeleteSpectralPeak(TIndex pos,bool deleteIndex)
	{
		GetSpectralPeakArray().DeleteSpectralPeak(pos,deleteIndex);
	}

	//For Residual
	TData Frame::GetResMag(TData freq) const
	{
		return GetResidualSpec().GetMag(freq);
	}
	TData Frame::GetResMag(TIndex pos) const 
	{
		return GetResidualSpec().GetMag(pos);
	}
	TData Frame::GetResPhase(TData freq) const 
	{
		return GetResidualSpec().GetPhase(freq);
	}
	TData Frame::GetResPhase(TIndex pos) const 
	{
		return GetResidualSpec().GetPhase(pos);
	}

	void Frame::SetResMag(TData freq, TData newMag)
	{
		GetResidualSpec().SetMag(freq,newMag);
	}
	void Frame::SetResMag(TIndex pos, TData newMag)
	{
		GetResidualSpec().SetMag(pos,newMag);
	}
	void Frame::SetResPhase(TData freq, TData newPhase)
	{
		GetResidualSpec().SetPhase(freq,newPhase);
	}
	void Frame::SetResPhase(TIndex pos, TData newPhase)
	{
		GetResidualSpec().SetPhase(pos,newPhase);
	}

	//For Fundamental
	TData Frame::GetFundamentalFreq(TIndex pos)const
	{
		return GetFundamental().GetFreq(pos);
	}
	TData Frame::GetFundamentalErr(TIndex pos)
	{
		return GetFundamental().GetErr(pos);
	}
	void Frame::SetFundamentalFreq(TIndex pos,TData newFreq)const
	{
		GetFundamental().SetFreq(pos,newFreq);
	}
	void Frame::SetFundamentalErr(TIndex pos,TData newErr)const
	{
		GetFundamental().SetErr(pos,newErr);
	}

	/*Comparison Operators*/
	bool Frame::operator ==(const Frame& newFrame) const 
	{
		return (newFrame.GetCenterTime()==GetCenterTime());
	}
	bool Frame::operator !=(const Frame& newFrame) const 
	{
		return (newFrame.GetCenterTime()!=GetCenterTime());
	}
	bool Frame::operator <(const Frame& newFrame) const 
	{
		return (newFrame.GetCenterTime()<GetCenterTime());
	}
	bool Frame::operator <=(const Frame& newFrame) const 
	{
		return (newFrame.GetCenterTime()<=GetCenterTime());
	}
	bool Frame::operator >(const Frame& newFrame) const 
	{
		return (newFrame.GetCenterTime()>GetCenterTime());
	}
	bool Frame::operator >=(const Frame& newFrame) const 
	{
		return (newFrame.GetCenterTime()>=GetCenterTime());
	}

};

