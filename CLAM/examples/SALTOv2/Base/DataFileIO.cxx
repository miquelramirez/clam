#include "DataFileIO.hxx"
#include "ErrOpenFile.hxx"	// error handling on openfile error
#include "ErrFormat.hxx"		// error handling on format error

using namespace CLAM;

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif

#ifdef macintosh
	
	char *strdup(const char* inStr);

	char *strdup(const char* inStr)
	{
		char* ret = (char*) malloc(strlen(inStr)+1);
		strcpy(ret,inStr);
		return ret;
	}
#endif





#include <fcntl.h>      

DataFileIO::DataFileIO(
	const char* pName,const EIOMode& mode)
:IO(mode)
{
//	mpName = strdup(pName);
	mpName = new char[ strlen(pName)+1 ];
	strncpy( mpName, pName, strlen(pName)+1 );
	mFile = -1;
}

DataFileIO::~DataFileIO()
{
//	free (mpName);
	delete [] mpName;
}
/* open audio file */
void DataFileIO::Open(void)
{
	int mode = 0;
	if (mMode==eInput) mode = O_RDONLY;		// defined in <fcntl.h>
	if (mMode==eOutput) mode = O_WRONLY|O_CREAT|O_TRUNC;
	if (mMode==eFullDuplex) mode = O_RDWR;

#ifdef WIN32
  mode |= O_BINARY;
#endif

	mFile = open(mpName,mode);
	if (mFile==-1)												// if open file error
	{
		throw ErrOpenFile(mpName); 			// throw filename
	}
	mSize = lseek(mFile,0,SEEK_END);
	lseek(mFile,0,SEEK_SET);
}

/* close audio file */
void DataFileIO::Close(void)
{
	close(mFile);
}

