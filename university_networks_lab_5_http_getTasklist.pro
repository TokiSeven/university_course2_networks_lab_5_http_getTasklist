QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

TARGET = university_networks_lab_5_http_getTasklist

CONFIG -= console

SOURCES += main.cpp \
    mainwindow.cpp \
    ts_network.cpp \
    ts_system.cpp

HEADERS += \
    mainwindow.h \
    ts_network.h \
    ts_system.h

FORMS += \
    mainwindow.ui

LIBS += -lSetupapi -lpsapi
