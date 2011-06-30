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
from Version import Version
import Images

class AboutDlg(QtGui.QDialog):
    def __init__(self, parent):
        QtGui.QDialog.__init__(self, parent)
        self.setWindowTitle(self.tr("About Fleeting Password Manager"))
        self.initWidgets()

    def initWidgets(self):
        frame = QtGui.QFrame(self)
        vLayout = QtGui.QVBoxLayout()
        hLayout = QtGui.QHBoxLayout()
        pixmapLabel = QtGui.QLabel()
        pixmapLabel.setPixmap(QtGui.QPixmap(":/about.png").scaledToWidth(256))
        hLayout.addWidget(pixmapLabel)
        infoLabel = QtGui.QLabel()
        infoLabel.setText("<h2>Fleeting Password Manager v" + Version.verString + "</h2>"
                       "<p>FPM is licenced under GNU General Public \
License v3.</p>"
                       "<p>Copyright (c) Jussi Lind 2011.</p>"
                       "<a href='http://fleetingpm.sourceforge.net'>\
http://fleetingpm.sourceforge.net</a>")
        hLayout.addWidget(infoLabel)
        vLayout.addLayout(hLayout)
        buttonLayout = QtGui.QHBoxLayout()
        button = QtGui.QPushButton("&Ok")
        button.clicked.connect(self.accept)
        buttonLayout.addWidget(button)
        buttonLayout.insertStretch(0)
        vLayout.addLayout(buttonLayout)
        self.setLayout(vLayout)
