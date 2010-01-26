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

#include "ProgressControlWidgetPlugin.hxx"
#include "ProgressControlWidget.hxx"

#include <QtCore/QtPlugin>

ProgressControlWidgetPlugin::ProgressControlWidgetPlugin(QObject *parent)
	: QObject(parent)
{
	initialized = false;
}

void ProgressControlWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (initialized)
	return;

	initialized = true;
}

bool ProgressControlWidgetPlugin::isInitialized() const
{
	return initialized;
}

QWidget *ProgressControlWidgetPlugin::createWidget(QWidget *parent)
{
	return new ProgressControlWidget(0, parent);
}

QString ProgressControlWidgetPlugin::name() const
{
	return "ProgressControlWidget";
}

QString ProgressControlWidgetPlugin::group() const
{
	return "CLAM Widgets";
}

QIcon ProgressControlWidgetPlugin::icon() const
{
	return QIcon(":/icons/images/progresscontrol.svg");
}

QString ProgressControlWidgetPlugin::toolTip() const
{
	return "";
}

QString ProgressControlWidgetPlugin::whatsThis() const
{
	return "";
}

bool ProgressControlWidgetPlugin::isContainer() const
{
	return false;
}

QString ProgressControlWidgetPlugin::domXml() const
{
	return "<widget class=\"ProgressControlWidget\" name=\"progressControl\">\n"
	       " <property name=\"geometry\">\n"
	       "  <rect>\n"
	       "   <x>0</x>\n"
	       "   <y>0</y>\n"
	       "   <width>100</width>\n"
	       "   <height>20</height>\n"
	       "  </rect>\n"
	       " </property>\n"
	       "</widget>\n";
}

QString ProgressControlWidgetPlugin::includeFile() const
{
    return "ProgressControlWidget.hxx";
}

// This is just for a singleton widget plugin
//Q_EXPORT_PLUGIN2(clamwidgets, ProgressControlWidgetPlugin)
