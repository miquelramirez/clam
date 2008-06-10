/*
 * Author: Günter Geiger
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

#include "RD_TimeDifference.hxx"
#include <CLAM/CLAM_Math.hxx>

namespace CLAM 
{

	namespace RhythmDescription
	{

		/* The  Configuration object has at least to have a name */
		void TimeDifferenceConfig::DefaultInit()
		{
			/* the dynamic type takes care if we add an existing attr .. */
			AddGaussianSize();

			/* All Attributes are added */
			UpdateData();
			SetGaussianSize(8192);
		}


		/* Processing  object Method  implementations */
		TimeDifference::TimeDifference()
		{
			Configure(TimeDifferenceConfig());
		}

		TimeDifference::TimeDifference(const TimeDifferenceConfig &c)
		{
			Configure(c);
		}

		TimeDifference::~TimeDifference()
		{
		}

		const char* TimeDifference::GetClassName() const
		{
			return "TimeDifference";
		}

		/* Configure the Processing Object according to the Config object */

		bool TimeDifference::ConcreteConfigure(const ProcessingConfig& c)
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

			//NB: full-width of gaussian at half max= s*2*sqrt(2)
			//	where s is the standard dev
			//	in WindowGenerator::Gaussian, s=0.15


			return true;
		}

		/* The supervised Do() function */
		bool  TimeDifference::Do(void) 
		{
			return false;
		}

		/* The  unsupervised Do() function */
		bool  TimeDifference::Do(const Array<TimeIndex>& in, IOIHistogram& out)
		{
			int gsize = mConfig.GetGaussianSize();
	
			TData* outp = out.GetBins().GetPtr();
			const TData* end = outp + out.GetBins().Size();
			const TData* win = mWindow.GetPtr();
			const int  size = in.Size();
			const TSize outSize = out.GetBins().Size();
    
			for (int i=0;i<outSize;i++)
				outp[i] = 0.;
	
			for (int j=0;j < size-1;j++) 
			{
				int apos = (int) in[j].GetPosition();
				int pos = (int)in[j+1].GetPosition() - (int)apos ; 

				for ( int k = j+1; 
				      (k < size) && pos+gsize < outSize;
				      k++ ) 
				{		
					pos = (int)in[k].GetPosition() - (int)apos;
					
					TData* outw = outp + pos - gsize/2;
					TData weight = std::min(in[k].GetWeight(),in[j].GetWeight());
					
					// add the gaussian 
					for (int i=0;
					     (i<gsize) && (outw >= outp && outw < end);
					     i++) 
					{
						*(outw++) += (win[i]*weight);
					}
				}
			}
			return true;
		}

	} // namespace RhythmDescription
  
} // namespace CLAM

