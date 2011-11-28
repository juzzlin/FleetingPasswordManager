TEMPLATE     = app
TARGET       = fleetingpm
DEPENDPATH  += . data/icons data/images
INCLUDEPATH += .
QT          += xml

# Input
RESOURCES += data/icons/Icons.qrc data/images/Images.qrc data/doc/Instructions.qrc
win32:RC_FILE = data/icons/Windows.rc

HEADERS += \
    src/aboutdlg.h \
    src/config.h \
    src/engine.h \
    src/instructionsdlg.h \
    src/logindata.h \
    src/loginio.h \
    src/mainwindow.h \
    src/settingsdlg.h

SOURCES += \
    src/aboutdlg.cpp \
    src/config.cpp \
    src/engine.cpp \
    src/instructionsdlg.cpp \
    src/logindata.cpp \
    src/loginio.cpp  \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdlg.cpp

# Installing the app like this makes sense only on Linux.
linux-g++ {

    # Check if PREFIX environment variable is set.
    # If not, then assume /usr.
    _PREFIX = $$(PREFIX)
    isEmpty(_PREFIX) {
        _PREFIX = /usr
    }

    message("Linux build. The project will be installed to "$$_PREFIX)

    target.path    = $$_PREFIX/bin
    desktop.path   = $$_PREFIX/share/applications
    desktop.files += data/fleetingpm.desktop
    icon1.path     = $$_PREFIX/share/icons/hicolor/64x64/apps
    icon1.files   += data/icons/fleetingpm.png
    icon2.path     = $$_PREFIX/share/pixmaps
    icon2.files   += data/icons/fleetingpm.png
    INSTALLS      += target desktop icon1 icon2
}
