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
#include "XMLSerializer.hxx"

#include "Segment.hxx"

using namespace CLAM;

XMLSerializer::XMLSerializer(  ) {  }

XMLSerializer::~XMLSerializer(  ) {  }

bool XMLSerializer::DoLoad( const char* fileName, Segment& segment )
{
	mXMLStorage.Restore( segment, fileName );

	return true;
}

bool XMLSerializer::DoStore( const char* fileName, Segment& segment )
{
	segment.RemoveAudio();
	segment.UpdateData();
	int i=0;
	int nFrames=segment.GetnFrames();

	for( i = 0; i < nFrames; i++ )
	{
		Frame& tmpFrame = segment.GetFrame( i );
		tmpFrame.RemoveAudioFrame(  ); //windowed audio frame
		tmpFrame.RemoveSinusoidalAudioFrame(  );
		tmpFrame.RemoveResidualAudioFrame(  );
		tmpFrame.RemoveSinusoidalSpec(  );
		tmpFrame.RemoveSpectrum(  );//this could be kept for direct IFFT

		//Now we remove auxiliary data formats for residual spectrum
		SpecTypeFlags tmpFl;
		tmpFrame.GetResidualSpec(  ).SetType( tmpFl );
	}

	mXMLStorage.Dump( segment, "Segment", fileName );

	//Now we add Spectrum back, it is needed for Melody analysis
	for( i = 0; i < nFrames; i++ )
	{
		Frame& tmpFrame = segment.GetFrame( i );
		tmpFrame.AddSpectrum(  );//this could be kept for direct IFFT
		tmpFrame.UpdateData(  );

	}

	return true;
}
