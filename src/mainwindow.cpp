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

#include "aboutdlg.h"
#include "engine.h"
#include "instructionsdlg.h"
#include "loginio.h"
#include "mainwindow.h"
#include "settingsdlg.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QTimeLine>

namespace
{
    // Values used in QSettings
    const char * COMPANY  = "fleetingpm";
    const char * SOFTWARE = "fleetingpm";
}

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent)
, m_defaultDelay(5)
, m_defaultLength(8)
, m_removeText(tr("&Remove URL && User"))
, m_rememberText(tr("&Remember URL && User"))
, m_rememberToolTip(tr("Remember current URL & User. Passwords are not saved."))
, m_removeToolTip(tr("Don't remember current URL & User anymore."))
, m_delay(m_defaultDelay)
, m_length(m_defaultLength)
, m_autoCopy(false)
, m_autoClear(false)
, m_masterEdit(new QLineEdit(this))
, m_userEdit(new QLineEdit(this))
, m_passwdEdit(new QLineEdit(this))
, m_urlCombo(new QComboBox(this))
, m_genButton(new QPushButton(tr("&Show password:"), this))
, m_rmbButton(new QPushButton(m_rememberText, this))
, m_timeLine(new QTimeLine())
, m_settingsDlg(new SettingsDlg(this))
{
    setWindowTitle("Fleeting Password Manager");
    setWindowIcon(QIcon(":/fleetingpm.png"));
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    resize(QSize(452, 208));
    setMaximumSize(size());
    setMinimumSize(size());

    initWidgets();
    initMenu();
    initBackground();
    loadSettings();

    m_timeLine->setDuration(m_delay * 1000);
    connect(m_timeLine, SIGNAL(frameChanged(int)), this, SLOT(decreasePasswordAlpha(int)));
    connect(m_timeLine, SIGNAL(finished()), this, SLOT(invalidate()));
    m_timeLine->setFrameRange(0, 255);

    // Load previous location or center the window.
    QRect geom(QApplication::desktop()->availableGeometry());
    int centerX = geom.width()  / 2 - frameGeometry().width() / 2;
    int centerY = geom.height() / 2 -frameGeometry().height() / 2;
    QSettings s(COMPANY, SOFTWARE);
    int x = s.value("x", centerX).toInt();
    int y = s.value("y", centerY).toInt();
    move(x, y);
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
    QGridLayout * layout = new QGridLayout();
    m_masterEdit->setToolTip(tr("Enter the master password common to all of your logins."));
    m_masterEdit->setEchoMode(QLineEdit::Password);

    m_urlCombo->setEditable(true);
    m_urlCombo->setToolTip(tr("Enter or select a saved URL/ID. For example facebook, google, gmail.com, myserver.."));
    connect(m_urlCombo, SIGNAL(activated(const QString &)), this, SLOT(updateUser(const QString &)));
    connect(m_urlCombo, SIGNAL(editTextChanged(const QString &)), this, SLOT(setRmbButtonText(const QString &)));

    m_userEdit->setToolTip(tr("Enter your user name corresponding to the selected URL/ID."));

    m_passwdEdit->setToolTip(tr("This is the generated password, which is always the same with the same master password, URL/ID and user name."));
    m_passwdEdit->setReadOnly(true);
    m_passwdEdit->setEnabled(false);

    layout->addWidget(m_masterEdit, 0, 1, 1, 3);
    layout->addWidget(m_urlCombo,   1, 1, 1, 3);
    layout->addWidget(m_userEdit,   2, 1, 1, 3);
    layout->addWidget(m_passwdEdit, 4, 1, 1, 3);

    QFrame * frame = new QFrame(this);
    frame->setFrameShape(QFrame::HLine);
    layout->addWidget(new QLabel(tr("<b><font color=#aa0000>Master password:</font></b>")), 0, 0);
    layout->addWidget(new QLabel(tr("<b>URL/ID:</b>")), 1, 0);
    layout->addWidget(new QLabel(tr("<b>User name:</b>")), 2, 0);
    layout->addWidget(frame, 3, 1, 1, 3);

    m_genButton->setEnabled(false);
    m_genButton->setToolTip(tr("Generate and show the password"));
    layout->addWidget(m_genButton, 4, 0);

    QLabel * starsLabel = new QLabel();
    starsLabel->setPixmap(QPixmap(":/stars.png"));
    layout->addWidget(starsLabel, 5, 0);

    m_rmbButton->setEnabled(false);
    connect(m_rmbButton, SIGNAL(clicked()), this, SLOT(rememberOrRemoveLogin()));
    m_rmbButton->setToolTip(m_rememberToolTip);
    layout->addWidget(m_rmbButton, 5, 1, 1, 2);

    QPushButton * quitButton = new QPushButton(tr("&Quit"));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    layout->addWidget(quitButton, 5, 3);

    QWidget * dummy = new QWidget();
    dummy->setLayout(layout);
    setCentralWidget(dummy);

    connect(m_genButton, SIGNAL(clicked()), this, SLOT(doGenerate()));

    connect(m_masterEdit, SIGNAL(textChanged(const QString &)), this, SLOT(invalidate()));
    connect(m_urlCombo, SIGNAL(textChanged(const QString &)), this, SLOT(invalidate()));
    connect(m_userEdit, SIGNAL(textChanged(const QString &)), this, SLOT(invalidate()));

    connect(m_masterEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableGenButton()));
    connect(m_urlCombo, SIGNAL(textChanged(const QString &)), this, SLOT(enableGenButton()));
    connect(m_userEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableGenButton()));

    connect(m_masterEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableRmbButton()));
    connect(m_urlCombo, SIGNAL(textChanged(const QString &)), this, SLOT(enableRmbButton()));
    connect(m_userEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableRmbButton()));
}

void MainWindow::initMenu()
{
    // Add file menu
    QMenu * fileMenu = menuBar()->addMenu(tr("&File"));

    // Add action for importing logins
    QAction * importAct = new QAction(tr("&Import logins.."), fileMenu);
    connect(importAct, SIGNAL(triggered()), this, SLOT(importLogins()));
    fileMenu->addAction(importAct);

    // Add action for exporting logins
    QAction * exportAct = new QAction(tr("&Export logins.."), fileMenu);
    connect(exportAct, SIGNAL(triggered()), this, SLOT(exportLogins()));
    fileMenu->addAction(exportAct);

    // Add action for settings
    QAction * setAct = new QAction(tr("&Settings.."), fileMenu);
    connect(setAct, SIGNAL(triggered()), this, SLOT(showSettingsDlg()));
    fileMenu->addAction(setAct);

    // Add action for quit
    QAction * quitAct = new QAction(tr("&Quit"), fileMenu);
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(quitAct);

    // Add help menu
    QMenu * helpMenu = menuBar()->addMenu(tr("&Help"));

    // Add action for instructions
    QAction * instructionsAct = new QAction(tr("&Instructions.."), helpMenu);
    connect(instructionsAct, SIGNAL(triggered()), this, SLOT(showInstructionsDlg()));
    helpMenu->addAction(instructionsAct);

    // Add action for about
    QAction * aboutAct = new QAction(tr("&About ") + windowTitle() + "..", helpMenu);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(showAboutDlg()));
    helpMenu->addAction(aboutAct);

    // Add action for about Qt
    QAction * aboutQtAct = new QAction(tr("About &Qt.."), helpMenu);
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(showAboutQtDlg()));
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::showSettingsDlg()
{
    m_settingsDlg->exec();
    m_settingsDlg->getSettings(m_delay, m_length, m_autoCopy, m_autoClear);
    m_timeLine->setDuration(m_delay * 1000);
    saveSettings();
}

void MainWindow::importLogins()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import logins"),
                                                    QDir::homePath(),
                                                    tr("Fleeting Password Manager files (*.fpm)"));
    if (fileName.length() > 0)
    {
        int newLogins = 0;
        int updated   = 0;

        LoginIO::LoginList logins;
        if (LoginIO::importLogins(logins, fileName))
        {
            for (int i = 0; i < logins.count(); i++)
            {
                QString url  = logins.at(i).first;
                QString user = logins.at(i).second;

                int index = m_urlCombo->findText(url);
                if (index != -1)
                {
                    m_urlCombo->setItemData(index, user);
                    updated++;
                }
                else
                {
                    m_urlCombo->addItem(url, user);
                    newLogins++;
                }
            }

            m_urlCombo->model()->sort(0);
            saveSettings();

            QString message(tr("Successfully imported logins from '") + fileName + tr("': %1 new, %2 updated."));
            message = message.arg(newLogins).arg(updated);
            QMessageBox::information(this, tr("Importing logins succeeded"), message);
        }
        else
        {
            QMessageBox::warning(this, tr("Exporting logins failed"), tr("Failed to import logins from '") + fileName + "'");
        }
    }
}

void MainWindow::exportLogins()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export logins"),
                                                    QDir::homePath(),
                                                    tr("Fleeting Password Manager files (*.fpm)"));
    if (fileName.length() > 0)
    {
        if (!fileName.endsWith(".fpm"))
            fileName.append(".fpm");

        LoginIO::LoginList logins;
        for (int i = 0; i < m_urlCombo->count(); i++)
            logins << QPair<QString, QString>(m_urlCombo->itemText(i),
                                              m_urlCombo->itemData(i).toString());

        if (LoginIO::exportLogins(logins, fileName))
        {
            QMessageBox::information(this, tr("Exporting logins succeeded"), tr("Successfully exported logins to '") + fileName + "'");
        }
        else
        {
            QMessageBox::warning(this, tr("Exporting logins failed"), tr("Failed to export logins to '") + fileName + "'");
        }
    }
}

void MainWindow::showInstructionsDlg()
{
    InstructionsDlg instructionsDlg(this);
    instructionsDlg.exec();
}

void MainWindow::showAboutDlg()
{
    AboutDlg aboutDlg(this);
    aboutDlg.exec();
}

void MainWindow::showAboutQtDlg()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::loadSettings()
{
    QSettings s(COMPANY, SOFTWARE);
    m_delay     = s.value("delay", m_defaultDelay).toInt();
    m_length    = s.value("length", m_defaultLength).toInt();
    m_autoCopy  = s.value("autoCopy", false).toBool();
    m_autoClear = s.value("autoClear", false).toBool();

    m_settingsDlg->setSettings(m_delay, m_length, m_autoCopy, m_autoClear);
    m_timeLine->setDuration(m_delay * 1000);

    // Read login data
    m_urlCombo->clear();
    int size = s.beginReadArray("logins");
    for (int i = 0; i < size; i++)
    {
        s.setArrayIndex(i);
        m_urlCombo->addItem(s.value("url").toString(),
                            s.value("user").toString());
    }
    s.endArray();

    m_urlCombo->model()->sort(0);

    updateUser(m_urlCombo->currentText());
}

void MainWindow::saveSettings()
{
    QSettings s(COMPANY, SOFTWARE);
    s.setValue("delay",     m_delay);
    s.setValue("length",    m_length);
    s.setValue("autoCopy",  m_autoCopy);
    s.setValue("autoClear", m_autoClear);

    // Write login data that user wants to be saved
    s.beginWriteArray("logins");
    for (int i = 0; i < m_urlCombo->count(); i++)
    {
        s.setArrayIndex(i);
        s.setValue("url",  m_urlCombo->itemText(i));
        s.setValue("user", m_urlCombo->itemData(i));
    }
    s.endArray();
}

void MainWindow::doGenerate()
{
    QString passwd = Engine::generate(m_masterEdit->text(),
                                      m_urlCombo->currentText(),
                                      m_userEdit->text(),
                                      m_length);

    // Enable the text field and  show the generated passwd
    m_passwdEdit->setEnabled(true);
    m_passwdEdit->setText(passwd);

    // Copy to clipboard if wanted
    if (m_autoCopy)
    {
        m_passwdEdit->selectAll();
        m_passwdEdit->copy();
    }

    // Start timer to slowly fade out the text
    m_timeLine->stop();
    m_timeLine->start();
}

void MainWindow::decreasePasswordAlpha(int frame)
{
    QColor color = QColor();
    color.setAlpha(255 - frame);
    QPalette palette = QPalette(m_passwdEdit->palette());
    palette.setColor(QPalette::Text, color);
    m_passwdEdit->setPalette(palette);
}

void MainWindow::invalidate()
{
    // Clear the password edit
    m_passwdEdit->setText("");

    // Clear the clipboard
    if (m_autoClear)
    {
        QApplication::clipboard()->clear();
    }

    m_passwdEdit->setEnabled(false);
}

void MainWindow::enableGenButton()
{
    m_genButton->setEnabled(m_masterEdit->text().length()      > 0 &&
                            m_urlCombo->currentText().length() > 0 &&
                            m_userEdit->text().length()        > 0);
}

void MainWindow::enableRmbButton()
{
    m_rmbButton->setEnabled(m_urlCombo->currentText().length() > 0 &&
                            m_userEdit->text().length()        > 0);
}

void MainWindow::rememberOrRemoveLogin()
{
    QString user = m_userEdit->text();
    QString url  = m_urlCombo->currentText();

    if (m_rmbButton->text() == m_rememberText)
    {
        // Remember url and user
        bool alreadyAdded = false;
        int index = m_urlCombo->findText(url);
        if (index != -1)
        {
            alreadyAdded = true;
            m_urlCombo->setItemData(index, user);
        }

        if (!alreadyAdded)
        {
            m_urlCombo->addItem(url, user);
            m_urlCombo->model()->sort(0);
        }

        saveSettings();

        m_rmbButton->setText(m_removeText);
    }
    else
    {
        // Remove url and user
        int index = m_urlCombo->findText(url);
        if (index != -1)
        {
            m_urlCombo->removeItem(index);
        }

        saveSettings();

        m_rmbButton->setText(m_rememberText);
    }
}

void MainWindow::updateUser(const QString & url)
{
    int index = m_urlCombo->findText(url);
    if (index != -1)
    {
        m_userEdit->setText(m_urlCombo->itemData(index).toString());
        m_rmbButton->setText(m_removeText);
    }
}

void MainWindow::setRmbButtonText(const QString & url)
{
    int index = m_urlCombo->findText(url);
    if (index != -1)
    {
        m_userEdit->setText(m_urlCombo->itemData(index).toString());
        m_rmbButton->setText(m_removeText);
        m_rmbButton->setToolTip(m_removeToolTip);
    }
    else
    {
        m_rmbButton->setText(m_rememberText);
        m_rmbButton->setToolTip(m_rememberToolTip);
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    QSettings s(COMPANY, SOFTWARE);
    s.setValue("x", x());
    s.setValue("y", y());

    QApplication::clipboard()->clear();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete m_timeLine;
}
