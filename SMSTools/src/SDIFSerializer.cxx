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

#include <iostream>

#include "SDIFSerializer.hxx"
#include <CLAM/SDIFIn.hxx>
#include <CLAM/SDIFOut.hxx>
#include <CLAM/Segment.hxx>


using namespace CLAM;

SDIFSerializer::SDIFSerializer(  )
{
}

SDIFSerializer::~SDIFSerializer(  )
{
}

bool SDIFSerializer::DoLoad( const char* fileName, Segment& segment )
{
	SDIFInConfig cfg;
	cfg.SetMaxNumPeaks( 100 );
	cfg.SetFileName( fileName );
	cfg.SetEnableResidual( true );
	mSDIFReader.Configure( cfg );
		
	segment.AddAll(  );
	segment.UpdateData(  );
	// @TODO: Needs InPort/OutPort fix!
	//mSDIFReader.Output.Attach( segment );

	try{
		mSDIFReader.Start(  );
		while( mSDIFReader.Do(segment) ) {  }
		mSDIFReader.Stop(  );
	} catch (Err e)
	{
		std::cout << e.what() << std::endl;
	}

	return true;
}

bool SDIFSerializer::DoStore( const char* fileName, Segment& segment )
{
	int i;
	int nFrames = segment.GetnFrames();
	
	SDIFOutConfig cfg;
	cfg.SetSamplingRate( segment.GetSamplingRate() );
	cfg.SetFrameSize((segment.GetFrame(0).GetResidualSpec().GetSize()-1)*2+1);
	cfg.SetSpectrumSize(segment.GetFrame(0).GetResidualSpec().GetSize());
		
	cfg.SetFileName( fileName );
	cfg.SetEnableResidual( true );

	// Unused variable: 
	bool configOk = mSDIFWriter.Configure( cfg );
	CLAM_DEBUG_ASSERT(configOk, "Error storing temporal SDIF for Morph" );

	mSDIFWriter.Start(  );
	for( i=0; i<nFrames; i++ )
	{
		mSDIFWriter.Do( segment.GetFrame( i ) );
	}
	mSDIFWriter.Stop(  );

	return true;
}

