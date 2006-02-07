# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUI.ui'
#
# Created: Tue Feb 7 12:18:03 2006
#      by: The PyQt User Interface Compiler (pyuic) 3.14.1
#
# WARNING! All changes made in this file will be lost!


from qt import *


class GUI(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("GUI")

        self.setSizePolicy(QSizePolicy(QSizePolicy.Fixed,QSizePolicy.Fixed,0,0,self.sizePolicy().hasHeightForWidth()))
        self.setMinimumSize(QSize(470,660))
        self.setMaximumSize(QSize(470,660))


        self.goButton = QPushButton(self,"goButton")
        self.goButton.setGeometry(QRect(20,600,200,30))

        self.uploadButton = QPushButton(self,"uploadButton")
        self.uploadButton.setGeometry(QRect(240,600,200,30))

        LayoutWidget = QWidget(self,"layout4")
        LayoutWidget.setGeometry(QRect(10,13,450,580))
        layout4 = QVBoxLayout(LayoutWidget,11,6,"layout4")

        layout3 = QVBoxLayout(None,0,6,"layout3")

        self.projectLabel = QLabel(LayoutWidget,"projectLabel")
        layout3.addWidget(self.projectLabel)

        self.projectEdit = QLineEdit(LayoutWidget,"projectEdit")
        layout3.addWidget(self.projectEdit)

        self.taskLabel = QLabel(LayoutWidget,"taskLabel")
        layout3.addWidget(self.taskLabel)

        layout2 = QHBoxLayout(None,0,6,"layout2")

        self.taskEdit = QLineEdit(LayoutWidget,"taskEdit")
        layout2.addWidget(self.taskEdit)

        self.taskButton = QPushButton(LayoutWidget,"taskButton")
        layout2.addWidget(self.taskButton)
        layout3.addLayout(layout2)
        layout4.addLayout(layout3)

        self.logEdit = QTextEdit(LayoutWidget,"logEdit")
        self.logEdit.setWordWrap(QTextEdit.WidgetWidth)
        layout4.addWidget(self.logEdit)

        self.languageChange()

        self.resize(QSize(470,660).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.setTabOrder(self.projectEdit,self.taskEdit)
        self.setTabOrder(self.taskEdit,self.taskButton)
        self.setTabOrder(self.taskButton,self.goButton)
        self.setTabOrder(self.goButton,self.uploadButton)
        self.setTabOrder(self.uploadButton,self.logEdit)


    def languageChange(self):
        self.setCaption(self.__tr("Annotator task handler"))
        self.goButton.setText(self.__tr("GO!"))
        self.uploadButton.setText(self.__tr("Upload!"))
        self.projectLabel.setText(self.__tr("Project Name"))
        self.projectEdit.setText(QString.null)
        self.taskLabel.setText(self.__tr("Task file"))
        self.taskEdit.setText(QString.null)
        self.taskButton.setText(self.__tr("Select file"))
        self.logEdit.setText(self.__tr("Press Go! when ready"))


    def __tr(self,s,c = None):
        return qApp.translate("GUI",s,c)
