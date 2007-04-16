/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __RHYTHMIR_CMDPROC__
#define __RHYTHMIR_CMDPROC__

#include "TickExtractorConfig.hxx"
#include <string>

namespace RhythmIR
{
	class CmdLineParms
	{
	public:
		
		std::string GetInputFilename() const;
		std::string GetOutputFilename() const;
		const CLAM::TickExtractorConfig& GetConfig() const;

		static CmdLineParms ParseArgs( int argc, char** argv );

	protected:
		static void PrintHelp();

	private:
		CmdLineParms( std::string in, std::string out,
			      const CLAM::TickExtractorConfig& cfg );

	protected:
		std::string                mInputFilename;
		std::string                mOutputFilename;
		CLAM::TickExtractorConfig  mConfig;
		
	};

	inline std::string CmdLineParms::GetInputFilename() const
	{
		return mInputFilename;
	}

	inline std::string CmdLineParms::GetOutputFilename() const
	{
		return mOutputFilename;
	}

	inline const CLAM::TickExtractorConfig& CmdLineParms::GetConfig() const
	{
		return mConfig;
	}
}

#endif // rhythmir_cmdporc.hxx

