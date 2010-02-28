/*
 * Copyright (C) 2009 Club de Audio FIUBA (Santiago Piccinini, Hernan Ordiales)
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

#ifndef _Notch2ndOrder_
#define _Notch2ndOrder_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include "../Filter/Filter.hxx"

#include <math.h>

namespace CLAM {

    /**	\brief 2nd order fixed notch filter
    *
    *	Transfer function
    *	Given:
    *		th = 2 * pi * centralFrequency * samplePeriod
    *		d = exp(-2 * pi * bandwidth/2.0 * samplePeriod)
    *		a = ( 1 + d^2) / 2
    *		b = ( 1 + d^2) * cos(th)
    *
    *	then:
    *	          z^2 - 2 cos (th) z + 1
    *	 H(z) = a ----------------------
    *	            z^2 - b z + d^2
    *
    *	Diference equation:
    *		y(n) - b * y(n-1) + d^2 * y(n-2) = a * x(n)  - a*2*cos(th) * x(n-1) + a * x(n-2)
    */
    class Notch2ndOrderConfig : public ProcessingConfig
    {
    public:
        DYNAMIC_TYPE_USING_INTERFACE( Notch2ndOrderConfig, 2, ProcessingConfig );
        DYN_ATTRIBUTE( 0, public, TData, CentralFrequency );
        DYN_ATTRIBUTE( 1, public, TData, Bandwidth );

    protected:
        void DefaultInit( )
        {
            AddAll();
            UpdateData();
            SetCentralFrequency(200.0);
            SetBandwidth(20.0);
        }
    };

    class Notch2ndOrder: public Processing
    {
        /** This method returns the name of the object
        *    @return Char pointer with the name of object
        */
        const char *GetClassName() const { return "Notch2ndOrder"; }

        /** Ports **/
        AudioInPort mInput;
        AudioOutPort mOutput;

        /**      **/
        TData centralFrequency;
        TData bandwidth;

    public:
        Notch2ndOrder()
            :
            mInput("Input", this),
            mOutput("Output", this)
        {
            Configure( mConfig );
        }

        ~Notch2ndOrder() {}

        typedef Notch2ndOrderConfig Config;

        bool Do()
        {
            mFilter.Do( mInput.GetAudio(), mOutput.GetAudio() );
            mInput.Consume();
            mOutput.Produce();
            return true;
        }


        const CLAM::ProcessingConfig & GetConfig() const
        {
            return mConfig;
        }

protected:
    bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
    {
        CopyAsConcreteConfig(mConfig, config);

            FilterConfig filter_cfg;
            filter_cfg.SetAmountOfInputCoefficients(3);
            filter_cfg.SetAmountOfOutputCoefficients(3);
            mFilter.Configure( filter_cfg );

            centralFrequency = mConfig.GetCentralFrequency();
            bandwidth = mConfig.GetBandwidth();

            TData samplePeriod  =  1.0/ mInput.GetSampleRate();

	    //Auxiliar machinery
            TData aux_th = 2 * 3.1416 * centralFrequency * samplePeriod;
            TData aux_d = exp( -2 * 3.1416 * (bandwidth / 2.0) * samplePeriod);
            TData aux_a = (1 + pow(aux_d, 2)) / 2.0;
            TData aux_b = (1 + pow(aux_d, 2)) * cos(aux_th);

            //FIR coefs
            TData b_k[] = {aux_a, -aux_a*2*cos(aux_th), aux_a};
            char bkstr[5];
            for(unsigned i=0;i<3;i++)
            {
                sprintf(bkstr, "b_%i", i);
                SendFloatToInControl(mFilter, bkstr, b_k[i]);
            }

            //IIR coefs
            TData a_k[] = {1, -aux_b, pow(aux_d, 2)};
            char akstr[5];
            for(unsigned i=0;i<3;i++)
            {
                sprintf(akstr, "a_%i", i);
                SendFloatToInControl(mFilter, akstr, a_k[i]);
            }
        return true;
    }


    private:
        /** Child processings **/
        Filter mFilter;
	
        /** Configuration **/
        Config mConfig;
    };

};//namespace CLAM

#endif // _Notch2ndOrder_
