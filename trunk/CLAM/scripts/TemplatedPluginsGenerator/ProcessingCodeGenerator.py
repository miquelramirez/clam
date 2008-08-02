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

	def __init__(self, parent=None):
		QWidget.__init__(self, parent)
		self.setupUi(self)

		self.setWindowTitle( "Processing Code Generator" )
		clam_logo_svg = "../../artwork/clamlogo.svg"
		self.setWindowIcon( QIcon(clam_logo_svg) )
		self.clamLogo.setPixmap( QPixmap(clam_logo_svg) )

		self.connect( self.pushButton_Generate, SIGNAL("clicked()"), self.generate )
	#__init__()

	def generate(self):
		""" Generates plugin base code """

		definitions_dict = {}
		definitions_dict["member_style"] = "_" # Member name convention. Option: "m" 
		definitions_dict["plugin_name"] = str( self.lineEdit_PluginName.text() )
		definitions_dict["template_name"] = str( self.lineEdit_PluginName.text() )
		definitions_dict["base_class_name"] = str( self.lineEdit_BaseClass.text() )
		definitions_dict["license"] = str( self.comboBox_License.currentText() )
		definitions_dict["copyright_holder"] = str( self.lineEdit_Copyright.text() )
		definitions_dict["year"] = str( self.lineEdit_Year.text() )

		definitions_dict["inputs"] = list()
		definitions_dict["outputs"] = list()
		for index in range(1,6):
			in_out = str( getattr(self, "comboBox_Port%i"%(index)).currentText() )
			port_type = str( getattr(self, "lineEdit_Port%i_Type"%(index)).text() )
			port_name = str( getattr(self, "lineEdit_Port%i_Name"%(index)).text() )
			if port_type=="Type" or port_name=="Name": continue
			if in_out=="In": definitions_dict["inputs"].append( (port_type,port_name) )
			elif in_out=="Out": definitions_dict["outputs"].append( (port_type,port_name) )

		definitions_dict["incontrols"] = list()
		definitions_dict["outcontrols"] = list()
		for index in range(1,6):
			in_out = str( getattr(self, "comboBox_Control%i"%(index)).currentText() )
			cmin = str( getattr(self, "lineEdit_Control%i_Min"%(index)).text() )
			cmax = str( getattr(self, "lineEdit_Control%i_Max"%(index)).text() )
			control_name = str( getattr(self, "lineEdit_Control%i_Name"%(index)).text() )
			if control_name=="Name" or cmin=="Min" or cmax=="Max": continue
			if in_out=="In": definitions_dict["incontrols"].append( (cmin,cmax,control_name) )
			elif in_out=="Out": definitions_dict["outcontrols"].append( (cmin,cmax,control_name) )

		if not os.path.isdir( "templates/" + definitions_dict["template_name"] ): os.mkdir( "templates/" + definitions_dict["template_name"] )

		print "Creating " + definitions_dict["template_name"] + " template"
		TemplateGenerator.make_base_processing_hxx_file(definitions_dict)
		TemplateGenerator.make_base_processing_cxx_file(definitions_dict)
		TemplateGenerator.make_sconstruct_file(definitions_dict)

		print "Creating " + definitions_dict["plugin_name"] + " plugin"
		definitions_dict["output_dir"] = "../../plugins"
		definitions_dict['license_text'] = TemplatedPluginsGenerator.make_license_text(definitions_dict)
		standard_name = "BaseProcessing"
		TemplatedPluginsGenerator.make_file( definitions_dict, standard_name, standard_name+".hxx" )
		TemplatedPluginsGenerator.make_file( definitions_dict, standard_name, standard_name+".cxx" )
		TemplatedPluginsGenerator.make_file( definitions_dict, standard_name, "SConstruct", "SConstruct", True )
		TemplatedPluginsGenerator.copy_file( definitions_dict["template_name"], "README" )
		TemplatedPluginsGenerator.copy_file( definitions_dict["template_name"], "options.cache" )

		print "Done"; self.statusBar().showMessage("Done")
	#generate()

#class MainWindow

def main(args): 
	app = QApplication( args )
	main_window = MainWindow()
	main_window.show()
	sys.exit( app.exec_() )

if __name__ == '__main__':
    main(sys.argv)


