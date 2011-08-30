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
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>

SettingsDlg::SettingsDlg(QWidget *parent)
: QDialog(parent)
, m_delaySpinBox(new QSpinBox(this))
, m_autoCopyCheck(new QCheckBox(this))
, m_autoClearCheck(new QCheckBox(this))
, m_alwaysOnTopCheck(new QCheckBox(this))
{
    setWindowTitle(tr("Settings"));

    initWidgets();
    initBackground();

    resize(QSize(Config::SETTINGSDLG_WIDTH, Config::SETTINGSDLG_HEIGHT));

    // Make the size fixed
    setMaximumSize(size());
    setMinimumSize(size());
}

void SettingsDlg::initWidgets()
{
    // Create "master" layout the includes only the frame widget
    QHBoxLayout * frameLayout = new QHBoxLayout(this);

    // Create layout for the rest of the widgets
    QGridLayout * layout = new QGridLayout();

    // Create labels
    QLabel * label1 = new QLabel(tr("Password timeout:"));
    label1->setToolTip(tr("Show password for this many secs after generation."));
    QLabel * label2 = new QLabel(tr("Automatically copy password to the clipboard:"));
    label2->setToolTip(tr("Automatically copy password to the clipboard after generation."));
    QLabel * label3 = new QLabel(tr("Automatically clear the clipboard on timeout:"));
    label3->setToolTip(tr("Automatically clear the clipboard on timeout."));
    QLabel * label4 = new QLabel(tr("Always on top:"));
    label4->setToolTip(tr("If set, the window remains always on top. Needs restarting to apply."));

    // Init delay spin box
    m_delaySpinBox->setRange(1, 60);
    m_delaySpinBox->setValue(5);

    // Create and connect ok-button
    QPushButton * okButton = new QPushButton(tr("Ok"), this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    // Add widgets to the grid layout
    layout->addWidget(label1,             0, 0);
    layout->addWidget(m_delaySpinBox,     0, 1);
    layout->addWidget(label2,             1, 0);
    layout->addWidget(m_autoCopyCheck,    1, 1);
    layout->addWidget(label3,             2, 0);
    layout->addWidget(m_autoClearCheck,   2, 1);
    layout->addWidget(label4,             3, 0);
    layout->addWidget(m_alwaysOnTopCheck, 3, 1);
    layout->addWidget(okButton,           4, 1);

    // Create the frame widget
    QFrame * frame = new QFrame(this);
    frame->setFrameShape(QFrame::Box);

    // Add the frame to its layout
    frameLayout->addWidget(frame);

    // Set widget layout to the frame
    frame->setLayout(layout);
}

void SettingsDlg::initBackground()
{
    QPalette palette = QPalette();
    palette.setBrush(QPalette::Window, QPixmap(":/back.png"));
    setPalette(palette);
    setAutoFillBackground(true);
}

void SettingsDlg::getSettings(int & rDelay,
                              bool & rAutoCopy, bool & rAutoClear,
                              bool & rAlwaysOnTop) const
{
   rDelay       = m_delaySpinBox->value();
   rAutoCopy    = m_autoCopyCheck->isChecked();
   rAutoClear   = m_autoClearCheck->isChecked();
   rAlwaysOnTop = m_alwaysOnTopCheck->isChecked();
}

void SettingsDlg::setSettings(int delay,
                              bool autoCopy, bool autoClear,
                              bool alwaysOnTop)
{
   m_delaySpinBox->setValue(delay);
   m_autoCopyCheck->setChecked(autoCopy);
   m_autoClearCheck->setChecked(autoClear);
   m_alwaysOnTopCheck->setChecked(alwaysOnTop);
}
