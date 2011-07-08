TEMPLATE     = app
TARGET       = fleetingpm
DEPENDPATH  += . data/icons data/images
INCLUDEPATH += .
DEFINES     += VERSION=\\\"2.0\\\"

# Input
RESOURCES += data/icons/Icons.qrc data/images/Images.qrc

HEADERS += \
    src/mainwindow.h \
    src/settingsdlg.h \
    src/engine.h \
    src/aboutdlg.h

SOURCES += \
    src/mainwindow.cpp \
    src/settingsdlg.cpp \
    src/engine.cpp \
    src/aboutdlg.cpp \
    src/main.cpp

_PREFIX = $$(PREFIX)
isEmpty(_PREFIX) {
    _PREFIX = /usr
}

message("The project will be installed to "$$_PREFIX)

target.path    = $$_PREFIX/bin
desktop.path   = $$_PREFIX/share/applications
desktop.files += data/fleetingpm.desktop
icon.path      = $$_PREFIX/share/pixmaps
icon.files    += data/icons/fleetingpm.png
INSTALLS      += target desktop icon
