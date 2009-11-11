/*
 * Copyright (c) 2009 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef BUFFERDELAYPREDICTION_INCLUDED
#define BUFFERDELAYPREDICTION_INCLUDED

#include <CLAM/Audio.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>


namespace CLAM
{

class BufferDelayPredictionConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (BufferDelayPredictionConfig , 3, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, NumberOfInPorts);
	DYN_ATTRIBUTE (1, public, int, QualityThreshold);
	DYN_ATTRIBUTE (2, public, int, SampleRate);
	//DYN_ATTRIBUTE (2, public, int, BufferSize);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetNumberOfInPorts(1);
		SetQualityThreshold(90);
		SetSampleRate(48000.0);
		//SetBufferSize(1024);
	}

};

class BufferDelayPrediction: public Processing
{
private:
	BufferDelayPredictionConfig mConfig;
	float mSampleRate;
	int mBufferNr; // debug only

	typedef InPort<Audio> AudioInBuffer;

	std::vector<AudioInBuffer*> mInputBufs;
	std::vector<FloatOutControl*> mOutputControls;
	std::vector<FloatInControl*> mInputControls;
	
	void RemovePortsAndControls();
	void CreatePortsAndControls();

public:
	BufferDelayPrediction();
	virtual ~BufferDelayPrediction()
	{
		RemovePortsAndControls();
	}

	bool ConcreteConfigure(const ProcessingConfig& c);
	bool ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}

	const char * GetClassName() const {return "BufferDelayPrediction";}
	const ProcessingConfig &GetConfig() const { return mConfig;}
	bool Do();
};

} // namespace CLAM

#endif 

