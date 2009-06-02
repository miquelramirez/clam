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
#include "CLAM/MonoAudioFileReader.hxx"
#include "CLAM/AudioInFilename.hxx"
#include "CLAM/AudioFile.hxx"
#include "CLAM/Text.hxx"
#include "CLAM/Filename.hxx"
#include "CLAM/AudioTextDescriptors.hxx"
#include "src/Definitions.hxx"
#include "AudioTextDescriptors.pypp.hpp"

namespace bp = boost::python;

void register_AudioTextDescriptors_class(){

    { //::CLAM::AudioTextDescriptors
        typedef bp::class_< CLAM::AudioTextDescriptors > AudioTextDescriptors_exposer_t;
        AudioTextDescriptors_exposer_t AudioTextDescriptors_exposer = AudioTextDescriptors_exposer_t( "AudioTextDescriptors", "" );
        bp::scope AudioTextDescriptors_scope( AudioTextDescriptors_exposer );
        bp::scope().attr("eNumAttr") = (int)CLAM::AudioTextDescriptors::eNumAttr;
        AudioTextDescriptors_exposer.def( bp::init< >("") );
        AudioTextDescriptors_exposer.def( bp::init< CLAM::AudioTextDescriptors const &, bp::optional< bool, bool > >(( bp::arg("prototype"), bp::arg("shareData")=(bool const)(false), bp::arg("deep")=(bool const)(true) ), "") );
        { //::CLAM::AudioTextDescriptors::AddAlbum
        
            typedef void ( ::CLAM::AudioTextDescriptors::*AddAlbum_function_type )(  ) ;
            
            AudioTextDescriptors_exposer.def( 
                "AddAlbum"
                , AddAlbum_function_type( &::CLAM::AudioTextDescriptors::AddAlbum )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::AddAll
        
            typedef void ( ::CLAM::AudioTextDescriptors::*AddAll_function_type )(  ) ;
            
            AudioTextDescriptors_exposer.def( 
                "AddAll"
                , AddAll_function_type( &::CLAM::AudioTextDescriptors::AddAll )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::AddArtist
        
            typedef void ( ::CLAM::AudioTextDescriptors::*AddArtist_function_type )(  ) ;
            
            AudioTextDescriptors_exposer.def( 
                "AddArtist"
                , AddArtist_function_type( &::CLAM::AudioTextDescriptors::AddArtist )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::AddComposer
        
            typedef void ( ::CLAM::AudioTextDescriptors::*AddComposer_function_type )(  ) ;
            
            AudioTextDescriptors_exposer.def( 
                "AddComposer"
                , AddComposer_function_type( &::CLAM::AudioTextDescriptors::AddComposer )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::AddPerformer
        
            typedef void ( ::CLAM::AudioTextDescriptors::*AddPerformer_function_type )(  ) ;
            
            AudioTextDescriptors_exposer.def( 
                "AddPerformer"
                , AddPerformer_function_type( &::CLAM::AudioTextDescriptors::AddPerformer )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::AddTitle
        
            typedef void ( ::CLAM::AudioTextDescriptors::*AddTitle_function_type )(  ) ;
            
            AudioTextDescriptors_exposer.def( 
                "AddTitle"
                , AddTitle_function_type( &::CLAM::AudioTextDescriptors::AddTitle )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::AddTrackNumber
        
            typedef void ( ::CLAM::AudioTextDescriptors::*AddTrackNumber_function_type )(  ) ;
            
            AudioTextDescriptors_exposer.def( 
                "AddTrackNumber"
                , AddTrackNumber_function_type( &::CLAM::AudioTextDescriptors::AddTrackNumber )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::GetAlbum
        
            typedef ::CLAM::Text & ( ::CLAM::AudioTextDescriptors::*GetAlbum_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetAlbum"
                , GetAlbum_function_type( &::CLAM::AudioTextDescriptors::GetAlbum )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::GetArtist
        
            typedef ::CLAM::Text & ( ::CLAM::AudioTextDescriptors::*GetArtist_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetArtist"
                , GetArtist_function_type( &::CLAM::AudioTextDescriptors::GetArtist )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::GetComposer
        
            typedef ::CLAM::Text & ( ::CLAM::AudioTextDescriptors::*GetComposer_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetComposer"
                , GetComposer_function_type( &::CLAM::AudioTextDescriptors::GetComposer )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::GetDynamicTypeCopy
        
            typedef ::CLAM::DynamicType & ( ::CLAM::AudioTextDescriptors::*GetDynamicTypeCopy_function_type )( bool const,bool const ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetDynamicTypeCopy"
                , GetDynamicTypeCopy_function_type(&::CLAM::AudioTextDescriptors::GetDynamicTypeCopy)
                , ( bp::arg("shareData")=(bool const)(false), bp::arg("deep")=(bool const)(false) )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::CLAM::AudioTextDescriptors::GetPerformer
        
            typedef ::CLAM::Text & ( ::CLAM::AudioTextDescriptors::*GetPerformer_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetPerformer"
                , GetPerformer_function_type( &::CLAM::AudioTextDescriptors::GetPerformer )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::GetTitle
        
            typedef ::CLAM::Text & ( ::CLAM::AudioTextDescriptors::*GetTitle_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetTitle"
                , GetTitle_function_type( &::CLAM::AudioTextDescriptors::GetTitle )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::GetTrackNumber
        
            typedef ::CLAM::Text & ( ::CLAM::AudioTextDescriptors::*GetTrackNumber_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetTrackNumber"
                , GetTrackNumber_function_type( &::CLAM::AudioTextDescriptors::GetTrackNumber )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::GetTypeId
        
            typedef ::std::type_info const & ( ::CLAM::AudioTextDescriptors::*GetTypeId_function_type )( unsigned int ) const;
            
            AudioTextDescriptors_exposer.def( 
                "GetTypeId"
                , GetTypeId_function_type(&::CLAM::AudioTextDescriptors::GetTypeId)
                , ( bp::arg("n") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::AudioTextDescriptors::HasAlbum
        
            typedef bool ( ::CLAM::AudioTextDescriptors::*HasAlbum_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "HasAlbum"
                , HasAlbum_function_type( &::CLAM::AudioTextDescriptors::HasAlbum )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::HasArtist
        
            typedef bool ( ::CLAM::AudioTextDescriptors::*HasArtist_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "HasArtist"
                , HasArtist_function_type( &::CLAM::AudioTextDescriptors::HasArtist )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::HasComposer
        
            typedef bool ( ::CLAM::AudioTextDescriptors::*HasComposer_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "HasComposer"
                , HasComposer_function_type( &::CLAM::AudioTextDescriptors::HasComposer )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::HasPerformer
        
            typedef bool ( ::CLAM::AudioTextDescriptors::*HasPerformer_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "HasPerformer"
                , HasPerformer_function_type( &::CLAM::AudioTextDescriptors::HasPerformer )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::HasTitle
        
            typedef bool ( ::CLAM::AudioTextDescriptors::*HasTitle_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "HasTitle"
                , HasTitle_function_type( &::CLAM::AudioTextDescriptors::HasTitle )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::HasTrackNumber
        
            typedef bool ( ::CLAM::AudioTextDescriptors::*HasTrackNumber_function_type )(  ) const;
            
            AudioTextDescriptors_exposer.def( 
                "HasTrackNumber"
                , HasTrackNumber_function_type( &::CLAM::AudioTextDescriptors::HasTrackNumber )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::SetAlbum
        
            typedef void ( ::CLAM::AudioTextDescriptors::*SetAlbum_function_type )( ::CLAM::Text const & ) ;
            
            AudioTextDescriptors_exposer.def( 
                "SetAlbum"
                , SetAlbum_function_type( &::CLAM::AudioTextDescriptors::SetAlbum )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::SetArtist
        
            typedef void ( ::CLAM::AudioTextDescriptors::*SetArtist_function_type )( ::CLAM::Text const & ) ;
            
            AudioTextDescriptors_exposer.def( 
                "SetArtist"
                , SetArtist_function_type( &::CLAM::AudioTextDescriptors::SetArtist )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::SetComposer
        
            typedef void ( ::CLAM::AudioTextDescriptors::*SetComposer_function_type )( ::CLAM::Text const & ) ;
            
            AudioTextDescriptors_exposer.def( 
                "SetComposer"
                , SetComposer_function_type( &::CLAM::AudioTextDescriptors::SetComposer )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::SetPerformer
        
            typedef void ( ::CLAM::AudioTextDescriptors::*SetPerformer_function_type )( ::CLAM::Text const & ) ;
            
            AudioTextDescriptors_exposer.def( 
                "SetPerformer"
                , SetPerformer_function_type( &::CLAM::AudioTextDescriptors::SetPerformer )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::SetTitle
        
            typedef void ( ::CLAM::AudioTextDescriptors::*SetTitle_function_type )( ::CLAM::Text const & ) ;
            
            AudioTextDescriptors_exposer.def( 
                "SetTitle"
                , SetTitle_function_type( &::CLAM::AudioTextDescriptors::SetTitle )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioTextDescriptors::SetTrackNumber
        
            typedef void ( ::CLAM::AudioTextDescriptors::*SetTrackNumber_function_type )( ::CLAM::Text const & ) ;
            
            AudioTextDescriptors_exposer.def( 
                "SetTrackNumber"
                , SetTrackNumber_function_type( &::CLAM::AudioTextDescriptors::SetTrackNumber )
                , ( bp::arg("arg") )
                , "" );
        
        }

    }

}
