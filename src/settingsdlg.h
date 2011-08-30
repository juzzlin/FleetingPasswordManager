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

#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>

class QCheckBox;
class QSpinBox;

//! The settings dialog.
class SettingsDlg : public QDialog
{
    Q_OBJECT

public:

    //! Constructor.
    explicit SettingsDlg(QWidget * parent = 0);

    //! Store current settings to the given arguments.
    void getSettings(int & rDelay, bool & rAutoCopy, bool & rAutoClear) const;

    //! Take current settings from the given arguments.
    void setSettings(int delay, bool autoCopy, bool autoClear);

private:

    //! Create and init the widgets.
    void initWidgets();

    //! Set the background image.
    void initBackground();

    //! Spin box for the password show delay.
    QSpinBox  * m_delaySpinBox;

    //! Check box for the clipboard auto copy.
    QCheckBox * m_autoCopyCheck;

    //! Check box for clearing the clipboard automatically
    //! on timeout.
    QCheckBox * m_autoClearCheck;
};

#endif // SETTINGSDLG_H
