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
#include "config.h"
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
#include <QSpinBox>
#include <QTimeLine>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, m_defaultMasterDelay(5)
, m_defaultLoginDelay(15)
, m_defaultLength(8)
, m_removeText(tr("&Remove URL && User"))
, m_saveText(tr("&Save URL && User"))
, m_saveToolTip(tr("Save current URL & User. Passwords are not saved."))
, m_removeToolTip(tr("Don't remember current URL & User anymore."))
, m_masterPasswordText(tr("<b>Master password:</b>"))
, m_masterDelay(m_defaultMasterDelay)
, m_loginDelay(m_defaultLoginDelay)
, m_autoCopy(false)
, m_autoClear(false)
, m_alwaysOnTop(true)
, m_masterEdit(new QLineEdit(this))
, m_masterLabel(new QLabel(this))
, m_userEdit(new QLineEdit(this))
, m_passwdEdit(new QLineEdit(this))
, m_urlCombo(new QComboBox(this))
, m_genButton(new QPushButton(tr("&Show password!"), this))
, m_saveButton(new QPushButton(m_saveText, this))
, m_lengthSpinBox(new QSpinBox(this))
, m_masterTimer(new QTimer)
, m_timeLine(new QTimeLine)
, m_settingsDlg(new SettingsDlg(this))
{
    setWindowTitle("Fleeting Password Manager");
    setWindowIcon(QIcon(":/fleetingpm.png"));

    initWidgets();
    initMenu();
    initBackground();
    loadSettings();

    // Apply window flags
    if (m_alwaysOnTop)
    {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }

    // Initialize the timer used when fading out the login details.
    m_timeLine->setDuration(m_loginDelay * 1000);
    connect(m_timeLine, SIGNAL(frameChanged(int)), this, SLOT(decreasePasswordAlpha(int)));
    connect(m_timeLine, SIGNAL(finished()), this, SLOT(invalidateAll()));
    m_timeLine->setFrameRange(0, 255);

    // Initialize the timer used when showing the
    // master password.
    m_masterTimer->setInterval(m_masterDelay * 60 * 1000);
    m_masterTimer->setSingleShot(true);
    connect(m_masterTimer, SIGNAL(timeout()),
        m_masterEdit, SLOT(clear()));
    connect(m_masterEdit, SIGNAL(textChanged(QString)),
        m_masterTimer, SLOT(start()));

    // Load previous location or center the window.
    centerOrRestoreLocation();
}

void MainWindow::centerOrRestoreLocation()
{
    // Calculate center coordinates
    QRect geom(QApplication::desktop()->availableGeometry());
    const int centerX = geom.width()  / 2 - frameGeometry().width()  / 2;
    const int centerY = geom.height() / 2 - frameGeometry().height() / 2;

    // Try to load previous location and use the
    // calculated center as the fallback
    QSettings s(Config::COMPANY, Config::SOFTWARE);
    const int x = s.value("x", centerX).toInt();
    const int y = s.value("y", centerY).toInt();
    move(x, y);

    // Restore saved window size
    const int width = s.value("width", 0).toInt();
    const int height = s.value("height", 0).toInt();
    if (width && height)
    {
        resize(QSize(width, height));
    }
}

void MainWindow::initBackground()
{
    setStyleSheet("MainWindow { background-image: url(:/back.png) }");
}

void MainWindow::initWidgets()
{
    // Create main layout as a grid layout
    // No need to store as a member.
    QGridLayout * layout = new QGridLayout();

    // Create a horizontal line between the user name field
    // and the password field.
    // No need to store as a member.
    QFrame * frame = new QFrame(this);
    frame->setFrameShape(QFrame::HLine);

    // Create and connect the clear-button
    // No need to store as a member.
    QPushButton * clearButton = new QPushButton(tr("&Clear"));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearFields()));
    clearButton->setAutoDefault(true);

    // Set tooltip for the master password field
    m_masterEdit->setToolTip(tr("Enter the master password common to all of your logins.\n"
                                "Note that the master password is not validated: different\n"
                                "master passwords just result in different "
                                "generated passwords."));

    // Set password-styled echo mode
    m_masterEdit->setEchoMode(QLineEdit::Password);

    // Make the URL combo box editable
    m_urlCombo->setEditable(true);

    // Set tooltip for the URL combo box
    m_urlCombo->setToolTip(tr("Enter or select a saved URL/ID.\n"
                              "For example \"facebook\", \"google\","
                              "\"gmail.com\", \"myserver\".."));

    // Set tooltip for the username combo box
    m_userEdit->setToolTip(tr("Enter your user name corresponding to the selected URL/ID."));

    // Set range from 8 to 32 for the password length spin box
    m_lengthSpinBox->setRange(8, 32);

    // Set tooltip for the password length spin box
    m_lengthSpinBox->setToolTip(tr("The length of the generated password."));

    // Set tooltip for the password field
    m_passwdEdit->setToolTip(tr("This is the generated password,\n"
                                "which is always the same with the same master password,\n"
                                "URL/ID and user name."));

    // Make the password line edit read-only and disabled by default
    m_passwdEdit->setReadOnly(true);
    m_passwdEdit->setEnabled(false);

    // Set the generate-button disabled by default
    m_genButton->setEnabled(false);
    m_genButton->setAutoDefault(true);

    // Set tooltip for the generate-button
    m_genButton->setToolTip(tr("Generate and show the password"));

    // Set the save-button disabled by default
    m_saveButton->setEnabled(false);
    m_saveButton->setAutoDefault(true);

    // Set tooltip for the save-button
    m_saveButton->setToolTip(m_saveToolTip);

    // Add the widgets to the grid layout
    const int COLS = 6;
    layout->addWidget(m_masterEdit,    0, 1, 1, COLS - 1);
    layout->addWidget(m_urlCombo,      1, 1, 1, COLS - 1);
    layout->addWidget(m_userEdit,      2, 1, 1, COLS - 1);
    layout->addWidget(frame,           3, 0, 1, COLS);
    layout->addWidget(m_lengthSpinBox, 4, 0);
    layout->addWidget(m_passwdEdit,    4, 1, 1, COLS - 1);
    layout->addWidget(m_genButton,     5, 0);
    layout->addWidget(m_saveButton,    5, 1, 1, COLS - 2);
    layout->addWidget(clearButton,     5, COLS - 1);

    // Add the "master password:"-label to the layout
    m_masterLabel->setText(m_masterPasswordText);
    setMasterPasswordLabelColor();
    layout->addWidget(m_masterLabel, 0, 0);

    // Create and add the "URL/ID:"-label to the layout
    // No need to store as a member.
    layout->addWidget(new QLabel(tr("<b>URL/ID:</b>")), 1, 0);

    // Create and add the "User name:"-label to the layout
    // No need to store as a member.
    layout->addWidget(new QLabel(tr("<b>User name:</b>")), 2, 0);

    // Create the central widget and set the layout to it.
    // No need to store as a member.
    QWidget * dummy = new QWidget();
    dummy->setLayout(layout);
    setCentralWidget(dummy);

    // Set tab order for the layout
    QWidget::setTabOrder(m_masterEdit   , m_urlCombo);
    QWidget::setTabOrder(m_urlCombo     , m_userEdit);
    QWidget::setTabOrder(m_userEdit     , m_lengthSpinBox);
    QWidget::setTabOrder(m_lengthSpinBox, m_genButton);
    QWidget::setTabOrder(m_genButton    , m_passwdEdit);
    QWidget::setTabOrder(m_passwdEdit   , m_saveButton);
    QWidget::setTabOrder(m_saveButton    , clearButton);

    // Connect the rest of the signals emitted by the widgets
    connectSignalsFromWidgets();
}

void MainWindow::connectSignalsFromWidgets()
{
    // Connect signal to update user name field when a URL gets
    // selected in the combo box
    connect(m_urlCombo, SIGNAL(activated(const QString &)),
        this, SLOT(updateUser(const QString &)));

    // Decide the text of the save/remove-button if URL is changed
    connect(m_urlCombo, SIGNAL(editTextChanged(const QString &)),
        this, SLOT(toggleSaveButtonText()));

    // Decide the text of the save/remove-button if user name is changed
    connect(m_userEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(toggleSaveButtonText()));

    // Decide the text of the save/remove-button if length is changed
    connect(m_lengthSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(toggleSaveButtonText()));

    // Connect signal to update user name field if URL-field is changed
    connect(m_urlCombo, SIGNAL(editTextChanged(const QString &)),
        this, SLOT(updateUser(const QString &)));

    // Save of remove a saved login when save/remove-button is clicked
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(saveOrRemoveLogin()));

    // Generate the password when generate-button is clicked
    connect(m_genButton, SIGNAL(clicked()), this, SLOT(doGenerate()));

    // Invalidate generated password if one of the inputs gets changed
    connect(m_masterEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(invalidate()));
    connect(m_urlCombo, SIGNAL(editTextChanged(const QString &)),
        this, SLOT(invalidate()));
    connect(m_userEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(invalidate()));

    // Enable generate-button if all inputs are valid
    connect(m_masterEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableGenButton()));
    connect(m_urlCombo, SIGNAL(editTextChanged(const QString &)),
        this, SLOT(enableGenButton()));
    connect(m_userEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableGenButton()));

    // Enable save-button if the URL not already saved
    connect(m_masterEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableSaveButton()));
    connect(m_urlCombo, SIGNAL(editTextChanged(const QString &)),
        this, SLOT(enableSaveButton()));
    connect(m_userEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableSaveButton()));

    // Set "master password"-label color
    connect(m_masterEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(setMasterPasswordLabelColor()));
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
    quitAct->setShortcut(QKeySequence("Ctrl+W"));
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
    m_settingsDlg->setSettings(m_masterDelay,
        m_loginDelay, m_autoCopy, m_autoClear, m_alwaysOnTop);
    if (m_settingsDlg->exec() == QDialog::Accepted)
    {
        m_settingsDlg->getSettings(m_masterDelay,
            m_loginDelay, m_autoCopy, m_autoClear, m_alwaysOnTop);
        m_timeLine->setDuration(m_loginDelay * 1000);
        m_masterTimer->setInterval(m_masterDelay * 60 * 1000);
        saveSettings();
    }
}

void MainWindow::importLogins()
{
    const QString fileName = QFileDialog::getOpenFileName(this,
        tr("Import logins"), QDir::homePath(),
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
                const QString url    = logins.at(i).url();
                const QString user   = logins.at(i).userName();
                const int     length = logins.at(i).passwordLength();

                if (m_loginHash.contains(url))
                {
                    updated++;
                }
                else
                {
                    m_urlCombo->addItem(url);
                    newLogins++;
                }

                m_loginHash[url] = LoginData(url, user, length);
            }

            m_urlCombo->model()->sort(0);
            m_urlCombo->setCurrentIndex(0);
            saveSettings();

            QString message(tr("Successfully imported logins from '") +
                fileName + tr("': %1 new, %2 updated."));
            message = message.arg(newLogins).arg(updated);
            QMessageBox::information(this, tr("Importing logins succeeded"),
                message);
        }
        else
        {
            QMessageBox::warning(this, tr("Exporting logins failed"),
                tr("Failed to import logins from '") + fileName + "'");
        }
    }
}

void MainWindow::exportLogins()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export logins"), QDir::homePath(),
        tr("Fleeting Password Manager files (*.fpm)"));

    if (fileName.length() > 0)
    {
        if (!fileName.endsWith(".fpm"))
            fileName.append(".fpm");

        if (LoginIO::exportLogins(m_loginHash.values(), fileName))
        {
            QMessageBox::information(this, tr("Exporting logins succeeded"),
                tr("Successfully exported logins to '") + fileName + "'");
        }
        else
        {
            QMessageBox::warning(this, tr("Exporting logins failed"),
                tr("Failed to export logins to '") + fileName + "'");
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
    QSettings s(Config::COMPANY, Config::SOFTWARE);

    m_masterDelay = s.value("masterDelay", m_defaultMasterDelay).toInt();
    m_loginDelay  = s.value("delay", m_defaultLoginDelay).toInt();
    m_autoCopy    = s.value("autoCopy", false).toBool();
    m_autoClear   = s.value("autoClear", false).toBool();
    m_alwaysOnTop = s.value("alwaysOnTop", true).toBool();

    const int defaultLength = s.value("length", m_defaultLength).toInt();

    m_settingsDlg->setSettings(m_masterDelay,
        m_loginDelay, m_autoCopy, m_autoClear, m_alwaysOnTop);
    m_timeLine->setDuration(m_loginDelay * 1000);
    m_masterTimer->setInterval(m_masterDelay * 60 * 1000);

    // Read login data
    m_urlCombo->clear();
    m_loginHash.clear();

    // Loop through saved logins
    const int size = s.beginReadArray("logins");
    for (int i = 0; i < size; i++)
    {
        s.setArrayIndex(i);

        // Read the url
        const QString url = s.value("url").toString();

        // Add url to the combo box
        m_urlCombo->addItem(url);

        // Add url to the login data hash
        m_loginHash[url] = LoginData(url,
            s.value("user").toString(),
            s.value("length", defaultLength).toInt());
    }
    s.endArray();

    // Sort the combo box
    m_urlCombo->model()->sort(0);

    // Set the current index to zero
    // and update related fields.
    m_urlCombo->setCurrentIndex(0);
    updateUser(m_urlCombo->currentText());
}

void MainWindow::saveSettings()
{
    QSettings s(Config::COMPANY, Config::SOFTWARE);
    s.setValue("delay",       m_loginDelay);
    s.setValue("masterDelay", m_masterDelay);
    s.setValue("autoCopy",    m_autoCopy);
    s.setValue("autoClear",   m_autoClear);
    s.setValue("alwaysOnTop", m_alwaysOnTop);

    // Write login data that user wants to be saved
    s.beginWriteArray("logins");
    QList<LoginData> values = m_loginHash.values();
    for (int i = 0; i < values.count(); i++)
    {
        s.setArrayIndex(i);
        s.setValue("url",    values.at(i).url());
        s.setValue("user",   values.at(i).userName());
        s.setValue("length", values.at(i).passwordLength());
    }
    s.endArray();
}

void MainWindow::doGenerate()
{
    QString passwd = Engine::generate(m_masterEdit->text(),
        m_urlCombo->currentText(),
        m_userEdit->text(),
        m_lengthSpinBox->value());

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
    // Clear the login details
    m_passwdEdit->setText("");

    // Clear the clipboard
    if (m_autoClear)
    {
        QApplication::clipboard()->clear();
    }

    m_passwdEdit->setEnabled(false);
}

void MainWindow::invalidateAll()
{
    invalidate();

    m_userEdit->setText("");
    m_urlCombo->setEditText("");
}

void MainWindow::enableGenButton()
{
    m_genButton->setEnabled(m_masterEdit->text().length() > 0 &&
        m_urlCombo->currentText().length() > 0 &&
        m_userEdit->text().length() > 0);
}

void MainWindow::enableSaveButton()
{
    m_saveButton->setEnabled(m_urlCombo->currentText().length() > 0 &&
        m_userEdit->text().length() > 0);
}

void MainWindow::setMasterPasswordLabelColor()
{
    float scale = static_cast<float>(m_masterEdit->text().length()) / 8;
    scale = scale > 1.0 ? 1.0 : scale;

    QColor color;
    color.setRed(255.0 * (1.0 - scale) * 0.75);
    color.setGreen(255.0 * scale  * 0.75);
    color.setBlue(0);

    QPalette palette;
    palette.setColor(QPalette::Foreground, color);
    m_masterLabel->setPalette(palette);
}

void MainWindow::saveOrRemoveLogin()
{
    const QString user = m_userEdit->text();
    const QString url  = m_urlCombo->currentText();

    if (m_saveButton->text() == m_saveText)
    {
        // Save url and user
        const int index = m_urlCombo->findText(url);
        if (index == -1)
        {
            m_urlCombo->addItem(url);
            m_urlCombo->model()->sort(0);
        }

        // Update the corresponding login data in the hash
        m_loginHash[url] = LoginData(url, user, m_lengthSpinBox->value());

        // Save settings
        saveSettings();

        // Change the button text to "remove"
        m_saveButton->setText(m_removeText);

#ifndef __ANDROID__
        // Show a message box
        QString message(tr("Added to the saved logins: '") +
            url + tr("'."));
        QMessageBox::information(this, Config::NAME, message);
#endif
    }
    else
    {
        // Remove url and user
        const int index = m_urlCombo->findText(url);
        if (index != -1)
        {
            m_urlCombo->removeItem(index);
        }

        // Remove the corresponding login data from the hash
        m_loginHash.remove(url);

        // Save settings
        saveSettings();

        // Update the button text
        toggleSaveButtonText();

#ifndef __ANDROID__
        // Show a message box
        QString message(tr("Removed from the saved logins: '") +
            url + tr("'."));
        QMessageBox::information(this, Config::NAME, message);
#endif
    }
}

void MainWindow::updateUser(const QString & url)
{
    if (m_loginHash.contains(url))
    {
        // Update the user name field
        m_userEdit->setText(m_loginHash.value(url).userName());

        // Update the password length spinbox
        m_lengthSpinBox->setValue(m_loginHash.value(url).passwordLength());

        // Change the text in save-button to "remove"
        m_saveButton->setText(m_removeText);
    }
}

void MainWindow::toggleSaveButtonText()
{
    // Set remove-text if url is saved.
    // Set save-text if url is not
    // saved or user/length is changed.

    const QString url = m_urlCombo->currentText();
    if (m_loginHash.contains(url))
    {
        const QString currentUser   = m_userEdit->text();
        const int     currentLength = m_lengthSpinBox->value();
        const QString savedUser     = m_loginHash.value(url).userName();
        const int     savedLength   = m_loginHash.value(url).passwordLength();

        if (currentUser != savedUser || currentLength != savedLength)
        {
            m_saveButton->setText(m_saveText);
            m_saveButton->setToolTip(m_saveToolTip);
        }
        else
        {
            m_saveButton->setText(m_removeText);
            m_saveButton->setToolTip(m_removeToolTip);
        }
    }
    else
    {
        m_saveButton->setText(m_saveText);
        m_saveButton->setToolTip(m_saveToolTip);
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    QSettings s(Config::COMPANY, Config::SOFTWARE);
    s.setValue("x", x());
    s.setValue("y", y());
    s.setValue("width", width());
    s.setValue("height", height());

    QApplication::clipboard()->clear();
    event->accept();
}

void MainWindow::clearFields()
{
    m_userEdit->clear();
    m_urlCombo->clearEditText();
    m_lengthSpinBox->setValue(m_defaultLength);
}

MainWindow::~MainWindow()
{
    delete m_timeLine;
    delete m_masterTimer;
}
