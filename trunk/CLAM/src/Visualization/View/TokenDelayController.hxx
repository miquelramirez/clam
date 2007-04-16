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

#ifndef __TOKENDELAYCONTROLLER__
#define __TOKENDELAYCONTROLLER__

#include "ProcessingController.hxx"
namespace CLAM
{
	class Spectrum;
	template <class T> class TokenDelay;
};

namespace CLAMVM
{
	using CLAM::TokenDelay;
	using CLAM::Spectrum;
		

	class SpectralTokenDelayController 
		: public ProcessingController
	{
	protected:
		const TokenDelay<Spectrum>*  mObserved;

	public:
		SpectralTokenDelayController();

		~SpectralTokenDelayController();

		virtual const char* GetClassName() const
		{
			return "SpectralTokenDelayController";
		}

		virtual bool BindTo( Processing& proc );

		virtual bool Publish();
	};
}

#endif // TokenDelayController.hxx

