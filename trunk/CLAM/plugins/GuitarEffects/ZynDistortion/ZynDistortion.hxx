/*
  ZynAddSubFX - a software synthesizer

  Distorsion.C - Distorsion effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Ported to CLAM by Hernan Ordiales

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef _ZynDistortion_
#define _ZynDistortion_

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>

#include <CLAM/Audio.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControlArray.hxx>
#include <CLAM/Enum.hxx>

#include <CLAM/CLAM_Math.hxx>

namespace CLAM{

	static inline TData Sign( TData value )
	{
		return ( value > 0. ) ? 1. : -1.;
	}

	class EDistType : public Enum
	{
	public:
		EDistType() : Enum(ValueTable(), eArctangent) {}
		EDistType(tValue v) : Enum(ValueTable(), v) {};
		EDistType(std::string s) : Enum(ValueTable(), s) {};
		virtual Component* Species() const { return new EDistType(eArctangent); }
	
		typedef enum {
			eArctangent,
			eAsymmetric,
			ePow,
			eSine,
			eQuantisize,
			eZigzag,
			eLimiter,
			eUpperLimiter,
			eLowerLimiter,
			eInverseLimiter,
			eClip,
			eAsym2,
			ePow2,
			eSigmoid
		};
	
		static tEnumValue * ValueTable()
		{
			static tEnumValue sValueTable[] =
			{
				{eArctangent,"Arctangent"},
				{eAsymmetric,"Asymmetric"},
				{ePow,"Pow"},
				{eSine,"Sine"},
				{eQuantisize,"Quantisize"},
				{eZigzag,"Zigzag"},
				{eLimiter,"Limiter"},
				{eUpperLimiter,"Upper Limiter"},
				{eLowerLimiter,"Lower Limiter"},
				{eInverseLimiter,"Inverse Limiter"},
				{eClip,"Clip"},
				{eAsym2,"Asym2"},
				{ePow2,"Pow2"},
				{eSigmoid,"Sigmoid"},
				{0,NULL}
			};
			return sValueTable;
		}
	};

	/**
	 *	ZynDistortion configuration object.
	 */
	class ZynDistortionConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( ZynDistortionConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, EDistType, DistortionType );

	protected:
		void DefaultInit();
	};

	/**	\brief ZynAddSubFX Distortions
	 *
	 *	Many distorsion effects
	 */
	class ZynDistortion: public Processing
	{	
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "ZynDistortion"; }
		
		/** Ports **/
		AudioInPort mInputAudio;
		AudioOutPort mOutputAudio;

		/** Controls **/
		InControl mDrive; ///< Drive amount


		EDistType mDistType; ///< kind of distortion selector

	public:
		ZynDistortion()
			:
			mInputAudio("Input Audio",this ),
			mOutputAudio("Audio Output",this),

			mDrive("Drive amount", this)
		{
			Configure( mConfig );
		}

 		~ZynDistortion() {}

		bool Do()
		{
			bool result = false;
			result = DoDistortion( mInputAudio.GetAudio(), mOutputAudio.GetAudio() );
			mInputAudio.Consume(); 
			mOutputAudio.Produce();
			return result;
		}
	
		bool DoDistortion(const Audio& in, Audio& out)
		{
			int size = in.GetSize();
			DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			TData ws = mDrive.GetLastValue();
			TData tmpv;

			switch( mDistType )
			{
			case EDistType::eArctangent:
				ws = CLAM_pow(10,ws*ws*3.0) - 1.0 + 0.001; //never 0, avoids atan(0)=0 and further zero division
				for (int i=0;i<size;i++) 
				{
					outb[i] = CLAM_atan(inb[i]*ws)/CLAM_atan(ws);
				}
				break;
			case EDistType::eAsymmetric:
				ws = ws*ws*32.0 + 0.0001;
				if (ws<1.0) tmpv = CLAM_sin(ws) + 0.1; else tmpv = 1.1;
				for (int i=0;i<size;i++)
				{
					outb[i] = CLAM_sin( inb[i]*(0.1+ws-ws*inb[i]) ) / tmpv;
				}
				break;
			case EDistType::ePow:
				ws = ws*ws*ws*20.0 + 0.0001;
				for (int i=0;i<size;i++)
				{
					outb[i] = inb[i]*ws;
					if (fabs(outb[i])<1.0)
					{
						outb[i] = ( outb[i] - CLAM_pow(outb[i],3.0) ) * 3.0;
						if (ws<1.0) outb[i]/=ws; 
					}
					else outb[i]=0.0;
				};
				break;
			case EDistType::eSine:
				ws = ws*ws*ws*32.0 + 0.0001;
				if (ws<1.57) tmpv = CLAM_sin(ws); else tmpv = 1.0;
				for (int i=0;i<size;i++)
				{
					outb[i] = CLAM_sin(inb[i]*ws)/tmpv;
				}
				break;
			case EDistType::eQuantisize:
				ws = ws*ws + 0.000001;
				for (int i=0;i<size;i++)
				{
					outb[i] = floor(inb[i]/ws+0.5)*ws;
				}
				break;
			case EDistType::eZigzag:
				ws = ws*ws*ws*32 + 0.0001;
				if (ws<1.0) tmpv = CLAM_sin(ws); else tmpv = 1.0;
				for (int i=0;i<size;i++)
				{
					outb[i] = asin( CLAM_sin(inb[i]*ws) ) / tmpv;
				}
				break;
			case EDistType::eLimiter:
				ws = CLAM_pow(2.0,-ws*ws*8.0);
				for (int i=0;i<size;i++)
				{
					if (fabs(inb[i])>ws)
					{
						if (inb[i]>=0.0) outb[i]=1.0; else outb[i]=-1.0;
					}
					else outb[i] = inb[i]/ws;
				}
				break;
			case EDistType::eUpperLimiter:
				ws = CLAM_pow(2.0,-ws*ws*8.0);
				for (int i=0;i<size;i++)
				{
					if (inb[i]>ws) outb[i]=ws;
					outb[i] *= 2.0;
				}
				break;
			case EDistType::eLowerLimiter:
				ws = CLAM_pow(2.0,-ws*ws*8.0);
				for (int i=0;i<size;i++)
				{
					if (inb[i]<-ws) outb[i]=-ws;
					outb[i] *= 2.0;
				}
				break;
			case EDistType::eInverseLimiter:
				ws = (CLAM_pow(2.0,ws*6.0)-1.0)/CLAM_pow(2.0,6.0);
				for (int i=0;i<size;i++)
				{
					if (fabs(inb[i])>ws)
					{
						if (inb[i]>=0.0) outb[i]=inb[i]-ws; else outb[i]=inb[i]+ws;
					}
					else outb[i]=0;
				}
				break;
			case EDistType::eClip:
				ws = CLAM_pow(5,ws*ws*1.0) - 1.0;
				for (int i=0;i<size;i++)
				{
					outb[i] = inb[i]*(ws+0.5)*0.9999 - floor(0.5+inb[i]*(ws+0.5)*0.9999);
				}
				break;
			case EDistType::eAsym2:
				ws = ws*ws*ws*30 + 0.001;
				if (ws<0.3) tmpv=ws; else tmpv=1.0;
				for (int i=0;i<size;i++)
				{
					TData tmp = inb[i]*ws;
					if ((tmp>-2.0) && (tmp<1.0)) outb[i] = tmp*(1.0-tmp)*(tmp+2.0)/tmpv; else outb[i]=0.0;
				}
				break;
			case EDistType::ePow2:
				ws = ws*ws*ws*32.0 + 0.0001;
				if (ws<1.0) tmpv=ws*(1+ws)/2.0; else tmpv=1.0;
				for (int i=0;i<size;i++)
				{
					TData tmp = inb[i]*ws;
					if ((tmp>-1.0)&&(tmp<1.618034))
					{
						outb[i] = tmp*(1.0-tmp)/tmpv;
					}
					else
					{
						if (tmp>0.0) outb[i]=-1.0; else outb[i]=-2.0;
					}
				}
				break;
			case EDistType::eSigmoid:
				ws = CLAM_pow(ws,5.0)*80.0 + 0.0001;
				if (ws>10.0) tmpv=0.5; else tmpv=0.5-1.0/(CLAM_exp(ws)+1.0);
				for (int i=0;i<size;i++)
				{
					TData tmp = inb[i]*ws;
					if (tmp<-10.0) tmp=-10.0; else if (tmp>10.0) tmp=10.0;
					tmp = 0.5-1.0/(CLAM_exp(tmp)+1.0);
					outb[i]=tmp/tmpv;
				}
				break;
			default:
				CLAM_ASSERT( false, "Bad mDistType" );
			}
			return true;
		}


		typedef ZynDistortionConfig Config;


	protected:

		const ProcessingConfig& GetConfig() const {	return mConfig; }

		bool ConcreteConfigure(const ProcessingConfig& config) {

			CopyAsConcreteConfig( mConfig, config );

			mDistType = mConfig.GetDistortionType();

			mDrive.SetBounds(0.,1.);
			mDrive.SetDefaultValue(0.);
			mDrive.DoControl(0.);

			return true;
		}

	private:

		/** Configuration **/
		Config mConfig;
	};	
	
};//namespace CLAM

#endif // _ZynDistortion_
