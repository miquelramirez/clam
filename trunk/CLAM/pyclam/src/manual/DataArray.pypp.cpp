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
#include "CLAM/Array.hxx"
#include "src/Definitions.hxx"
#include "DataArray.pypp.hpp"

namespace bp = boost::python;

void register_DataArray_class(){

    { //::CLAM::Array< float >
        typedef bp::class_< CLAM::Array<float> > DataArray_exposer_t;
		DataArray_exposer_t DataArray_exposer = DataArray_exposer_t( "DataArray" );
        bp::scope DataArray_scope( DataArray_exposer );
        { //::CLAM::Array< float >::AddElem
        
            typedef CLAM::Array< float > exported_class_t;
            typedef void ( exported_class_t::*AddElem_function_type )( float const & ) ;
            
            DataArray_exposer.def( 
                "AddElem"
                , AddElem_function_type( &::CLAM::Array< float >::AddElem )
                , ( bp::arg("elem") ) );
        
        }
        { //::CLAM::Array< float >::Empty
        
            typedef CLAM::Array< float > exported_class_t;
            typedef bool ( exported_class_t::*Empty_function_type )(  ) const;
            
            DataArray_exposer.def( 
                "Empty"
                , Empty_function_type( &::CLAM::Array< float >::Empty ) );
        
        }
        { //::CLAM::Array< float >::InsertElem
        
            typedef CLAM::Array< float > exported_class_t;
            typedef void ( exported_class_t::*InsertElem_function_type )( int,float const & ) ;
            
            DataArray_exposer.def( 
                "InsertElem"
                , InsertElem_function_type( &::CLAM::Array< float >::InsertElem )
                , ( bp::arg("where"), bp::arg("elem") ) );
        
        }
        { //::CLAM::Array< float >::Resize
        
            typedef CLAM::Array< float > exported_class_t;
            typedef void ( exported_class_t::*Resize_function_type )( ::CLAM::TSize ) ;
            
            DataArray_exposer.def( 
                "Resize"
                , Resize_function_type( &::CLAM::Array< float >::Resize )
                , ( bp::arg("newAllocSize") ) );
        
        }
        { //::CLAM::Array< float >::SetPtr
        
            typedef CLAM::Array< float > exported_class_t;
            typedef void ( exported_class_t::*SetPtr_function_type )( float *,int ) ;
            
            DataArray_exposer.def( 
                "SetPtr"
                , SetPtr_function_type( &::CLAM::Array< float >::SetPtr )
                , ( bp::arg("ptr"), bp::arg("size")=(int)(0) ) );
        
        }
        { //::CLAM::Array< float >::SetSize
        
            typedef CLAM::Array< float > exported_class_t;
            typedef void ( exported_class_t::*SetSize_function_type )( ::CLAM::TSize ) ;
            
            DataArray_exposer.def( 
                "SetSize"
                , SetSize_function_type( &::CLAM::Array< float >::SetSize )
                , ( bp::arg("size") ) );
        
        }
        { //::CLAM::Array< float >::SetStep
        
            typedef CLAM::Array< float > exported_class_t;
            typedef void ( exported_class_t::*SetStep_function_type )( ::CLAM::TSize ) ;
            
            DataArray_exposer.def( 
                "SetStep"
                , SetStep_function_type( &::CLAM::Array< float >::SetStep )
                , ( bp::arg("step") ) );
        
        }
        { //::CLAM::Array< float >::Size
        
            typedef CLAM::Array< float > exported_class_t;
            typedef ::CLAM::TSize ( exported_class_t::*Size_function_type )(  ) const;
            
            DataArray_exposer.def( 
                "Size"
                , Size_function_type( &::CLAM::Array< float >::Size ) );
        
        }
        { //::CLAM::Array< float >::SizeInBytes
        
            typedef CLAM::Array< float > exported_class_t;
            typedef ::CLAM::TSize ( exported_class_t::*SizeInBytes_function_type )(  ) const;
            
            DataArray_exposer.def( 
                "SizeInBytes"
                , SizeInBytes_function_type( &::CLAM::Array< float >::SizeInBytes ) );
        
        }
        DataArray_exposer.def( bp::self += bp::self );
        { //::CLAM::Array< float >::operator=
        
            typedef CLAM::Array< float > exported_class_t;
            typedef ::CLAM::Array< float > & ( exported_class_t::*assign_function_type )( ::CLAM::Array< float > const & ) ;
            
            DataArray_exposer.def( 
                "assign"
                , assign_function_type( &::CLAM::Array< float >::operator= )
                , ( bp::arg("src") )
                , bp::return_self< >() );
        
        }
        { //::CLAM::Array< float >::operator[]
        
            typedef CLAM::Array< float > exported_class_t;
            typedef float const & ( exported_class_t::*__getitem___function_type )( int const & ) const;
            
            DataArray_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::CLAM::Array< float >::operator[] )
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::Array< float >::operator[]
        
            typedef CLAM::Array< float > exported_class_t;
            typedef float & ( exported_class_t::*__getitem___function_type )( int const & ) ;
            
            DataArray_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::CLAM::Array< float >::operator[] )
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_non_const_reference >() );
        
        }
        { //::CLAM::Array< float >::operator[]
            typedef CLAM::Array< float > exported_class_t;
            typedef float & ( exported_class_t::*__getitem___function_type )( int const & ) ;
			typedef void ( exported_class_t::*__setitem___function_type )( int,float const & ) ;

            DataArray_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::CLAM::Array< float >::operator[] )
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_non_const_reference >() );

			DataArray_exposer.def( 
                "__setitem__"
				, __setitem___function_type(&::CLAM::Array< float >::SetElem )
                , ( bp::arg("where"), bp::arg("elem") ) );
        }
    }

}
