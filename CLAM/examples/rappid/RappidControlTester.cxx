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

#include "RappidControlTester.hxx"
#include "Rappid.hxx"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

RappidControlTester::RappidControlTester(Rappid &r,
										 unsigned long sleep)
	: mThread(0),
	  mSleep(sleep),
	  mVictim(r)
{
}

void RappidControlTester::Start(void)
{
	mThread = pthread_create(&mThread,0,TestThread,this);
}

void *RappidControlTester::TestThread(void* arg)
{
	RappidControlTester *t = static_cast<RappidControlTester*>(arg);

	while (1) {
		t->SendControls();
		t->Wait();
	}

	return 0;
}


void RappidControlTester::SendControls()
{
	puts("Sending controls...");
	for (int i = Rappid::eFirstControl; i< Rappid::eNUMControls; i++) {
		mVictim.Control.EnqueueControl(i,(TControlData(rand())/TControlData(RAND_MAX)));
	}
}


void RappidControlTester::Wait()
{
	usleep(mSleep);
}

