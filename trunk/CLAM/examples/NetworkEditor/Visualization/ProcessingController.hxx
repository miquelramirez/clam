/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _PROCESSINGCONTROLLER_
#define _PROCESSINGCONTROLLER_

#include "ModelController.hxx"
#include "ProcessingModel.hxx"

namespace CLAM
{
	class Processing;
	class ProcessingConfig;
}

namespace CLAMVM
{

class InPortAdapter;
class OutPortAdapter;
class InControlAdapter;
class OutControlAdapter;

class ProcessingController : public ModelController, public ProcessingModel
{
private:
	CLAM::Processing* mObserved;
	CLAM::ProcessingConfig* mConfig;
	std::list<InPortAdapter*> mInPortAdapters;
	std::list<OutPortAdapter*> mOutPortAdapters;
	std::list<InControlAdapter*> mInControlAdapters;
	std::list<OutControlAdapter*> mOutControlAdapters;
	typedef std::list<OutPortAdapter*>::iterator OutPortAdapterIterator;
	typedef std::list<InPortAdapter*>::iterator InPortAdapterIterator;
	typedef std::list<OutControlAdapter*>::iterator OutControlAdapterIterator;
	typedef std::list<InControlAdapter*>::iterator InControlAdapterIterator;

protected:
	void OnUpdateConfigFromGUI( CLAM::ProcessingConfig* );
public:
	ProcessingController();
	virtual ~ProcessingController();
	virtual const char* GetClassName() const
	{
		return "ProcessingController";
	}
	virtual bool Publish();
	virtual bool BindTo( CLAM::Processing& obj );
	virtual bool Update();
};

} // namespace CLAMVM

#endif // __PROCESSINGCONTROLLER__
