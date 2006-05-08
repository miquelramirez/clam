# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'unknown'
#
# Created: Wed May  3 10:58:47 2006
#      by: PyQt4 UI code generator 4.0beta1
#
# WARNING! All changes made in this file will be lost!

import sys
from PyQt4 import QtCore, QtGui

class Ui_GUI(object):
    def setupUi(self, GUI):
        GUI.setObjectName("GUI")
        GUI.resize(QtCore.QSize(QtCore.QRect(0,0,600,420).size()).expandedTo(GUI.minimumSizeHint()))

        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Policy(0),QtGui.QSizePolicy.Policy(0))
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(GUI.sizePolicy().hasHeightForWidth())
        GUI.setSizePolicy(sizePolicy)
        GUI.setWindowIcon(QtGui.QIcon("../src/images/annotator-icon1.png"))

        self.statusbar = QtGui.QStatusBar(GUI)
        self.statusbar.setGeometry(QtCore.QRect(0,396,600,24))
        self.statusbar.setObjectName("statusbar")

        self.centralwidget = QtGui.QWidget(GUI)
        self.centralwidget.setGeometry(QtCore.QRect(0,0,611,401))
        self.centralwidget.setObjectName("centralwidget")

        self.projectEdit = QtGui.QLineEdit(self.centralwidget)
        self.projectEdit.setGeometry(QtCore.QRect(180,10,310,30))
        self.projectEdit.setObjectName("projectEdit")

        self.pathLabel = QtGui.QLabel(self.centralwidget)
        self.pathLabel.setGeometry(QtCore.QRect(30,90,134,30))
        self.pathLabel.setObjectName("pathLabel")

        self.taskEdit = QtGui.QLineEdit(self.centralwidget)
        self.taskEdit.setGeometry(QtCore.QRect(180,50,310,30))
        self.taskEdit.setObjectName("taskEdit")

        self.pathButton = QtGui.QPushButton(self.centralwidget)
        self.pathButton.setGeometry(QtCore.QRect(500,90,90,31))
        self.pathButton.setObjectName("pathButton")

        self.taskLabel = QtGui.QLabel(self.centralwidget)
        self.taskLabel.setGeometry(QtCore.QRect(30,50,70,30))
        self.taskLabel.setObjectName("taskLabel")

        self.pathEdit = QtGui.QLineEdit(self.centralwidget)
        self.pathEdit.setGeometry(QtCore.QRect(180,90,310,30))
        self.pathEdit.setObjectName("pathEdit")

        self.exitButton = QtGui.QPushButton(self.centralwidget)
        self.exitButton.setGeometry(QtCore.QRect(450,360,120,30))
        self.exitButton.setObjectName("exitButton")

        self.taskButton = QtGui.QPushButton(self.centralwidget)
        self.taskButton.setGeometry(QtCore.QRect(500,50,90,31))
        self.taskButton.setObjectName("taskButton")

        self.projectLabel = QtGui.QLabel(self.centralwidget)
        self.projectLabel.setGeometry(QtCore.QRect(30,10,120,30))
        self.projectLabel.setObjectName("projectLabel")

        self.logEdit = QtGui.QTextBrowser(self.centralwidget)
        self.logEdit.setGeometry(QtCore.QRect(20,130,571,221))

        font = QtGui.QFont(self.logEdit.font())
        font.setFamily("FreeMono")
        font.setPointSize(9)
        font.setWeight(50)
        font.setItalic(False)
        font.setUnderline(False)
        font.setStrikeOut(False)
        font.setBold(False)
        self.logEdit.setFont(font)
        self.logEdit.setObjectName("logEdit")

        self.goButton = QtGui.QPushButton(self.centralwidget)
        self.goButton.setGeometry(QtCore.QRect(50,360,121,31))
        self.goButton.setObjectName("goButton")

        self.retranslateUi(GUI)
        QtCore.QMetaObject.connectSlotsByName(GUI)

    def tr(self, string):
        return QtGui.QApplication.translate("GUI", string, None, QtGui.QApplication.UnicodeUTF8)

    def retranslateUi(self, GUI):
        GUI.setWindowTitle(self.tr("CLAM Annotator Simac Task Handler"))
        self.pathLabel.setText(self.tr("Path to save files"))
        self.pathButton.setText(self.tr("Choose..."))
        self.taskLabel.setText(self.tr("Task file"))
        self.exitButton.setText(self.tr("Exit"))
        self.taskButton.setText(self.tr("Choose..."))
        self.projectLabel.setText(self.tr("Project Name"))
        self.logEdit.setHtml(self.tr("<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\" white-space: pre-wrap; font-family:FreeMono; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Press Go! when ready</p></body></html>"))
        self.goButton.setText(self.tr("Go"))
