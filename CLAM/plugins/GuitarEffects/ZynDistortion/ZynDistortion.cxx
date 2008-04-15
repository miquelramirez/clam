/*
  ZynAddSubFX - a software synthesizer

  Distorsion.C - Distorsion effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Ported to CLAM by Hernan Ordiales

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "ZynDistortion.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "ZynDistortion",
		"category", "[plugin] Guitar Effects",
		"description", "ZynDistortion",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ZynDistortion> reg = metadata;
}

	void ZynDistortionConfig::DefaultInit( )
	{
		AddAll();
		UpdateData();
		SetDistortionType(EDistType::eArctangent);
	}
}
