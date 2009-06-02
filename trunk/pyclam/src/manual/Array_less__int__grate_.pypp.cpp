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
#include "Array_less__int__grate_.pypp.hpp"

namespace bp = boost::python;


void register_Array_less__int__grate__class(){

    { //::CLAM::Array< int >
		typedef bp::class_<  CLAM::Array< int >  > Array_less__int__grate__exposer_t;
        Array_less__int__grate__exposer_t Array_less__int__grate__exposer = Array_less__int__grate__exposer_t( "Array_less__int__grate_", bp::init< bp::optional< CLAM::TSize, CLAM::TSize > >(( bp::arg("size")=(int)(0), bp::arg("step")=(int)(1) )) );

        bp::scope Array_less__int__grate__scope( Array_less__int__grate__exposer );

        { //::CLAM::Array< int >::AddElem
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*AddElem_function_type )( int const & ) ;
            
            Array_less__int__grate__exposer.def( 
                "AddElem"
                , AddElem_function_type( &::CLAM::Array< int >::AddElem )
                , ( bp::arg("elem") ) );
        
        }
        { //::CLAM::Array< int >::AllocatedSize
        
            typedef CLAM::Array< int > exported_class_t;
            typedef ::CLAM::TSize ( exported_class_t::*AllocatedSize_function_type )(  ) const;
            
            Array_less__int__grate__exposer.def( 
                "AllocatedSize"
                , AllocatedSize_function_type( &::CLAM::Array< int >::AllocatedSize ) );
        
        }
        { //::CLAM::Array< int >::AllocatedSizeInBytes
        
            typedef CLAM::Array< int > exported_class_t;
            typedef ::CLAM::TSize ( exported_class_t::*AllocatedSizeInBytes_function_type )(  ) const;
            
            Array_less__int__grate__exposer.def( 
                "AllocatedSizeInBytes"
                , AllocatedSizeInBytes_function_type( &::CLAM::Array< int >::AllocatedSizeInBytes ) );
        
        }
        { //::CLAM::Array< int >::CopyChunk
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*CopyChunk_function_type )( int,int,::CLAM::Array< int > & ) const;
            
            Array_less__int__grate__exposer.def( 
                "CopyChunk"
                , CopyChunk_function_type( &::CLAM::Array< int >::CopyChunk )
                , ( bp::arg("pos"), bp::arg("size"), bp::arg("a") ) );
        
        }
        { //::CLAM::Array< int >::DeleteElem
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*DeleteElem_function_type )( int ) ;
            
            Array_less__int__grate__exposer.def( 
                "DeleteElem"
                , DeleteElem_function_type( &::CLAM::Array< int >::DeleteElem )
                , ( bp::arg("where") ) );
        
        }
        { //::CLAM::Array< int >::Empty
        
            typedef CLAM::Array< int > exported_class_t;
            typedef bool ( exported_class_t::*Empty_function_type )(  ) const;
            
            Array_less__int__grate__exposer.def( 
                "Empty"
                , Empty_function_type( &::CLAM::Array< int >::Empty ) );
        
        }
        { //::CLAM::Array< int >::GetStep
        
            typedef CLAM::Array< int > exported_class_t;
            typedef ::CLAM::TSize ( exported_class_t::*GetStep_function_type )(  ) const;
            
            Array_less__int__grate__exposer.def( 
                "GetStep"
                , GetStep_function_type( &::CLAM::Array< int >::GetStep ) );
        
        }
        { //::CLAM::Array< int >::GiveChunk
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*GiveChunk_function_type )( int,int,::CLAM::Array< int > & ) const;
            
            Array_less__int__grate__exposer.def( 
                "GiveChunk"
                , GiveChunk_function_type( &::CLAM::Array< int >::GiveChunk )
                , ( bp::arg("pos"), bp::arg("size"), bp::arg("a") ) );
        
        }
        { //::CLAM::Array< int >::Init
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*Init_function_type )(  ) ;
            
            Array_less__int__grate__exposer.def( 
                "Init"
                , Init_function_type( &::CLAM::Array< int >::Init ) );
        
        }
        { //::CLAM::Array< int >::InsertElem
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*InsertElem_function_type )( int,int const & ) ;
            
            Array_less__int__grate__exposer.def( 
                "InsertElem"
                , InsertElem_function_type( &::CLAM::Array< int >::InsertElem )
                , ( bp::arg("where"), bp::arg("elem") ) );
        
        }
        { //::CLAM::Array< int >::OwnsMemory
        
            typedef CLAM::Array< int > exported_class_t;
            typedef bool ( exported_class_t::*OwnsMemory_function_type )(  ) const;
            
            Array_less__int__grate__exposer.def( 
                "OwnsMemory"
                , OwnsMemory_function_type( &::CLAM::Array< int >::OwnsMemory ) );
        
        }
        { //::CLAM::Array< int >::Resize
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*Resize_function_type )( ::CLAM::TSize ) ;
            
            Array_less__int__grate__exposer.def( 
                "Resize"
                , Resize_function_type( &::CLAM::Array< int >::Resize )
                , ( bp::arg("newAllocSize") ) );
        
        }
        { //::CLAM::Array< int >::SetPtr
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*SetPtr_function_type )( int *,int ) ;
            
            Array_less__int__grate__exposer.def( 
                "SetPtr"
                , SetPtr_function_type( &::CLAM::Array< int >::SetPtr )
                , ( bp::arg("ptr"), bp::arg("size")=(int)(0) ) );
        
        }
        { //::CLAM::Array< int >::SetSize
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*SetSize_function_type )( ::CLAM::TSize ) ;
            
            Array_less__int__grate__exposer.def( 
                "SetSize"
                , SetSize_function_type( &::CLAM::Array< int >::SetSize )
                , ( bp::arg("size") ) );
        
        }
        { //::CLAM::Array< int >::SetStep
        
            typedef CLAM::Array< int > exported_class_t;
            typedef void ( exported_class_t::*SetStep_function_type )( ::CLAM::TSize ) ;
            
            Array_less__int__grate__exposer.def( 
                "SetStep"
                , SetStep_function_type( &::CLAM::Array< int >::SetStep )
                , ( bp::arg("step") ) );
        
        }
        { //::CLAM::Array< int >::Size
        
            typedef CLAM::Array< int > exported_class_t;
            typedef ::CLAM::TSize ( exported_class_t::*Size_function_type )(  ) const;
            
            Array_less__int__grate__exposer.def( 
                "Size"
                , Size_function_type( &::CLAM::Array< int >::Size ) );
        
        }
        { //::CLAM::Array< int >::SizeInBytes
        
            typedef CLAM::Array< int > exported_class_t;
            typedef ::CLAM::TSize ( exported_class_t::*SizeInBytes_function_type )(  ) const;
            
            Array_less__int__grate__exposer.def( 
                "SizeInBytes"
                , SizeInBytes_function_type( &::CLAM::Array< int >::SizeInBytes ) );
        
        }
        Array_less__int__grate__exposer.def( bp::self += bp::self );
        { //::CLAM::Array< int >::operator=
        
            typedef CLAM::Array< int > exported_class_t;
            typedef ::CLAM::Array< int > & ( exported_class_t::*assign_function_type )( ::CLAM::Array< int > const & ) ;
            
            Array_less__int__grate__exposer.def( 
                "assign"
                , assign_function_type( &::CLAM::Array< int >::operator= )
                , ( bp::arg("src") )
                , bp::return_self< >() );
        
        }
        { //::CLAM::Array< int >::operator[]
        
            typedef CLAM::Array< int > exported_class_t;
            typedef int const & ( exported_class_t::*__getitem___function_type )( int const & ) const;
            
            Array_less__int__grate__exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::CLAM::Array< int >::operator[] )
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::CLAM::Array< int >::operator[]
        
            typedef CLAM::Array< int > exported_class_t;
            typedef int & ( exported_class_t::*__getitem___function_type )( int const & ) ;
            
            Array_less__int__grate__exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::CLAM::Array< int >::operator[] )
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_non_const_reference >() );
        
        }
    }

}
