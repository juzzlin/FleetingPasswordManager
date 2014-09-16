// This file is part of Fleeting Password Manager (Fleetingpm).
// Copyright (C) 2011 Jussi Lind <jussi.lind@iki.fi>
//
// Fleetingpm is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// Fleetingpm is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Fleetingpm. If not, see <http://www.gnu.org/licenses/>.
//

#include "aboutdlg.h"
#include "config.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

AboutDlg::AboutDlg(QWidget * parent)
: QDialog(parent)
{
    setWindowTitle(tr("About Fleeting Password Manager"));
    initWidgets();
}

void AboutDlg::initWidgets()
{
    QVBoxLayout * vLayout     = new QVBoxLayout(this);
    QHBoxLayout * hLayout     = new QHBoxLayout();
    QLabel      * pixmapLabel = new QLabel(this);

    pixmapLabel->setPixmap(QPixmap(":/about.png").scaledToWidth(256));
    hLayout->addWidget(pixmapLabel);

    QLabel * infoLabel = new QLabel(this);
    infoLabel->setText(QString("<h2>") + Config::NAME + " v" + Config::VERSION + "</h2>"
                       + "<p>" + Config::NAME + " is licenced under GNU GPLv3.</p>"
                       + "<p>Copyright (c) Jussi Lind 2011-2014.</p>"
                       + "<a href='http://fleetingpm.sourceforge.net'>"
                       + "http://fleetingpm.sourceforge.net</a>");

    hLayout->addWidget(infoLabel);
    vLayout->addLayout(hLayout);
    QHBoxLayout * buttonLayout = new QHBoxLayout();
    QPushButton * button = new QPushButton("&Ok", this);
    connect(button, SIGNAL(clicked()), this, SLOT(accept()));
    buttonLayout->addWidget(button);
    buttonLayout->insertStretch(0);
    vLayout->addLayout(buttonLayout);
}
