/***************************************************************************
 *                          rondebosse.cpp
 *
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

#include <qlcdnumber.h>
#include <qslider.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <kled.h>
#include <qcheckbox.h>

//#include <kfiledialog.h>
#include <qfiledialog.h>

#include "rondebosse.h"

Rappid *papp = 0;

RondeBosse::RondeBosse(QWidget *parent, const char *name) : MainWindow(parent, name)
{
}

RondeBosse::~RondeBosse()
{
}

void RondeBosse::OnbtnExitClick()
{
  qApp->quit();
}

void RondeBosse::OnbtnStartClick()
{
  if (papp->StartRunning())
  {
	  tab_2->setDisabled( TRUE );
	  btnStart->setDisabled( TRUE );
	  btnHelp->setDisabled( TRUE );
	  btnExit->setDisabled( TRUE );
	  btnFileOpen->setDisabled( TRUE );
	  btnFileSave->setDisabled( TRUE );
	  StatusLabel->setText("Processing...");
  }
  else
  {
	  StatusLabel->setText("Failed to start...");
  }

}

void RondeBosse::OnbtnStopClick()
{
  tab_2->setDisabled( FALSE );
  btnStart->setDisabled( FALSE );
  btnExit->setDisabled( FALSE );
  btnHelp->setDisabled( FALSE );
  btnFileOpen->setDisabled( FALSE );
  btnFileSave->setDisabled( FALSE );

  StatusLabel->setText("Stoped...");

  papp->StopRunning();
}

void RondeBosse::OnbtnFileOpenClick()
{
  FileOpen();
}

void RondeBosse::FileOpen()
{
  static QString FileName = "/home";

  QString s = QFileDialog::getOpenFileName(
                  FileName,
                  "Images (*.png *.xpm *.jpg)",
                  this,
                  "open file dialog",
                  "Open configuration file" );

  if(!s.isEmpty())
  {
    FileName = s;
  }

  //KURL url = KFileDialog::getOpenURL(QString::null,
  //    i18n("*|All files"), this, i18n("Open File..."));

  /*if(!url.isEmpty())
  {
    openDocumentFile(url);
    fileOpenRecent->addURL( url );
  }

  slotStatusMsg(i18n("Ready."));*/
}

void RondeBosse::OnbtnFileSaveClick()
{
  FileSave();
}

void RondeBosse::FileSave()
{
  static QString FileName = "/home";

  QString s = QFileDialog::getSaveFileName(
                  FileName,
                  "Images (*.png *.xpm *.jpg)",
                  this,
                  "save file dialog"
                  "Save configuration file" );

  if(!s.isEmpty())
  {
    FileName = s;
  }
}

void RondeBosse::OnbtnRecallClick()
{
  StatusLabel->setText("Configuration recalled...");
  LedConfig->setState( KLed::Off );
}

void RondeBosse::OnbtnStoreClick()
{
  StatusLabel->setText("Configuration stored...");
  LedConfig->setState( KLed::Off );
}

void RondeBosse::OnsldConfigMove()
{
  int i;
  i = 99 - sldConfig->value();
  LCDConfig->display(i);
  LedConfig->setState( KLed::On );
}

void RondeBosse::OnsldControlMove()
{
  int i;
  i = sldControl->value();
  ProgressBarLeft->setProgress( 100 - i );
  ProgressBarRight->setProgress( i );

  OnsldLeft3Move();
  OnsldRight3Move();
}

void RondeBosse::OnsldLeft1Move()
{
  //Interpolation Period
  int i;
  CLAM::TControlData NewValue;

  i = sldLeft1->value();
  LCDLeft1->display(i);

  NewValue = TControlData(i) / 100.0;
  papp->Control.EnqueueControl(Rappid::eEM1InterpolationPeriod,NewValue);
}

void RondeBosse::OnsldLeft2Move()
{
  // cIntegrationLength
  int i;
  CLAM::TControlData NewValue;

  i = sldLeft2->value();
  LCDLeft2->display(i);

  NewValue = TControlData(i) / 100.0;
  papp->Control.EnqueueControl(Rappid::eEM1IntegrationLength,NewValue);
}

void RondeBosse::OnsldLeft3Move()
{
  int i;
  CLAM::TControlData NewValue;

  i = sldLeft3->value();
  LCDLeft3->display(i);

  NewValue = (TControlData(100 - i) / 100.0) * (ProgressBarLeft->progress()/100.0);
  papp->Control.EnqueueControl(Rappid::eEM1NormalLevel,NewValue);
}

void RondeBosse::OnsldLeft4Move()
{
  int i;
  CLAM::TControlData NewValue;

  i = sldLeft4->value();
  LCDLeft4->display(i);

  NewValue = TControlData(i) / 100.0;
  papp->Control.EnqueueControl(Rappid::eEM1SilenceLevel,NewValue);
}

void RondeBosse::OnsldRight1Move()
{
  //Interpolation Period
  int i;
  CLAM::TControlData NewValue;

  i = sldRight1->value();
  LCDRight1->display(i);

  NewValue = TControlData(i) / 100.0;
  papp->Control.EnqueueControl(Rappid::eEM2InterpolationPeriod,NewValue);
}

void RondeBosse::OnsldRight2Move()
{
  // cIntegrationLength
  int i;
  CLAM::TControlData NewValue;

  i = sldRight2->value();
  LCDRight2->display(i);

  NewValue = TControlData(i) / 100.0;
  papp->Control.EnqueueControl(Rappid::eEM2IntegrationLength,NewValue);
}

void RondeBosse::OnsldRight3Move()
{
  int i;
  CLAM::TControlData NewValue;

  i = sldRight3->value();
  LCDRight3->display(i);

  NewValue = (TControlData(100 - i) / 100.0) * (ProgressBarRight->progress()/100.0);
  papp->Control.EnqueueControl(Rappid::eEM2NormalLevel,NewValue);
}

void RondeBosse::OnsldRight4Move()
{
  int i;
  CLAM::TControlData NewValue;

  i = sldRight4->value();
  LCDRight4->display(i);

  NewValue = TControlData(i) / 100.0;
  papp->Control.EnqueueControl(Rappid::eEM2SilenceLevel,NewValue);
}


// Harp file
void RondeBosse::OnbtnFileOpenLeftClick()
{
  static QString LeftWaveFile = "/home";

  QString s = QFileDialog::getOpenFileName(
              LeftWaveFile,
              "Waves (*.wav *.au)",
              this,
              "open file dialog",
              "Open wave file" );

  if(!s.isEmpty())
  {
    RappidConfig cfg = papp->GetConfiguration();
    cfg.GetHarpIOConfig().SetFileName(s.latin1());
    papp->Configure(cfg);
    UpdateSliderValues();
    LeftWaveFile = s;
  }
}

// Viola file
void RondeBosse::OnbtnFileOpenRightClick()
{
  static QString RightWaveFile = "/home";

  QString s = QFileDialog::getOpenFileName(
              RightWaveFile,
              "Waves (*.wav *.au)",
              this,
              "open file dialog",
              "Open wave file" );

  if(!s.isEmpty())
  {
    RappidConfig cfg = papp->GetConfiguration();
    cfg.GetViolaIOConfig().SetFileName(s.latin1());
    papp->Configure(cfg);
    UpdateSliderValues();
    RightWaveFile = s;
  }
}

// Synthesizer file
void RondeBosse::OnbtnFileSaveOutputClick()
{
  static QString SaveWaveFile = "/home";

  QString s = QFileDialog::getSaveFileName(
              SaveWaveFile,
              "Waves (*.wav *.au)",
              this,
              "Save file dialog",
              "Save output wave file as..." );

  if(!s.isEmpty())
  {
    RappidConfig cfg = papp->GetConfiguration();
    cfg.GetSynthLeftIOConfig().SetFileName(s.latin1());
    papp->Configure(cfg);
    UpdateSliderValues();
    SaveWaveFile = s;
  }
}

// Harp set to real time
void RondeBosse::OnrbRealTimeLeftClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  cfg.GetHarpIOConfig().SetIOType(ERappidIOType::eRealTime);
  papp->Configure(cfg);
  UpdateSliderValues();
}

// Viola set to real time
void RondeBosse::OnrbRealTimeRightClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  cfg.GetViolaIOConfig().SetIOType(ERappidIOType::eRealTime);
  papp->Configure(cfg);
  UpdateSliderValues();
}

// Harp set to debug waveform.
void RondeBosse::OnrbSynthLeftClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  cfg.GetHarpIOConfig().SetIOType(ERappidIOType::eWaveform);
  papp->Configure(cfg);
  UpdateSliderValues();
}

// Viola set to debug waveform.
void RondeBosse::OnrbSynthRightClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  cfg.GetViolaIOConfig().SetIOType(ERappidIOType::eWaveform);
  papp->Configure(cfg);
  UpdateSliderValues();
}

// Harp set to use file
void RondeBosse::OnrbWaveFileLeftClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  cfg.GetHarpIOConfig().SetIOType(ERappidIOType::eFile);
  papp->Configure(cfg);
  UpdateSliderValues();
}

// Viola set to use file
void RondeBosse::OnrbWaveFileRightClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  cfg.GetViolaIOConfig().SetIOType(ERappidIOType::eFile);
  papp->Configure(cfg);
  UpdateSliderValues();
}

void RondeBosse::OnbtnHelpClick()
{
  //Actualizar
}

// Output in real time
void RondeBosse::OnchbRealTimeClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  ERappidIOType type = cfg.GetSynthLeftIOConfig().GetIOType();

  if (chbRealTime->isChecked())
  {
    type =  ERappidIOType::eRealTimeAndFile;
  }
  else
  {
    type = ERappidIOType::eFile;
    chbWaveFile->setChecked(TRUE);
  }

  cfg.GetSynthLeftIOConfig().SetIOType(type);
  papp->Configure(cfg);
  UpdateSliderValues();
}

// Output to file
void RondeBosse::OnchbWaveFileClick()
{
  RappidConfig cfg = papp->GetConfiguration();
  ERappidIOType type = cfg.GetSynthLeftIOConfig().GetIOType();

  if (chbWaveFile->isChecked())
  {
    type =  ERappidIOType::eRealTimeAndFile;
  }
  else
  {
    type = ERappidIOType::eRealTime;
    chbRealTime->setChecked(TRUE);
  }

  cfg.GetSynthLeftIOConfig().SetIOType(type);
  papp->Configure(cfg);
  UpdateSliderValues();
}


void RondeBosse::UpdateSliderValues()

{
	TControlData val = 0.0;

	//Interpolation Period EM1
	val = papp->Control.LastDequeuedValue(Rappid::eEM1InterpolationPeriod);
	sldLeft1->setValue(int(val*100.0));

	// cIntegrationLength EM1
	val = papp->Control.LastDequeuedValue(Rappid::eEM1IntegrationLength);
	sldLeft2->setValue(int(val*100.0));

	//NormalLevel EM1
	val = papp->Control.LastDequeuedValue(Rappid::eEM1NormalLevel);
	sldLeft3->setValue(100-int(val*100.0));

	// SilenceLevel EM1
	val = papp->Control.LastDequeuedValue(Rappid::eEM1SilenceLevel);
	sldLeft4->setValue(int(val*100.0));


	//Interpolation Period EM2
	val = papp->Control.LastDequeuedValue(Rappid::eEM2InterpolationPeriod);
	sldRight1->setValue(int(val*100.0));

	// cIntegrationLength EM2
	val = papp->Control.LastDequeuedValue(Rappid::eEM2IntegrationLength);
	sldRight2->setValue(int(val*100.0));

	//NormalLevel EM2
	val = papp->Control.LastDequeuedValue(Rappid::eEM2NormalLevel);
	sldRight3->setValue(100-int(val*100.0));

	// SilenceLevel EM2
	val = papp->Control.LastDequeuedValue(Rappid::eEM2SilenceLevel);
	sldRight4->setValue(int(val*100.0));

}
