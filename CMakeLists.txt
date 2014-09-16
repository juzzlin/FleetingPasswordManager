project(FleetingPasswordManager)

# Global version
set(VERSION "2.9.0")

# Enable Qt5 support
option(UseQt5 "UseQt5" OFF)

# Enable install layout required by USC.
option(Ubuntu "Ubuntu" OFF)

# Enable standard Linux/Unix install layout.
option(ReleaseBuild "ReleaseBuild" OFF)

if(UseQt5)
    message(STATUS "Using Qt5.")
    cmake_minimum_required(VERSION 2.8.8)
else()
    message(STATUS "Using Qt4. Add -DUseQt5=ON to build for Qt5.")
    cmake_minimum_required(VERSION 2.8.7)
endif()

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -W -Wall -O3 -pedantic")

set(BINARY_NAME "fleetingpm")

add_definitions(-DPROGRAM_VERSION="${VERSION}")

if (UseQt5)
    add_definitions(-DUSE_QT5)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Xml REQUIRED)
    find_package(Qt5Widgets REQUIRED)
else()
    # Find Qt4 and needed additional components.
    find_package(Qt4 4.7.0 REQUIRED COMPONENTS QtGui QtXml)
    include(${QT_USE_FILE})
    include_directories(${QT_INCLUDES})
endif()

# Set sources
set(SRC
    src/aboutdlg.cpp
    src/config.cpp
    src/engine.cpp
    src/instructionsdlg.cpp
    src/logindata.cpp
    src/loginio.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/settingsdlg.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(RCS 
    ${CMAKE_SOURCE_DIR}/data/icons/Icons.qrc
    ${CMAKE_SOURCE_DIR}/data/images/Images.qrc
    ${CMAKE_SOURCE_DIR}/data/doc/Instructions.qrc) 

if (UseQt5)
    qt5_add_resources(RC_SRC ${RCS})
else()
    set(MOC_HDRS
        src/aboutdlg.h
        src/instructionsdlg.h
        src/mainwindow.h
        src/settingsdlg.h)
    qt4_add_resources(RC_SRC ${RCS})
    qt4_wrap_cpp(MOC_SRC ${MOC_HDRS})
endif()

# Add target to copy runtime files to the binary dir.
add_custom_target(runtime ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)

# Copy desktop file
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(DesktopFileSourcePath)
    if(Ubuntu)
        set(DesktopFileSourcePath ${CMAKE_SOURCE_DIR}/data/fleetingpm.desktop.opt.in)
    else()
        set(DesktopFileSourcePath ${CMAKE_SOURCE_DIR}/data/fleetingpm.desktop.in)
    endif()
    add_custom_target(desktop-file ALL
        COMMAND cmake -E copy ${DesktopFileSourcePath}
            ${CMAKE_BINARY_DIR}/fleetingpm.desktop
    DEPENDS ${BINARY_NAME})
endif()

# Resource compilation for MinGW
if(MINGW)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/windowsrc.o
        COMMAND
            ${CMAKE_RC_COMPILER}
            -I${CMAKE_SOURCE_DIR} -i${CMAKE_SOURCE_DIR}/data/icons/Windows.rc
            -o ${CMAKE_CURRENT_BINARY_DIR}/windowsrc.o)
    set(SRC ${SRC} ${CMAKE_CURRENT_BINARY_DIR}/windowsrc.o)
endif()

# The main executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_executable(${BINARY_NAME} WIN32 ${SRC} ${MOC_SRC} ${RC_SRC})

if(UseQt5)
    qt5_use_modules(${BINARY_NAME} Widgets Xml)
else()
    target_link_libraries(${BINARY_NAME} ${QT_LIBRARIES})
endif()

# Set default install paths
set(BIN_PATH bin)
set(DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/${BINARY_NAME}/data)
set(DOC_PATH ${CMAKE_INSTALL_PREFIX}/share/${BINARY_NAME}/)

# Install binaries & data
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(ReleaseBuild)
        if(Ubuntu)
            message(STATUS "Installing to /opt.")
            set(BIN_PATH /opt/${BINARY_NAME})
            set(CMAKE_INSTALL_PREFIX /usr)
        endif()

        install(PROGRAMS ${BINARY_NAME} DESTINATION ${BIN_PATH})
        install(FILES AUTHORS CHANGELOG COPYING README DESTINATION ${DOC_PATH})
        install(FILES fleetingpm.desktop DESTINATION share/applications)
        install(FILES data/icons/fleetingpm.png DESTINATION share/pixmaps)
        install(FILES data/icons/fleetingpm.png DESTINATION share/icons/hicolor/64x64/apps)

        # Create symlinks to targets
        if (Ubuntu)
            execute_process(COMMAND cmake -E create_symlink ${BIN_PATH}/${BINARY_NAME} /usr/bin/${BINARY_NAME})
        endif()
        
    else()

        install(PROGRAMS ${BINARY_NAME} DESTINATION .)
        install(FILES AUTHORS CHANGELOG COPYING README DESTINATION .)

        # CPack config to create e.g. self-extracting packages
        set(CPACK_BINARY_STGZ "ON")
        set(CPACK_BINARY_TGZ "ON")
        set(CPACK_BINARY_TZ "OFF")
        
        set(QT_VER_STR "qt4")
        if(UseQt5)
            set(QT_VER_STR "qt5")
        endif()
        
        set(CPACK_PACKAGE_FILE_NAME "fleetingpm-${VERSION}-linux-${CMAKE_HOST_SYSTEM_PROCESSOR}-${QT_VER_STR}")
        
        set(CPACK_RESOURCE_FILE_LICENSE "COPYING")
        set(CPACK_RESOURCE_FILE_README "README")
        include(CPack)
       
    endif()
endif()

