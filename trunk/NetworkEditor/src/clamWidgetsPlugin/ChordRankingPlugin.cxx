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

#include "ChordRankingPlugin.hxx"
#include "ChordRanking.hxx"

#include <QtCore/QtPlugin>

ChordRankingPlugin::ChordRankingPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void ChordRankingPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool ChordRankingPlugin::isInitialized() const
{
    return initialized;
}

QWidget *ChordRankingPlugin::createWidget(QWidget *parent)
{
    return new CLAM::VM::ChordRanking(parent);
}

QString ChordRankingPlugin::name() const
{
    return "CLAM::VM::ChordRanking";
}

QString ChordRankingPlugin::group() const
{
    return "CLAM Widgets";
}

QIcon ChordRankingPlugin::icon() const
{
    return QIcon(":/icons/images/chordranking.svg");
}

QString ChordRankingPlugin::toolTip() const
{
    return "";
}

QString ChordRankingPlugin::whatsThis() const
{
    return "";
}

bool ChordRankingPlugin::isContainer() const
{
    return false;
}

QString ChordRankingPlugin::domXml() const
{
    return "<widget class=\"CLAM::VM::ChordRanking\" name=\"chordRanking\">\n"
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

QString ChordRankingPlugin::includeFile() const
{
    return "ChordRanking.hxx";
}

// This is just for a singleton widget plugin
// Q_EXPORT_PLUGIN2(clamwidgets, ChordRankingPlugin)

