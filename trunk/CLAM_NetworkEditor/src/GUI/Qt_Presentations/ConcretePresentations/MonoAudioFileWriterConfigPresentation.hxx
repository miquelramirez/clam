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

#ifndef __MonoAudioFileWriterConfigPresentation_hxx__
#define __MonoAudioFileWriterConfigPresentation_hxx__

#include "MonoAudioFileWriterConfig.hxx"
#include "Qt_ProcessingConfigPresentation.hxx"

class QLineEdit;
class QComboBox;

namespace CLAM
{
	class ProcessingConfig;
}

namespace NetworkGUI
{

class MonoAudioFileWriterConfigPresentation : public Qt_ProcessingConfigPresentation
{
protected:
	CLAM::MonoAudioFileWriterConfig mConfig;
	QVBox * mLayout;

	QLineEdit * mLocation;
	QLineEdit * mSampleRate;
	QComboBox * mFormat;

	void SetConfig( const CLAM::ProcessingConfig & cfg);
	virtual void ConfigureProcessing();
	virtual void CreateGUI();
	void CreateLocation();
	void CreateSampleRate();
	void CreateFormat();
public:
	MonoAudioFileWriterConfigPresentation( QWidget * parent = 0 );
	virtual ~MonoAudioFileWriterConfigPresentation();
};

} // namespace NetworkGUI

#endif  // __MonoAudioFileWriterConfigPresentation_hxx__

