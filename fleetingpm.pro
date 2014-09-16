TEMPLATE    = app
TARGET      = fleetingpm
DEPENDPATH  += . src data/doc data/icons data/images
INCLUDEPATH += . src
QT          += widgets xml
DEFINES     += "UseQt5=ON" "PROGRAM_VERSION=\\\"2.9.0\\\""

# Check Qt version
!contains(QT_VERSION, ^5\\.[1-9]\\..*) {
message("Cannot build Qt Creator with Qt version $${QT_VERSION}.")
error("Use at least Qt 5.1.")
}

# Input
HEADERS += src/aboutdlg.h \
           src/config.h \
           src/engine.h \
           src/instructionsdlg.h \
           src/logindata.h \
           src/loginio.h \
           src/mainwindow.h \
           src/settingsdlg.h
           
SOURCES += src/aboutdlg.cpp \
           src/config.cpp \
           src/engine.cpp \
           src/instructionsdlg.cpp \
           src/logindata.cpp \
           src/loginio.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/settingsdlg.cpp
           
RESOURCES += data/doc/Instructions.qrc \
             data/icons/Icons.qrc \
             data/images/Images.qrc

