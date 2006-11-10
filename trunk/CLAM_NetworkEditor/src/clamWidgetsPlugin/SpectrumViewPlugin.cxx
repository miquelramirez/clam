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

#include "SpectrumViewPlugin.hxx"
#include "SpectrumView.hxx"

#include <QtPlugin>

SpectrumViewPlugin::SpectrumViewPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void SpectrumViewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool SpectrumViewPlugin::isInitialized() const
{
    return initialized;
}

QWidget *SpectrumViewPlugin::createWidget(QWidget *parent)
{
    return new SpectrumView(parent);
}

QString SpectrumViewPlugin::name() const
{
    return "SpectrumView";
}

QString SpectrumViewPlugin::group() const
{
    return "CLAM Widgets";
}

QIcon SpectrumViewPlugin::icon() const
{
    return QIcon();
}

QString SpectrumViewPlugin::toolTip() const
{
    return "";
}

QString SpectrumViewPlugin::whatsThis() const
{
    return "";
}

bool SpectrumViewPlugin::isContainer() const
{
    return false;
}

QString SpectrumViewPlugin::domXml() const
{
    return "<widget class=\"SpectrumView\" name=\"spectrumView\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString SpectrumViewPlugin::includeFile() const
{
    return "SpectrumView.hxx";
}

// This is just for a singleton widget plugin
// Q_EXPORT_PLUGIN2(clamwidgets, SpectrumViewPlugin)

