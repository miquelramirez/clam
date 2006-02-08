# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUI.ui'
#
# Created: Tue Feb 7 18:09:56 2006
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
        self.setMinimumSize(QSize(470,670))
        self.setMaximumSize(QSize(470,660))
        f = QFont(self.font())
        self.setFont(f)


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
        logEdit_font = QFont(self.logEdit.font())
        logEdit_font.setFamily("FreeMono")
        logEdit_font.setPointSize(9)
        self.logEdit.setFont(logEdit_font)
        self.logEdit.setHScrollBarMode(QTextEdit.Auto)
        self.logEdit.setTextFormat(QTextEdit.PlainText)
        self.logEdit.setWordWrap(QTextEdit.WidgetWidth)
        self.logEdit.setWrapPolicy(QTextEdit.Anywhere)
        layout4.addWidget(self.logEdit)

        self.exitButton = QPushButton(self,"exitButton")
        self.exitButton.setGeometry(QRect(330,620,120,30))

        self.groupBox1 = QGroupBox(self,"groupBox1")
        self.groupBox1.setGeometry(QRect(10,600,270,60))

        self.uploadButton = QPushButton(self.groupBox1,"uploadButton")
        self.uploadButton.setGeometry(QRect(140,20,120,30))

        self.goButton = QPushButton(self.groupBox1,"goButton")
        self.goButton.setGeometry(QRect(10,20,120,30))

        self.languageChange()

        self.resize(QSize(470,670).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.exitButton,SIGNAL("clicked()"),self.close)

        self.setTabOrder(self.projectEdit,self.taskEdit)
        self.setTabOrder(self.taskEdit,self.taskButton)
        self.setTabOrder(self.taskButton,self.goButton)
        self.setTabOrder(self.goButton,self.uploadButton)
        self.setTabOrder(self.uploadButton,self.logEdit)


    def languageChange(self):
        self.setCaption(self.__tr("Annotator Task Handler"))
        self.projectLabel.setText(self.__tr("Project Name"))
        self.projectEdit.setText(QString.null)
        self.taskLabel.setText(self.__tr("Task file"))
        self.taskEdit.setText(QString.null)
        self.taskButton.setText(self.__tr("Select file"))
        self.logEdit.setText(self.__tr("Press Go! when ready"))
        self.exitButton.setText(self.__tr("Exit"))
        self.groupBox1.setTitle(self.__tr("Task processing"))
        self.uploadButton.setText(self.__tr("Upload!"))
        self.goButton.setText(self.__tr("GO!"))


    def __tr(self,s,c = None):
        return qApp.translate("GUI",s,c)
