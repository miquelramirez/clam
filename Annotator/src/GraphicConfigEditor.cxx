/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "GraphicConfigEditor.hxx"

GraphicConfigEditor::~GraphicConfigEditor()
{
}

void GraphicConfigEditor::setConfiguration(std::string config)
{
	mConfig=config;
	mAggregationEditor->loadConfig(config);
	mAggregationEditor->setSchema();
}

std::string GraphicConfigEditor::getConfiguration()
{
	return mConfig;
}

void GraphicConfigEditor::on_okButton_clicked()
{
	mAggregationEditor->setConfiguration();
	mConfig=mAggregationEditor->outputConfig(); //Does this work? will the mConfig be deleted right now?
	this->accept();
}

void GraphicConfigEditor::on_cancelButton_clicked()
{
	this->reject();
}