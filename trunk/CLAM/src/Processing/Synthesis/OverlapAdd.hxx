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

#ifndef _OverlapAdd_
#define _OverlapAdd_

#include "Array.hxx"
#include "Audio.hxx"
#include "AudioCircularBuffer.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Processing.hxx"
#include "Storage.hxx"


namespace CLAM {

	class OverlapAdd;

	/** Configuration class for Overlapp Add Processing
	 *  HopSize configures the write size of the circular buffer
	 *  FrameSize configures the read size of the circular buffer
	 *  BufferSize configures the overall physical size of the circular buffer 
	 *  For good results FrameSize should be twice HopSize and BufferSize should
	 *  be at least FrameSize+HopSize
	 */
	class OverlapAddConfig: public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (OverlapAddConfig, 4, CLAM::ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TSize, HopSize);
		DYN_ATTRIBUTE (2, public, TSize, FrameSize);
		DYN_ATTRIBUTE (3, public, TSize, BufferSize);

		~OverlapAddConfig(){};
	protected:
		void DefaultInit();
		void DefaultValues();
		

	};

	/** OverlapAdd Processing class.
	 *  First half of the input buffer or audio is added to existing data in the
	 *  circular buffer, second half overwrites existing data. When reading, only
	 *  already overlapped regions should be given as output.
	 *  @see OverlapAddConfig 
	 */
	class OverlapAdd: public Processing 
	{
	
	public:

		/* configuration methods */
		OverlapAdd(void);

		OverlapAdd(const OverlapAddConfig &c);

		~OverlapAdd();

		const ProcessingConfig &GetConfig() const { return mConfig;}


		/* do methods */

		/** Unsupervised Do method, not implemented */
		bool Do(void);

		/** Do method 
		 *  @param in: input Data Array 
		 *  @param out: output Data Array 
		 */
		bool Do(DataArray &in, DataArray &out);
		/** Do method 
		 *  @param in: input Audio 
		 *  @param out: output Audio 
		 */
		bool Do(Audio &in, Audio &out);


		/* prototype methods */

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}


				
	private:

		const char *GetClassName() {return "OverlapAdd";}

		/** Configuration change method
		 * @throw
		 * bad_cast exception when the argument is not an OverlapAddConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

		/* protected methods */
	protected:
		OverlapAddConfig   mConfig;
		AudioCircularBuffer mCircBuffer;
	
	};
} //end of namespace CLAM


#endif // _OverlapAdd_
