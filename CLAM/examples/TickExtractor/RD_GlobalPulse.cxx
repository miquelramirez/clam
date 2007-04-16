/*
 * Authors: Günter Geiger
 *			fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "Complex.hxx"
#include "RD_GlobalPulse.hxx"

namespace CLAM 
{

	namespace RhythmDescription
	{

		void GlobalPulseGeneratorConfig::DefaultInit()
		{
			AddGaussianSize();
			UpdateData();
			SetGaussianSize(8192);
		}


		GlobalPulseGenerator::GlobalPulseGenerator()
		{

		}


		GlobalPulseGenerator::~GlobalPulseGenerator()
		{
		}

		const char* GlobalPulseGenerator::GetClassName() const
		{
			return "GlobalPulseGenerator";
		}

		bool GlobalPulseGenerator::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			WindowGeneratorConfig winconf;
			winconf.AddType();
			winconf.UpdateData();
			winconf.SetType(EWindowType::eGaussian);
			winconf.SetSize(mConfig.GetGaussianSize());

			mWindowGen.Configure(winconf);
			
			mWindow.Resize(mConfig.GetGaussianSize());
			mWindow.SetSize(mConfig.GetGaussianSize());

			mWindowGen.Start();


			mWindowGen.Do(mWindow);
			mWindowGen.Stop();	    

			return true;
		}

		/* Setting Prototypes for faster processing */

		bool GlobalPulseGenerator::SetPrototypes(Array<TData>& in,const Audio& out)
		{ return false;
		}

		/* The supervised Do() function */
		bool  GlobalPulseGenerator::Do(void) 
		{
			return false;
		}

		/* The  unsupervised Do() function */
		bool  GlobalPulseGenerator::Do(const Array<TData>& in, IOIHistogram& out)
		{
			int gsize = mConfig.GetGaussianSize();
	
			TData* outp = out.GetBins().GetPtr();
			TData* end = outp + out.GetBins().Size();
			TData* win = mWindow.GetPtr();
			
			int i;
			int j;
			int  size;
			size = in.Size();
    
			for (i=0;i<out.GetBins().Size();i++)
				outp[i] = 0.0;

			// convolution
			for (j=0;j < size;j++) 
			{
				int apos = in[j];
				TData* outw = outp + apos - gsize/2;
			
				for ( i=0; 
				      (i<gsize) ; i++) 
				{
					if (outw >= outp && outw < end)
						*outw += win[i];
					
					outw++;
				}
			}
			
			return true;
		}

	}
  
} // namespace CLAM

