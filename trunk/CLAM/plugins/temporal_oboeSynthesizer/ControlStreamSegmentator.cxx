#include "ControlStreamSegmentator.hxx"
#include <time.h>
#include <math.h>
#include <iostream>

ControlStreamSegmentator::ControlStreamSegmentator()
 :	previousPitch(-1.0), previousAmplitude(-1.0),
	pitchChangeThreshold(DEFAULT_PITCH_CHANGE_THRESHHOLD)
{
}

ControlStreamSegmentator::~ControlStreamSegmentator()
{
}

bool ControlStreamSegmentator::IsNewVoice(float pitch, float amplitude)
{
	// if this is the very first value, then the currentMotifId == -1
	// in this case generate a new motif id and return it
	if (previousPitch == -1.0)
	{
		SaveControlStream(pitch, amplitude);
		return true;
	}
	
	// we convert the frequencies to MIDI note numbers (with fractional parts)
	// so that we can compare the change between this and the last pitch
	// in terms of semitones rather than hertz
	float midiPitch = FrequencyToMidiNote( pitch );
	float previousMidiPitch = FrequencyToMidiNote( previousPitch );
	double pitchDerivative = fabs( midiPitch - previousMidiPitch );

	//std::cout << "Segmentator: deriv: " << pitchDerivative << std::endl;
	
	if (pitchDerivative < pitchChangeThreshold)
	{
		SaveControlStream(pitch, amplitude);
		return false;
	}
	else
	{
		SaveControlStream(pitch, amplitude);
		return true;
	}
}

void ControlStreamSegmentator::SaveControlStream(float pitch, float amplitude)
{
	previousPitch = pitch;
	previousAmplitude = amplitude;
}

float ControlStreamSegmentator::FrequencyToMidiNote(float freq)
{
	return (float) (( 12.0 * log(freq / 220.0) / log(2.0) ) + 57.01);
}
