/*
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
 
#include "boost/python.hpp"
#include "CLAM/DynamicTypeMacros.hxx"
#include "CLAM/Flags.hxx"
#include "CLAM/SpecTypeFlags.hxx"
#include "CLAM/Component.hxx"
#include "CLAM/Storage.hxx"
#include "CLAM/XMLStorage.hxx"
#include "CLAM/OutControl.hxx"
#include "CLAM/InControl.hxx"
#include "CLAM/Processing.hxx"
#include "CLAM/ProcessingConfig.hxx"
#include "CLAM/ProcessingData.hxx"
#include "CLAM/ProcessingDataConfig.hxx"
#include "CLAM/ProcessingDataPlugin.hxx"
#include "CLAM/FlowControl.hxx"
#include "CLAM/FlattenedNetwork.hxx"
#include "CLAM/Network.hxx"
#include "CLAM/NetworkPlayer.hxx"
#include "CLAM/DataTypes.hxx"
#include "CLAM/Enum.hxx"
#include "CLAM/CLAM_Math.hxx"
#include "CLAM/Err.hxx"
#include "CLAM/MonoAudioFileReader.hxx"
#include "CLAM/AudioInFilename.hxx"
#include "CLAM/Spectrum.hxx"
#include "src/Definitions.hxx"
#include "Spectrum.pypp.hpp"

namespace bp = boost::python;

void register_Spectrum_class(){

    { //::CLAM::Spectrum
        typedef bp::class_< CLAM::Spectrum > Spectrum_exposer_t;
        Spectrum_exposer_t Spectrum_exposer = Spectrum_exposer_t( "Spectrum" );
        bp::scope Spectrum_scope( Spectrum_exposer );

        bp::scope().attr("eNumAttr") = (int)CLAM::Spectrum::eNumAttr;

        { //::CLAM::Spectrum::AddAll
        
            typedef void ( ::CLAM::Spectrum::*AddAll_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddAll"
                , AddAll_function_type( &::CLAM::Spectrum::AddAll ) );
        
        }
        { //::CLAM::Spectrum::AddComplexArray
        
            typedef void ( ::CLAM::Spectrum::*AddComplexArray_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddComplexArray"
                , AddComplexArray_function_type( &::CLAM::Spectrum::AddComplexArray ) );
        
        }
        { //::CLAM::Spectrum::AddMagBPF
        
            typedef void ( ::CLAM::Spectrum::*AddMagBPF_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddMagBPF"
                , AddMagBPF_function_type( &::CLAM::Spectrum::AddMagBPF ) );
        
        }
        { //::CLAM::Spectrum::AddMagBuffer
        
            typedef void ( ::CLAM::Spectrum::*AddMagBuffer_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddMagBuffer"
                , AddMagBuffer_function_type( &::CLAM::Spectrum::AddMagBuffer ) );
        
        }
        { //::CLAM::Spectrum::AddPhaseBPF
        
            typedef void ( ::CLAM::Spectrum::*AddPhaseBPF_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddPhaseBPF"
                , AddPhaseBPF_function_type( &::CLAM::Spectrum::AddPhaseBPF ) );
        
        }
        { //::CLAM::Spectrum::AddPhaseBuffer
        
            typedef void ( ::CLAM::Spectrum::*AddPhaseBuffer_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddPhaseBuffer"
                , AddPhaseBuffer_function_type( &::CLAM::Spectrum::AddPhaseBuffer ) );
        
        }
        { //::CLAM::Spectrum::AddPolarArray
        
            typedef void ( ::CLAM::Spectrum::*AddPolarArray_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddPolarArray"
                , AddPolarArray_function_type( &::CLAM::Spectrum::AddPolarArray ) );
        
        }
        { //::CLAM::Spectrum::AddScale
        
            typedef void ( ::CLAM::Spectrum::*AddScale_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddScale"
                , AddScale_function_type( &::CLAM::Spectrum::AddScale ) );
        
        }
        { //::CLAM::Spectrum::AddSpectralRange
        
            typedef void ( ::CLAM::Spectrum::*AddSpectralRange_function_type )(  ) ;
            
            Spectrum_exposer.def( 
                "AddSpectralRange"
                , AddSpectralRange_function_type( &::CLAM::Spectrum::AddSpectralRange ) );
        
        }
        { //::CLAM::Spectrum::SetSize
        
            typedef void ( ::CLAM::Spectrum::*SetSize_function_type )( int ) ;
            
            Spectrum_exposer.def( 
                "SetSize"
                , SetSize_function_type( &::CLAM::Spectrum::SetSize )
                , ( bp::arg("s") ) );
        
        }
        { //::CLAM::Spectrum::SetType
        
            typedef void ( ::CLAM::Spectrum::*SetType_function_type )( ::CLAM::SpecTypeFlags const & ) ;
            
            Spectrum_exposer.def( 
                "SetType"
                , SetType_function_type( &::CLAM::Spectrum::SetType )
                , ( bp::arg("newFlags") ) );
        
        }
    }

}
