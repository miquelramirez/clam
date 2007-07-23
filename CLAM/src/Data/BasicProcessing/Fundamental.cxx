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

#include "Fundamental.hxx"
#include "ProcessingDataPlugin.hxx"


namespace CLAM
{

	namespace Hidden
	{
		static ProcessingDataPlugin::Registrator<Fundamental> dataRegistrator("sandybrown");
	}
	//////////////////////////////////////////////////////////////////////
	//
	// Fundamental
	//
	//////////////////////////////////////////////////////////////////////

	void Fundamental::DefaultInit()
	{
		//Initializing minimum set of attributes
		AddCandidatesFreq();
		AddCandidatesErr();
		UpdateData();
		
		//Default values
		SetnMaxCandidates(1);
		SetnCandidates(0);
	}
 
		
	TData Fundamental::GetFreq(TIndex pos) const //inefficient Get, for efficiency work directly on the buffer
	{
		
		CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::GetFreq(): Fundamental not initialized");
		if (pos < GetnCandidates())
		{
			return GetCandidatesFreq()[pos];
		}
		else
		{
			return 0.f;//if no candidates were found, 0 is used as the default non valid value
		}
	}

	TData Fundamental::GetErr(TIndex pos) const //inefficient Get, for efficiency work directly on the buffer
	{
		
		CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::GetErr(): Fundamental not initialized");
		return GetCandidatesErr()[pos];
	}

	void Fundamental::SetFreq(TIndex pos,TData freq)//inefficient Set, for efficiency work directly on the buffer
	{
		
		CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::SetFreq(): Fundamental not initialized");
		CLAM_DEBUG_ASSERT(pos<GetnCandidates(),"Fundamental::SetFreq(): You are trying to set a candidate that does not exist");
		GetCandidatesFreq()[pos]=freq;
	}

	void Fundamental::SetErr(TIndex pos,TData err)//inefficient Set, for efficiency work directly on the buffer
	{
		
		CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::SetErr(): Fundamental not initialized");
		CLAM_DEBUG_ASSERT(pos<GetnCandidates(),"Fundamental::SetFreq(): You are trying to set a candidate that does not exist");
		GetCandidatesErr()[pos]=err;
	}

		
	void Fundamental::AddElem (TData freq, TData err)
	{
		CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::AddElem(): Fundamental not initialized");
		// MRJ: Does this assert make sense any longer after Merlijn changes?
		//CLAM_ASSERT(GetnCandidates()<GetnMaxCandidates(),
		//		   "Fundamental::AddElem(): Number of Candidates exceeds maximum");
		GetCandidatesFreq().AddElem(freq);
		GetCandidatesErr().AddElem(err);
	//	SetnCandidates(GetnCandidates()+1);
	}

	void Fundamental::InsertElem (TIndex pos, TData freq, TData err)
	{
		CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::InsertElem(): Fundamental not initialized");
		CLAM_ASSERT(GetnCandidates()<GetnMaxCandidates(),
				   "Fundamental::InsertElem(): Number of Candidates exceeds maximum");
		GetCandidatesFreq().InsertElem(pos,freq);
		GetCandidatesErr().InsertElem(pos,err);
	//	SetnCandidates(GetnCandidates()+1);
	}

	void Fundamental::DeleteElem (TIndex pos)
	{
		CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::DeleteElem(): Fundamental not initialized");
		CLAM_ASSERT(pos<GetnCandidates()&&pos>=0,
				   "Fundamental::DeleteElem(): Wrong index, element does not exist");
		GetCandidatesFreq().DeleteElem(pos);
		GetCandidatesErr().DeleteElem(pos);
	//	SetnCandidates(GetnCandidates()-1);
	}

	void Fundamental::SortByFrequency()
	{
		// TODO: Is a bubble sort, not so eficient O(N^2)
		// TODO: Enhancement 1: Do a Quick Sort
		// TODO: Engancement 2: Sort an index array
		DataArray & errors = GetCandidatesErr();
		DataArray & freqs = GetCandidatesFreq();
		const int nCandidates = GetnCandidates();
		for (int i=0; i<nCandidates; i++) // Ordering
		for (int j=i+1; j<nCandidates; j++)
		{
			if (freqs[i] <= freqs[j]) continue;
			std::swap(errors[i],errors[j]);
			std::swap(freqs[i],freqs[j]);
		}
	}

	void Fundamental::SortByError()
	{
		// TODO: Is a bubble sort, not so eficient O(N^2)
		// TODO: Enhancement 1: Do a Quick Sort
		// TODO: Engancement 2: Sort an index array
		DataArray & errors = GetCandidatesErr();
		DataArray & freqs = GetCandidatesFreq();
		const int nCandidates = GetnCandidates();
		for (int i=0; i<nCandidates; i++) // Ordering
		for (int j=i+1; j<nCandidates; j++)
		{
			if (errors[i] <= errors[j]) continue;
			std::swap(errors[i],errors[j]);
			std::swap(freqs[i],freqs[j]);
		}
	}

}

