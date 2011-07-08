// This file is part of Fleeting Password Manager (FleetingPM).
// Copyright (C) 2011 Jussi Lind <jussi.lind@iki.fi>
//
// FleetingPM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// FleetingPM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FleetingPM. If not, see <http://www.gnu.org/licenses/>.
//

#include "instructionsdlg.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QSizeGrip>

InstructionsDlg::InstructionsDlg(QWidget * pParent) :
    QDialog(pParent)
{
    QTextBrowser * browser = new QTextBrowser();
    browser->setStyleSheet("background-color: #ffffee;");
    browser->setSource(QUrl("qrc:instructions.html"));
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(browser);
    QHBoxLayout * buttonLayout = new QHBoxLayout();
    QPushButton * button = new QPushButton("&Ok");
    connect(button, SIGNAL(clicked()), this, SLOT(accept()));
    buttonLayout->addWidget(button);
    buttonLayout->addWidget(new QSizeGrip(this));
    buttonLayout->insertStretch(0);
    layout->addLayout(buttonLayout);
    layout->setContentsMargins(5, 5, 5, 5);
    resize(640, 400);
}
