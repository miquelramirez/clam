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

#ifndef __BaseAudioApplication__
#define __BaseAudioApplication__

#include "Application.hxx"

#include <pthread.h>
#undef GetClassName

namespace CLAM {

	class BaseAudioApplication:public Application
	{
	protected:
		BaseAudioApplication();
		bool Canceled()
		{
			return cancel;
		}
	private:
		pthread_t thread;
		
		static void* SAudioThread(BaseAudioApplication *pThis);
		static void SAudioThreadCleanup(BaseAudioApplication *pThis);
		virtual void AudioMain(void) = 0;
		virtual void UserMain(void);
		virtual void AppCleanup() {};
	protected:
		bool cancel;
		virtual void Run(int argc = 0,char** argv = 0);
		void Start(void);
	public:
		void Stop(void);
	};

} // namespace CLAM

#endif

