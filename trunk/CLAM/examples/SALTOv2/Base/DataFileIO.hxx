/* -----------------------------------------------------------------
   The Class DataFileIO deals with different Audio IO Situations:
   
	 create Audiostream for input, output or fullduplex
   open, close, read, write Audiofile
   Audiosettings are stored in mSettings which is a class of the
   type 'AudioSettings' 
-------------------------------------------------------------------*/

#ifndef _DataFileIO_
#define _DataFileIO_

#include "IO.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Err.hxx"

#ifndef WIN32
	#ifdef macintosh
		#include <types.h>
		#include <unistd.h>	
	#else
		#include <sys/types.h>
		#include <unistd.h>
	#endif
#else
  #include <stdio.h>
  #include <io.h>
#endif

namespace CLAM
{

class DataFileIO:public IO
{
private:
	int mFile;
	TSize mSize;
	char* mpName;
public:
/* Creates an data stream for input/output/fullduplex */	
	DataFileIO(const char* pName,const EIOMode& mode);
	~DataFileIO();

	TIndex Pos(void);
	TIndex Pos(TIndex pos);
	bool Done(void);
	
	void Open(void);										//open data file
	void Close(void);										//close data file


  template <class T> void Read(Array<T>& data)					//read from data file
  {
  	if (mFile==-1)													// check if file is opened 
  		throw Err("DataFileIO not opened");
  	
  	// added (char*) typecast to compile in MCW	
  	int size = read(mFile,(char*)data.GetPtr(),data.AllocatedSizeInBytes());
  	data.SetSize(size/sizeof(T));
  }

  template <class T> void Write(Array<T>& data)			  //write to data file
  {
  	if (mFile==-1)													// check if file is open
  		throw Err("DataFileIO not opened");

  	// added (char*) typecast to compile in MCW
  	write(mFile,(char*)data.GetPtr(),data.SizeInBytes());
  }

	void Read(TInt32& t);
	void Read(TFloat64& t);
	void Read(TByte* ptr,int n);

	void Write(const TInt32& t);
	void Write(const TFloat64& t);
	void Write(const TByte* ptr,int n);
};


inline void DataFileIO::Read(TInt32& t)
{
	Read((TByte*)&t,sizeof(t));
}

inline void DataFileIO::Read(TFloat64& t)
{
	Read((TByte*)&t,sizeof(t));
}

inline void DataFileIO::Read(TByte* ptr,int n)
{
	if (read(mFile,(char*)ptr,n)!=n) {
  		throw Err("DataFileIO read error");
	}
}

inline int DataFileIO::Pos(void)
{	
	int pos = lseek(mFile,0,SEEK_CUR);
	return pos;
}

inline int DataFileIO::Pos(int pos)
{
	return lseek(mFile,pos,SEEK_SET);	
}

inline bool DataFileIO::Done(void)
{
	return Pos()>=mSize;
}

inline void DataFileIO::Write(const TInt32& t)
{
	Write((const TByte*)&t,sizeof(t));
}

inline void DataFileIO::Write(const TFloat64& t)
{
	Write((const TByte*)&t,sizeof(t));
}

inline void DataFileIO::Write(const TByte* ptr,int n)
{
	if (write(mFile,(const char*)ptr,n)!=n) {
  		throw Err("DataFileIO read error");
	}
}

}; // end of namespace CLAM

#endif
