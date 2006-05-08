/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <iostream>
#include <cmath>
#include <iomanip>
#include "FileIO.hxx"

//  constructor
fileio::fileio() {
	// initialize some data to 0
	channels = 0;
	FS = 0; 
	bits = 0;
	norm =1;
	totbytes = 0;
	totlength = 0;
	seconds = 0;

	initsegment = 0;
	insegment = 0;
	hopisegment = 0;
	hopsegment =0;
}

fileio::~fileio()
{
	if (initsegment) delete initsegment;
	if (insegment) delete insegment;
	if (hopisegment) delete hopisegment;
	if (hopsegment) delete hopsegment;
}

void fileio::opinfile(char* filename) {
	source.open(filename, std::ios::binary|std::ios::in);
	if (!source)
	{
		std::cerr << "Cannot open source file " << filename << ".\n";
		exit(-1);
	}
}


void fileio::opcqfile(char* filename) {
	cqbuff.open(filename);
	if (!cqbuff)
	{
		std::cerr << "Cannot open file " << filename << " for output.\n";
		exit(-1);
	}
}


void fileio::opchrfile(char* filename) {
	writefile = fopen(filename, "wb");
	if (!writefile) {
		std::cout << "Cannot open file " << filename << " for output.\n";
		exit(1);
	}
	std::string outputFilename = filename;
	outputFilename+=".txt";
	dest.open(outputFilename.c_str());
}


//end opfiles
//------------------------------------------------------------------------------------
void fileio::clinfile() {
	source.close();
}

void fileio::cloutfile() {
	dest.close();
}

void fileio::clcqfile() {
	cqbuff.close();
}

void fileio::clffile() {
	fbuff.close();
}

void fileio::clchrfile() {
	int stat = fclose(writefile);
}


//end clfiles
//-------------------------------------------------------------------------------------
void fileio::readheader() {
	if (dest && source) {
		unsigned short int header[22];
		source.read((char*)&header, sizeof(header));	//read header
		//dest.write((char*)&header, sizeof(header)); //(and write)

		// interpret header
		channels = header[11];						// = bytes 22 and 23	
		FS = header[12] + header[13]*65536;			// = bytes 24 to 27
		bits = header[17];							// = bytes 34 and 35
		norm = (bits-1)*(bits-1);
		totbytes = header[20] + header[21]*65536;   // = bytes 40 to 44
		totlength = totbytes/(channels*(bits/8));
		seconds = (float)totlength/FS;
	}
}
//end readheader
//----------------------------------------------------------------------------------------

void fileio::displayheader() {
	// output info to screen
	std::cout << "\n" << "Number of channels: " << channels << "\n";
	std::cout  << "Sample rate: " << FS << " Hz.\n";
	std::cout << std::dec << "Bits per sample: " << bits << "\n";
	std::cout << "Length: " << seconds << " seconds\n";
	std::cout << "Number of samples: " << totlength << "\n";
}
//end displayheader
//----------------------------------------------------------------------------------------

void fileio::readframe() {
		source.read((char*)initsegment, framesize*(bits/8));
		//dest.write((char*)initsegment, framesize*sizeof(*initsegment)/*(sizeof(insegment) >> 1)*/);	//and copy across
		int y;
		for(y=0; y<framesize; y++) {
			insegment[y] = ((double)(initsegment[y]))/norm;
		}	
}

//end readframe
//------------------------------------------------------------------------------------
void fileio::readhop() {
		source.read((char*)hopisegment, hop*sizeof(*hopisegment));
		int y;
		for(y=0; y<hop; y++) {
			hopsegment[y] = ((double)(hopisegment[y]))/norm;
		}
		//dest.write((char*)hopisegment, hop*sizeof(*hopisegment)/*(sizeof(insegment) >> 1)*/);	//and copy across
}

//end readhop
//------------------------------------------------------------------------------------
void fileio::slidedata(int nhop) {
	int z;
	int diff = framesize-nhop;
	//slide data along
	for (z=0; z<diff; z++) {
		insegment[z] = insegment[z+nhop];
	}
	//add hop to the end
	for (z=0; z<nhop; z++) {
		insegment[diff+z] = hopsegment[z];
	}
}

//------------------------------------------------------------------------------------
void fileio::setinmemory(int framesize1, int hop1) { 
	//initialize members framesize and hop
	framesize = framesize1;
	hop = hop1;

	initsegment = new short int[framesize];
	if (!initsegment){
		std::cerr << "Not enough memory for initsegment. \n";
		exit(-1);
	}

	insegment = new double[framesize];
	if (!insegment){
		std::cerr << "Not enough memory for insegment. \n";
		exit(-1);
	}
	hopisegment = new short int[hop];
	if (!hopisegment){
		std::cerr << "Not enough memory for hopisegment. \n";
		exit(-1);
	}
	
	hopsegment = new double[hop];
	if (!hopsegment){
		std::cerr << "Not enough memory for hopsegment. \n";
		exit(-1);
	}
}

void fileio::writechrframe(const double* fpointer, int bpo) {
	//writes bpo doubles in fpointer to stream writefile
	const char * separator = "";
	for (unsigned int i = 0; i < bpo; i++)
	{
		dest << separator << std::setprecision(20) << fpointer[i];
		separator = " ";
	}
	dest << std::endl;
	fwrite(fpointer, sizeof(double), bpo, writefile);
}
