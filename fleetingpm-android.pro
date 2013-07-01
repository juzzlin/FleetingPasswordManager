TEMPLATE    = app
TARGET      = fleetingpm
DEPENDPATH  += . src data/doc data/icons data/images
INCLUDEPATH += . src
QT          += widgets xml
DEFINES     += UseQt5=ON PROGRAM_VERSION=\\\"2.8.8-android\\\"

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

OTHER_FILES += \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/values-rs/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-nl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fr/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/values-rs/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-nl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fr/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java
