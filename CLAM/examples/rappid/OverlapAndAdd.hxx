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

/**
 * @file OverlapAndAdd class for Rappid.
 */

#ifndef _OVERLAP_AND_ADD_H
#define _OVERLAP_AND_ADD_H

#include "Processing.hxx"
#include "Audio.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"


namespace CLAM {


	class OverlapAndAddConfig : public ProcessingConfig {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (OverlapAndAddConfig, 3,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, int,         HopSize);
		DYN_ATTRIBUTE (2, public, int,         Length);
	protected:
		void DefaultInit(void);
	};


	/**
	 * This is an experimental overlap and add class, oriented to supervised
	 * mode work. It does not handle overlap; it asumes that a StreamBuffer takes
	 * care of that at its output.
	 */
	class OverlapAndAdd : public Processing {

		OverlapAndAddConfig mConfig;

		// Configuration shortcuts
		int mHop;
		int mLen;
		int mOverlapEnd;

		InPortTmpl<Audio>   mInput;
		OutPortTmpl<Audio>  mOutput;

		bool ConcreteConfigure(const ProcessingConfig&);

	public:

		OverlapAndAdd();

		OverlapAndAdd(const OverlapAndAddConfig&);

		~OverlapAndAdd();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		void Attach(Audio &in, Audio &out);

		bool Do(void);

		bool Do(Audio &in, Audio &out);

	};

}

#endif

