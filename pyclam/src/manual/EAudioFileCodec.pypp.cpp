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
#include "CLAM/Filename.hxx"
#include "CLAM/AudioFileFormats.hxx"
#include "src/Definitions.hxx"
#include "EAudioFileCodec.pypp.hpp"

namespace bp = boost::python;

void register_EAudioFileCodec_class(){

    { //::CLAM::EAudioFileCodec
        typedef bp::class_< CLAM::EAudioFileCodec, bp::bases< CLAM::Enum > > EAudioFileCodec_exposer_t;
        EAudioFileCodec_exposer_t EAudioFileCodec_exposer = EAudioFileCodec_exposer_t( "EAudioFileCodec", "" );
        bp::scope EAudioFileCodec_scope( EAudioFileCodec_exposer );
        bp::scope().attr("ePCM") = (int)CLAM::EAudioFileCodec::ePCM;
        bp::scope().attr("eOggVorbis") = (int)CLAM::EAudioFileCodec::eOggVorbis;
        bp::scope().attr("eMpeg") = (int)CLAM::EAudioFileCodec::eMpeg;
        bp::scope().attr("eUnknown") = (int)CLAM::EAudioFileCodec::eUnknown;
        EAudioFileCodec_exposer.def( bp::init< >("") );
        EAudioFileCodec_exposer.def( bp::init< int >(( bp::arg("val") ), "") );
        EAudioFileCodec_exposer.def( bp::init< std::string >(( bp::arg("s") ), "") );
        { //::CLAM::EAudioFileCodec::DefaultValue
        
            typedef int ( *DefaultValue_function_type )(  );
            
            EAudioFileCodec_exposer.def( 
                "DefaultValue"
                , DefaultValue_function_type( &::CLAM::EAudioFileCodec::DefaultValue )
                , "" );
        
        }
        { //::CLAM::EAudioFileCodec::EnumValues
        
            typedef ::CLAM::Enum::tEnumValue * ( *EnumValues_function_type )(  );
            
            EAudioFileCodec_exposer.def( 
                "EnumValues"
                , EnumValues_function_type( &::CLAM::EAudioFileCodec::EnumValues )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }

        EAudioFileCodec_exposer.staticmethod( "DefaultValue" );
        EAudioFileCodec_exposer.staticmethod( "EnumValues" );
    }

}
