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
import Icons, Images

class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent):
        QtGui.QMainWindow.__init__(self, parent)
        self.setWindowTitle("Fleeting Password Manager")
        self.setWindowIcon(QtGui.QIcon(":/fleetingpm.png"))
        self.company = "fleetingpm"
        self.software = "fleetingpm"
        self.defaultDelay = 5
        self.defaultLength = 8
        self.removeText = self.tr("&Remove URL && User")
        self.rememberText = self.tr("&Remember URL && User")
        self.rememberToolTip = self.tr("Remember current URL & User. Passwords are not saved.")
        self.removeToolTip = self.tr("Don't remember current URL & User anymore.")
        self.delay = self.defaultDelay
        self.length = self.defaultLength
        self.autoCopy = False
        self.initWidgets()
        self.initMenu()
        self.initBackground()
        self.resize(QtCore.QSize(452, 208))
        self.setMaximumSize(self.size())
        self.setMinimumSize(self.size())
        self.engine = Engine()
        self.loadSettings()

    def initBackground(self):
        self.palette = QtGui.QPalette()
        self.palette.setBrush(QtGui.QPalette.Window, QtGui.QPixmap(":back.png"))
        self.setPalette(self.palette)
        self.setAutoFillBackground(True)
                
    def initWidgets(self):
        """ Create widgets for the main window. """
        tr = self.tr
        self.layout = QtGui.QGridLayout()
        self.baseEdit = QtGui.QLineEdit()
        self.baseEdit.setToolTip(tr("Enter the master password common to all of your logins."))
        self.baseEdit.setEchoMode(QtGui.QLineEdit.Password)
        
        self.urlCombo = QtGui.QComboBox()
        self.urlCombo.setEditable(True)
        self.urlCombo.activated.connect(self.updateUser)
        self.urlCombo.editTextChanged.connect(self.setRmbButtonText)
        self.urlCombo.setToolTip(tr("Enter or select a saved URL/ID. For example facebook, google, gmail.com, myserver.."))
        
        self.userEdit = QtGui.QLineEdit()
        self.userEdit.setToolTip(tr("Enter your user name corresponding to the selected URL/ID."))
        self.passwdEdit = QtGui.QLineEdit()
        self.passwdEdit.setToolTip(tr("This is the generated password, which is always the same with the same master password, URL/ID and user name."))
        self.passwdEdit.setReadOnly(True)
        self.passwdEdit.setEnabled(False)

        self.layout.addWidget(self.baseEdit,   0, 1, 1, 3)
        self.layout.addWidget(self.urlCombo,   1, 1, 1, 3)
        self.layout.addWidget(self.userEdit,   2, 1, 1, 3)
        self.layout.addWidget(self.passwdEdit, 4, 1, 1, 3)

        frame = QtGui.QFrame()
        frame.setFrameShape(QtGui.QFrame.HLine)
        self.layout.addWidget(QtGui.QLabel(tr("<b><font color=#aa0000>Master password:</font></b>")), 0, 0)
        self.layout.addWidget(QtGui.QLabel(tr("<b>URL/ID:</b>")), 1, 0)
        self.layout.addWidget(QtGui.QLabel(tr("<b>User name:</b>")), 2, 0)
        self.layout.addWidget(frame, 3, 1, 1, 3)

        self.genButton = QtGui.QPushButton(tr("&Show password:"))
        self.genButton.setEnabled(False)
        self.genButton.setToolTip(tr("Generate and show the password"))
        self.layout.addWidget(self.genButton, 4, 0)

        starsLabel = QtGui.QLabel()
        starsLabel.setPixmap(QtGui.QPixmap(":/stars.png"))
        self.layout.addWidget(starsLabel, 5, 0)

        self.rmbButton = QtGui.QPushButton(self.rememberText)
        self.rmbButton.setEnabled(False)
        self.rmbButton.clicked.connect(self.rememberOrRemove)
        self.rmbButton.setToolTip(self.rememberToolTip)
        self.layout.addWidget(self.rmbButton, 5, 1, 1, 2)

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

    def initMenu(self):
        """ Create menus and actions. """

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
        """ Show the settings dialog. """
        d = self.settingsDlg
        d.exec_()
        self.length = d.lengthSpinBox.value()
        self.delay = d.delaySpinBox.value()
        self.autoCopy = d.autoCopyCheck.isChecked()
        self.timeLine.setDuration(self.delay * 1000)
        self.saveSettings()

    def loadSettings(self):
        """ Load settings by using QSettings """
        s = QtCore.QSettings(self.company, self.software)
        d = self.settingsDlg
        d.lengthSpinBox.setValue(int(s.value("length", self.defaultLength)))
        d.delaySpinBox.setValue(int(s.value("delay", self.defaultDelay)))
        d.autoCopyCheck.setChecked(s.value("autoCopy") == "true")
        self.length = d.lengthSpinBox.value()
        self.delay = d.delaySpinBox.value()
        self.autoCopy = d.autoCopyCheck.isChecked()
        self.timeLine.setDuration(self.delay * 1000)

        # Read login data
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
        """ Save settings by using QSettings """
        s = QtCore.QSettings(self.company, self.software)
        s.setValue("length", self.length)
        s.setValue("delay", self.delay)
        s.setValue("autoCopy", self.autoCopy)
        
        # Write login data that user wants to be saved
        s.beginWriteArray("logins")
        for i in xrange(self.urlCombo.count()):
            url  = self.urlCombo.itemText(i)
            user = self.urlCombo.itemData(i)
            s.setArrayIndex(i)
            s.setValue("url", url)
            s.setValue("user", user)
        s.endArray()

    def showAbout(self):
        """ Show the about dialog. """
        aboutDlg = AboutDlg(self)
        aboutDlg.exec_()

    def showAboutQt(self):
        """ Show the about Qt dialog. """
        QtGui.QMessageBox.aboutQt(self, self.tr("About Qt"))

    @Slot()
    def doGenerate(self):
        """ Generate the password. """
        # Generate the passwd
        passwd = self.engine.generate(self.baseEdit.text(),
                                      self.urlCombo.currentText(),
                                      self.userEdit.text(),
                                      self.length)
        
        # Enable the text field, show the generated passwd
        # and start timer to slowly fade out the text
        self.passwdEdit.setEnabled(True)
        self.passwdEdit.setText(passwd)
        if self.autoCopy:
            self.passwdEdit.selectAll()
            self.passwdEdit.copy()
        self.timeLine.stop()
        self.timeLine.start()

    @Slot(int)
    def updateFrame(self, frame):
        """ Decrease the alpha of the color of the password by one. """
        # Update alpha of the text color
        color = QtGui.QColor()
        color.setAlpha(255 - frame)
        palette = QtGui.QPalette(self.passwdEdit.palette())
        palette.setColor(QtGui.QPalette.Text, color)
        self.passwdEdit.setPalette(palette)

    @Slot()
    def invalidate(self):
        """ Clear the generated password and disable the text field. """
        # Clear the text and disable the text field
        self.passwdEdit.setText("")
        self.passwdEdit.setEnabled(False)

    @Slot()
    def enableGenButton(self):
        """ Enable the generate-button. """
        self.genButton.setEnabled(len(self.baseEdit.text()) > 0 and
                                  len(self.urlCombo.currentText()) > 0 and
                                  len(self.userEdit.text()) > 0)

    @Slot()
    def enableRmbButton(self):
        """ Enable the remember-button. """
        self.rmbButton.setEnabled(len(self.urlCombo.currentText()) > 0 and
                                  len(self.userEdit.text()) > 0)

    @Slot()
    def rememberOrRemove(self):
        """ Remember or remove the active url/user-pair. """
        user = self.userEdit.text()
        url  = self.urlCombo.currentText()
        
        if self.rmbButton.text() == self.rememberText:
            # Remember url and user
            alreadyAdded = False
            index = self.urlCombo.findText(url)
            if index != -1:
                alreadyAdded = True
                self.urlCombo.setItemData(index, user)
            if not alreadyAdded:
                self.urlCombo.addItem(url, user)
            self.saveSettings()
            self.rmbButton.setText(self.removeText)
        else:
            # Remove url and user
            index = self.urlCombo.findText(url)
            if index != -1:
                self.urlCombo.removeItem(index)
            self.saveSettings()
            self.rmbButton.setText(self.rememberText)

    @Slot()
    def updateUser(self, url):
        """ Update the user field if the url/user-pair is known. """
        index = self.urlCombo.findText(url)
        if index != -1:
            self.userEdit.setText(self.urlCombo.itemData(index))
            self.rmbButton.setText(self.removeText)

    @Slot()
    def setRmbButtonText(self, url):
        """ Set the text for remember-button. """
        index = self.urlCombo.findText(url)
        if index != -1:
            self.userEdit.setText(self.urlCombo.itemData(index))
            self.rmbButton.setText(self.removeText)
            self.rmbButton.setToolTip(self.removeToolTip)
        else:
            self.userEdit.setText("")
            self.rmbButton.setText(self.rememberText)
            self.rmbButton.setToolTip(self.rememberToolTip)
