# This file is part of Fleeting Password Manager (FleetingPM).
# Copyright (C) 2011 Jussi Lind <jussi.lind@iki.fi>
#
# FleetingPM is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# FleetingPM is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FleetingPM. If not, see <http://www.gnu.org/licenses/>.

from PySide import QtCore, QtGui
from PySide.QtCore import Slot
from Engine import Engine
from AboutDlg import AboutDlg
from SettingsDlg import SettingsDlg

class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent):
        QtGui.QMainWindow.__init__(self, parent)
        self.setWindowTitle("Fleeting Password Manager")
        self.company = "FleetingPM"
        self.software = "FleetingPM"
        self._defaultDelay = 5
        self._defaultLength = 8
        self.delay = self._defaultDelay
        self.length = self._defaultLength
        self.createWidgets()
        self.createMenu()
        self.engine = Engine()
        self.loadSettings()
                
    def createWidgets(self):
        self.layout = QtGui.QGridLayout()
        self.baseEdit = QtGui.QLineEdit()
        
        self.urlCombo = QtGui.QComboBox()
        self.urlCombo.setEditable(True)
        self.urlCombo.activated.connect(self.updateUser)
        
        self.userEdit = QtGui.QLineEdit()
        self.passwdEdit = QtGui.QLineEdit()

        self.baseEdit.setEchoMode(QtGui.QLineEdit.Password)
        self.passwdEdit.setReadOnly(True)
        self.passwdEdit.setEnabled(False)

        self.layout.addWidget(self.baseEdit,   0, 1, 1, 3)
        self.layout.addWidget(self.urlCombo,    1, 1, 1, 3)
        self.layout.addWidget(self.userEdit,   2, 1, 1, 3)
        self.layout.addWidget(self.passwdEdit, 4, 1, 1, 3)

        frame = QtGui.QFrame()
        frame.setFrameShape(QtGui.QFrame.HLine)
        tr = self.tr
        self.layout.addWidget(QtGui.QLabel(tr("Base password:")), 0, 0)
        self.layout.addWidget(QtGui.QLabel(tr("URL/ID:")), 1, 0)
        self.layout.addWidget(QtGui.QLabel(tr("User name:")), 2, 0)
        self.layout.addWidget(frame, 3, 1, 1, 3)
        self.layout.addWidget(QtGui.QLabel(tr("Password:")), 4, 0)

        self.genButton = QtGui.QPushButton(tr("&Generate"))
        self.genButton.setEnabled(False)
        self.layout.addWidget(self.genButton, 5, 1)

        self.rmbButton = QtGui.QPushButton(tr("&Remember URL && User"))
        self.rmbButton.setEnabled(False)
        self.rmbButton.clicked.connect(self.remember)
        self.layout.addWidget(self.rmbButton, 5, 2)

        self.quitButton = QtGui.QPushButton(tr("&Quit"))
        self.quitButton.clicked.connect(self.close)
        self.layout.addWidget(self.quitButton, 5, 3)

        dummy = QtGui.QWidget()
        dummy.setLayout(self.layout)
        self.setCentralWidget(dummy)
        
        self.genButton.clicked.connect(self.doGenerate)
        self.baseEdit.textChanged.connect(self.invalidate)
        self.urlCombo.textChanged.connect(self.invalidate)
        self.userEdit.textChanged.connect(self.invalidate)
        self.baseEdit.textChanged.connect(self.enableGenButton)
        self.urlCombo.textChanged.connect(self.enableGenButton)
        self.userEdit.textChanged.connect(self.enableGenButton)
        self.baseEdit.textChanged.connect(self.enableRmbButton)
        self.urlCombo.textChanged.connect(self.enableRmbButton)
        self.userEdit.textChanged.connect(self.enableRmbButton)

        self.timeLine = QtCore.QTimeLine()
        self.timeLine.setDuration(self.delay * 1000)
        self.timeLine.frameChanged.connect(self.updateFrame)
        self.timeLine.finished.connect(self.invalidate)
        self.timeLine.setFrameRange(0, 255)

        self.settingsDlg = SettingsDlg(self)

    def createMenu(self):

        # Add file menu
        tr = self.tr
        fileMenu = self.menuBar().addMenu(tr("&File"))

        # Add action for settings
        setAct = QtGui.QAction(tr("&Settings.."), fileMenu)
        setAct.triggered.connect(self.showSettings)
        fileMenu.addAction(setAct)
        
        # Add action for quit
        quitAct = QtGui.QAction(tr("&Quit"), fileMenu)
        quitAct.triggered.connect(self.close)
        fileMenu.addAction(quitAct)
        
        # Add help menu
        helpMenu = self.menuBar().addMenu(tr("&Help"))
        
        # Add action for about
        aboutAct = QtGui.QAction(tr("&About ") + self.windowTitle() + "..", helpMenu)
        aboutAct.triggered.connect(self.showAbout)
        helpMenu.addAction(aboutAct)

        # Add action for about Qt
        aboutQtAct = QtGui.QAction(tr("About &Qt.."), helpMenu)
        aboutQtAct.triggered.connect(self.showAboutQt)
        helpMenu.addAction(aboutQtAct)

    def showSettings(self):
        d = self.settingsDlg
        d.exec_()
        self.length = d.lengthSpinBox.value()
        self.delay = d.delaySpinBox.value()
        self.timeLine.setDuration(self.delay * 1000)
        self.saveSettings()

    def loadSettings(self):
        s = QtCore.QSettings(self.company, self.software)
        d = self.settingsDlg
        d.lengthSpinBox.setValue(int(s.value("length", self._defaultLength)))
        d.delaySpinBox.setValue(int(s.value("delay", self._defaultDelay)))
        self.length = d.lengthSpinBox.value()
        self.delay = d.delaySpinBox.value()
        self.timeLine.setDuration(self.delay * 1000)

        self.urlCombo.clear()
        size = s.beginReadArray("logins")
        for i in xrange(size):
            s.setArrayIndex(i)
            url = s.value("url")
            user = s.value("user")
            self.urlCombo.addItem(url, user)
        s.endArray()

        self.updateUser(self.urlCombo.currentText())

    def saveSettings(self):
        s = QtCore.QSettings(self.company, self.software)
        s.setValue("length", self.length)
        s.setValue("delay", self.delay)
        
        s.beginWriteArray("logins")
        for i in xrange(self.urlCombo.count()):
            url  = self.urlCombo.itemText(i)
            user = self.urlCombo.itemData(i)
            s.setArrayIndex(i)
            s.setValue("url", url)
            s.setValue("user", user)
        s.endArray()

    def showAbout(self):
        aboutDlg = AboutDlg(self)
        aboutDlg.exec_()

    def showAboutQt(self):
        QtGui.QMessageBox.aboutQt(self, self.tr("About Qt"))

    @Slot()
    def doGenerate(self):

        # Generate the passwd
        passwd = self.engine.generate(self.baseEdit.text(),
                                      self.urlCombo.currentText(),
                                      self.userEdit.text(),
                                      self.length)
        
        # Enable the text field, show the generated passwd
        # and start timer to slowly fade out the text
        self.passwdEdit.setEnabled(True)
        self.passwdEdit.setText(passwd)
        self.timeLine.stop()
        self.timeLine.start()

    @Slot(int)
    def updateFrame(self, frame):

        # Update aplha of the text color
        color = QtGui.QColor()
        color.setAlpha(255 - frame)
        palette = QtGui.QPalette(self.passwdEdit.palette())
        palette.setColor(QtGui.QPalette.Text, color)
        self.passwdEdit.setPalette(palette)

    @Slot()
    def invalidate(self):
        # Clear the text and disable the text field
        self.passwdEdit.setText("")
        self.passwdEdit.setEnabled(False)

    @Slot()
    def enableGenButton(self):
        self.genButton.setEnabled(len(self.baseEdit.text()) > 0 and
                                  len(self.urlCombo.currentText()) > 0 and
                                  len(self.userEdit.text()) > 0)

    @Slot()
    def enableRmbButton(self):
        self.rmbButton.setEnabled(len(self.urlCombo.currentText()) > 0 and
                                  len(self.userEdit.text()) > 0)

    @Slot()
    def remember(self):
        user = self.userEdit.text()
        url  = self.urlCombo.currentText()
            
        alreadyAdded = False
        index = self.urlCombo.findText(url)
        if index != -1:
            alreadyAdded = True
            self.urlCombo.setItemData(index, user)
        
        if not alreadyAdded:
            self.urlCombo.addItem(url, user)

        self.saveSettings()

    @Slot()
    def updateUser(self, url):
        index = self.urlCombo.findText(url)
        if index != -1:
            self.userEdit.setText(self.urlCombo.itemData(index))
