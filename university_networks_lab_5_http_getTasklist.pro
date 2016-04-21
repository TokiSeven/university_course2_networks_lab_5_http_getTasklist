QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

TARGET = university_networks_lab_5_http_getTasklist

CONFIG -= console

SOURCES += main.cpp \
    ts_system.cpp \
    ts_network.cpp \
    mainwindow.cpp

HEADERS += \
    ts_system.h \
    ts_network.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

LIBS += -lSetupapi -lpsapi
