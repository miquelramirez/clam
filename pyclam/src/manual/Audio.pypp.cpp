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
#include "CLAM/FFT.hxx"
#include "CLAM/FFT_base.hxx"
#include "CLAM/FFT_ooura.hxx"
#include "CLAM/FFT_fftw3.hxx"
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
#include "CLAM/Audio.hxx"
#include "CLAM/MonoAudioFileReader.hxx"
#include "CLAM/AudioInFilename.hxx"
#include "src/Definitions.hxx"
#include "Audio.pypp.hpp"

namespace bp = boost::python;

void register_Audio_class(){

    { //::CLAM::Audio
        typedef bp::class_< CLAM::Audio > Audio_exposer_t;
        Audio_exposer_t Audio_exposer = Audio_exposer_t( "Audio" );
        bp::scope Audio_scope( Audio_exposer );

        bp::scope().attr("eNumAttr") = (int)CLAM::Audio::eNumAttr;

        { //::CLAM::Audio::AddBeginTime
        
            typedef void ( ::CLAM::Audio::*AddBeginTime_function_type )(  ) ;
            
            Audio_exposer.def( 
                "AddBeginTime"
                , AddBeginTime_function_type( &::CLAM::Audio::AddBeginTime ) );
        
        }
        { //::CLAM::Audio::AddBuffer
        
            typedef void ( ::CLAM::Audio::*AddBuffer_function_type )(  ) ;
            
            Audio_exposer.def( 
                "AddBuffer"
                , AddBuffer_function_type( &::CLAM::Audio::AddBuffer ) );
        
        }
        { //::CLAM::Audio::AddSampleRate
        
            typedef void ( ::CLAM::Audio::*AddSampleRate_function_type )(  ) ;
            
            Audio_exposer.def( 
                "AddSampleRate"
                , AddSampleRate_function_type( &::CLAM::Audio::AddSampleRate ) );
        
        }
        { //::CLAM::Audio::GetBuffer
        
            typedef ::CLAM::DataArray & ( ::CLAM::Audio::*GetBuffer_function_type )(  ) const;
            
            Audio_exposer.def( 
                "GetBuffer"
                , GetBuffer_function_type( &::CLAM::Audio::GetBuffer )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::CLAM::Audio::GetDynamicTypeCopy
        
            typedef ::CLAM::DynamicType & ( ::CLAM::Audio::*GetDynamicTypeCopy_function_type )( bool const,bool const ) const;
            
            Audio_exposer.def( 
                "GetDynamicTypeCopy"
                , GetDynamicTypeCopy_function_type(&::CLAM::Audio::GetDynamicTypeCopy)
                , ( bp::arg("shareData")=(bool const)(false), bp::arg("deep")=(bool const)(false) )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::CLAM::Audio::GetSize
        
            typedef int ( ::CLAM::Audio::*GetSize_function_type )(  ) const;
            
            Audio_exposer.def( 
                "GetSize"
                , GetSize_function_type( &::CLAM::Audio::GetSize ) );
        
        }
        { //::CLAM::Audio::GetTypeId
        
            typedef ::std::type_info const & ( ::CLAM::Audio::*GetTypeId_function_type )( unsigned int ) const;
            
            Audio_exposer.def( 
                "GetTypeId"
                , GetTypeId_function_type(&::CLAM::Audio::GetTypeId)
                , ( bp::arg("n") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::Audio::HasBeginTime
        
            typedef bool ( ::CLAM::Audio::*HasBeginTime_function_type )(  ) const;
            
            Audio_exposer.def( 
                "HasBeginTime"
                , HasBeginTime_function_type( &::CLAM::Audio::HasBeginTime ) );
        
        }
        { //::CLAM::Audio::HasBuffer
        
            typedef bool ( ::CLAM::Audio::*HasBuffer_function_type )(  ) const;
            
            Audio_exposer.def( 
                "HasBuffer"
                , HasBuffer_function_type( &::CLAM::Audio::HasBuffer ) );
        
        }
        { //::CLAM::Audio::HasSampleRate
        
            typedef bool ( ::CLAM::Audio::*HasSampleRate_function_type )(  ) const;
            
            Audio_exposer.def( 
                "HasSampleRate"
                , HasSampleRate_function_type( &::CLAM::Audio::HasSampleRate ) );
        
        }
        { //::CLAM::Audio::RemoveBeginTime
        
            typedef void ( ::CLAM::Audio::*RemoveBeginTime_function_type )(  ) ;
            
            Audio_exposer.def( 
                "RemoveBeginTime"
                , RemoveBeginTime_function_type( &::CLAM::Audio::RemoveBeginTime ) );
        
        }
        { //::CLAM::Audio::RemoveBuffer
        
            typedef void ( ::CLAM::Audio::*RemoveBuffer_function_type )(  ) ;
            
            Audio_exposer.def( 
                "RemoveBuffer"
                , RemoveBuffer_function_type( &::CLAM::Audio::RemoveBuffer ) );
        
        }
        { //::CLAM::Audio::RemoveSampleRate
        
            typedef void ( ::CLAM::Audio::*RemoveSampleRate_function_type )(  ) ;
            
            Audio_exposer.def( 
                "RemoveSampleRate"
                , RemoveSampleRate_function_type( &::CLAM::Audio::RemoveSampleRate ) );
        
        }
        { //::CLAM::Audio::SetBuffer
        
            typedef void ( ::CLAM::Audio::*SetBuffer_function_type )( ::CLAM::DataArray const & ) ;
            
            Audio_exposer.def( 
                "SetBuffer"
                , SetBuffer_function_type( &::CLAM::Audio::SetBuffer )
                , ( bp::arg("arg") ) );
        
        }
        { //::CLAM::Audio::SetSampleRate
        
            typedef void ( ::CLAM::Audio::*SetSampleRate_function_type )( ::CLAM::TData const & ) ;
            
            Audio_exposer.def( 
                "SetSampleRate"
                , SetSampleRate_function_type( &::CLAM::Audio::SetSampleRate )
                , ( bp::arg("arg") ) );
        
        }
        { //::CLAM::Audio::SetSize
        
            typedef void ( ::CLAM::Audio::*SetSize_function_type )( int ) ;
            
            Audio_exposer.def( 
                "SetSize"
                , SetSize_function_type( &::CLAM::Audio::SetSize )
                , ( bp::arg("s") ) );
        
        }

    }

}
