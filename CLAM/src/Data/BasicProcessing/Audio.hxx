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

#ifndef _AUDIO_HXX
#define _AUDIO_HXX


//
// Finally, we have decided to use a single buffer of TDatas, all
// previously existing data formats have proven useless and have
// been removed.
//


#include "DynamicType.hxx"
#include "Array.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "DataTypes.hxx"

namespace CLAM {


	/** Audio class with the Data Buffer, information about it (SampleRate, BeginTime), and methods to insert and get Audio chunks.
	 * You can also get 'slices' that's an audio that maps to 
	 * some other audio's data.
	 *  @see AudioDescriptors, Spectrum
	 */
	class Audio: public ProcessingData {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Audio, 3, ProcessingData);

		/** Sample rate in Hz */
		DYN_ATTRIBUTE (0, public, TData, SampleRate);
		/** Temporal Tags for audio chunk in miliseconds */
		DYN_ATTRIBUTE (1, public, TTime, BeginTime);
		/** The Audio buffer */
		DYN_ATTRIBUTE (2, public, DataArray, Buffer);

	protected:

		void DefaultInit(); // To be used from the dynamic-type-generated constructor.

	public:

	
		/** @return the audio buffer size in samples*/
		int GetSize() const {return GetBuffer().Size();}

		/** Getter for the end time in miliseconds
		 *  @return The current end time for the buffer size */
		TTime GetEndTime() const {return GetBeginTime()+GetTimeFromIndex(GetSize());}

	 	/** Method for getting the total duration of Buffer
		 *  @return The duration of the audio buffer in miliseconds */
		TTime GetDuration() const {return GetTimeFromIndex(GetSize());}

		/** Resizes the array buffers to a size in samples.
		 *  Increases or shrinks by the end of the buffer preserving the data.
		 *  If size increases, new samples are set to 0.
		 *  @todo Document how data in the buffer is discarded
		 *  @param s The new size in samples */
		void SetSize(int s);

		/** 
		 *  @deprecated use ResizeToEndTime instead
		 */
		void SetEndTime(TTime time);

		/** Resizes the array buffer to last from the begin time
		 *  until the specified time position (time in miliseconds)
		 *  @todo Document how data in the buffer is discarded
		 *  @param time End time of the buffer once resized
		 *  @see SetSize
		 */
		void ResizeToEndTime(TTime time);

		/** 
		 *  @deprecated use ResizeToDuration instead
		 */
		void SetDuration(TTime duration);
		
		/** Resizes the array buffer to last the specified
		 *  time duration (miliseconds).
		 *  @todo Document how data in the buffer is discarded
		 *  @param duration The required duration for the buffer once resized
		 *  @see SetSize
		 *
		 */
		void ResizeToDuration(TTime duration);

		/** Method for copying an audio chunk out of the existing data. Time in milliseconds.
		 *  @param beginTime Beginning of the chunk in time measure.
		 *  @param endTime Ending of the chunk in time measure.
		 *  @param chunk The Audio data chunk
		 *  @param configureChunk Optional parameter to set the configuration of the chunk. True by default.
		*/
		void GetAudioChunk(TTime beginTime, TTime endTime,Audio& chunk,bool configureChunk=true) const;

		/**Method for copying an audio chunk out of the existing data
		 *  @param beginIndex Beginning of the chunk inside the Buffer.
		 *  @param endIndex Ending of the chunk inside the Buffer
		 *  @param chunk The Audio data chunk
		 *  @param configureChunk Optional parameter to set the configuration of the chunk. True by default.
		*/
		void GetAudioChunk(TIndex beginIndex,TIndex endIndex,Audio& chunk, bool configureChunk=true) const;

		/** Method for copying an audio "slice" out of the existing data. The difference between slices
		 *  Time in miliseconds.
		 *  and chunks is that slices don't own any memory.
		 *  @param beginTime Beginning of the chunk in time measure.
		 *  @param endTime Ending of the chunk in time measure.
		 *  @param slice The Audio data slice
		 *  @param configureSlice Optional parameter to set the configuration of the slice. True by default.
		*/
		void GetAudioSlice(TTime beginTime, TTime endTime,Audio& slice,bool configureSlice=true) const;


		/** Method for getting an audio "slice" out of the existing data. The difference between slices
		 *  and chunks is that slices don't own any memory.
		 *  @param beginIndex Beginning of the chunk inside the Buffer.
		 *  @param endIndex Ending of the chunk inside the Buffer
		 *  @param slice The Audio slice
		 *  @param configureSlice Optional parameter to set the configuration of the slice. True by default.
		*/
		void GetAudioSlice( TIndex beginIndex, TIndex endIndex, Audio& slice, bool configureSlice=true) const;

		/**Method for setting an audio chunk of the audio out of an existing chunk. Time in miliseconds.
		 *  @param beginTime Beginning location of the chunk in the Buffer in time measure.
		 *  @param chunk The Audio data chunk to insert
		 */
		void SetAudioChunk(TTime beginTime,const Audio& chunk);

		/**Method for setting an audio chunk of the audio out of an existing chunk
		 *  @param beginIndex Beginning location of the chunk in the Buffer.
		 *  @param chunk The Audio data chunk to insert
		 */
		void SetAudioChunk(TIndex beginIndex,const Audio& chunk);

	private:
		/** Conversion utility for handling indices and time tags
		 *  @param index Point to get time tag
		 *  @return  Exact time for the index indicated
		 */
		TTime GetTimeFromIndex(TIndex index) const;

		/** Conversion utility for handling indices and time tags. Time in miliseconds
		 *  @param time Point to get index tag
		 *  @return Index of the position marked in time
		 */
		TIndex GetIndexFromTime(TTime time) const;
	};
};//namespace

#endif

