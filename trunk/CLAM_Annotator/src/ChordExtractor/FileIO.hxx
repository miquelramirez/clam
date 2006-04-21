#ifndef FILEIO_H
#define FILEIO_H
#include <fstream>
#include <cstdio>
class fileio {	
	unsigned short int channels;		// number of channels
	unsigned short int FS;				// sample rate
	unsigned short int bits;			// bits per sample
	double norm;						// data multiplier
	unsigned int totbytes;				// total number of bytes in whole file
	unsigned int totlength;				// total number of samples per channel
	float seconds;						// length in seconds
	int framesize;						// size of data to be read by readframe
	int hop;							// size of data to be read by readhop
	
	std::ifstream source;					// stream buffers
	std::ofstream dest;
	std::ofstream cqbuff;
	std::ofstream fbuff;
	FILE* writefile;

	short int* initsegment;
	double* insegment;			//array for input
	short int* hopisegment;
	double* hopsegment;
	
public:
	fileio();
	~fileio();
	void opinfile(char*);
	void opcqfile(char*);
	void opchrfile(char*);
	void clinfile();
	void cloutfile();
	void clcqfile();
	void clffile();
	void clchrfile();
	void readheader();
	void displayheader();
	void readframe();
	void readhop();
	void setinmemory(int, int);
	void slidedata(int);
	void writechrframe(const double*, int);

	// functions for access to data
	unsigned short int getFS() {return(FS);}
	unsigned short int getbits() {return(bits);}
	std::ifstream & getsource() {return(source);}
	std::ofstream & getdest()   {return(dest);}
	double* getsegpointer() {return(insegment);}
	unsigned int getlength() {return(totlength);}

};
#endif
