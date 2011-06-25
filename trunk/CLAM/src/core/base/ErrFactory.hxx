/*
 * Copyright (c) 2007 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _ErrFactory_hxx_
#define _ErrFactory_hxx_

#include "Err.hxx"

namespace CLAM {

/// Error type thrown by Factory classes (Factory, FactoryRegistry)
class ErrFactory : public Err
{
public:
	ErrFactory(const char* msg) : Err(msg)
	{}

};

} // namespace
#endif // _ErrFactory_hxx_

