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

#include "QSynthKnobPlugin.hxx"
#include "QSynthKnob.hxx"

#include <QtCore/QtPlugin>

QSynthKnobPlugin::QSynthKnobPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QSynthKnobPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QSynthKnobPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QSynthKnobPlugin::createWidget(QWidget *parent)
{
    return new QSynthKnob(parent);
}

QString QSynthKnobPlugin::name() const
{
    return "QSynthKnob";
}

QString QSynthKnobPlugin::group() const
{
    return "CLAM Widgets";
}

QIcon QSynthKnobPlugin::icon() const
{
//    return QIcon(":/icons/images/keyspace.svg");
    return QIcon();
}

QString QSynthKnobPlugin::toolTip() const
{
    return "";
}

QString QSynthKnobPlugin::whatsThis() const
{
    return "";
}

bool QSynthKnobPlugin::isContainer() const
{
    return false;
}

QString QSynthKnobPlugin::domXml() const
{
    return "<widget class=\"QSynthKnob\" name=\"knob\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>70</width>\n"
           "   <height>70</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString QSynthKnobPlugin::includeFile() const
{
    return "QSynthKnob.hxx";
}

// This is just for a singleton widget plugin
// Q_EXPORT_PLUGIN2(clamwidgets, QSynthKnobPlugin)

