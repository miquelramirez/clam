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
#include "EAudioFileFormat.pypp.hpp"

namespace bp = boost::python;

void register_EAudioFileFormat_class(){

    { //::CLAM::EAudioFileFormat
        typedef bp::class_< CLAM::EAudioFileFormat, bp::bases< CLAM::Enum > > EAudioFileFormat_exposer_t;
        EAudioFileFormat_exposer_t EAudioFileFormat_exposer = EAudioFileFormat_exposer_t( "EAudioFileFormat", "" );
        bp::scope EAudioFileFormat_scope( EAudioFileFormat_exposer );
        bp::scope().attr("eWAV") = (int)CLAM::EAudioFileFormat::eWAV;
        bp::scope().attr("eAIFF") = (int)CLAM::EAudioFileFormat::eAIFF;
        bp::scope().attr("eAU") = (int)CLAM::EAudioFileFormat::eAU;
        bp::scope().attr("eRAW") = (int)CLAM::EAudioFileFormat::eRAW;
        bp::scope().attr("ePAF") = (int)CLAM::EAudioFileFormat::ePAF;
        bp::scope().attr("eSVX") = (int)CLAM::EAudioFileFormat::eSVX;
        bp::scope().attr("eNIST") = (int)CLAM::EAudioFileFormat::eNIST;
        bp::scope().attr("eVOC") = (int)CLAM::EAudioFileFormat::eVOC;
        bp::scope().attr("eIRCAM") = (int)CLAM::EAudioFileFormat::eIRCAM;
        bp::scope().attr("eW64") = (int)CLAM::EAudioFileFormat::eW64;
        bp::scope().attr("eMAT4") = (int)CLAM::EAudioFileFormat::eMAT4;
        bp::scope().attr("eMAT5") = (int)CLAM::EAudioFileFormat::eMAT5;
        bp::scope().attr("eVorbisMk1") = (int)CLAM::EAudioFileFormat::eVorbisMk1;
        bp::scope().attr("eMpegLayer1") = (int)CLAM::EAudioFileFormat::eMpegLayer1;
        bp::scope().attr("eMpegLayer2") = (int)CLAM::EAudioFileFormat::eMpegLayer2;
        bp::scope().attr("eMpegLayer3") = (int)CLAM::EAudioFileFormat::eMpegLayer3;
        EAudioFileFormat_exposer.def( bp::init< >("") );
        EAudioFileFormat_exposer.def( bp::init< int >(( bp::arg("val") ), "") );
        EAudioFileFormat_exposer.def( bp::init< std::string >(( bp::arg("s") ), "") );
        { //::CLAM::EAudioFileFormat::DefaultValue
        
            typedef int ( *DefaultValue_function_type )(  );
            
            EAudioFileFormat_exposer.def( 
                "DefaultValue"
                , DefaultValue_function_type( &::CLAM::EAudioFileFormat::DefaultValue )
                , "" );
        
        }
        { //::CLAM::EAudioFileFormat::EnumValues
        
            typedef ::CLAM::Enum::tEnumValue * ( *EnumValues_function_type )(  );
            
            EAudioFileFormat_exposer.def( 
                "EnumValues"
                , EnumValues_function_type( &::CLAM::EAudioFileFormat::EnumValues )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::EAudioFileFormat::FormatFromFilename
        
            typedef ::CLAM::EAudioFileFormat ( *FormatFromFilename_function_type )( ::std::string );
            
            EAudioFileFormat_exposer.def( 
                "FormatFromFilename"
                , FormatFromFilename_function_type( &::CLAM::EAudioFileFormat::FormatFromFilename )
                , ( bp::arg("filename") )
                , "" );
        
        }

        EAudioFileFormat_exposer.staticmethod( "DefaultValue" );
        EAudioFileFormat_exposer.staticmethod( "EnumValues" );
        EAudioFileFormat_exposer.staticmethod( "FormatFromFilename" );
    }

}
