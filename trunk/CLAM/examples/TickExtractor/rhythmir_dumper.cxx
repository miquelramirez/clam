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

#include "rhythmir_dumper.hxx"
#include <CLAM/XMLStorage.hxx>
#include <CLAM/XmlScoped.hxx>
#include <CLAM/Array.hxx>
#include <fstream>
#include <iostream>
#include "Meter.hxx"

using namespace CLAM;

std::ostream& operator<<( std::ostream& os, const DataArray& array )
{
	for ( int i = 0; i < array.Size(); i++ )
		os << array[i] << " ";

	return os;
}

namespace RhythmIR
{

	void DumpToSimacXML( CLAM::DescriptionDataPool& pool,
			     std::string filename )
	{
		std::ofstream fileStream( filename.c_str() );

	

		XmlFragment frag( fileStream );
		{
			XmlElement element( "Simac:RhythmDescription" );
			{
				XmlElement element( "Simac:MediaDigitalFile" );
				XmlAttribute attrib( "path", *pool.GetWritePool<std::string>("Global","Path") );
			}		
			{
				XmlElement element( "Simac:BeatsPerMinute" );
				XmlAttribute attrib( "value", *pool.GetWritePool<unsigned>("Global","BeatsPerMinute") );
			}
			{
				XmlElement   element( "Simac:BeatSequence" );
				int nBeats = pool.GetNumberOfContexts( "Beat" );

				XmlAttribute attrib( "size", nBeats );
				XmlAttribute attrib1( "timeunit", *pool.GetWritePool<std::string>("Global", "Units.Beat.Position") );

				if ( nBeats > 0 )
				{
					CLAM::TTime* beatPtr = pool.GetWritePool<CLAM::TTime>("Beat", "Position" );
				
					for ( int k = 0; k < nBeats-1; k++ )
					{
						XmlContent conel( beatPtr[k] );
						XmlContent blank( " " );
					}
					XmlContent conel( beatPtr[nBeats-1] );
				}
			
			}
			{
				XmlElement element( "Simac:TicksPerMinute" );
				XmlAttribute attrib( "value", *pool.GetWritePool<unsigned>("Global","TicksPerMinute") );
			}
			{
				XmlElement   element( "Simac:TicksSequence" );

				int nTicks = pool.GetNumberOfContexts( "Tick" );

				XmlAttribute attrib( "size", nTicks );
				XmlAttribute attrib1( "timeunit", *pool.GetWritePool<std::string>("Global", "Units.Tick.Position")  );

				if ( nTicks > 0 )
				{
					CLAM::TTime* tickPtr = pool.GetWritePool<CLAM::TTime>("Tick", "Position" );
				
					for ( int k = 0; k < nTicks; k++ )
					{
						XmlContent conel( tickPtr[k] );
						XmlContent blank( " " );
					}
					XmlContent conel( tickPtr[nTicks-1] );
				}
			
			}
			{
				XmlElement element( "Simac:Meter");

				const CLAM::RhythmDescription::Meter& value = *pool.GetWritePool<CLAM::RhythmDescription::Meter>("Global","Meter");

				XmlAttribute attr_num( "numerator", value.GetNumerator() );
				XmlAttribute attr_den( "denominator", value.GetDenominator() );
			}

		}


		fileStream.close();
	
	}

	void DumpToWavesurferLabs( CLAM::DescriptionDataPool& pool, std::string outputFilename )
	{
		// Onsets dumping
		{
			std::string filename = outputFilename;
			filename.replace( filename.find(".xml"), 4, "_onsets.lab" );

			std::ofstream file( filename.c_str() );

			int   nOnsets = pool.GetNumberOfContexts( "Onset" );
			CLAM::TData srate = *pool.GetWritePool<CLAM::TData>("Global","SampleRate");

			if ( nOnsets > 0 )
			{
				CLAM::TTime* onsetPositions = pool.GetWritePool<CLAM::TTime>("Onset", "Position" );
				for ( int k = 1; k < nOnsets; k++ )
					file << onsetPositions[k-1]/srate<< " " << onsetPositions[k]/srate<< " " << "onset" << std::endl;
			}

			file.close();
		}

		// Beats dumping
		{
			std::string filename = outputFilename;
			filename.replace( filename.find(".xml"), 4, "_beats.lab" );

			std::ofstream file( filename.c_str() );

			int   nBeats = pool.GetNumberOfContexts( "Beat" );
			CLAM::TData srate = *pool.GetWritePool<CLAM::TData>("Global","SampleRate");

			if ( nBeats > 0 )
			{
				CLAM::TTime* beatPositions = pool.GetWritePool<CLAM::TTime>("Beat", "Position" );
				file << 0.0 << " " << beatPositions[0] << " " << "beat" << std::endl;
				for ( int k = 1; k < nBeats; k++ )
					file << beatPositions[k-1]<< " " << beatPositions[k]<< " " << "beat" << std::endl;
			}

			file.close();
		}

		// Ticks dumping
		{
			std::string filename = outputFilename;
			filename.replace( filename.find(".xml"), 4, "_ticks.lab" );

			std::ofstream file( filename.c_str() );

			int   nTicks = pool.GetNumberOfContexts( "Tick" );
			CLAM::TData srate = *pool.GetReadPool<CLAM::TData>("Global","SampleRate");

			if ( nTicks > 0 )
			{
				CLAM::TTime* tickPositions = pool.GetWritePool<CLAM::TTime>("Tick", "Position" );
				file << 0.0 << " " << tickPositions[0] << " " << "tick" << std::endl;
				for ( int k = 1; k < nTicks; k++ )
					file << tickPositions[k-1]<< " " << tickPositions[k]<< " " << "tick" << std::endl;
			}

			file.close();
		}



	}

}

