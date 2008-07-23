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

#include "SMSDeesser.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{	
	static const char * metadata[] = {
		"key", "SMSDeesser",
		"category", "SMS Transformations",
		"description", "SMSDeesser",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SMSDeesser> reg = metadata;
}


bool SMSDeesser::Do(const Spectrum& in, Spectrum& out)
{
	//this could be done by connecting controls but then I would need outControls
	SendFloatToInControl(mDeesser,"Amount",mAmount.GetLastValue());
	SendFloatToInControl(mDeesser,"LowCutoff",mLowCutoffFreqCtl.GetLastValue());
	SendFloatToInControl(mDeesser,"HighCutoff",mHighCutoffFreqCtl.GetLastValue());
	SendFloatToInControl(mDeesser,"Threshold",mThresholdCtl.GetLastValue());
	
	mDeesser.Do(in, out);
	return true;
}


}

