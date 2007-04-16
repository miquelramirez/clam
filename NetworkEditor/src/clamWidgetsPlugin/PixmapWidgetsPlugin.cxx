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

#include "PixmapWidgetsPlugin.hxx"
#include "PixmapWidgets.hxx"

#include <QtCore/QtPlugin>

PixmapSliderPlugin::PixmapSliderPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void PixmapSliderPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool PixmapSliderPlugin::isInitialized() const
{
    return initialized;
}

QWidget *PixmapSliderPlugin::createWidget(QWidget *parent)
{
    return new QPixmapRange(":/pkwidgets/thinMixerSlider_",parent);
}

QString PixmapSliderPlugin::name() const
{
    return "QPixmapRange";
}

QString PixmapSliderPlugin::group() const
{
    return "PK Widgets";
}

QIcon PixmapSliderPlugin::icon() const
{
    return QIcon();
//    return QIcon(":/icons/images/keyspace.svg");
}

QString PixmapSliderPlugin::toolTip() const
{
    return "";
}

QString PixmapSliderPlugin::whatsThis() const
{
    return "";
}

bool PixmapSliderPlugin::isContainer() const
{
    return false;
}

QString PixmapSliderPlugin::domXml() const
{
    return "<widget class=\"QPixmapRange\" name=\"slider\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>30</width>\n"
           "   <height>170</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString PixmapSliderPlugin::includeFile() const
{
    return "PixmapWidgets.hxx";
}

// ThinPixmapButtonPlugin

ThinPixmapButtonPlugin::ThinPixmapButtonPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void ThinPixmapButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool ThinPixmapButtonPlugin::isInitialized() const
{
    return initialized;
}

QWidget *ThinPixmapButtonPlugin::createWidget(QWidget *parent)
{
    return new QPixmapButton(parent);
}

QString ThinPixmapButtonPlugin::name() const
{
    return "QPixmapButton";
}

QString ThinPixmapButtonPlugin::group() const
{
    return "PK Widgets";
}

QIcon ThinPixmapButtonPlugin::icon() const
{
    return QIcon();
//    return QIcon(":/icons/images/keyspace.svg");
}

QString ThinPixmapButtonPlugin::toolTip() const
{
    return "";
}

QString ThinPixmapButtonPlugin::whatsThis() const
{
    return "";
}

bool ThinPixmapButtonPlugin::isContainer() const
{
    return false;
}

QString ThinPixmapButtonPlugin::domXml() const
{
    return "<widget class=\"QPixmapButton\" name=\"button\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>50</width>\n"
           "   <height>32</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString ThinPixmapButtonPlugin::includeFile() const
{
    return "PixmapWidgets.hxx";
}

