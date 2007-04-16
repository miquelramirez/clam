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

#ifndef __EnvelopeDescriptors__
#define __EnvelopeDescriptors__



#include "ProcessingData.hxx"
#include "DataTypes.hxx"



namespace CLAM{

class Frame;

/*
 * This class holds Descriptors computed from Frame data. Right now no specific
 * descriptors are computed for frame (only for spectrums, audios... that are held
 * inside the frame. TODO: add specific descriptors.
 *
 */
class EnvelopeDescriptors : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (EnvelopeDescriptors, 23, ProcessingData);
	DYN_ATTRIBUTE (0, public, TData, TC2TotSize);
	DYN_ATTRIBUTE (1, public, TData, MidEnvSize);
	DYN_ATTRIBUTE (2, public, TData, MidEnvSize2TotSize);
	DYN_ATTRIBUTE (3, public, TData, TotSize);
	DYN_ATTRIBUTE (4, public, TData, nSilence);
	DYN_ATTRIBUTE (5, public, TData, MidEnvEnVar);
	DYN_ATTRIBUTE (6, public, TData, HighAmpDerAv);
	DYN_ATTRIBUTE (7, public, TData, LogAttackTime);
	DYN_ATTRIBUTE (8, public, TData, MaxAtDer);
	DYN_ATTRIBUTE (9, public, TData, AttackDerAv);
	DYN_ATTRIBUTE (10, public, TData, Attack2TotSize);
	DYN_ATTRIBUTE (11, public, TData, DerAAttackAv);
	DYN_ATTRIBUTE (12, public, TData, Pos2NegDer);
	DYN_ATTRIBUTE (13, public, TData, LogReleaseTime);
	DYN_ATTRIBUTE (14, public, TData, MinReleaseDer);
	DYN_ATTRIBUTE (15, public, TData, ReleaseDerAv);
	DYN_ATTRIBUTE (16, public, TData, Release2TotSize);
	DYN_ATTRIBUTE (17, public, TData, DerBMaxAv);
	DYN_ATTRIBUTE (18, public, TData, MaxDerBMax);
	DYN_ATTRIBUTE (19, public, TData, DerAMaxAv);
	DYN_ATTRIBUTE (20, public, TData, MinDerAMax);
	DYN_ATTRIBUTE (21, public, TData, MaxT2TotSize);
	DYN_ATTRIBUTE (22, public, TData, Velocity);
	

private:
	void DefaultInit();
};



};//CLAM



#endif

