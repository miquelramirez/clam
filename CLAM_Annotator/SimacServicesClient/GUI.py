# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUIhorizontal.ui'
#
# Created: Fri Feb 10 12:43:12 2006
#      by: The PyQt User Interface Compiler (pyuic) 3.14.1
#
# WARNING! All changes made in this file will be lost!


from qt import *


class GUI(QMainWindow):
    def __init__(self,parent = None,name = None,fl = 0):
        QMainWindow.__init__(self,parent,name,fl)
        self.statusBar()

        if not name:
            self.setName("GUI")

        self.setSizePolicy(QSizePolicy(QSizePolicy.Fixed,QSizePolicy.Fixed,0,0,self.sizePolicy().hasHeightForWidth()))
        self.setMinimumSize(QSize(640,430))
        self.setMaximumSize(QSize(640,430))
        f = QFont(self.font())
        self.setFont(f)

        self.setCentralWidget(QWidget(self,"qt_central_widget"))

        self.projectLabel = QLabel(self.centralWidget(),"projectLabel")
        self.projectLabel.setGeometry(QRect(20,10,120,30))

        self.projectEdit = QLineEdit(self.centralWidget(),"projectEdit")
        self.projectEdit.setGeometry(QRect(180,10,340,30))

        self.taskLabel = QLabel(self.centralWidget(),"taskLabel")
        self.taskLabel.setGeometry(QRect(20,50,70,30))

        self.taskEdit = QLineEdit(self.centralWidget(),"taskEdit")
        self.taskEdit.setGeometry(QRect(180,50,340,30))

        self.taskButton = QPushButton(self.centralWidget(),"taskButton")
        self.taskButton.setGeometry(QRect(527,51,90,31))

        self.pathButton = QPushButton(self.centralWidget(),"pathButton")
        self.pathButton.setGeometry(QRect(527,91,90,31))

        self.pathEdit = QLineEdit(self.centralWidget(),"pathEdit")
        self.pathEdit.setGeometry(QRect(180,90,340,30))

        self.pathLabel = QLabel(self.centralWidget(),"pathLabel")
        self.pathLabel.setGeometry(QRect(20,90,134,30))

        self.logEdit = QTextEdit(self.centralWidget(),"logEdit")
        self.logEdit.setGeometry(QRect(10,130,620,220))
        logEdit_font = QFont(self.logEdit.font())
        logEdit_font.setFamily("FreeMono")
        logEdit_font.setPointSize(9)
        self.logEdit.setFont(logEdit_font)
        self.logEdit.setHScrollBarMode(QTextEdit.Auto)
        self.logEdit.setTextFormat(QTextEdit.PlainText)
        self.logEdit.setWordWrap(QTextEdit.WidgetWidth)
        self.logEdit.setWrapPolicy(QTextEdit.Anywhere)

        self.exitButton = QPushButton(self.centralWidget(),"exitButton")
        self.exitButton.setGeometry(QRect(500,380,120,30))

        self.groupBox1 = QGroupBox(self.centralWidget(),"groupBox1")
        self.groupBox1.setGeometry(QRect(20,360,270,60))

        self.goButton = QPushButton(self.groupBox1,"goButton")
        self.goButton.setGeometry(QRect(10,20,120,30))

        self.languageChange()

        self.resize(QSize(640,430).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.exitButton,SIGNAL("clicked()"),self.close)

        self.setTabOrder(self.projectEdit,self.taskEdit)
        self.setTabOrder(self.taskEdit,self.taskButton)
        self.setTabOrder(self.taskButton,self.pathEdit)
        self.setTabOrder(self.pathEdit,self.pathButton)
        self.setTabOrder(self.pathButton,self.goButton)
        self.setTabOrder(self.goButton,self.exitButton)
        self.setTabOrder(self.exitButton,self.logEdit)


    def languageChange(self):
        self.setCaption(self.__tr("Annotator Task Handler"))
        self.projectLabel.setText(self.__tr("Project Name"))
        self.projectEdit.setText(QString.null)
        self.taskLabel.setText(self.__tr("Task file"))
        self.taskEdit.setText(QString.null)
        self.taskButton.setText(self.__tr("Choose..."))
        self.pathButton.setText(self.__tr("Choose..."))
        self.pathEdit.setText(QString.null)
        self.pathLabel.setText(self.__tr("Path to save files"))
        self.logEdit.setText(self.__tr("Press Go! when ready"))
        self.exitButton.setText(self.__tr("Exit"))
        self.groupBox1.setTitle(self.__tr("Task processing"))
        self.goButton.setText(self.__tr("GO!"))


    def __tr(self,s,c = None):
        return qApp.translate("GUI",s,c)
