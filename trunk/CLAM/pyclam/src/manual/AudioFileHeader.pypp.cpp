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
#include "CLAM/AudioFileHeader.hxx"
#include "src/Definitions.hxx"
#include "AudioFileHeader.pypp.hpp"

namespace bp = boost::python;

void register_AudioFileHeader_class(){

    { //::CLAM::AudioFileHeader
        typedef bp::class_< CLAM::AudioFileHeader > AudioFileHeader_exposer_t;
        AudioFileHeader_exposer_t AudioFileHeader_exposer = AudioFileHeader_exposer_t( "AudioFileHeader" );
        bp::scope AudioFileHeader_scope( AudioFileHeader_exposer );
        bp::scope().attr("eNumAttr") = (int)CLAM::AudioFileHeader::eNumAttr;
        AudioFileHeader_exposer.def( bp::init< >("") );
        AudioFileHeader_exposer.def( bp::init< CLAM::AudioFileHeader const &, bp::optional< bool, bool > >(( bp::arg("prototype"), bp::arg("shareData")=(bool const)(false), bp::arg("deep")=(bool const)(true) ), "") );
        { //::CLAM::AudioFileHeader::AddAll
        
            typedef void ( ::CLAM::AudioFileHeader::*AddAll_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddAll"
                , AddAll_function_type( &::CLAM::AudioFileHeader::AddAll )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::AddChannels
        
            typedef void ( ::CLAM::AudioFileHeader::*AddChannels_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddChannels"
                , AddChannels_function_type( &::CLAM::AudioFileHeader::AddChannels )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::AddEncoding
        
            typedef void ( ::CLAM::AudioFileHeader::*AddEncoding_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddEncoding"
                , AddEncoding_function_type( &::CLAM::AudioFileHeader::AddEncoding )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::AddEndianess
        
            typedef void ( ::CLAM::AudioFileHeader::*AddEndianess_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddEndianess"
                , AddEndianess_function_type( &::CLAM::AudioFileHeader::AddEndianess )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::AddFormat
        
            typedef void ( ::CLAM::AudioFileHeader::*AddFormat_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddFormat"
                , AddFormat_function_type( &::CLAM::AudioFileHeader::AddFormat )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::AddLength
        
            typedef void ( ::CLAM::AudioFileHeader::*AddLength_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddLength"
                , AddLength_function_type( &::CLAM::AudioFileHeader::AddLength )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::AddSampleRate
        
            typedef void ( ::CLAM::AudioFileHeader::*AddSampleRate_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddSampleRate"
                , AddSampleRate_function_type( &::CLAM::AudioFileHeader::AddSampleRate )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::AddSamples
        
            typedef void ( ::CLAM::AudioFileHeader::*AddSamples_function_type )(  ) ;
            
            AudioFileHeader_exposer.def( 
                "AddSamples"
                , AddSamples_function_type( &::CLAM::AudioFileHeader::AddSamples )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::GetChannels
        
            typedef ::CLAM::TSize const & ( ::CLAM::AudioFileHeader::*GetChannels_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "GetChannels"
                , GetChannels_function_type( &::CLAM::AudioFileHeader::GetChannels )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::AudioFileHeader::GetEncoding
        
            typedef ::CLAM::EAudioFileEncoding const & ( ::CLAM::AudioFileHeader::*GetEncoding_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "GetEncoding"
                , GetEncoding_function_type( &::CLAM::AudioFileHeader::GetEncoding )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::GetEndianess
        
            typedef ::CLAM::EAudioFileEndianess const & ( ::CLAM::AudioFileHeader::*GetEndianess_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "GetEndianess"
                , GetEndianess_function_type( &::CLAM::AudioFileHeader::GetEndianess )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::GetFormat
        
            typedef ::CLAM::EAudioFileFormat & ( ::CLAM::AudioFileHeader::*GetFormat_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "GetFormat"
                , GetFormat_function_type( &::CLAM::AudioFileHeader::GetFormat )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::GetLength
        
            typedef ::CLAM::TTime const & ( ::CLAM::AudioFileHeader::*GetLength_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "GetLength"
                , GetLength_function_type( &::CLAM::AudioFileHeader::GetLength )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::GetSampleRate
        
            typedef ::CLAM::TData const & ( ::CLAM::AudioFileHeader::*GetSampleRate_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "GetSampleRate"
                , GetSampleRate_function_type( &::CLAM::AudioFileHeader::GetSampleRate )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::AudioFileHeader::GetSamples
        
            typedef ::CLAM::TSize const & ( ::CLAM::AudioFileHeader::*GetSamples_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "GetSamples"
                , GetSamples_function_type( &::CLAM::AudioFileHeader::GetSamples )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::GetTypeId
        
            typedef ::std::type_info const & ( ::CLAM::AudioFileHeader::*GetTypeId_function_type )( unsigned int ) const;
            
            AudioFileHeader_exposer.def( 
                "GetTypeId"
                , GetTypeId_function_type(&::CLAM::AudioFileHeader::GetTypeId)
                , ( bp::arg("n") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::AudioFileHeader::HasChannels
        
            typedef bool ( ::CLAM::AudioFileHeader::*HasChannels_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "HasChannels"
                , HasChannels_function_type( &::CLAM::AudioFileHeader::HasChannels )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::HasEncoding
        
            typedef bool ( ::CLAM::AudioFileHeader::*HasEncoding_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "HasEncoding"
                , HasEncoding_function_type( &::CLAM::AudioFileHeader::HasEncoding )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::HasEndianess
        
            typedef bool ( ::CLAM::AudioFileHeader::*HasEndianess_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "HasEndianess"
                , HasEndianess_function_type( &::CLAM::AudioFileHeader::HasEndianess )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::HasFormat
        
            typedef bool ( ::CLAM::AudioFileHeader::*HasFormat_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "HasFormat"
                , HasFormat_function_type( &::CLAM::AudioFileHeader::HasFormat )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::HasLength
        
            typedef bool ( ::CLAM::AudioFileHeader::*HasLength_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "HasLength"
                , HasLength_function_type( &::CLAM::AudioFileHeader::HasLength )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::HasSampleRate
        
            typedef bool ( ::CLAM::AudioFileHeader::*HasSampleRate_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "HasSampleRate"
                , HasSampleRate_function_type( &::CLAM::AudioFileHeader::HasSampleRate )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::HasSamples
        
            typedef bool ( ::CLAM::AudioFileHeader::*HasSamples_function_type )(  ) const;
            
            AudioFileHeader_exposer.def( 
                "HasSamples"
                , HasSamples_function_type( &::CLAM::AudioFileHeader::HasSamples )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetChannels
        
            typedef void ( ::CLAM::AudioFileHeader::*SetChannels_function_type )( ::CLAM::TSize const & ) ;
            
            AudioFileHeader_exposer.def( 
                "SetChannels"
                , SetChannels_function_type( &::CLAM::AudioFileHeader::SetChannels )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetEncoding
        
            typedef void ( ::CLAM::AudioFileHeader::*SetEncoding_function_type )( ::CLAM::EAudioFileEncoding const & ) ;
            
            AudioFileHeader_exposer.def( 
                "SetEncoding"
                , SetEncoding_function_type( &::CLAM::AudioFileHeader::SetEncoding )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetEndianess
        
            typedef void ( ::CLAM::AudioFileHeader::*SetEndianess_function_type )( ::CLAM::EAudioFileEndianess const & ) ;
            
            AudioFileHeader_exposer.def( 
                "SetEndianess"
                , SetEndianess_function_type( &::CLAM::AudioFileHeader::SetEndianess )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetFormat
        
            typedef void ( ::CLAM::AudioFileHeader::*SetFormat_function_type )( ::CLAM::EAudioFileFormat const & ) ;
            
            AudioFileHeader_exposer.def( 
                "SetFormat"
                , SetFormat_function_type( &::CLAM::AudioFileHeader::SetFormat )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetLength
        
            typedef void ( ::CLAM::AudioFileHeader::*SetLength_function_type )( ::CLAM::TTime const & ) ;
            
            AudioFileHeader_exposer.def( 
                "SetLength"
                , SetLength_function_type( &::CLAM::AudioFileHeader::SetLength )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetSampleRate
        
            typedef void ( ::CLAM::AudioFileHeader::*SetSampleRate_function_type )( ::CLAM::TData const & ) ;
            
            AudioFileHeader_exposer.def( 
                "SetSampleRate"
                , SetSampleRate_function_type( &::CLAM::AudioFileHeader::SetSampleRate )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetSamples
        
            typedef void ( ::CLAM::AudioFileHeader::*SetSamples_function_type )( ::CLAM::TSize const & ) ;
            
            AudioFileHeader_exposer.def( 
                "SetSamples"
                , SetSamples_function_type( &::CLAM::AudioFileHeader::SetSamples )
                , ( bp::arg("arg") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetValues
        
            typedef void ( ::CLAM::AudioFileHeader::*SetValues_function_type )( ::CLAM::TData,::CLAM::TSize,::CLAM::EAudioFileFormat ) ;
            
            AudioFileHeader_exposer.def( 
                "SetValues"
                , SetValues_function_type( &::CLAM::AudioFileHeader::SetValues )
                , ( bp::arg("rate"), bp::arg("numberOfChannels"), bp::arg("fmt") )
                , "" );
        
        }
        { //::CLAM::AudioFileHeader::SetValues
        
            typedef void ( ::CLAM::AudioFileHeader::*SetValues_function_type )( ::CLAM::TData,::CLAM::TSize,char const * ) ;
            
            AudioFileHeader_exposer.def( 
                "SetValues"
                , SetValues_function_type( &::CLAM::AudioFileHeader::SetValues )
                , ( bp::arg("rate"), bp::arg("numberOfChannels"), bp::arg("fmtString") )
                , "" );
        
        }

    }

}
