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

#include<iostream>
#include "OSDefines.hxx"
#include "LPC.hxx"
#ifdef CLAM_USE_XML
#include "XMLStorage.hxx"
#endif

int main( int argc, char* argv[] )
{
	try{
		int i , Size = 1024;
		int LPC_order = 8;
		float SampleRate = 8000.0;

		// Processing data declaration
		CLAM::Audio myAudio;
		CLAM::DataArray A;	// filter coefficients
		CLAM::DataArray K;	// reflection coefficients
		CLAM::TData E;	// average of mean squared error of the filter

		// Audio configuration and initialization
		myAudio.SetSize( Size );

		for( i = 0; i < Size; i++ )
			myAudio.GetBuffer()[ i ] = 0.625 + 0.5 * sin(2.0*PI*400.0*(((float)i)/SampleRate));

		// DataArray configuration
		A.Resize( LPC_order );
		A.SetSize( LPC_order );
		K.Resize( LPC_order );
		K.SetSize( LPC_order );

		// Processing composite configuration
		CLAM::LPCConfig LPC_cfg;
		LPC_cfg.SetName( "My_LPC" );
		LPC_cfg.SetOrder( LPC_order );

		// Processing composite creation
		CLAM::LPC myLPC( LPC_cfg );

		// End of Processing composite configuration
		myLPC.Start();

		// Processing composite execution
		std::cout << "Running object" << std::endl;
		myLPC.Do( myAudio, A, K, E );

#ifdef CLAM_USE_XML
		CLAM::XMLStorage storage;
		storage.Dump( A, "LPCResult", "LPC_exampleA.xml" );
#endif // CLAM_USE_XML

	}
	catch( CLAM::Err err )
	{
		err.Print();
	}

	return 0;
}