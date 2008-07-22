/*
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

#ifndef __SDIFDatabaseProcessing_hxx
#define __SDIFDatabaseProcessing_hxx

#include "SDIFDatabase.hxx"
#include "ProcessingComposite.hxx"
#include "InControl.hxx"
#include "OutPort.hxx"
#include "OutPortPublisher.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"
#include "DirectoryName.hxx"

namespace CLAM {

#define SDIFDATABASEPROCESSING_FUNDAMENTALFREQUENCY_CONTROL_INPORT_NAME "Pitch"
#define SDIFDATABASEPROCESSING_AMPLITUDE_CONTROL_INPORT_NAME "Amplitude"
#define SDIFDATABASEPROCESSING_SPECTRALCENTROID_CONTROL_INPORT_NAME "Brightness"
#define SDIFDATABASEPROCESSING_SPECTRALPEAKS_OUTPORT_NAME "SpectralPeaks"
#define SDIFDATABASEPROCESSING_RESIDUALSPECTRUM_OUTPORT_NAME "Residual"

class SDIFDatabaseConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE(SDIFDatabaseConfig, 1, ProcessingConfig);
	// TODO change this to a directory name?
	DYN_ATTRIBUTE (0, public, DirectoryName, DirectoryName);

protected:
	/** Dynamic type initialization: All attributes are instantiated. */
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetDirectoryName( "" );
	}
};

/**
 * Processing for audio file input. It can be configured
 * using an AudioFileConfig Config class. Only file name
 * and optionally file type must be configured, other
 * fields will be set by the Processing itself once
 * Start() has been called.
 */
class SDIFDatabaseProcessing : public Processing
{
public:
	SDIFDatabaseProcessing();
	SDIFDatabaseProcessing(const SDIFDatabaseConfig &c);
	~SDIFDatabaseProcessing();

	const char *GetClassName() const {return "SDIFDatabaseProcessing";}

	bool ConcreteConfigure(const ProcessingConfig&);

	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool Do(void);

protected:
	bool ConcreteStart();
	bool ConcreteStop();
	void CopyResidualSpectrumsMembers(	Spectrum& sourceResidualSpectrum,
										Spectrum& targetResidualSpectrum);

private:
	InControl mPitchControl;
	InControl mAmplitudeControl;
	InControl mBrightnessControl;
	OutPort<SpectralPeakArray> mOutputSpectralPeaks;
	OutPort<Spectrum> mOutputResidualSpectrum;

	SDIFDatabaseConfig mConfig;
	SDIFDatabase mSDIFDatabase;
	VoiceId mVoiceId;
};

} // end namespace CLAM

#endif

