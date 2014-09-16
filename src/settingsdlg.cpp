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

#include "settingsdlg.h"
#include "config.h"

#include <QCheckBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>

SettingsDlg::SettingsDlg(QWidget *parent)
: QDialog(parent)
, m_masterDelaySpinBox(new QSpinBox(this))
, m_loginDelaySpinBox(new QSpinBox(this))
, m_autoCopyCheck(new QCheckBox(this))
, m_autoClearCheck(new QCheckBox(this))
, m_alwaysOnTopCheck(new QCheckBox(this))
{
    setWindowTitle(tr("Settings"));

    initWidgets();
    initBackground();
}

void SettingsDlg::initWidgets()
{
    // Create "master" layout the includes only the frame widget
    QVBoxLayout * frameLayout = new QVBoxLayout(this);

    // Create layout for the button
    QHBoxLayout * buttonLayout = new QHBoxLayout();

    // Create layout for the rest of the widgets
    QGridLayout * layout = new QGridLayout();

    // Create labels
    QLabel * label1 = new QLabel(tr("Master password timeout in <b>mins</b>:"));
    label1->setToolTip(tr("Show master password for this many <b>mins</b>."));
    QLabel * label2 = new QLabel(tr("Login details timeout in <b>secs</b>:"));
    label2->setToolTip(tr("Show password for this many <b>secs</b> after generation."));
    QLabel * label3 = new QLabel(tr("Automatically copy password to the clipboard:"));
    label3->setToolTip(tr("Automatically copy password to the clipboard after generation."));
    QLabel * label4 = new QLabel(tr("Automatically clear the clipboard on timeout:"));
    label4->setToolTip(tr("Automatically clear the clipboard on timeout."));
    QLabel * label5 = new QLabel(tr("Always on top:"));
    label5->setToolTip(tr("If set, the window remains always on top. Needs restarting to apply."));

    // Init master password delay spin box
    m_masterDelaySpinBox->setRange(1, 60);
    m_masterDelaySpinBox->setValue(15);

    // Init generated password delay spin box
    m_loginDelaySpinBox->setRange(1, 60);
    m_loginDelaySpinBox->setValue(5);

    // Create and connect ok-button
    QPushButton * okButton = new QPushButton(tr("Ok"), this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    // Add widgets to the grid layout
    layout->addWidget(label1,               0, 0);
    layout->addWidget(m_masterDelaySpinBox, 0, 1);
    layout->addWidget(label2,               1, 0);
    layout->addWidget(m_loginDelaySpinBox,  1, 1);
    layout->addWidget(label3,               2, 0);
    layout->addWidget(m_autoCopyCheck,      2, 1);
    layout->addWidget(label4,               3, 0);
    layout->addWidget(m_autoClearCheck,     3, 1);
    layout->addWidget(label5,               4, 0);
    layout->addWidget(m_alwaysOnTopCheck,   4, 1);

    // Create the frame widget
    QFrame * frame = new QFrame(this);
    frame->setFrameShape(QFrame::Box);

    // Add the frame to its layout
    frameLayout->addWidget(frame);

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    frameLayout->addLayout(buttonLayout);

    // Set widget layout to the frame
    frame->setLayout(layout);
}

void SettingsDlg::initBackground()
{
    setStyleSheet("SettingsDlg { background-image: url(:/back.png) }");
}

void SettingsDlg::getSettings(int & rMasterDelay, int & rLoginDelay,
    bool & rAutoCopy, bool & rAutoClear, bool & rAlwaysOnTop) const
{
   rMasterDelay = m_masterDelaySpinBox->value();
   rLoginDelay  = m_loginDelaySpinBox->value();
   rAutoCopy    = m_autoCopyCheck->isChecked();
   rAutoClear   = m_autoClearCheck->isChecked();
   rAlwaysOnTop = m_alwaysOnTopCheck->isChecked();
}

void SettingsDlg::setSettings(int masterDelay, int loginDelay,
    bool autoCopy, bool autoClear, bool alwaysOnTop)
{
   m_masterDelaySpinBox->setValue(masterDelay);
   m_loginDelaySpinBox->setValue(loginDelay);
   m_autoCopyCheck->setChecked(autoCopy);
   m_autoClearCheck->setChecked(autoClear);
   m_alwaysOnTopCheck->setChecked(alwaysOnTop);
}
