/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "CepstralTransform.hxx"
#include "OSDefines.hxx"
#include "Assert.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden 
{
	static const char* metadata[] = {
		"key", "CepstralTransform",
		"category", "Analysis",
		"description", "CepstralTransform",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, CepstralTransform> regCepstralTransform("CepstralTransform");
	static FactoryRegistrator<ProcessingFactory, CepstralTransform> reg = metadata;
}

void CepstralTransformConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetNumMelCoefficients(20);
	SetNumCepstrumCoefficients(20);
	SetUseBase10(false);
}

CepstralTransform::CepstralTransform(  )
	: mIn("Mel Spectrum", this)
	, mOut("Mel Cepstrum", this)
{
	Configure(CepstralTransformConfig());
}

CepstralTransform::CepstralTransform( const CepstralTransformConfig& cfg )
	: mIn("Mel Spectrum", this)
	, mOut("Mel Cepstrum", this)
{
	Configure( cfg );
}

bool CepstralTransform::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( mConfig, cfg );

	mLogBuffer.Resize(mConfig.GetNumMelCoefficients());
	mLogBuffer.SetSize(mConfig.GetNumMelCoefficients());

	return true;
}

bool CepstralTransform::Do()
{
	const MelSpectrum & spectrum = mIn.GetData();
	MelCepstrum & cepstrum = mOut.GetData();
	bool ok = Do(spectrum, cepstrum);
	mIn.Consume();
	mOut.Produce();
	return ok;
}

bool CepstralTransform::ConcreteStart()
{
	return true;
}

bool CepstralTransform::Do( const MelSpectrum& melSpec, MelCepstrum& melCeps )
{
	if ( !AbleToExecute() ) return false; // object was disabled

	const DataArray& melCoeffs = melSpec.GetCoefficients();
	DataArray&  cepstrumCoeffs = melCeps.GetCoefficients();
	const TSize numMelCoefficients = melCoeffs.Size();
	const TSize numCepstrumCoeffs  = mConfig.GetNumCepstrumCoefficients();

	if ( cepstrumCoeffs.Size() < numCepstrumCoeffs )
	{
		cepstrumCoeffs.Resize(numCepstrumCoeffs);
		cepstrumCoeffs.SetSize(numCepstrumCoeffs);
	}

	melCeps.SetCenterTime(melSpec.GetCenterTime());
	melCeps.SetLowCutoff(melSpec.GetLowCutoff());
	melCeps.SetHighCutoff(melSpec.GetHighCutoff());

	/* Take the logarithm of the Mel spectrum.  NOTE: What is the
		 significance of log vs. log10? */
	const TData melFloor  = 1.0;
	const bool  useBase10 = mConfig.GetUseBase10();

	if ( mLogBuffer.Size()!=numMelCoefficients)
	{
		// TODO: This has realtime penalty
		mLogBuffer.Resize(numMelCoefficients);
		mLogBuffer.SetSize(numMelCoefficients);
	}
	for (TIndex i=0; i<numMelCoefficients; i++) {
		if (melCoeffs[i] < melFloor)
			mLogBuffer[i] = useBase10 ? log10(melFloor) : log(melFloor);
		else
			mLogBuffer[i] = useBase10 ? log10(melCoeffs[i]) : log(melCoeffs[i]);
	}

	/* Take the DCT of the logarithm. */
	const TData piord = PI / (TData)numMelCoefficients;
	const TData scale = sqrt(2.0/(TData)numMelCoefficients);

	// Compute N-1 last coefficients.
	TData freq = 0;
	for (TIndex i=1; i<numCepstrumCoeffs; i++)
	{
		freq = i*piord;
		cepstrumCoeffs[i] = 0.0;
		for (TIndex j=0; j<numMelCoefficients; j++)
			cepstrumCoeffs[i] += mLogBuffer[j] * cos(freq*(j+1 - 0.5));
		cepstrumCoeffs[i] *= scale;
	}

	// Compute first coefficient.
	for (TIndex j=0; j<numMelCoefficients; j++)
		cepstrumCoeffs[0] += mLogBuffer[j];
	cepstrumCoeffs[0] *= scale;


	/* NOTE: How about implementing cepstral liftering from HTK? Probably not. */

	return true;
}

}

