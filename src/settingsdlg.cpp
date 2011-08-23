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

SettingsDlg::SettingsDlg(QWidget *parent) :
QDialog(parent)
, m_lengthSpinBox(new QSpinBox(this))
, m_delaySpinBox(new QSpinBox(this))
, m_autoCopyCheck(new QCheckBox(this))
, m_autoClearCheck(new QCheckBox(this))
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
    QLabel * label1 = new QLabel(tr("Show password for this many <b>secs</b>:"));
    QLabel * label2 = new QLabel(tr("Default <b>password length</b>:"));
    QLabel * label3 = new QLabel(tr("<b>Automatically copy</b> password to the clipboard:"));
    QLabel * label4 = new QLabel(tr("<b>Automatically clear</b> the clipboard on timeout:"));

    // Init delay spin box
    m_delaySpinBox->setRange(1, 60);
    m_delaySpinBox->setValue(5);

    // Init password length spin box
    m_lengthSpinBox->setRange(8, 32);
    m_lengthSpinBox->setValue(8);

    // Create and connect ok-button
    QPushButton * okButton = new QPushButton(tr("Ok"), this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    // Add widgets to the grid layout
    layout->addWidget(label1,           0, 0);
    layout->addWidget(m_delaySpinBox,   0, 1);
    layout->addWidget(label2,           1, 0);
    layout->addWidget(m_lengthSpinBox,  1, 1);
    layout->addWidget(label3,           2, 0);
    layout->addWidget(m_autoCopyCheck,  2, 1);
    layout->addWidget(label4,           3, 0);
    layout->addWidget(m_autoClearCheck, 3, 1);
    layout->addWidget(okButton,         4, 1);

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

void SettingsDlg::getSettings(int & rDelay, int & rLength,
                              bool & rAutoCopy, bool & rAutoClear) const
{
   rDelay     = m_delaySpinBox->value();
   rLength    = m_lengthSpinBox->value();
   rAutoCopy  = m_autoCopyCheck->isChecked();
   rAutoClear = m_autoClearCheck->isChecked();
}

void SettingsDlg::setSettings(int delay, int length,
                              bool autoCopy, bool autoClear)
{
   m_delaySpinBox->setValue(delay);
   m_lengthSpinBox->setValue(length);
   m_autoCopyCheck->setChecked(autoCopy);
   m_autoClearCheck->setChecked(autoClear);
}
