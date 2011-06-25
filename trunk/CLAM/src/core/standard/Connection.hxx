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

/** \file Connection.hxx
 */
#ifndef __CONNECTION__
#define __CONNECTION__

namespace SigSlot
{

class Signal;

/** \class Connection
 *  \brief Class that model connections between signals and slots.
 *
 *  This class model the concept of signal and slot connection i.e. the knowledge
 *  a signal has about who it has to notify whenever the client invoke the Emit( ) 
 *  method on a Signal object. Also it becomes the passage through the Slot::Unbind
 *  method can access the bound signals to disconnect them.
 *  Each time a Signal and a Slot are bound together a Connection object is created,
 *  being tagged by a Global Unique IDenfier ( GUID ). These GUID's are reused whenever
 *  the Connection object holding it is destroyed.
 *
 *  \sa   Signal, Signalv0, Signalv1, Signalv2, Signalv3, Signalv4
 *  \sa   Slot, Slotv0, Slotv1, Slotv2, Slotv3, Slotv4
 */
class Connection
{
public:
		typedef unsigned tConnectionId; /**< Type declaration for the ConnectionId */

		/** \brief Default constructor
		 *
		 *  Initializes the Connection so to be nil. This constructor should not
		 *  be used from the client: its existence is justified for allowing to have
		 *  Connection objects inside STL containers
		 */
		Connection();

		/** \brief Explicit constructor
		 *
		 *  Initializes the object with a GUID and a reference to the Signal the Slot is
		 *  being connected to.
		 */
		Connection( tConnectionId id, Signal* connectedSignal );

		/** \brief Assignment operator.
		 *
		 *  Copies the Connection object provided, keeping the GUID and the reference
		 *  to the signal. To avoid owning problems, the source Connection object is marked
		 *  as 'disabled'.
		 */
		Connection& operator=( Connection& s );

		/** \brief Copy constructor.
		 *
		 *  \see operator=
		 */
		Connection( const Connection& s );
		
		/** \brief Accessor to the GUID.
		 *
		 *  Accessor for getting the GUID hold by the Connection object.
		 *  \returns  The GUID.
		 */
		tConnectionId GetID() const
		{
				return mID;
		}
		
		/** \brief Destructor.
		 *
		 *  When connections are destroyed it is checked that the
		 *  connection is active, if that is the case then the Signal is notified
		 *  about not calling anymore the involved slot object.
		 */
		~Connection();
		
private:
		mutable bool  mMustFreeSignal;  /**< Mutable flag for determining if the Connection is active or not. */
		tConnectionId mID;              /**< The GUID being hold by the object. */
		Signal*       mConnectedSignal; /**< A reference to the Signal whom the Slot is connected to. */
};

}

#endif // Connection.hxx

