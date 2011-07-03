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
import Icons

class SettingsDlg(QtGui.QDialog):
    def __init__(self, parent):
        QtGui.QDialog.__init__(self, parent)
        tr = self.tr
        self.setWindowTitle(tr("Settings"))
        layout = QtGui.QGridLayout()
        label1 = QtGui.QLabel(tr("<b>Show password for this many secs:</b>"))
        label2 = QtGui.QLabel(tr("<b>Password length:</b>"))
        label3 = QtGui.QLabel(tr("<b>Automatically copy password to the clipboard:</b>"))

        self.delaySpinBox = QtGui.QSpinBox()
        self.delaySpinBox.setRange(1, 60)
        self.lengthSpinBox = QtGui.QSpinBox()
        self.lengthSpinBox.setRange(8, 32)
        self.autoCopyCheck = QtGui.QCheckBox()

        okButton = QtGui.QPushButton(tr("Ok"))
        okButton.clicked.connect(self.accept)

        layout.addWidget(label1, 0, 0)
        layout.addWidget(label2, 1, 0)
        layout.addWidget(label3, 2, 0)
        layout.addWidget(self.delaySpinBox, 0, 1)
        layout.addWidget(self.lengthSpinBox, 1, 1)
        layout.addWidget(self.autoCopyCheck, 2, 1)
        layout.addWidget(okButton, 3, 1)

        self.setLayout(layout)
        self.initBackground()
        self.resize(QtCore.QSize(450, 142))
        self.setMaximumSize(self.size())
        self.setMinimumSize(self.size())

    def initBackground(self):
        self.palette = QtGui.QPalette()
        self.palette.setBrush(QtGui.QPalette.Window, QtGui.QPixmap(":/fleetingpm.png"))
        self.setPalette(self.palette)
        self.setAutoFillBackground(True)
