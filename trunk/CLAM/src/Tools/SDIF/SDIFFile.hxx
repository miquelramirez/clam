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

#ifndef __SDIFFile__
#define __SDIFFile__

#include "SDIFType.hxx"
#include "SDIFHeader.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"
#include "SDIFStorage.hxx"
#include "ByteOrder.hxx"

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

/** Used to read or write an SDIF file. When reading, the File
* parses the whole files and passes the read Frames to a 
* Storage, typically a Collection to store all in memory.
*       
*       Note that the Storage is responsable for the 
*       freeing memory of the read frames, though File
*       creates them.
*       
*       SDIF files use BIG ENDIAN byte order for the data storage.
*       SDIFFile takes care of fixing the byte order in case
*       the operating system is not BIG ENDIANm, @see CLAMByteOrder
*/

namespace SDIF
{

class File
{
public:
	enum Mode
	{
		eInput = 1, eOutput = 2, eFullDuplex = 3
	};

	File(const char* filename, Mode mode);
	~File();

	void Open(void);										//open data file
	void Close(void);										//close data file

	bool Done(void);

	CLAM::TIndex Pos(void);
	CLAM::TIndex Pos(CLAM::TIndex pos);

private:
	bool mSkipData;
	bool mFirstAccess;
	char* mpName;
	Mode mMode;
	int mFile;
	CLAM::TSize mSize;


/*	
  template <class T> void ReadArray(Array<T>& data)					//read from data file
  {
  	if (mFile==-1)													// check if file is opened 
  		throw Err("DataFileIO not opened");
  	
  	// added (char*) typecast to compile in MCW	
  	int size = read(mFile,(char*)data.GetPtr(),data.AllocatedSizeInBytes());
  	data.SetSize(size/sizeof(T));
  }

  template <class T> void WriteArray(const Array<T>& data)			  //write to data file
  {
  	if (mFile==-1)													// check if file is open
  		throw Err("DataFileIO not opened");

  	// added (char*) typecast to compile in MCW
  	write(mFile,(char*)data.GetPtr(),data.SizeInBytes());
  }
*/

	void Read(CLAM::TByte* ptr,int n);

	template <class T> void TRead(T& t)
	{
		Read((CLAM::TByte*) &t,sizeof(T));
		FixByteOrder((CLAM::TByte*) &t,1,sizeof(T));
	}

	void Write(const CLAM::TByte* ptr,int n);

	template <class T> void TWrite(const T& t)
	{
		T tmp(t);
		FixByteOrder((CLAM::TByte*) &tmp,1,sizeof(T));
		Write((CLAM::TByte*) &tmp,sizeof(T));
	}

	inline void FixByteOrder(CLAM::TByte* ptr,
		CLAM::TUInt32 nElems,CLAM::TUInt32 elemSize);
	
	void Read(DataFrameHeader& header);
	void Write(const DataFrameHeader& header);

	void Read(FrameHeader& header);
	void Write(const FrameHeader& header);

	void Read(MatrixHeader& header);
	void Write(const MatrixHeader& header);

	void Read(OpeningsFrame& frame);
	void Write(const OpeningsFrame& frame);

	void Read(Matrix& matrix);
	void Write(const Matrix& matrix);

	void Read(TypeId& header);
	void Write(const TypeId& header);

	void SkipMatrixData(const Matrix& matrix);
	void ReadMatrixData(Matrix& matrix);
	void WriteMatrixData(const Matrix& matrix);

public:
	void Read(Storage& storage);
	void Write(const Storage& storage);

	void Read(Frame& frame);
	void Write(const Frame& frame);

private:
	void _FixByteOrder(
		CLAM::TByte* ptr,CLAM::TUInt32 nElems,CLAM::TUInt32 elemSize);
};

inline int File::Pos(void)
{	
	int pos = lseek(mFile,0,SEEK_CUR);
	return pos;
}

inline int File::Pos(int pos)
{
	return lseek(mFile,pos,SEEK_SET);	
}

inline void File::Read(CLAM::TByte* ptr,int n)
{
	if (read(mFile,(char*)ptr,n)!=n) {
  		throw CLAM::Err("DataFileIO read error");
	}
}

inline void File::Write(const CLAM::TByte* ptr,int n)
{
	if (write(mFile,(const char*)ptr,n)!=n) {
  		throw CLAM::Err("DataFileIO read error");
	}
}

inline bool File::Done(void)
{
	return Pos()>=mSize;
}

inline void File::FixByteOrder(CLAM::TByte* ptr,
	CLAM::TUInt32 nElems,CLAM::TUInt32 elemSize)
{
#ifdef CLAM_LITTLE_ENDIAN
	_FixByteOrder(ptr,nElems,elemSize);
#else
#ifndef CLAM_BIG_ENDIAN
#pragma message ("BYTE ORDER NOT DEFINED!")
#endif
#endif
}

}

#endif

