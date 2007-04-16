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

#ifndef _CharCopierExtractor_hxx_
#define _CharCopierExtractor_hxx_

#include "Extractor.hxx" // CLAM


namespace CLAMTest
{

class CharCopierExtractor
{
public:
	CLAM::ReadHook<char> & GetInHook()
	{
		return _inputHook;
	}

	CLAM::WriteHook<char> & GetOutHook()
	{
		return _outputHook;
	}

	void Extract()
	{
		const char  & input = _inputHook.GetForReading();
		char & output = _outputHook.GetForWriting();
		output = input;
	}
	bool IsInsideScope()
	{
		return _inputHook.IsInsideScope() && _outputHook.IsInsideScope();
	}
		
	void Next()
	{
		_inputHook.Next();
		_outputHook.Next();
	}

	void Init(CLAM::DescriptionDataPool & pool)
	{
		_inputHook.Init(pool);
		_outputHook.Init(pool);
	}
private:
	CLAM::ReadHook<char> _inputHook;
	CLAM::WriteHook<char> _outputHook;
};

class CharJoinExtractor
{
public:
	void SetHooks(CLAM::ReadRangedHook<char> & inputHook, CLAM::WriteHook<std::string> & outputHook)
	{
	}
	CLAM::ReadRangedHook<char> & GetInHook()
	{
		return _inputHook;
	}
	CLAM::WriteHook<std::string> & GetOutHook()
	{
		return _outputHook;
	}

	void Extract()
	{
		const char * input;
		const char * inputEnd;
		_inputHook.GetRangeForReading(input, inputEnd);
		std::string & output = _outputHook.GetForWriting();
		for (output = ""; input<inputEnd; input++)
			output += *input;
	}
	bool IsInsideScope()
	{
		return _inputHook.IsInsideScope() && _outputHook.IsInsideScope();
	}

	void Next()
	{
		_inputHook.Next();
		_outputHook.Next();
	}

	void Init(CLAM::DescriptionDataPool & pool)
	{
		_inputHook.Init(pool);
		_outputHook.Init(pool);
	}
private:
	CLAM::ReadRangedHook<char> _inputHook;
	CLAM::WriteHook<std::string> _outputHook;
};

} // namespace CLAMTest

#endif// _CharCopierExtractor_hxx_

