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

#include <list>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "Array.hxx"
#include "Complex.hxx"
#include "DataTypes.hxx"

using CLAM::TData;
using CLAM::Complex;

namespace CLAMTest {
	using namespace CLAM;

	bool WriteOctaveVector(std::ofstream &f, 
						   const Array<Complex > &array,
						   const std::string &name)
	{
		// Vector header.
		f << "# Created by CLAM library" << std::endl
		  << "# name: " << name.c_str() << std::endl
		  << "# type: complex matrix" << std::endl
		  << "# rows: 1" << std::endl
		  << "# columns: " << array.Size() << std::endl;
		int i;
		for (i=0;i<array.Size(); i++)
			f << " (" << array[i].Real() << "," << array[i].Imag() << ")";

		f << std::endl;

		if (!f)
			return false;
		return true;
	}

	bool WriteOctaveVector(std::ofstream &f, 
						   const Array<TData > &array,
						   const std::string &name)
	{
		// Vector header.
		f << "# Created by CLAM library" << std::endl
		  << "# name: " << name.c_str() << std::endl
		  << "# type: matrix" << std::endl
		  << "# rows: 1" << std::endl
		  << "# columns: " << array.Size() << std::endl;

		int i;
		for (i=0;i<array.Size(); i++)
			f << " " << array[i];

		f << std::endl;

		if (!f)
			return false;
		return true;
	}

	bool LoadOctaveTDataVector(std::istream &f,
							   const std::string &name,
							   Array<TData> &array)
	{
		enum { eUnknown,  // Reading vector header, about which we still know nothing.
			   eMatch ,   // Reading vector header, with matching name
			   eOther,    // Reading vector header, with non-matching name or type
			   eSkipData, // Skiping data.
			   eData,     // Reading our data.
			   eError     // Name not found, or found with wrong type
		} state = eUnknown;

//  		const std::string state_str[] = {
//  			std::string("eUnknown"),
//  			std::string("eMatch"),
//  			std::string("eOther"),    
//  			std::string("eSkipData"), 
//  			std::string("eData"),     
//  			std::string("eError")
//  		};

		std::string line;

		std::string name_line = "# name: ";
		name_line += name;
		name_line += '\n';
		std::string type_start = "# type: ";
		std::string rows_start = "# rows: ";
		std::string columns_start = "# columns: ";
		const std::string good_type_line = "# type: matrix\n";
		const std::string good_rows_line = "# rows: 1\n";

		std::stringstream sstr;
		sstr << array.Size();
		std::string good_columns_line = "# columns: ";
		good_columns_line += sstr.str();

//  		std::cerr << "Looking for: " << std::endl
//  				  << name_line 
//  				  << good_type_line
//  				  << good_rows_line
//  				  << good_columns_line;

		std::stringstream ss;
		int pos;
		TData *buffer = array.GetPtr();

		// NOTE: The parser asumes that there is always "type", "rows"
		// and "columns" fields in the header of a vector definition.
//		f.seekg(0);
		while (f) {
			std::getline(f,line);
//			std::cerr << "   R: " << line << std::endl;
			switch(state) {
			case eUnknown:
				if (line[0]!='#') { // End of header
					state=eSkipData;
					break;
				}
				if (line == name_line) {
					state=eMatch;
					break;
				}
				if (line != good_type_line &&
					line.substr(0,type_start.size())==type_start ) {
					state=eOther;
					break;
				}
				if (line != good_rows_line &&
					line.substr(0,rows_start.size())==rows_start) {
					state=eOther;
					break;
				}
				if (line != good_columns_line &&
					line.substr(0,columns_start.size()) == columns_start) {
					state=eOther;
					break;
				}
				break;
			case eOther:
				if (line[0]!='#') { // End of header
					state=eSkipData;
					break;
				}
				if (line == name_line)
					throw(Err("LoadOctaveTData: found vector with wrong parameters")); 
				break;
			case eSkipData:
				if (line[0]=='#') {
					// First header line. We can skip it.
					state=eUnknown;
					break;
				}
				break;
			case eData:
				throw(Err("CLAM: Inconsistent state")); //Should never get here
			case eMatch:
				if ( line != good_type_line &&
					 line.substr(0,type_start.size())==type_start )
					throw(Err("LoadOctaveTData: found vector with wrong type")); 
				if (line != good_rows_line &&
					line.substr(0,rows_start.size())==rows_start)
					throw(Err("LoadOctaveTData: found vector with wrong number of rows")); 
				if (line != good_columns_line &&
					line.substr(0,columns_start.size()) == columns_start)
					throw(Err("LoadOctaveTData: found vector with wrong number of columns")); 
				if (line[0]=='#')  // End of header
					break;
				ss.str(line);
				ss.seekg(0);
				pos=0;
				while (1) {
					TData point;
					ss >> point;
					if (ss.eof())
						if (pos == array.Size()-1)
							return true;
						else {
							std::cerr << "Eof found while reading pos " << pos << std::endl;
							return false;
						}
					if (ss.good())
						if (pos == array.Size()-1) {
							std::cerr << "pos = << "<<array.Size()<<"and more data available"<<std::endl;
							return false;
						}
					if (ss.bad()) {
						std::cerr << "Input error" <<  std::endl;
						return false;
					}
					buffer[pos++]=point;
				}
			default:
				throw(Err("Parse error loading octave vector file"));
			}
//			std::cerr << state_str[state] << std::endl;
		}
		return false;
	}

	bool LoadOctaveComplexVector(std::istream &f,
								 const std::string &name,
								 Array<Complex> &array)
	{
		enum { eUnknown,  // Reading vector header, about which we still know nothing.
			   eMatch ,   // Reading vector header, with matching name
			   eOther,    // Reading vector header, with non-matching name or type
			   eSkipData, // Skiping data.
			   eData,     // Reading our data.
			   eError     // Name not found, or found with wrong type
		} state = eUnknown;

//  		const std::string state_str[] = {
//  			std::string("eUnknown"),
//  			std::string("eMatch"),
//  			std::string("eOther"),    
//  			std::string("eSkipData"), 
//  			std::string("eData"),     
//  			std::string("eError")
//  		};

		std::string line;
		std::stringstream sstr;

		sstr.str(name);
		sstr.seekp(name.size());
//		sstr << std::endl;
		std::string name_line = "# name: ";
		name_line += sstr.str();
		std::string type_start = "# type: ";
		std::string rows_start = "# rows: ";
		std::string columns_start = "# columns: ";
		sstr.str("");
//		sstr << std::endl;
		std::string good_type_line = "# type: complex matrix";
		good_type_line += sstr.str();
		std::string good_rows_line = "# rows: 1";
		good_rows_line += sstr.str();
		sstr.str("");
		sstr << array.Size();// << std::endl;
		std::string good_columns_line = "# columns: ";
		good_columns_line += sstr.str();

//  		std::cerr << "Looking for: " << std::endl
//  				  << name_line 
//  				  << good_type_line
//  				  << good_rows_line
//  				  << good_columns_line;

		std::stringstream ss;
		int pos;
		Complex *buffer = array.GetPtr();

		// NOTE: The parser asumes that there is always "type", "rows"
		// and "columns" fields in the header of a vector definition.
//		f.seekg(0);
		while (f) {
			std::getline(f,line);
//			std::cerr << "   R: " << line << std::endl;
			switch(state) {
			case eUnknown:
				if (line[0]!='#') { // End of header
					state=eSkipData;
					break;
				}
				if (line == name_line) {
					state=eMatch;
					break;
				}
				if (line != good_type_line &&
					line.substr(0,type_start.size())==type_start ) {
					state=eOther;
					break;
				}
				if (line != good_rows_line &&
					line.substr(0,rows_start.size())==rows_start) {
					state=eOther;
					break;
				}
				if (line != good_columns_line &&
					line.substr(0,columns_start.size()) == columns_start) {
					state=eOther;
					break;
				}
				break;
			case eOther:
				if (line[0]!='#') { // End of header
					state=eSkipData;
					break;
				}
				if (line == name_line)
					throw(Err("LoadOctaveTData: found vector with wrong parameters")); 
				break;
			case eSkipData:
				if (line[0]=='#') {
					// First header line. We can skip it.
					state=eUnknown;
					break;
				}
				break;
			case eData:
				throw(Err("CLAM: Inconsistent state")); //Should never get here
			case eMatch:
				if ( line != good_type_line &&
					 line.substr(0,type_start.size())==type_start )
					throw(Err("LoadOctaveTData: found vector with wrong type")); 
				if (line != good_rows_line &&
					line.substr(0,rows_start.size())==rows_start)
					throw(Err("LoadOctaveTData: found vector with wrong number of rows")); 
				if (line != good_columns_line &&
					line.substr(0,columns_start.size()) == columns_start)
					throw(Err("LoadOctaveTData: found vector with wrong number of columns")); 
				if (line[0]=='#')  // End of header
					break;
				ss.str(line);
				ss.seekg(0);
				pos=0;
				while (1) {
					std::string point;
					ss >> point;
					std::stringstream spoint(point);
					char c;
					TData real,imag;
					spoint >> c >> real >> c >> imag >> c;
					if (ss.bad()) {
						std::cerr << "Input error" <<  std::endl;
						return false;
					}
					buffer[pos++]=Complex(real,imag);
					if (ss.good())
						if (pos == array.Size()) {
							std::cerr << "pos = << "<<array.Size()<<"and more data available"<<std::endl;
							return false;
						}
					if (ss.eof())
						if (pos == array.Size())
							return true;
						else {
							std::cerr << "Eof found while reading pos " << pos << std::endl;
							return false;
						}
				}
			default:
				throw(Err("Parse error loading octave vector file"));
			}
//			std::cerr << state_str[state] << std::endl;
		}
		return false;
	}

}

