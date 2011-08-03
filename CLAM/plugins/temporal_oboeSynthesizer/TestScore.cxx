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

#include "TestScore.hxx"
#include <math.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

namespace CLAM {

TestScore::TestScore() :
	counter(0),
	numberOfIterations(1000),
	numberOfEventsLoaded(0),
	scoreVector(2, std::vector<float>(8000))
{
	SetFeedForever(false);
}

TestScore::TestScore(std::string filename) :
	counter(0),
	numberOfIterations(1000),
	scoreVector(2, std::vector<float>())
{
	SetFeedForever(false);
	
	isScoreLoaded = LoadScore(filename);
}

TestScore::~TestScore()
{
}

bool TestScore::LoadScore(std::string filename)
{
	std::cout << "TestScore::LoadScore. filename: " << filename << std::endl;

	bool debug = true;
	std::string line;
	char ch = '\r';
	std::string delimiters;
	std::ifstream myfile( filename.c_str() );
	if (!myfile.is_open())
	{
		printf("TestScore: LoadScore; could not open <%s>\n", filename.c_str());
		fflush(stdout);
		return false;
	}
	else
	{
		int rowNumber = 0;
		while (! myfile.eof() )
		{
			// getline needs a specific delimiter to work with 3 plotforms // 
			// '\r' works for CR in Mac as a line break
			// but '\r' as a delimiting character does not work for Linux where it uses '\n'
			// It could be a problem in Windows also where they use \r\n as a line break
			// my solution is a bit arkward, but it works
			
			if( getline(myfile, line, ch ).eof() == true && rowNumber == 0 )  
			{
				std::cout << "getline(myfile, line, \'\\r\' ).eof() == true. && rowNumber == 0 ch <- \'\\n\'" << std::endl;
				ch = '\n';
				delimiters.push_back('\n');
				myfile.clear();
				myfile.seekg( 0 );
				line.erase( 0 );
				getline(myfile, line, ch );
				
			}
			else 
			{
				if( rowNumber == 0 ) delimiters.push_back('\r');
				if( rowNumber == 0 ) std::cout << "TestScore::LoadScore. line: " << line << std::endl;
				if ((line.size() > 0) && (line[line.size() - 1] == '\n') )
				{
					if( debug ) std::cout << "window delimiters \\r\\n detected." << std::endl;
					line.resize(line.size() - 1);
					if( rowNumber == 0 ) delimiters.push_back('\n');
				}
			}
			
			if (debug) std::cout << "TestScore::LoadScore. line: " << line << " delimiters==" << "\\" << delimiters << "\\" << std::endl;

			std::vector<std::string> tokens;
			tokenize(line, tokens );

			if (debug) std::cout << "TestScore::LoadScore. number of tokens: " << tokens.size() << std::endl;
			
			if (tokens.size() == 0)
				continue;

			for (int columnNumber = 0; columnNumber < tokens.size(); columnNumber++)
			{
				const char* stringToken = tokens.at(columnNumber).c_str();
				float floatToken = (float) atof(stringToken);
//				if( columnNumber == 1 ) floatToken = 0.5; // for constant amplitude
				scoreVector[columnNumber][rowNumber] =  floatToken;
				std::cout << "scoreVetor [" << columnNumber << "][" << rowNumber << "] = floatToken =" << scoreVector[columnNumber][rowNumber] << std::endl;
			}
			rowNumber++;
		}
		myfile.close();
		
		std::cout << "myfile.close() rowNumber==" << rowNumber << std::endl;
		
		numberOfEventsLoaded = ( rowNumber + 1 );

//		if (debug) std::cout << "TestScore::LoadScore. loaded " << numberOfEventsLoaded << " score events." << std::endl;
		std::cout << "TestScore::LoadScore. loaded " << numberOfEventsLoaded << " score events." << std::endl;
	}

	return true;
}

void TestScore::tokenize(	const std::string& str,
							std::vector<std::string>& tokens,
							const std::string& delimiters)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

bool TestScore::GetControlValues(float& pitch, float& amplitude)
{
	if (isScoreLoaded)
		return NextFrameFromScore(pitch, amplitude);
	else
		return NextFrameAutogenerated(pitch, amplitude);
}

bool TestScore::NextFrameFromScore(float& pitch, float& amplitude)
{
//	if ( (counter+1) > (2*numberOfEventsLoaded) )
	if ( (counter) > (numberOfEventsLoaded) )
	{
		if ( !IsFeedForever() )
		{
			return false;
		}
		else
		{
			counter = 0;
			return NextFrameFromScore(pitch, amplitude);
		}
	}
	else
	{
		pitch = scoreVector[0][counter];
		amplitude = scoreVector[1][counter];
//		counter = counter + 2;  // why?
		counter++;
		return true;
	}
}

bool TestScore::NextFrameAutogenerated(float& pitch, float& amplitude)
{
	if (counter > 1000)
		return false;

	float basePitch = 440.f;
	float baseAmplitude = 0.f;

	float amplitudeOffset;
	float noteOffset;
	double pitchOffset;

	if (counter < 50)
	{
		noteOffset = (float) (counter / 100.);
		amplitudeOffset = (float) (2 * counter / 100.);
	}
	else if (counter > (numberOfIterations-50))
	{
		noteOffset = (float) ( (numberOfIterations-counter) / 100.);
		amplitudeOffset = (float) (2 * (numberOfIterations-counter) / 100.);
	}
	else
	{
		noteOffset = (float) (50. / 100.);
		amplitudeOffset = 1.f;
	}

	pitchOffset = 440.0 * pow(2, noteOffset/12);

	pitch = basePitch + pitchOffset;
	amplitude = baseAmplitude + amplitudeOffset;

	counter++;

	return true;
}

} // end namespace CLAM


