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

#include "mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Fleeting Password Manager");
    setWindowIcon(QIcon(":/fleetingpm.png"));
    resize(QSize(452, 208));
    setMaximumSize(size());
    setMinimumSize(size());

    initBackground();
}

void MainWindow::initBackground()
{
     QPalette palette = QPalette();
     palette.setBrush(QPalette::Window, QPixmap(":back.png"));
     setPalette(palette);
     setAutoFillBackground(true);
}

void MainWindow::initWidgets()
{
}

void MainWindow::initMenu()
{
}

void MainWindow::showSettings()
{
}

void MainWindow::showAbout()
{
}

void MainWindow::showAboutQt()
{
}

void MainWindow::loadSettings()
{
}

void MainWindow::saveSettings()
{
}

void MainWindow::doGenerate()
{
}

void MainWindow::decreasePasswordAlpha(int frame)
{
}

void MainWindow::invalidate()
{
}

void MainWindow::enableGenButton()
{
}

void MainWindow::enableRmbButton()
{
}

void MainWindow::rememberOrRemoveLogin()
{
}

void MainWindow::updateUser(const QString & url)
{
}

void MainWindow::setRmbButtonText(const QString & url)
{
}
