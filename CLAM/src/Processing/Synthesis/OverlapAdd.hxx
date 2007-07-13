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

#ifndef _OverlapAdd_
#define _OverlapAdd_

#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Processing.hxx"
#include "Storage.hxx"
 
#include "OverlapAddConfig.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"

namespace CLAM {

	class OverlapAdd;

/*	  Was:
	  For good results FrameSize should be the same as HopSize; and BufferSize should
	  be at least FrameSize+HopSize
*/

	/** Configuration class for Overlapp Add Processing.
	 *  HopSize configures the write size of the circular buffer (note that the write size is
	 *	here understood as the portion of the frame that is overwritten, not added).
	 *  FrameSize configures the read size of the circular buffer.
	 *  BufferSize configures the overall physical size of the circular buffer.
	 *  Currently, HopSize must be hop size, FrameSize must be hop size and 
	 *  BufferSize must be twice hop size (as it will do 50% overlap).
	 */
	/* OverlapAddConfig moved to OverlapAddConfig.hxx */


	/** OverlapAdd Processing class.
	 *  First half of the input buffer or audio (which is of buffer size) is added to 
	 *  existing data in the circular buffer (ie. 'mixed in'), second half overwrites 
	 *  existing data. When reading, only already overlapped regions should be given as 
	 *  output (this overlapped region and thus the output will be of frame size).
	 *  @see OverlapAddConfig 
	 */
	class OverlapAdd: public Processing 
	{
	public:

		OverlapAdd(const OverlapAddConfig &c=OverlapAddConfig());
		~OverlapAdd();
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/* do methods */

		/** Unsupervised Do method, not implemented */
		bool Do(void);

		/** Do method 
		 *  @param in: input Audio (size must be buffer size).
		 *  @param out: output Audio (size must be frame size).
		 */
		bool Do( const Audio &in, Audio &out);

	private:

		const char *GetClassName() const {return "OverlapAdd";}

		/** Configuration change method
		 * @pre argument should be an OverlapAddConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		/* protected methods */
	protected:
		OverlapAddConfig   mConfig;

		AudioInPort mInput;
		AudioOutPort mOutput;

		Audio mTmp;
	};
} //end of namespace CLAM


#endif // _OverlapAdd_

