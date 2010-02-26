/*
 * Copyright (C) 2009 Club Audio Fiuba
 *
 * Normal/Gaussian distributed real number generators were taken from "GSL - GNU Scientific Library" (http://www.gnu.org/software/gsl/)
 * randist/gauss.c
 * Copyright (C) 1996, 1997, 1998, 1999, 2000, 2006 James Theiler, Brian Gough
 * Copyright (C) 2006 Charles Karney
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

#ifndef _WhiteNoise_
#define _WhiteNoise_

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Enum.hxx>
#include <CLAM/CLAM_Math.hxx>

#include <CLAM/AudioOutPort.hxx>

#include <time.h>

namespace CLAM {

	class EDistributionType : public Enum
	{
	public:
		EDistributionType() : Enum(ValueTable(), eUniform) {}
		EDistributionType(tValue v) : Enum(ValueTable(), v) {};
		EDistributionType(std::string s) : Enum(ValueTable(), s) {};
		virtual Component* Species() const { return new EDistributionType(eUniform); }
	
		enum {
			eUniform,
			eGaussian,
		};
	
		static tEnumValue * ValueTable()
		{
			static tEnumValue sValueTable[] =
			{
				{eUniform,"Uniform"},
				{eGaussian,"Gaussian"},
				{0,NULL}
			};
			return sValueTable;
		}
	};

	/**
	 *	WhiteNoise configuration object.
	 */
	class WhiteNoiseConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( WhiteNoiseConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, EDistributionType, DistributionType );

	protected:
		void DefaultInit();
	};

	/**	\brief White noise
	*
	*	NoiseType: White
	*	NoiseDistribution: Uniform/Gaussian
	*	Results were statically analized with Octave.
	*
	*
	*	Normal/Gaussian random number generators were taken from "GSL - GNU Scientific Library" (http://www.gnu.org/software/gsl/)
	*	/randist/gauss.c
	*
	*	Some comments about the two different algorithms are copied here by convenience:
	*		"Of the two methods provided below, I think the Polar method is more
	*		efficient, but only when you are actually producing two random
	*		deviates.  We don't produce two, because then we'd have to save one
	*		in a static variable for the next call, and that would screws up
	*		re-entrant or threaded code, so we only produce one.  This makes
	*		the Ratio method suddenly more appealing.
	*
	*		[Added by Charles Karney] We use Leva's implementation of the Ratio
	*		method which avoids calling log() nearly all the time and makes the
	*		Ratio method faster than the Polar method (when it produces just one
	*		result per call).  Timing per call (gcc -O2 on 866MHz Pentium,
	*		average over 10^8 calls)
	*
	*		  Polar method: 660 ns
	*		  Ratio method: 368 ns
	*		"
	*
	*
	*/
	class WhiteNoise: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "WhiteNoise"; }

		/** Ports **/
		AudioOutPort mOutput;

		/** Controls **/
		FloatInControl mMean;
		FloatInControl mStd; ///< Standard deviation

		/** Configuration **/
		EDistributionType mDistributionType; ///<  Uniform/Gaussian

	public:
		WhiteNoise()
			:
			mOutput("mOutput", this),
			mMean("Noise Mean", this),
			mStd("Noise Standard Deviation", this)
		{
			Configure( mConfig );

			mMean.SetBounds(-1,1);
			mMean.SetDefaultValue(0.0);
			mMean.DoControl(0.0);

			mStd.SetBounds(0,1); //NOTE: use std > 0.0
			mStd.SetDefaultValue(1.0);
			mStd.DoControl(1.0);

			/* initialize random seed: */
			srand( time(NULL) );

			_normalMax = 1.;
		}

		~WhiteNoise() {}

		bool Do()
		{
			bool result = false;
			switch( mDistributionType )
			{
			case EDistributionType::eUniform:
				result = DoUniform( mOutput.GetAudio() );
				break;
			case EDistributionType::eGaussian:
				result =  DoGaussian( mOutput.GetAudio() );
				break;
			default:
				CLAM_ASSERT( false, "Bad mDistributionType" );
			}

			mOutput.Produce();

			return result;
		}

		typedef WhiteNoiseConfig Config;

	protected:	
		bool DoUniform(Audio& out)
		{
			int size = out.GetSize();

			DataArray& buf = out.GetBuffer();

			TData mean = mMean.GetLastValue();
			TData std = mStd.GetLastValue();
			for (int i=0;i<size;i++) 
			{
				//Uniform noise
				buf[i] = 2.*rand()/RAND_MAX - 1.;

				//Scaling to meet required noise parameters
				buf[i] *= std;
				buf[i] += mean;
			}
			return true;
		}

		bool DoGaussian(Audio& out)
		{
			int size = out.GetSize();

			DataArray& buf = out.GetBuffer();

			TData mean = mMean.GetLastValue();
			TData std = mStd.GetLastValue();
			double normalValue = 0.;
			for (int i=0;i<size;i++) 
			{
				//TODO: select the algorithm in the configuration?
				//Gaussian noise
 				normalValue = NormalPolarMethod();
// 				normalValue = NormalRatioMethod();
				if (fabs(normalValue)>_normalMax) _normalMax = fabs(normalValue);
 				buf[i] = normalValue/_normalMax;


				//Scaling to meet required noise parameters
    				buf[i] *= fabs(std/0.2); //NOTE: Normal*Method() throws values greater than 1 and when normalization to -1..1 range occurs, std value moves too. So multiplying by std/constant you get std equal to 1 again. Use 0.2 as constant to the NormalPolarMethod and 0.215 to NormalRatioMethod
  				buf[i] += mean;
			}
			return true;
		}

 	protected:
		const ProcessingConfig& GetConfig() const {	return mConfig; }

		bool ConcreteConfigure(const ProcessingConfig& config) {

			CopyAsConcreteConfig( mConfig, config );

			mDistributionType = mConfig.GetDistributionType();

			return true;
		}

		/**
		*	Returns a normal (Gaussian) distributed real number.
		*	Taken from GSL - GNU Scientific Library (http://www.gnu.org/software/gsl/)
		*
		*	Polar (Box-Mueller) method; See Knuth v2, 3rd ed, p122
		*/
		double NormalPolarMethod()
		{
			double x, y, r2;
			const double sigma=1.; //std value
			do
			{
				/* choose x,y in uniform square (-1,-1) to (+1,+1) */
				x = -1 + 2 * ((double)rand())/RAND_MAX;
				y = -1 + 2 * ((double)rand())/RAND_MAX;
			
				/* see if it is in the unit circle */
				r2 = x * x + y * y;
			}
			while (r2 > 1.0 || r2 == 0);
			
			/* Box-Muller transform */
			return sigma * y * sqrt (-2.0 * log (r2) / r2);
		}

		/**
		*	Returns a normal (Gaussian) distributed real number.
		*	Taken from GSL - GNU Scientific Library (http://www.gnu.org/software/gsl/)
		*
		*	Ratio method (Kinderman-Monahan); see Knuth v2, 3rd ed, p130.
		*	K+M, ACM Trans Math Software 3 (1977) 257-260.
		*	[Added by Charles Karney] This is an implementation of Leva's
		*	modifications to the original K+M method; see:
		*	J. L. Leva, ACM Trans Math Software 18 (1992) 449-453 and 454-455.
		*/
		double NormalRatioMethod ()
		{
			const double sigma = 1.; //std value
			
			double u, v, x, y, Q;
			const double s = 0.449871;    /* Constants from Leva */
			const double t = -0.386595;
			const double a = 0.19600;
			const double b = 0.25472;
			const double r1 = 0.27597;
			const double r2 = 0.27846;
			
			do                            /* This loop is executed 1.369 times on average  */
			{
				/* Generate a point P = (u, v) uniform in a rectangle enclosing
					the K+M region v^2 <= - 4 u^2 log(u). */
			
				/* u in (0, 1] to avoid singularity at u = 0 */
				u = 1 - ((double)rand())/RAND_MAX;
			
				/* v is in the asymmetric interval [-0.5, 0.5).  However v = -0.5
					is rejected in the last part of the while clause.  The
					resulting normal deviate is strictly symmetric about 0
					(provided that v is symmetric once v = -0.5 is excluded). */
				v = ((double)rand())/RAND_MAX - 0.5;
			
				/* Constant 1.7156 > sqrt(8/e) (for accuracy); but not by too
					much (for efficiency). */
				v *= 1.7156;
			
				/* Compute Leva's quadratic form Q */
				x = u - s;
				y = fabs (v) - t;
				Q = x * x + y * (a * y - b * x);
			
				/* Accept P if Q < r1 (Leva) */
				/* Reject P if Q > r2 (Leva) */
				/* Accept if v^2 <= -4 u^2 log(u) (K+M) */
				/* This final test is executed 0.012 times on average. */
			}
			while (Q >= r1 && (Q > r2 || v * v > -4 * u * u * log (u)));
			
			return sigma * (v / u);       /* Return slope */
		}

	private:
		/** Configuration **/
		Config mConfig;

		TData _normalMax;
	};

};//namespace CLAM

#endif // _WhiteNoise_
