#!/usr/bin/python
# -*- coding: UTF-8 -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

import sys, os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from PyQt4 import uic
import TemplateGenerator
import TemplatedPluginsGenerator

(Ui_MainWindow, Ui_MainWindow_base) = uic.loadUiType('main.ui')
class MainWindow(Ui_MainWindow_base, Ui_MainWindow):

	class RowItemsManager():
		""" Items/rows manager for a table """

		#Table fields names
		FPORTS_INOUT, FPORTS_TYPE, FPORTS_NAME = range(3)
		FCONTROLS_INOUT, FCONTROLS_MIN, FCONTROLS_MAX, FCONTROLS_NAME = range(4)

		def __init__(self, tableWidget, rowsWidth=[100, 200, 450]):
			self.tableWidget = tableWidget

			for i in range(len(rowsWidth)):
				self.tableWidget.setColumnWidth(i, rowsWidth[i])
		#__init__

		def getCount(self):
			return self.tableWidget.rowCount()
		#getCount()

		def add(self):
			rowCount = self.tableWidget.rowCount()
			self.tableWidget.insertRow( rowCount )
			comboBox_InOut = QComboBox()
			comboBox_InOut.addItem("In")
			comboBox_InOut.addItem("Out")
			self.tableWidget.setCellWidget(rowCount, self.FPORTS_INOUT, comboBox_InOut);
		#add()

		def remove(self):
			self.tableWidget.removeRow( self.tableWidget.rowCount()-1 )
		#remove()

		def getItem(self, row, column):
			return self.tableWidget.item(row, column)
		#getItem()

		def getWidget(self, row, column):
			return self.tableWidget.cellWidget(row, column)
		#getWidget()
	#class RowItemsManager


	def __init__(self, parent=None):
		QWidget.__init__(self, parent)
		self.setupUi(self)

		self.setWindowTitle( "Processing Code Generator" )
		clam_logo_svg = "../../artwork/clamlogo.svg"
		self.setWindowIcon( QIcon(clam_logo_svg) )
		self.clamLogo.setPixmap( QPixmap(clam_logo_svg) )

		#Ports table setup:
		self.ports_manager = MainWindow.RowItemsManager( self.tableWidget_Ports, rowsWidth=[100, 200, 400] )
		self.connect( self.pushButton_Ports_Plus, SIGNAL("clicked()"), self.ports_manager.add )
		self.connect( self.pushButton_Ports_Minus, SIGNAL("clicked()"), self.ports_manager.remove )

		#Controls table setup:
		self.controls_manager = MainWindow.RowItemsManager( self.tableWidget_Controls, rowsWidth=[100, 100, 100, 400])
		self.connect( self.pushButton_Controls_Plus, SIGNAL("clicked()"), self.controls_manager.add )
		self.connect( self.pushButton_Controls_Minus, SIGNAL("clicked()"), self.controls_manager.remove )

		#Generate button:
		self.connect( self.pushButton_Generate, SIGNAL("clicked()"), self.generate )
	#__init__()

	def generate(self):
		""" Generates processing plugin base code """

		if self.lineEdit_PluginName.text()=="":
			self.statusBar.showMessage("Name required.")
			return

		definitions_dict = {}
		definitions_dict["member_style"] = "m" # Member name convention. Option: "_"
		definitions_dict["plugin_name"] = str( self.lineEdit_PluginName.text() )
		definitions_dict["template_name"] = str( self.lineEdit_PluginName.text() )
		definitions_dict["base_class_name"] = str( self.lineEdit_BaseClass.text() )
		definitions_dict["license"] = str( self.comboBox_License.currentText() )
		definitions_dict["copyright_holder"] = str( self.lineEdit_Copyright.text() )
		definitions_dict["year"] = str( self.lineEdit_Year.text() )

		definitions_dict["inputs"] = list()
		definitions_dict["outputs"] = list()
		Ports = self.ports_manager
		for index in range(Ports.getCount()):
			try:
				in_out = str( Ports.getWidget(index, Ports.FPORTS_INOUT ).currentText() )
				port_type = str( Ports.getItem(index, Ports.FPORTS_TYPE).text() )
				port_name = str( Ports.getItem(index, Ports.FPORTS_NAME).text() )
			except AttributeError:
				self.statusBar.showMessage("Fields description error.")
				return
			if in_out=="In": definitions_dict["inputs"].append( (port_type,port_name) )
			elif in_out=="Out": definitions_dict["outputs"].append( (port_type,port_name) )

		definitions_dict["incontrols"] = list()
		definitions_dict["outcontrols"] = list()
		Controls = self.controls_manager
		for index in range(Controls.getCount()):
			try:
				in_out = str( Controls.getWidget(index, Controls.FCONTROLS_INOUT ).currentText() )
				cmin = str( Controls.getItem(index, Controls.FCONTROLS_MIN).text() )
				cmax = str( Controls.getItem(index, Controls.FCONTROLS_MAX).text() )
				control_name = str( Controls.getItem(index, Controls.FCONTROLS_NAME).text() )
			except AttributeError:
				self.statusBar.showMessage("Fields description error.")
				return
			if in_out=="In": definitions_dict["incontrols"].append( (cmin,cmax,control_name) )
			elif in_out=="Out": definitions_dict["outcontrols"].append( (cmin,cmax,control_name) )

		if not os.path.isdir( "templates/" + definitions_dict["template_name"] ): os.mkdir( "templates/" + definitions_dict["template_name"] )

		print "Creating " + definitions_dict["template_name"] + " template"
		TemplateGenerator.make_base_processing_hxx_file(definitions_dict)
		TemplateGenerator.make_base_processing_cxx_file(definitions_dict)
		TemplateGenerator.make_sconstruct_file(definitions_dict)
		TemplateGenerator.make_readme_file(definitions_dict)

		print "Creating " + definitions_dict["plugin_name"] + " plugin"
		definitions_dict["output_dir"] = "../../plugins"
		definitions_dict['license_text'] = TemplatedPluginsGenerator.make_license_text(definitions_dict)
		standard_name = "BaseProcessing"
		TemplatedPluginsGenerator.make_file( definitions_dict, standard_name, standard_name+".hxx" )
		TemplatedPluginsGenerator.make_file( definitions_dict, standard_name, standard_name+".cxx" )
		TemplatedPluginsGenerator.make_file( definitions_dict, standard_name, "SConstruct", "SConstruct", True )
		TemplatedPluginsGenerator.copy_file( definitions_dict, "README" )

		print "Done"; self.statusBar.showMessage("Done")
	#generate()

#class MainWindow

def main(args): 
	app = QApplication( args )
	main_window = MainWindow()
	main_window.show()
	sys.exit( app.exec_() )

if __name__ == '__main__':
	main(sys.argv)
