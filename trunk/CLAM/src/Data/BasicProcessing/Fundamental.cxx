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

#include "Fundamental.hxx"


namespace CLAM{



//////////////////////////////////////////////////////////////////////
//
// Fundamental
//
//////////////////////////////////////////////////////////////////////



void Fundamental::DefaultInit()
{
	//Initializing minimum set of attributes
	AddprnMaxCandidates();
	AddnCandidates();
	AddCandidatesFreq();
	AddCandidatesErr();
	UpdateData();
	
	//Default values
	SetnMaxCandidates(1);
	SetnCandidates(0);

}
 
	
TData Fundamental::GetFreq(TIndex pos)//inefficient Get, for efficiency work directly on the buffer
{
	
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::GetFreq::Fundamental not initialized");
	if (pos<GetnCandidates())
			return GetCandidatesFreq()[pos];
	else return 0;//if no candidates were found, 0 is used as the default non valid value
}

TData Fundamental::GetErr(TIndex pos)//inefficient Get, for efficiency work directly on the buffer
{
	
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::GetErr::Fundamental not initialized");
	return GetCandidatesErr()[pos];
}

void Fundamental::SetFreq(TIndex pos,TData freq)//inefficient Set, for efficiency work directly on the buffer
{
	
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::SetFreq::Fundamental not initialized");
	GetCandidatesFreq()[pos]=freq;
}

void Fundamental::SetErr(TIndex pos,TData err)//inefficient Set, for efficiency work directly on the buffer
{
	
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::SetErr::Fundamental not initialized");
	GetCandidatesErr()[pos]=err;
}

	
void Fundamental::AddElem (double freq, double err)
{
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::AddElem::Fundamental not initialized");
	CLAM_ASSERT(GetnCandidates()<GetprnMaxCandidates(),
	           "Fundamental::AddElem::Number of Candidates exceeds maximum");
	GetCandidatesFreq().AddElem(freq);
	GetCandidatesErr().AddElem(err);
	SetnCandidates(GetnCandidates()+1);
}

void Fundamental::InsertElem (TIndex pos, double freq, double err)
{
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::InsertElem::Fundamental not initialized");
	CLAM_ASSERT(GetnCandidates()<GetprnMaxCandidates(),
	           "Fundamental::InsertElem::Number of Candidates exceeds maximum");
	GetCandidatesFreq().InsertElem(pos,freq);
	GetCandidatesErr().InsertElem(pos,err);
	SetnCandidates(GetnCandidates()+1);
}

void Fundamental::DeleteElem (TIndex pos)
{
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::DeleteElem::Fundamental not initialized");
	CLAM_ASSERT(pos<GetnCandidates()&&pos>=0,
	           "Fundamental::DeleteElem::Wrong index, element does not exist");
	GetCandidatesFreq().DeleteElem(pos);
	GetCandidatesErr().DeleteElem(pos);
	SetnCandidates(GetnCandidates()-1);
}

void Fundamental::Exchange(TIndex pos1, TIndex pos2)
{
	CLAM_DEBUG_ASSERT(HasCandidatesFreq(),"Fundamental::Exchange::Fundamental not initialized");
	CLAM_ASSERT(pos1<GetnCandidates()&&pos2<GetnCandidates()&&pos1>=0&&pos2>=0,
	           "Fundamental::DeleteElem::Wrong index, one of the elements does not exist");
	TData tmpFreq=GetFreq(pos1);
	TData tmpErr=GetErr(pos1);
	SetFreq(pos1,GetFreq(pos2));
	SetErr(pos1,GetErr(pos2));
	SetFreq(pos2,tmpFreq);
	SetErr(pos2,tmpErr);
}

void Fundamental::SortByFrequency()
{
	//TODO: needs enhancement in the QSort Class
}

void Fundamental::SortByError()
{
	//TODO: needs enhancement in the QSort Class
}

};

