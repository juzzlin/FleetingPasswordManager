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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QMainWindow>

#include "logindata.h"

class SettingsDlg;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTimeLine;
class QTimer;

//! The main window.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //! Constructor
    explicit MainWindow(QWidget * parent = 0);

    //! Destructor
    ~MainWindow();

protected:

    //! \reimp
    virtual void closeEvent(QCloseEvent * event);

private:

    //! Center the window or load previous location.
    void centerOrRestoreLocation();

    //! Init the background image.
    void initBackground();

    //! Init the widgets.
    void initWidgets();

    //! Create menus and actions.
    void initMenu();

    //! Connect the signals emitted by widgets.
    void connectSignalsFromWidgets();

    //! Load settings by using QSettings.
    void loadSettings();

    //! Save settings by using QSettings.
    void saveSettings();

    //! Show the master password for this long in mins.
    int m_defaultMasterDelay;

    //! Show the login details for this long in secs.
    int m_defaultLoginDelay;

    //! Default lenght of the password.
    int m_defaultLength;

    //! "Remove.." text of the remove/save button.
    QString m_removeText;

    //! "Save.." text of the remove/save button.
    QString m_saveText;

    //! "Save.."-tooltip of the remove/save button.
    QString m_saveToolTip;

    //! "Remove.."-tooltip of the remove/save button.
    QString m_removeToolTip;

    //! "Master password:"
    QString m_masterPasswordText;

    //! Current delay for master password in mins.
    int m_masterDelay;

    //! Current delay for login details in secs.
    int m_loginDelay;

    //! Copy generated password automatically to the clipboard if true.
    bool m_autoCopy;

    //! Clear clipboard on timeout if true.
    bool m_autoClear;

    //! True, if window is always on top.
    bool m_alwaysOnTop;

    //! Master password edit field
    QLineEdit * m_masterEdit;

    //! Master password label
    QLabel * m_masterLabel;

    //! User name edit field
    QLineEdit * m_userEdit;

    //! Password edit field
    QLineEdit * m_passwdEdit;

    //! Url combo box
    QComboBox * m_urlCombo;

    //! Generate button
    QPushButton * m_genButton;

    //! Save/remove button
    QPushButton * m_saveButton;

    //! Spin box to set password length
    //! other than the default.
    QSpinBox * m_lengthSpinBox;

    //! Timer used when showing the master password.
    QTimer * m_masterTimer;

    //! Time line used when showing the generated password.
    QTimeLine * m_timeLine;

    //! Settings dialog
    SettingsDlg * m_settingsDlg;

    typedef QHash<QString, LoginData> LoginHash;
    LoginHash m_loginHash;

private slots:

    //! Generate the password.
    void doGenerate();

    //! Decrease the alpha of the color of the password by one.
    void decreasePasswordAlpha(int frame);

    //! Clear the generated password and disable the text field.
    void invalidate();

    //! Like invalidate + clear all login details.
    void invalidateAll();

    //! Enable the generate-button.
    void enableGenButton();

    //! Enable the save-button.
    void enableSaveButton();

    //! Set green or red color for the master password label.
    void setMasterPasswordLabelColor();

    //! Save or remove the active url/user-pair depending
    //! on the button state.
    void saveOrRemoveLogin();

    //! Update the user field if the url/user-pair is known.
    void updateUser(const QString & url);

    //! Set the text for save/remove-button.
    void toggleSaveButtonText();

    //! Import logins
    void importLogins();

    //! Export logins
    void exportLogins();

    //! Show the settings dialog.
    void showSettingsDlg();

    //! Show the instructions dialog.
    void showInstructionsDlg();

    //! Show the about dialog.
    void showAboutDlg();

    //! Show the about Qt dialog.
    void showAboutQtDlg();

    //! Clear url and user name
    void clearFields();
};

#endif // MAINWINDOW_H
