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

#ifndef AggregationEditor_hxx
#define AggregationEditor_hxx

#include "Schema.hxx"
#include "SchemaBrowser.hxx"
#include "Project.hxx"
#include <QtCore/QVariant>
#include <QtGui/QPixmap>
#include <QtGui/QTreeWidget>

#include <string>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSplitter;
class QTreeWidget;
class QFrame;
class QSpinBox;
class QLabel;
class QTextBrowser;
class QLineEdit;
class QTreeWidgetItem;
namespace CLAM_Annotator { class Schema; }

class AggregationEditor : public QTreeWidget
{
    Q_OBJECT
	CLAM_Annotator::Project * _project;
public:
	AggregationEditor(QWidget * parent = 0);
	~AggregationEditor();
	void takeMaps(); //< update the project with maps
	void bindProject(CLAM_Annotator::Project & project); //< Sets the work project and reloads the data in the interface
	void reloadProject(); //< update the maps with the ones in the project
	void loadConfig(std::string config);
	std::string outputConfig();

	

protected:
    QHBoxLayout* aggregationEditorLayout;
    //QGridLayout* attributePropertiesLayout;

protected slots:
	void editConfiguration();
	

private:
	void addSource(const std::string & source, const CLAM_Annotator::Schema & schema);
	void addAttribute(const std::string & scope, const std::string & name, QTreeWidgetItem* parent);

	void parseSources(const std::string & config);
	void parseMap(const std::string & config);
	void renameTarget(QTreeWidgetItem * current);
	QTreeWidgetItem *  hasScope(const std::string & scope,  QTreeWidgetItem * parent);
	std::string parseQuotationMark(const std::string & config, size_t beginPos, size_t limitedPos, std::string keyWord );

private:
	QPixmap sourceIcon;
	QPixmap scopeIcon;
	QPixmap attributeIcon;
		
	bool _reloading;
};

#endif

