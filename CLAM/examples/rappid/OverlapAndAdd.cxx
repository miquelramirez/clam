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

#include "OverlapAndAdd.hxx"


namespace CLAM {

	void OverlapAndAddConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetHopSize(0);
		SetLength(0);
	}

	OverlapAndAdd::OverlapAndAdd()
		: mInput ("Input",this,1),
		  mOutput("Output",this,1)
	{
		Configure(OverlapAndAddConfig());
	}

	OverlapAndAdd::OverlapAndAdd(const OverlapAndAddConfig &cfg)
		: mInput ("Input",this,1),
		  mOutput("Output",this,1)
	{
		Configure(cfg);
	}

	bool OverlapAndAdd::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mHop = mConfig.GetHopSize();
		mLen = mConfig.GetLength();
		mOverlapEnd = mLen - mHop;

		return true;
	}

	void OverlapAndAdd::Attach(Audio &in, Audio &out)
	{
		mInput.Attach(in);
		mOutput.Attach(out);
	}

	bool OverlapAndAdd::Do(void)
	{
		Do(mInput.GetData(),mOutput.GetData());
		return true;
	}

	bool OverlapAndAdd::Do(Audio &in, Audio &out)
	{
		int i;

		CLAM_DEBUG_ASSERT(in.GetSize() == out.GetSize() &&
						  in.GetSize() == mLen,
						  "OverlapAndAdd::Do(...): Size mismatch.");

		Array<TData> &in_buffer  =  in.GetBuffer();
		Array<TData> &out_buffer = out.GetBuffer();

		for (i=0; i<mOverlapEnd; i++)
			out_buffer[i] = out_buffer[i] + in_buffer[i];
		for (i=mOverlapEnd; i<mLen; i++)
			out_buffer[i] = in_buffer[i];

		return true;
	}

	OverlapAndAdd::~OverlapAndAdd()
	{
	}

}
