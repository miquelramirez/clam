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

#ifndef _RAPPID_INPUT_H
#define _RAPPID_INPUT_H

#include "RappidIO.hxx"
#include "ProcessingComposite.hxx"
#include "OutPortTmpl.hxx"


namespace CLAM {
	class AudioFileIn;
	class AudioIn;
	class WaveGenerator;
	class AudioManager;
}
using namespace CLAM;

class RappidInput: public ProcessingComposite, public RappidIO<RappidInput>
{
	AudioIn       *pRTInput;
	AudioFileIn   *pInputFile;
	WaveGenerator *pWaveformIn;

	const char *GetClassName() const {return "RappidInput";}

	bool ConfigureRealTime();

	bool ConfigureFile();

	bool ConfigureWaveform();

	bool ConfigureChildren();

	bool ConcreteConfigure(const ProcessingConfig&);

	bool ConcreteStart();

	void Adopt(Processing& o) { o.SetParent(this);}

	void ConfigError(const std::string &str) { mStatus += str; }

public:

	OutPortTmpl<Audio> Output;

	RappidInput();

	RappidInput(const RappidIOConfig &c);

	~RappidInput();

	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool Do(void);

	bool Do(Audio& in);

};


#endif
