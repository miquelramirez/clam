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

#include "SDIFHeader.hxx"

namespace SDIF
{

	FrameHeader::FrameHeader(
		const TypeId& type, CLAM::TInt32 size)
	{
		mType = type;
		mSize = size;
	}

	DataFrameHeader::DataFrameHeader(const TypeId& type,CLAM::TFloat64 time,CLAM::TInt32 id)
	:FrameHeader(type,SizeInFile()) 
	{
		mTime = time;
		mStreamId = id;
		mnMatrices = 0;	
	}

	MatrixHeader::MatrixHeader(const TypeId& type = "\0\0\0\0",DataType dataType = eUnknown,
		CLAM::TInt32 nRows = 0, CLAM::TInt32 nColumns = 0)
	{
		mType = type;
		mDataType = dataType;
		mnRows = nRows;
		mnColumns = nColumns;
	}

}

