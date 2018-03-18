#-------------------------------------------------
#
# Project created by QtCreator 2018-02-13T11:15:19
#
#-------------------------------------------------

QT       += core gui multimedia quickwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SwMusic
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    fileinfo.cpp \
    tablemodel.cpp \
    lrcwidget.cpp \
    savethread.cpp \
    imageprovider.cpp \
    setting.cpp \
    listmodel.cpp

HEADERS  += dialog.h \
    fileinfo.h \
    tablemodel.h \
    lrcwidget.h \
    savethread.h \
    imageprovider.h \
    setting.h \
    listmodel.h

FORMS    += dialog.ui \
    setting.ui

RESOURCES += \
    image.qrc

DISTFILES +=
RC_FILE = logo.rc
