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

class AboutDlg(QtGui.QDialog):
    def __init__(self, parent):
        QtGui.QDialog.__init__(self, parent)
        self.setWindowTitle(self.tr("About Fleeting Password Manager"))
        self.initWidgets()

    def initWidgets(self):
        frame = QtGui.QFrame(self)
        layout  = QtGui.QVBoxLayout()
        layout2 = QtGui.QHBoxLayout()
        label   = QtGui.QLabel()
        pixmap  = QtGui.QPixmap(":/fleetingpm-about.png").scaledToWidth(256)
        label.setPixmap(pixmap)
        layout2.addWidget(label)
        label2 = QtGui.QLabel()
        label2.setText("<h2>Fleeting Password Manager</h2>"
                       "<p>FPM is licenced under GNU General Public \
License v3.</p>"
                       "<p>Copyright (c) Jussi Lind 2011.</p>"
                       "<a href='http://fleetingpm.sourceforge.net'>\
http://fleetingpm.sourceforge.net</a>")
        layout2.addWidget(label2)
        layout.addLayout(layout2)
        layout3 = QtGui.QHBoxLayout()
        button = QtGui.QPushButton("&Ok")
        button.clicked.connect(self.accept)
        layout3.addWidget(button)
        layout3.insertStretch(0)
        layout.addLayout(layout3)
        self.setLayout(layout)
        
