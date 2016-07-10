#-------------------------------------------------
#
# Project created by QtCreator 2016-06-25T14:08:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGTalker
TEMPLATE = app


SOURCES += main.cpp\
        clientwindow.cpp \
    session.cpp \
    network.cpp \
    clientnetwork.cpp \
    chatnetwork.cpp \
    chatserver.cpp \
    userbutton.cpp

HEADERS  += clientwindow.h \
    session.h \
    network.h \
    clientnetwork.h \
    chatnetwork.h \
    chatserver.h \
    userbutton.h

FORMS    += clientwindow.ui \
    session.ui
