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

#ifndef __QT_PROCESSINGCONFIGPRESENTATION_HXX__
#define __QT_PROCESSINGCONFIGPRESENTATION_HXX__

#include <qdialog.h>
#include "Slotv1.hxx"
#include "ProcessingConfigPresentation.hxx"

class QGroupBox;
class QLabel;

namespace NetworkGUI
{

class ActionButton;

class Qt_ProcessingConfigPresentation : public QDialog, public ProcessingConfigPresentation
{
protected:
	QGroupBox* mGroupBox;
//	QPushButton* mOkButton;
//	QPushButton* mCancelButton;
	ActionButton * mOkButton;
	ActionButton * mCancelButton;

	virtual void Ok( bool );
	virtual void Cancel( bool );
	virtual void ApplyChangesToConfig() = 0;
	virtual void OnNewConfig( CLAM::ProcessingConfig* ) = 0;
public:
	Qt_ProcessingConfigPresentation( QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ProcessingConfigPresentation();
	virtual void Show();
	virtual void Hide();

public: // slots
	SigSlot::Slotv1< bool > mOkButtonIsPressed;
	SigSlot::Slotv1< bool > mCancelButtonIsPressed;
};

} // namespace NetworkGUI

#endif // __QT_PROCESSINGCONFIGPRESENTATION_HXX__
