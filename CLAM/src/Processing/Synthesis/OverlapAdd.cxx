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

#include "OverlapAdd.hxx"


using namespace CLAM;


//////////////////////////////////////////////////////////////////////
//
// OverlapAddConfig
//
//////////////////////////////////////////////////////////////////////

/* the configuration object has at least to have a name */

void OverlapAddConfig::DefaultInit()
{
	/* the dynamic type takes care if we add an existing attr .. */
	AddName();
	AddHopSize();
	AddFrameSize();
	AddBufferSize();

	/* all attributes are added */
	UpdateData();
	DefaultValues();
}

void OverlapAddConfig::DefaultValues()
{
	/* set default values */
	SetHopSize(0);
	SetFrameSize(0);
	SetBufferSize(0);
}

//////////////////////////////////////////////////////////////////////
//
// OverlapAdd
//
//////////////////////////////////////////////////////////////////////

/* processing object method implementations */

OverlapAdd::OverlapAdd(void)
{
	Configure(OverlapAddConfig());
}

OverlapAdd::OverlapAdd(const OverlapAddConfig &c)
{
	Configure(c);
}

OverlapAdd::~OverlapAdd()
{
}


/* configure the processing object according to the config object */

bool OverlapAdd::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const OverlapAddConfig&>(c);
	mCircBuffer.SetBufferSize(mConfig.GetBufferSize());
	mCircBuffer.SetReadSize(mConfig.GetFrameSize());
	mCircBuffer.SetWriteSize(mConfig.GetHopSize());
	return true;
}

/* other functions */



/* setting prototypes for faster processing */

bool OverlapAdd::SetPrototypes(){
	return false;
}

bool OverlapAdd::UnsetPrototypes(){
	return false;
}

/* the supervised Do() function */

bool OverlapAdd::Do(void)
{
	throw(ErrProcessingObj("OverlapAdd::Do(): Supervised mode not implemented"),this);
	return false;
}

bool OverlapAdd::Do(DataArray &in, DataArray &out)
{
	mCircBuffer.Add(in);
	mCircBuffer.Write(in,mConfig.GetHopSize());

	//modify write pointer
	mCircBuffer.DecreaseWriteIndex(mConfig.GetHopSize());

	//read frame from buffer
	mCircBuffer.Read(out);
	return true;
} 

bool OverlapAdd::Do(Audio &in, Audio &out)
{
	return Do(in.GetBuffer(), out.GetBuffer());
}
