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

#ifndef __LadspaLoaderConfigPresentation_hxx__
#define __LadspaLoaderConfigPresentation_hxx__

#include "Qt_ProcessingConfigPresentation.hxx"
#include <CLAM/LadspaLoader.hxx>

class QLineEdit;
class QComboBox;

namespace CLAM
{
	class ProcessingConfig;
}

namespace NetworkGUI
{

class LadspaLoaderConfigPresentation: public Qt_ProcessingConfigPresentation
{
	Q_OBJECT
protected:
	CLAM::LadspaLoaderConfig mConfig;
	QVBox * mLayout;

	QLineEdit * mLocation;
	QComboBox * mPluginsList;
	QLineEdit * mSampleRate;
	QLineEdit * mFrameSize;
	
	void SetConfig( const CLAM::ProcessingConfig & cfg);
	void ConfigureProcessing();
	void CreateGUI();
	void CreateLocation();
	void CreatePluginsList();
	void CreateSampleRate();
	void CreateFrameSize();
public:
	LadspaLoaderConfigPresentation( QWidget * parent = 0 );
	virtual ~LadspaLoaderConfigPresentation();
	CLAM::ProcessingConfig * GetConfig()
	{
		return &mConfig;
	}
protected slots:
	void UpdatePluginsList( const QString & );
};

} // namespace NetworkGUI

#endif  // ___LadspaLoaderConfigPresentation_hxx_

