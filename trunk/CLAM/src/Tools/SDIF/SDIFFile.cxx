#include "SDIFFile.hxx"
#include "ErrOpenFile.hxx"	// error handling on openfile error
#include "ErrFormat.hxx"		// error handling on format error

#ifdef WIN32
	#include <io.h>
	#include <fcntl.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
#endif


using namespace CLAM;


namespace SDIF
{

	File::File(const char* filename,Mode mode):
		mSkipData(false), 
		mFirstAccess(true),
		mMode(mode),
		mFile(-1),
		mSize(0)
	{
		mpName = new char[ strlen(filename)+1 ];
		strncpy( mpName, filename, strlen(filename)+1 );
	}

	File::~File()
	{
		delete [] mpName;
	}

	void File::Open(void)
	{
		int mode = 0;
		if (mMode==eInput) mode = O_RDONLY;		// defined in <fcntl.h>
		if (mMode==eOutput) mode = O_WRONLY|O_CREAT|O_TRUNC;
		if (mMode==eFullDuplex) mode = O_RDWR;

	#ifdef WIN32
  	mode |= O_BINARY;
	#endif

		mFile = open(mpName,mode,0644);
		if (mFile==-1)												// if open file error
		{
			throw ErrOpenFile(mpName); 			// throw filename
		}
		mSize = lseek(mFile,0,SEEK_END);
		lseek(mFile,0,SEEK_SET);
	}

	/* close audio file */
	void File::Close(void)
	{
		close(mFile);
	}

	void File::Read(Storage& storage)
	{
		while (!Done()) {
			Frame* frame = new Frame;
			Read(*frame);
			storage.Add(frame);
		}
	}

	void File::Write(const Storage& storage)
	{
		typedef std::list<Frame*>::const_iterator iterator;
		
		iterator it = storage.Begin();
		iterator end = storage.End();
		
		while (it!=end)
		{
			Frame* frame = *it;
			Write(*frame);
			it++;
		}
	}

	void File::Read(TypeId& type)
	{
		Read(type.mData,4);
	}

	void File::Write(const TypeId& type)
	{
		Write(type.mData,4);
	}

	void File::Read(FrameHeader& header)
	{
		Read(header.mType);
		TRead(header.mSize);
	}

	void File::Write(const FrameHeader& header)
	{
		Write(header.mType);
		TWrite(header.mSize);
	}

	void File::Read(DataFrameHeader& header)
	{
		Read((FrameHeader&)header);
		TRead(header.mTime);
		TRead(header.mStreamId);
		TRead(header.mnMatrices);
	}

	void File::Write(const DataFrameHeader& header)
	{
		Write((FrameHeader&)header);
		TWrite(header.mTime);
		TWrite(header.mStreamId);
		TWrite(header.mnMatrices);
	}

	void File::Read(Frame& frame)
	{
		if (mFirstAccess)
		{
			mFirstAccess = false;
			OpeningsFrame opening;
			Read(opening);
		}

		TInt32 p = Pos()+FrameHeader::SizeInFile();
		
		Read(frame.mHeader);

		TInt32 readSize = frame.mHeader.mSize;
		frame.mHeader.mSize = DataFrameHeader::SizeInFile();
		
		int tmp = frame.mHeader.mnMatrices;
		frame.mHeader.mnMatrices = 0; // frame.Add will increase this
		for (int i=0;i<tmp;i++)
		{
			MatrixHeader header;
			File::Read(header);
			Matrix* pMatrix = 0;
			switch (header.mDataType)
			{
			case eFloat32:
				pMatrix = new ConcreteMatrix<TFloat32>(header); break;
			case eFloat64:
				pMatrix = new ConcreteMatrix<TFloat64>(header); break;
			case eInt32:
				pMatrix = new ConcreteMatrix<TInt32>(header); break;
			case eInt64:
				pMatrix = new ConcreteMatrix<TInt64>(header); break;
			case eUTF8byte:
				pMatrix = new ConcreteMatrix<TUTF8byte>(header); break;
			case eByte:
				pMatrix = new ConcreteMatrix<TByte>(header); break;
			default:
			// +++ how do we handle unknown types ???
				pMatrix = new ConcreteMatrix<TByte>(header); break; 
			};
			File::ReadMatrixData(*pMatrix);
			frame.Add(pMatrix);
		}
		
		if (readSize-frame.mHeader.mSize==8)
		{
			// This is a kludge to read SDIF files as generated by the
			// SMSTools smscommandline application. The framesize as 
			// specified in the frame header of these files has been
			// calculate incorrectly (the size of the frame type and
			// frame size fields itself are included in the total size)
			// This result in the frame size as read from the file 
			// (readSize) being the real (calculated during read) frame
			// size frame.mHeader.mSize + 8. If we encounter this, and
			// the following data a frame header as used in the SMS-SDIF
			// files, then we silently accept and continue.
			int tmp = Pos();
			if (Done())
			{
				readSize -= 8;
			}
			else
			{
				TypeId t;
				Read(t);
				Pos(tmp);
				if (t=="1STF" || t=="1FQ0" || t=="1TRC")
				{
					readSize -= 8;
				}
			}
		}
		CLAM_ASSERT(frame.mHeader.mSize == readSize,
			"Reading Frame, calculated size does not match read size");
		CLAM_ASSERT(Pos()-p == readSize,
			"Reading Frame, size-in-file does not match read size");
	}

	void File::Write(const Frame& frame)
	{
		if (mFirstAccess)
		{
			mFirstAccess = false;
			OpeningsFrame opening;
			Write(opening);
		}

		TInt32 writeSize = frame.mHeader.mSize;
		TInt32 p = Pos()+FrameHeader::SizeInFile();

		Write(frame.mHeader);
		
		typedef std::list<Matrix*>::const_iterator iterator;

		iterator it = frame.mMatrixList.begin();
		iterator end = frame.mMatrixList.end();

		while (it!=end)
		{
			Matrix* pMatrix = *it;
			File::Write(*pMatrix);
			it++;
		}
		
		CLAM_ASSERT(Pos()-p == writeSize,
			"Incorrect framesize written in SDIF file");
	}

	void File::Read(OpeningsFrame& frame)
	{
		Read(frame.mHeader);
		TRead(frame.mSpecVersion);
		TRead(frame.mStandardTypesVersion);
	}

	void File::Write(const OpeningsFrame& frame)
	{
		Write(frame.mHeader);
		TWrite(frame.mSpecVersion);
		TWrite(frame.mStandardTypesVersion);
	}

	void File::Read(MatrixHeader& header)
	{
		Read(header.mType);
		TInt32 tmp;
		TRead(tmp);
		header.mDataType = (DataType) tmp;
		TRead(header.mnRows);
		TRead(header.mnColumns);
	}

	void File::Write(const MatrixHeader& header)
	{ 
		Write(header.mType);
		TInt32 tmp = header.mDataType;
		TWrite(tmp);
		TWrite(header.mnRows);
		TWrite(header.mnColumns);
	}

	void File::Read(Matrix& matrix)
	{
		Read(matrix.mHeader);
		if (mSkipData)
		{
			SkipMatrixData(matrix);
		} else {
			ReadMatrixData(matrix);
		}
	}

	void File::Write(const Matrix& matrix)
	{
		Write(matrix.mHeader);
		WriteMatrixData(matrix);
	}

	void File::SkipMatrixData(const Matrix& matrix)
	{
		TUInt32 size = matrix.mHeader.mnColumns*matrix.mHeader.mnRows*
			((matrix.mHeader.mDataType)&0xFF);
		TUInt32 padding = 8-size&7;
		Pos(Pos()+size+padding);
	}

	#ifdef linux
	#include <byteswap.h>
	#endif
	
	#ifdef WIN32
	#include <stdlib.h>
	#endif

	TUInt16 Swap(const TUInt16& val)
	{
	#if defined linux 
		return bswap_16(val);
	/*#elif defined WIN32
		TUInt16 ret;
		_swab((char*)(&val),(char*)(&ret),16);
		return ret;*/
	#else
		return (val>>8)|(val<<8);
	#endif
	}

	TUInt32 Swap(const TUInt32& val)
	{
	#if defined linux 
		return bswap_32(val);
	/*#elif defined WIN32
		TUInt32 ret;
		_swab((char*)(&val),(char*)(&ret),32);
		return ret;*/
	#else
  		TUInt32 cp = val;
		TByte* ptr=(TByte*) &cp;
		static TByte tmp;
		tmp=ptr[0]; ptr[0]=ptr[3]; ptr[3]=tmp;
		tmp=ptr[1]; ptr[1]=ptr[2]; ptr[2]=tmp;
  	return cp;
	#endif
	}

	TUInt64 Swap(const TUInt64& val)
	{
	#if defined linux 
		return bswap_64(val);
	/*#elif defined WIN32
		TUInt64 ret;
		_swab((char*)(&val),(char*)(&ret),64);
		return ret;*/
	#else
  		TUInt64 cp = val;
		TByte* ptr=(TByte*) &cp;
		static TByte tmp;
		tmp=ptr[0]; ptr[0]=ptr[7]; ptr[7]=tmp;
		tmp=ptr[1]; ptr[1]=ptr[6]; ptr[6]=tmp;
		tmp=ptr[2]; ptr[2]=ptr[5]; ptr[5]=tmp;
		tmp=ptr[3]; ptr[3]=ptr[4]; ptr[4]=tmp;
  	return cp;
	#endif
	}

	void File::_FixByteOrder(
		TByte* ptr,TUInt32 nElems,TUInt32 elemSize)
	{
		switch (elemSize)
		{
			case 1: return;
			case 2:
			{
				TUInt16* fptr = (TUInt16*) ptr;
				for (TUInt32 i=0;i<nElems;i++)
				{
					*fptr = Swap(*fptr);
					fptr++;
				}
				return;
			}
			case 4:
			{
				TUInt32* fptr = (TUInt32*) ptr;
				for (TUInt32 i=0;i<nElems;i++)
				{
					*fptr = Swap(*fptr);
					fptr++;
				}
				return;
			}
			case 8:
			{
				TUInt64* fptr = (TUInt64*) ptr;
				for (TUInt32 i=0;i<nElems;i++)
				{
					*fptr = Swap(*fptr);
					fptr++;
				}
				return;
			}
			default: throw;
		}
	}

	void File::ReadMatrixData(Matrix& matrix)
	{
  		TByte dum[8];
		TUInt32 nElems = matrix.mHeader.mnColumns*matrix.mHeader.mnRows;
		TUInt32 elemSize = matrix.mHeader.mDataType&0xFF;
		TUInt32 size = nElems*elemSize;

		TUInt32 padding = 8-size&7;

		matrix.Resize(nElems);
		matrix.SetSize(nElems);

		Read((TByte*) matrix.GetPtr(),size);
		FixByteOrder((TByte*) matrix.GetPtr(),nElems,elemSize);

		Read(dum,padding);  
	}

	void File::WriteMatrixData(const Matrix& matrix)
	{
  	TByte dum[8];
		TUInt32 nElems = matrix.mHeader.mnColumns*matrix.mHeader.mnRows;
		TUInt32 elemSize = matrix.mHeader.mDataType&0xFF;
		TUInt32 size = nElems*elemSize;

		TUInt32 padding = 8-size&7;

		TByte tmp[1024];
		const TByte *ptr = (const TByte*) const_cast<Matrix&>(matrix).GetPtr();
		while (size)
		{
			int blocksize = size;
			if (blocksize>1024) blocksize = 1024;
			memcpy(tmp,ptr,blocksize);

			FixByteOrder(tmp,blocksize/elemSize,elemSize);
			Write((TByte*) tmp,blocksize);

			ptr+=blocksize;
			size-=blocksize;
		}

		Write(dum,padding);  
	}

}
