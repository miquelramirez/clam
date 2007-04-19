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

#ifndef __APLAYER__ 
#define __APLAYER__ 
 
#include <vector>
#include <CLAM/Player.hxx>
 
namespace CLAM
{
	class Audio;

	namespace VM
	{
		class APlayer : public Player
		{
		public:
			APlayer();
			~APlayer();
				
			void SetData(std::vector<const Audio*> data, bool setTime=true);

			void SetLeftChannelMuted(bool b);
			void SetRightChannelMuted(bool b);
				
		protected:
			void thread_code();
				
		private:
			const Audio* mLeftChannel;
			const Audio* mRightChannel;

			bool mMuteLeft, mMuteRight;

			const bool& IsMutedLChannel() const;
			const bool& IsMutedRChannel() const;
		};
	}
}
 
#endif
 
