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
#include <qvbox.h>
#include "Signalv1.hxx"
#include "ProcessingConfigPresentation.hxx"

class QGroupBox;
class QLabel;

namespace CLAM
{
	class ProcessingConfig;
}

namespace NetworkGUI
{

class Qt_ProcessingConfigPresentation : public QDialog, public ProcessingConfigPresentation
{
	Q_OBJECT
protected:
	QVBox * mAttributeContainer;

	virtual void ConfigureProcessing() = 0;
	virtual void SetConfig( const CLAM::ProcessingConfig & ) = 0;
	virtual CLAM::ProcessingConfig * GetConfig() = 0;
public:
	Qt_ProcessingConfigPresentation( QWidget *parent = 0, const char *name = 0);
	void Show();
	void Hide();

public slots:
	void SlotOk();
	void SlotCancel();
public: // signals
	SigSlot::Signalv1<bool> SignalConfigurationUpdated;
};

} // namespace NetworkGUI

#endif // __QT_PROCESSINGCONFIGPRESENTATION_HXX__
