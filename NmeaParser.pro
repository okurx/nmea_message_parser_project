#-------------------------------------------------
#
# Project created by Burak Okur 2023-08-05
#
#-------------------------------------------------

# NMEA Message Parser Project
# This project provides a parser for NMEA messages commonly used in GPS and navigation systems.
# It can extract relevant information from NMEA sentences and provide structured data.
# Feel free to use and modify this code for your own projects. If you have any questions or suggestions,
# please reach out to me at [burak1837burak@gmail.com].

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
