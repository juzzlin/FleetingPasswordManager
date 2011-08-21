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
#include <QGridLayout>
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

    setMaximumSize(size());
    setMinimumSize(size());
}

void SettingsDlg::initWidgets()
{
    QGridLayout * layout = new QGridLayout(this);
    QLabel      * label1 = new QLabel(tr("<b>Show password for this many secs:</b>"));
    QLabel      * label2 = new QLabel(tr("<b>Password length:</b>"));
    QLabel      * label3 = new QLabel(tr("<b>Automatically copy password to the clipboard:</b>"));
    QLabel      * label4 = new QLabel(tr("<b>Automatically clear the clipboard on timeout:</b>"));

    m_delaySpinBox->setRange(1, 60);
    m_delaySpinBox->setValue(5);

    m_lengthSpinBox->setRange(8, 32);
    m_lengthSpinBox->setValue(8);

    QPushButton * okButton = new QPushButton(tr("Ok"), this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    layout->addWidget(label1, 0, 0);
    layout->addWidget(m_delaySpinBox,   0, 1);

    layout->addWidget(label2, 1, 0);
    layout->addWidget(m_lengthSpinBox,  1, 1);

    layout->addWidget(label3, 2, 0);
    layout->addWidget(m_autoCopyCheck,  2, 1);

    layout->addWidget(label4, 3, 0);
    layout->addWidget(m_autoClearCheck, 3, 1);

    layout->addWidget(okButton, 4, 1);

    setLayout(layout);
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
