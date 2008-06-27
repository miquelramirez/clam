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

#include "TonnetzPlugin.hxx"
#include "Tonnetz.hxx"

#include <QtCore/QtPlugin>
#include <iostream>

TonnetzPlugin::TonnetzPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void TonnetzPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	std::cout << "initializing Tonnetz" << std::endl;
    if (initialized)
        return;

    initialized = true;
}

bool TonnetzPlugin::isInitialized() const
{
    return initialized;
}

QWidget *TonnetzPlugin::createWidget(QWidget *parent)
{
    return new CLAM::VM::Tonnetz(parent);
}

QString TonnetzPlugin::name() const
{
    return "CLAM::VM::Tonnetz";
}

QString TonnetzPlugin::group() const
{
    return "CLAM Widgets";
}

QIcon TonnetzPlugin::icon() const
{
    return QIcon(":/icons/images/tonnetz.svg");
}

QString TonnetzPlugin::toolTip() const
{
    return "";
}

QString TonnetzPlugin::whatsThis() const
{
    return "";
}

bool TonnetzPlugin::isContainer() const
{
    return false;
}

QString TonnetzPlugin::domXml() const
{
    return "<widget class=\"CLAM::VM::Tonnetz\" name=\"tonnetz\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>300</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString TonnetzPlugin::includeFile() const
{
    return "Tonnetz.hxx";
}

// This is just for a singleton widget plugin
// Q_EXPORT_PLUGIN2(clamwidgets, TonnetzPlugin)

