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

#ifndef __ControlStreamSegmentator__
#define __ControlStreamSegmentator__

#include "ContinuousExcitationConstants.hxx"

#define DEFAULT_PITCH_CHANGE_THRESHHOLD 0.75

class ControlStreamSegmentator
{
public:
	ControlStreamSegmentator();
	~ControlStreamSegmentator();
	
	bool IsNewVoice(float pitch, float amplitude);

private:
	void SaveControlStream(float pitch, float amplitude);
	float FrequencyToMidiNote(float frequency);
	
	float previousPitch;
	float previousAmplitude;
	// if the change in pitch exceeds this value, then a new motif
	// will be created
	float pitchChangeThreshold;
};


#endif

