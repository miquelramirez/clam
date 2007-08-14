/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "SpectrogramPlugin.hxx"
#include "Spectrogram.hxx"

#include <QtCore/QtPlugin>

SpectrogramPlugin::SpectrogramPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void SpectrogramPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool SpectrogramPlugin::isInitialized() const
{
    return initialized;
}

QWidget *SpectrogramPlugin::createWidget(QWidget *parent)
{
    return new CLAM::VM::Spectrogram(parent);
}

QString SpectrogramPlugin::name() const
{
    return "CLAM::VM::Spectrogram";
}

QString SpectrogramPlugin::group() const
{
    return "CLAM Widgets";
}

QIcon SpectrogramPlugin::icon() const
{
    return QIcon(":/icons/images/spectrogram.svg");
}

QString SpectrogramPlugin::toolTip() const
{
    return "";
}

QString SpectrogramPlugin::whatsThis() const
{
    return "";
}

bool SpectrogramPlugin::isContainer() const
{
    return false;
}

QString SpectrogramPlugin::domXml() const
{
    return "<widget class=\"CLAM::VM::Spectrogram\" name=\"Spectrogram\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>200</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString SpectrogramPlugin::includeFile() const
{
    return "Spectrogram.hxx";
}

// This is just for a singleton widget plugin
// Q_EXPORT_PLUGIN2(clamwidgets, SpectrogramPlugin)

