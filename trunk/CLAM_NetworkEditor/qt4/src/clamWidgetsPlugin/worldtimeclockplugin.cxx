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

#include "worldtimeclock.hxx"
#include "worldtimeclockplugin.hxx"

#include <QtPlugin>

WorldTimeClockPlugin2::WorldTimeClockPlugin2(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void WorldTimeClockPlugin2::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool WorldTimeClockPlugin2::isInitialized() const
{
    return initialized;
}

QWidget *WorldTimeClockPlugin2::createWidget(QWidget *parent)
{
    return new WorldTimeClock2(parent);
}

QString WorldTimeClockPlugin2::name() const
{
    return "WorldTimeClock2";
}

QString WorldTimeClockPlugin2::group() const
{
    return "CLAM";
}

QIcon WorldTimeClockPlugin2::icon() const
{
    return QIcon();
}

QString WorldTimeClockPlugin2::toolTip() const
{
    return "";
}

QString WorldTimeClockPlugin2::whatsThis() const
{
    return "";
}

bool WorldTimeClockPlugin2::isContainer() const
{
    return false;
}

QString WorldTimeClockPlugin2::domXml() const
{
    return "<widget class=\"WorldTimeClock2\" name=\"worldTimeClock\">\n"
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

QString WorldTimeClockPlugin2::includeFile() const
{
    return "worldtimeclock.h";
}

// This is just for a singleton widget plugin
//Q_EXPORT_PLUGIN2(clamwidgetsplugin, WorldTimeClockPlugin2)

