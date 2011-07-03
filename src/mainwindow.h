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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//! The main window.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //! Constructor
    explicit MainWindow(QWidget * parent = 0);

private:

    //! Init the background image.
    void initBackground();

    //! Init the widgets.
    void initWidgets();

    //! Create menus and actions.
    void initMenu();

    //! Show the settings dialog.
    void showSettings();

    //! Show the about dialog.
    void showAbout();

    //! Show the about Qt dialog.
    void showAboutQt();

    //! Load settings by using QSettings.
    void loadSettings();

    //! Save settings by using QSettings.
    void saveSettings();

private slots:

    //! Generate the password.
    void doGenerate();

    //! Decrease the alpha of the color of the password by one.
    void decreasePasswordAlpha(int frame);

    //! Clear the generated password and disable the text field.
    void invalidate();

    //! Enable the generate-button.
    void enableGenButton();

    //! Enable the remember-button.
    void enableRmbButton();

    //! Remember or remove the active url/user-pair depending
    //! on the button state.
    void rememberOrRemoveLogin();

    //! Update the user field if the url/user-pair is known.
    void updateUser(const QString & url);

    //! Set the text for remember-button.
    void setRmbButtonText(const QString & url);
};

#endif // MAINWINDOW_H
