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
