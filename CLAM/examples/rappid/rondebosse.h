/***************************************************************************
 *                           rondebosse.h
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

#ifndef RONDEBOSSE_H
#define RONDEBOSSE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapp.h>
#include <qwidget.h>
#include <mainwindow.h>

#include "Rappid.hxx"

/** RondeBosse is the base class of the project */
class RondeBosse : public MainWindow
{
  Q_OBJECT 
  public:
    /** construtor */
    RondeBosse(QWidget* parent=0, const char *name=0);
    /** destructor */
    ~RondeBosse();

    void UpdateSliderValues();

public slots:
    virtual void OnbtnExitClick();
    virtual void OnbtnFileOpenClick();
    virtual void OnbtnFileOpenLeftClick();
    virtual void OnbtnFileSaveOutputClick();
    virtual void OnbtnFileOpenRightClick();
    virtual void OnbtnFileSaveClick();
    virtual void OnbtnHelpClick();
    virtual void OnbtnRecallClick();
    virtual void OnbtnStartClick();
    virtual void OnbtnStopClick();
    virtual void OnbtnStoreClick();
    virtual void OnchbRealTimeClick();
    virtual void OnchbWaveFileClick();
    virtual void OnrbRealTimeLeftClick();
    virtual void OnrbRealTimeRightClick();
    virtual void OnrbSynthLeftClick();
    virtual void OnrbSynthRightClick();
    virtual void OnrbWaveFileLeftClick();
    virtual void OnrbWaveFileRightClick();
    virtual void OnsldConfigMove();
    virtual void OnsldControlMove();
    virtual void OnsldLeft1Move();
    virtual void OnsldLeft2Move();
    virtual void OnsldLeft3Move();
    virtual void OnsldLeft4Move();
    virtual void OnsldRight1Move();
    virtual void OnsldRight2Move();
    virtual void OnsldRight3Move();
    virtual void OnsldRight4Move();


  private:
    void FileOpen();  // Metodos para abrir y salvar archivos
    void FileSave();  // de configuracion
};

extern Rappid *papp;

#endif
