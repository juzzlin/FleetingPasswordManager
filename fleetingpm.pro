TEMPLATE     = app
TARGET       = fleetingpm
DEPENDPATH  += . data/icons data/images
INCLUDEPATH += .
QT          += xml

# Input
RESOURCES += data/icons/Icons.qrc data/images/Images.qrc data/doc/Instructions.qrc

HEADERS += \
    src/mainwindow.h \
    src/settingsdlg.h \
    src/engine.h \
    src/aboutdlg.h \
    src/instructionsdlg.h \
    src/loginio.h \
    src/version.h

SOURCES += \
    src/mainwindow.cpp \
    src/settingsdlg.cpp \
    src/engine.cpp \
    src/aboutdlg.cpp \
    src/main.cpp \
    src/instructionsdlg.cpp \
    src/loginio.cpp

# Check if PREFIX environment variable is set.
# If not, then assume /usr.
_PREFIX = $$(PREFIX)
isEmpty(_PREFIX) {
    _PREFIX = /usr
}

message("The project will be installed to "$$_PREFIX)

target.path    = $$_PREFIX/bin
desktop.path   = $$_PREFIX/share/applications
desktop.files += data/fleetingpm.desktop
icon1.path     = $$_PREFIX/share/icons/hicolor/64x64/apps
icon1.files   += data/icons/fleetingpm.png
icon2.path     = $$_PREFIX/share/pixmaps
icon2.files   += data/icons/fleetingpm.png
INSTALLS      += target desktop icon1 icon2

