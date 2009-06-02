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
#include "EAudioFileEncoding.pypp.hpp"

namespace bp = boost::python;

void register_EAudioFileEncoding_class(){

    { //::CLAM::EAudioFileEncoding
        typedef bp::class_< CLAM::EAudioFileEncoding, bp::bases< CLAM::Enum > > EAudioFileEncoding_exposer_t;
        EAudioFileEncoding_exposer_t EAudioFileEncoding_exposer = EAudioFileEncoding_exposer_t( "EAudioFileEncoding", "" );
        bp::scope EAudioFileEncoding_scope( EAudioFileEncoding_exposer );
        bp::scope().attr("ePCM_S8") = (int)CLAM::EAudioFileEncoding::ePCM_S8;
        bp::scope().attr("ePCM_16") = (int)CLAM::EAudioFileEncoding::ePCM_16;
        bp::scope().attr("ePCM_24") = (int)CLAM::EAudioFileEncoding::ePCM_24;
        bp::scope().attr("ePCM_32") = (int)CLAM::EAudioFileEncoding::ePCM_32;
        bp::scope().attr("ePCM_U8") = (int)CLAM::EAudioFileEncoding::ePCM_U8;
        bp::scope().attr("eFLOAT") = (int)CLAM::EAudioFileEncoding::eFLOAT;
        bp::scope().attr("eDOUBLE") = (int)CLAM::EAudioFileEncoding::eDOUBLE;
        bp::scope().attr("eU_LAW") = (int)CLAM::EAudioFileEncoding::eU_LAW;
        bp::scope().attr("eA_LAW") = (int)CLAM::EAudioFileEncoding::eA_LAW;
        bp::scope().attr("eIMA_ADPCM") = (int)CLAM::EAudioFileEncoding::eIMA_ADPCM;
        bp::scope().attr("eMS_ADPCM") = (int)CLAM::EAudioFileEncoding::eMS_ADPCM;
        bp::scope().attr("eGSM610") = (int)CLAM::EAudioFileEncoding::eGSM610;
        bp::scope().attr("eVOX_ADPCM") = (int)CLAM::EAudioFileEncoding::eVOX_ADPCM;
        bp::scope().attr("eG721_32") = (int)CLAM::EAudioFileEncoding::eG721_32;
        bp::scope().attr("eG723_24") = (int)CLAM::EAudioFileEncoding::eG723_24;
        bp::scope().attr("eG723_40") = (int)CLAM::EAudioFileEncoding::eG723_40;
        bp::scope().attr("eDWVW_12") = (int)CLAM::EAudioFileEncoding::eDWVW_12;
        bp::scope().attr("eDWVW_16") = (int)CLAM::EAudioFileEncoding::eDWVW_16;
        bp::scope().attr("eDWVW_24") = (int)CLAM::EAudioFileEncoding::eDWVW_24;
        bp::scope().attr("eDWVW_N") = (int)CLAM::EAudioFileEncoding::eDWVW_N;
        bp::scope().attr("eDefault") = (int)CLAM::EAudioFileEncoding::eDefault;
        bp::scope().attr("e5015US") = (int)CLAM::EAudioFileEncoding::e5015US;
        bp::scope().attr("eCCITTJ17") = (int)CLAM::EAudioFileEncoding::eCCITTJ17;
        EAudioFileEncoding_exposer.def( bp::init< >("") );
        EAudioFileEncoding_exposer.def( bp::init< int >(( bp::arg("val") ), "") );
        EAudioFileEncoding_exposer.def( bp::init< std::string >(( bp::arg("s") ), "") );
        { //::CLAM::EAudioFileEncoding::DefaultValue
        
            typedef int ( *DefaultValue_function_type )(  );
            
            EAudioFileEncoding_exposer.def( 
                "DefaultValue"
                , DefaultValue_function_type( &::CLAM::EAudioFileEncoding::DefaultValue )
                , "" );
        
        }
        { //::CLAM::EAudioFileEncoding::EnumValues
        
            typedef ::CLAM::Enum::tEnumValue * ( *EnumValues_function_type )(  );
            
            EAudioFileEncoding_exposer.def( 
                "EnumValues"
                , EnumValues_function_type( &::CLAM::EAudioFileEncoding::EnumValues )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }

        EAudioFileEncoding_exposer.staticmethod( "DefaultValue" );
        EAudioFileEncoding_exposer.staticmethod( "EnumValues" );
    }

}
