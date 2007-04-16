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

#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include <iostream>

namespace CLAMTest {

	using namespace CLAM;
	namespace TestUtils {

		/** Spectrum comparison */
		TData CompareMagPhase(Spectrum &s1, Spectrum &s2);
		TData CompareComplex(Spectrum &s1, Spectrum &s2);
		TData ComparePolar(Spectrum &s1, Spectrum &s2);
		TData CompareBPF(Spectrum &s1, Spectrum &s2);

		/** Returns the maximun of the magnitudes of the complex
		 * differences for each point */
		TData MaxDiff(Spectrum &s1, Spectrum &s2)
		{
			SpecTypeFlags f1,f2;
			
			if (s1.GetSize() !=
				s2.GetSize())
				throw(Err("MaxDif: Comparing spectrums of different size"));

			if (s1.GetSpectralRange() !=
				s2.GetSpectralRange())
				throw(Err("MaxDif: Comparing spectrums of different spectral range"));

			s1.GetType(f1);
			s2.GetType(f2);
			
			// Check if comparision with no conversion is possible.
			if (f1.bComplex && f2.bComplex)
				return CompareComplex(s1,s2);
			if (f1.bMagPhase && f2.bMagPhase)
				return CompareMagPhase(s1,s2);
			if (f1.bPolar && f2.bPolar)
				return ComparePolar(s1,s2);
			if (f1.bMagPhaseBPF && f2.bMagPhaseBPF)
				return CompareBPF(s1,s2);

			// We need to convert.
			if (f1.bComplex || f2.bComplex)
				return CompareComplex(s1,s2);
			if (f1.bMagPhase || f2.bMagPhase)
				return CompareMagPhase(s1,s2);
			if (f1.bPolar || f2.bPolar)
				return ComparePolar(s1,s2);
			if (f1.bMagPhaseBPF || f2.bMagPhaseBPF)
				return CompareBPF(s1,s2);

			throw(Err("SpectrumCompare: No attributes in inputs."));
		}
		
		TData CompareComplex(Spectrum &s1, Spectrum &s2)
		{
			Complex *c1,*c2;
			TData max_dif=0.0;
			int i,size = s1.GetSize();
			bool remove1=false,remove2=false;
			SpecTypeFlags flags;

			s1.GetType(flags);
			if (!flags.bComplex) {
				remove1=true;
				flags.bComplex=true;
				s1.SetTypeSynchronize(flags);
			}
			s2.GetType(flags);
			if (!flags.bComplex) {
				remove2=true;
				flags.bComplex=true;
				s2.SetTypeSynchronize(flags);
			}

			c1 = s1.GetComplexArray().GetPtr();
			c2 = s2.GetComplexArray().GetPtr();
			if (!c1 || !c2)
				throw(Err("CompareMagPhase: Null complex array in spectrum."));
			
			for (i=0; i<size; i++) {
				TData real=c2[i].Real()-c1[i].Real();
				TData imag=c2[i].Imag()-c1[i].Imag();
				TData diff = fabs(sqrt(pow(real,2)+pow(imag,2)));
				if (diff > max_dif)
					max_dif=diff;
			}
			if (remove1) {
				s1.GetType(flags);
				flags.bComplex=false;
				s1.SetType(flags);
			}
			if (remove2) {
				s2.GetType(flags);
				flags.bComplex=false;
				s2.SetType(flags);
			}
			return max_dif;
		}

		TData CompareMagPhase(Spectrum &s1, Spectrum &s2)
		{
			TData *m1,*m2,*p1,*p2;
			TData max_dif=0.0;
			int i,size = s1.GetSize();
			bool remove1=false,remove2=false;
			SpecTypeFlags flags;

			s1.GetType(flags);
			if (!flags.bMagPhase) {
				remove1=true;
				flags.bMagPhase=true;
				s1.SetTypeSynchronize(flags);
			}
			s2.GetType(flags);
			if (!flags.bMagPhase) {
				remove2=true;
				flags.bMagPhase=true;
				s2.SetTypeSynchronize(flags);
			}


			m1 = s1.GetMagBuffer().GetPtr();
			m2 = s2.GetMagBuffer().GetPtr();
			p1 = s1.GetPhaseBuffer().GetPtr();
			p2 = s2.GetPhaseBuffer().GetPtr();
			if (!m1 || !m2 || !p1 || !p2)
				throw(Err("CompareMagPhase: Null buffer in spectrum Mag or Phase."));
			
			for (i=0; i<size; i++) {
				TData real=m2[i]*cos(p2[i])-m1[i]*cos(p1[i]);
				TData imag=m2[i]*sin(p2[i])-m1[i]*sin(p1[i]);
				TData diff = fabs(sqrt(pow(real,2)+pow(imag,2)));
				if (diff > max_dif)
					max_dif=diff;
			}
			if (remove1) {
				s1.GetType(flags);
				flags.bMagPhase=false;
				s1.SetType(flags);
			}
			if (remove2) {
				s2.GetType(flags);
				flags.bMagPhase=false;
				s2.SetType(flags);
			}
			return max_dif;
		}

		TData ComparePolar(Spectrum &s1, Spectrum &s2)
		{
			Polar *p1,*p2;
			TData max_dif=0.0;
			int i,size = s1.GetSize();
			bool remove1=false,remove2=false;
			SpecTypeFlags flags;

			s1.GetType(flags);
			if (!flags.bPolar) {
				remove1=true;
				flags.bPolar=true;
				s1.SetTypeSynchronize(flags);
			}
			s2.GetType(flags);
			if (!flags.bPolar) {
				remove2=true;
				flags.bPolar=true;
				s2.SetTypeSynchronize(flags);
			}


			p1 = s1.GetPolarArray().GetPtr();
			p2 = s2.GetPolarArray().GetPtr();
			if (!p1 || !p2)
				throw(Err("ComparePolar: Null Polar array in spectrum"));
			
			for (i=0; i<size; i++) {
				TData real=p2[i].Mag()*cos(p2[i].Ang())-p1[i].Mag()*cos(p1[i].Ang());
				TData imag=p2[i].Mag()*sin(p2[i].Ang())-p1[i].Mag()*sin(p1[i].Ang());
				TData diff = fabs(sqrt(pow(real,2)+pow(imag,2)));
				if (diff > max_dif)
					max_dif=diff;
			}
			if (remove1) {
				s1.GetType(flags);
				flags.bPolar=false;
				s1.SetType(flags);
			}
			if (remove2) {
				s2.GetType(flags);
				flags.bPolar=false;
				s2.SetType(flags);
			}
			return max_dif;
		}

		TData CompareBPF(Spectrum &s1, Spectrum &s2)
		{
			throw(Err("CompareBPF: Not implemented"));
		}

	}

}

