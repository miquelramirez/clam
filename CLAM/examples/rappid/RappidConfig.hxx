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

#ifndef _RAPPID_CONFIG_H
#define _RAPPID_CONFIG_H

#include "RappidTypes.hxx"
#include "Processing.hxx"
#include "RappidIO.hxx"
#include "EnvelopeMorpher.hxx"

using namespace CLAM;

class RappidConfig: public ProcessingConfig {
public:

	typedef enum {
		eNoSignal = 0,
		eHarpSignal,
		eViolaSignal,
		eEM1Signal,
		eEM2Signal,
		eSynthLSignal,
		eSynthRSignal
	} ERappidSignal;

	DYNAMIC_TYPE_USING_INTERFACE(RappidConfig,14,ProcessingConfig);
	DYN_ATTRIBUTE (0,  public, std::string,           Name);
	DYN_ATTRIBUTE (1,  public, RappidAppConfig,       GlobalConfig);
	DYN_ATTRIBUTE (2,  public, RappidIOConfig,        ViolaIOConfig);
	DYN_ATTRIBUTE (3,  public, RappidIOConfig,        HarpIOConfig);
	DYN_ATTRIBUTE (4,  public, RappidIOConfig,        SynthLeftIOConfig);
	DYN_ATTRIBUTE (5,  public, RappidIOConfig,        SynthRightIOConfig);
	DYN_ATTRIBUTE (6,  public, EnvelopeMorpherConfig, EnvMorph1Cfg);
	DYN_ATTRIBUTE (7,  public, EnvelopeMorpherConfig, EnvMorph2Cfg);
	DYN_ATTRIBUTE (8,  public, TTime,                 TimeLimit);
	DYN_ATTRIBUTE (9,  public, bool,                  RealTimeSched);
	DYN_ATTRIBUTE (10, public, int,                   RealTimePriority);
	DYN_ATTRIBUTE (11, public, bool,                  DoControlTest);
	DYN_ATTRIBUTE (12, public, TTime,                 ControlTestSleep);
	DYN_ATTRIBUTE (13, public, int,                   DebugSignal);

	void DefaultInit(void);
	void Synchronise(void);

	void Dump(void);

	void parse_command_line(int argc, char *argv[]);
	void print_command_line_help();
};


#endif
