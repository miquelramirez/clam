#ifndef __SDIFFrame__
#define __SDIFFrame__

#include "SDIFHeader.hxx"
#include <list>

namespace SDIF
{
	class Matrix;

	/** A special kind of frame at the beginning of each SDIF file
	*/
	class OpeningsFrame {
	public:
		FrameHeader mHeader;
		CLAM::TInt32 mSpecVersion;
		CLAM::TInt32 mStandardTypesVersion;
	public:	
		/** Create the frame */
		OpeningsFrame():
			mHeader("SDIF",sizeof(CLAM::TInt32)+sizeof(CLAM::TInt32)),
			mSpecVersion(3),mStandardTypesVersion(1)
		{
		}
	};

	/** SDIF data is stored in frames, that contain number of matrices.
	*       An SDIFFrame can be aware of it's successors and precessors in time 
	* (both in the whole list of frames and in the streams)
	*/
	class Frame {
	friend class File;
	friend class Collection;
	public:
		typedef std::list<Matrix*>::const_iterator MatrixIterator;
	private:
		DataFrameHeader mHeader;
		std::list<Matrix*> mMatrixList;
	public:
		/** Create a new Frame
		* @param type Identifier specifying the frame type
		* @param time Timestamp of the frame
		* @param id Stream ID
		*/
		Frame(const TypeId& type = TypeId::sDefault,
			CLAM::TFloat64 time =0.,CLAM::TInt32 streamId = 0);
		~Frame();

		/** Add a SDIFMatrix to the frame
		* @param pMatrix pointer to the SDIFMatrix to add to the frame
		*/
		void Add(Matrix* pMatrix);
		
		Matrix* GetMatrixAt(int i)
		{
			MatrixIterator it = mMatrixList.begin();
			std::advance(it,i);
			return *it;
		}
		
		MatrixIterator Begin(void) const { return mMatrixList.begin(); }
		MatrixIterator End(void) const { return mMatrixList.end(); }
		
		CLAM::TFloat64 Time(void) { return mHeader.mTime; }
		TypeId Type(void) {return mHeader.mType;}
		
	};
}

#endif
