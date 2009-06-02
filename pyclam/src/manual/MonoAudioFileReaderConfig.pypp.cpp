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
#include "CLAM/MonoAudioFileReaderConfig.hxx"
#include "src/Definitions.hxx"
#include "MonoAudioFileReaderConfig.pypp.hpp"

namespace bp = boost::python;

void register_MonoAudioFileReaderConfig_class(){

    { //::CLAM::MonoAudioFileReaderConfig
		typedef bp::class_< CLAM::MonoAudioFileReaderConfig > MonoAudioFileReaderConfig_exposer_t;
        MonoAudioFileReaderConfig_exposer_t MonoAudioFileReaderConfig_exposer = MonoAudioFileReaderConfig_exposer_t( "MonoAudioFileReaderConfig" );
        bp::scope MonoAudioFileReaderConfig_scope( MonoAudioFileReaderConfig_exposer );
        { //::CLAM::MonoAudioFileReaderConfig::GetLoop
        
            typedef bool const & ( ::CLAM::MonoAudioFileReaderConfig::*GetLoop_function_type )(  ) const;
            
            MonoAudioFileReaderConfig_exposer.def( 
                "GetLoop"
                , GetLoop_function_type( &::CLAM::MonoAudioFileReaderConfig::GetLoop )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::MonoAudioFileReaderConfig::GetSelectedChannel
        
            typedef ::CLAM::TIndex const & ( ::CLAM::MonoAudioFileReaderConfig::*GetSelectedChannel_function_type )(  ) const;
            
            MonoAudioFileReaderConfig_exposer.def( 
                "GetSelectedChannel"
                , GetSelectedChannel_function_type( &::CLAM::MonoAudioFileReaderConfig::GetSelectedChannel )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::MonoAudioFileReaderConfig::GetSourceFile
        
            typedef ::CLAM::AudioInFilename const & ( ::CLAM::MonoAudioFileReaderConfig::*GetSourceFile_function_type )(  ) const;
            
            MonoAudioFileReaderConfig_exposer.def( 
                "GetSourceFile"
                , GetSourceFile_function_type( &::CLAM::MonoAudioFileReaderConfig::GetSourceFile )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::MonoAudioFileReaderConfig::SetLoop
        
            typedef void ( ::CLAM::MonoAudioFileReaderConfig::*SetLoop_function_type )( bool const & ) ;
            
            MonoAudioFileReaderConfig_exposer.def( 
                "SetLoop"
                , SetLoop_function_type( &::CLAM::MonoAudioFileReaderConfig::SetLoop )
                , ( bp::arg("arg") ) );
        
        }
        { //::CLAM::MonoAudioFileReaderConfig::SetSelectedChannel
        
            typedef void ( ::CLAM::MonoAudioFileReaderConfig::*SetSelectedChannel_function_type )( ::CLAM::TIndex const & ) ;
            
            MonoAudioFileReaderConfig_exposer.def( 
                "SetSelectedChannel"
                , SetSelectedChannel_function_type( &::CLAM::MonoAudioFileReaderConfig::SetSelectedChannel )
                , ( bp::arg("arg") ) );
        
        }
        { //::CLAM::MonoAudioFileReaderConfig::SetSourceFile
        
            typedef void ( ::CLAM::MonoAudioFileReaderConfig::*SetSourceFile_function_type )( ::CLAM::AudioInFilename const & ) ;
            
            MonoAudioFileReaderConfig_exposer.def( 
                "SetSourceFile"
                , SetSourceFile_function_type( &::CLAM::MonoAudioFileReaderConfig::SetSourceFile )
                , ( bp::arg("arg") ) );
        
        }
	}
}
