/*
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
 */

#include "Turnaround.hxx"

#include "TurnaroundVersion.hxx"

#include <QtGui/QFileDialog>

Turnaround::Turnaround(const std::string & nameProject = "")
	: QMainWindow( 0 )
	, Ui::Turnaround( )
{
	setupUi(this);
	
	connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileExitAction, SIGNAL(triggered()), this, SLOT(close()));
}

Turnaround::~Turnaround()
{
}

void Turnaround::fileOpen()
{
	QString qFileName = QFileDialog::getOpenFileName(this, 
			tr("Choose an audio file"), QString::null, 
			   tr("Audio files (*.wav *.ogg *.mp3)"));
	if(qFileName == QString::null) return;
	loadAudioFile(qFileName.toStdString());
}

void Turnaround::loadAudioFile(const std::string & fileName)
{
	// TODO	
}
