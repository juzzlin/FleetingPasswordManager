TEMPLATE     = app
TARGET       = fleetingpm
DEPENDPATH  += . data/icons data/images
INCLUDEPATH += .
QT          += xml

# Input
RESOURCES += data/icons/Icons.qrc data/images/Images.qrc data/doc/Instructions.qrc
win32:RC_FILE = data/icons/Windows.rc

HEADERS += \
    src/Qt/aboutdlg.h \
    src/Qt/config.h \
    src/Qt/engine.h \
    src/Qt/instructionsdlg.h \
    src/Qt/logindata.h \
    src/Qt/loginio.h \
    src/Qt/mainwindow.h \
    src/Qt/settingsdlg.h

SOURCES += \
    src/Qt/aboutdlg.cpp \
    src/Qt/config.cpp \
    src/Qt/engine.cpp \
    src/Qt/instructionsdlg.cpp \
    src/Qt/logindata.cpp \
    src/Qt/loginio.cpp  \
    src/Qt/main.cpp \
    src/Qt/mainwindow.cpp \
    src/Qt/settingsdlg.cpp

# Check if PREFIX environment variable is set.
# If not, then assume /usr.
_PREFIX = $$(PREFIX)
isEmpty(_PREFIX) {
    _PREFIX = /usr
}

target.path    = $$_PREFIX/bin
desktop.path   = $$_PREFIX/share/applications
desktop.files += data/fleetingpm.desktop
icon1.path     = $$_PREFIX/share/icons/hicolor/64x64/apps
icon1.files   += data/icons/fleetingpm.png
icon2.path     = $$_PREFIX/share/pixmaps
icon2.files   += data/icons/fleetingpm.png
INSTALLS      += target desktop icon1 icon2

