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

#ifndef __MorphologicalSegmentDescriptors__
#define __MorphologicalSegmentDescriptors__

#include "Flags.hxx"
#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "EnvelopeDescriptors.hxx"

namespace CLAM{

class MorphologicalSegmentDescriptors : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MorphologicalSegmentDescriptors, 18, ProcessingData);
	DYN_ATTRIBUTE (0, public, int, MorphSegDescInt1);
	DYN_ATTRIBUTE (1, public, int, MorphSegDescInt2);
	DYN_ATTRIBUTE (2, public, int, MorphSegDescInt3);
	DYN_ATTRIBUTE (3, public, TData, PitchSalienceWAv);
	DYN_ATTRIBUTE (4, public, TData, PitchSalienceVar);
	DYN_ATTRIBUTE (5, public, TData, PitchSalienceFluxAv);
	DYN_ATTRIBUTE (6, public, TData, PitchSalienceWFluxAv);
	DYN_ATTRIBUTE (7, public, TData, LogSCVar);
	DYN_ATTRIBUTE (8, public, TData, LogSCWFluxAv);
	DYN_ATTRIBUTE (9, public, TData, LogPitchWFlux);
	DYN_ATTRIBUTE (10, public, TData, LogPitchVar);
	DYN_ATTRIBUTE (11, public, int, nPitchSeg);
	DYN_ATTRIBUTE (12, public, TData, gLogPitchFluxWAv);
	DYN_ATTRIBUTE (13, public, TData, gLogPitchVarWAv);
	DYN_ATTRIBUTE (14, public, TData, PitchAv);
	DYN_ATTRIBUTE (15, public, TData, StablePitch2Tot);
	DYN_ATTRIBUTE (16, public, TData, nSeg2NonSilentPitchedFr);
	DYN_ATTRIBUTE (17, public, EnvelopeDescriptors, EnvD);	


private:
	void DefaultInit();
	};



};//CLAM



#endif

