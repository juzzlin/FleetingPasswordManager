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
