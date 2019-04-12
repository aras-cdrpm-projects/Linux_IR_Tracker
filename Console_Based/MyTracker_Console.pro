QT += core
QT += serialport
QT -= gui
QT += network

CONFIG += c++11

TARGET = MyTracker_Console
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    markerdetector.cpp \
    stereoProcess.cpp \
    testclass.cpp

LIBS += `pkg-config opencv --libs`

HEADERS += \
    markerdetector.h \
    stereoProcess.h \
    testclass.h
