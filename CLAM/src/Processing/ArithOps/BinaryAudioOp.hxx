/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
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

#ifndef _BINARY_AUDIO_OP_H_
#define _BINARY_AUDIO_OP_H_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Audio.hxx"
#include <typeinfo> // std::bad_cast
#include "ErrProcessingObj.hxx"

namespace CLAM 
{
		
		class BinaryAudioOpConfig: public ProcessingConfig
		{
		public:
				DYNAMIC_TYPE_USING_INTERFACE (BinaryAudioOpConfig, 1, ProcessingConfig);
				DYN_ATTRIBUTE (0, public, std::string, Name);
		protected:
				void DefaultInit()
				{
						/* the dynamic type takes care if we add an existing attr .. */
						
						AddName();
						
						
						/* All Attributes are added */
						UpdateData();
						
				}
				
		};
		
		template < typename BinOp >
		class BinaryAudioOp
				: public Processing 
		{
				BinaryAudioOpConfig mConfig;
				BinOp		     mOperation;
				
				const char *GetClassName() {return "BinaryAudioOperation";}
				
				/** Config change method
				 * @throw
				 * bad_cast exception when the argument is not an SpecAdderConfig
				 * object.
				 */
				bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
				{
						mConfig = dynamic_cast<const BinaryAudioOpConfig&>(c);	    
						return true;
						
				}
				
		public:
				BinaryAudioOp()
				{
						Configure( BinaryAudioOpConfig() );
				}
				
				BinaryAudioOp(const BinaryAudioOpConfig &c)
				{
								Configure( c );
				}
				
				~BinaryAudioOp()
				{
				}
				
				const ProcessingConfig &GetConfig() const { return mConfig;}
				
				void Check(Audio& in1,Audio& in2, Audio& out)
				{
						
						if (in1.GetSize() != in2.GetSize() || in1.GetSize() != out.GetSize())
								throw(ErrProcessingObj("BinaryAudioOperation::Do(): invalid Audio Data Size"),this);
						
				}
				
				bool Do(void)
				{
						throw ( ErrProcessingObj( "BinaryAudioOperation::Do() : Supervised mode not implemented" ), this );
						return false;
				}
				
				bool Do(Audio& in1, Audio& in2, Audio& out)
				{
						int size = in1.GetSize();
						int i;
						
						Check(in1,in2,out);
										
						TData* inb1 = in1.GetBuffer().GetPtr();
						TData* inb2 = in2.GetBuffer().GetPtr();
						TData* outb = out.GetBuffer().GetPtr();
						
						for (i=0;i<size;i++) 
						{
								*outb++ = mOperation( *inb1++ , *inb2++ );
						}
						
										return true;
				}
				
				// Port interfaces.
				
				bool SetPrototypes(Audio& in1,Audio& in2, const Audio& out)
				{
						return false;
				}
				
				bool SetPrototypes()
				{
						return false;
				}
				
				bool UnsetPrototypes()
				{
						return true;
				}
				
				bool MayDisableExecution() const {return true;}
				
				void StoreOn(Storage &s) {};
				
		private:
		};
		
}

#endif //	_BINARY_AUDIO_OP_H_
