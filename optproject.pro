#-------------------------------------------------
#
# Project created by QtCreator 2012-02-21T18:05:04
#
#-------------------------------------------------

QT       += core gui

TARGET = optproject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    core/vector3d.cpp \
    core/point3d.cpp \
    core/ray3d.cpp \
    core/virtual3dobject.cpp \
    core/trianglepolygon.cpp \
    core/plane3d.cpp

HEADERS  += mainwindow.h \
    core/vector3d.h \
    core/point3d.h \
    core/ray3d.h \
    core/virtual3dobject.h \
    core/trianglepolygon.h \
    core/plane3d.h

FORMS    += mainwindow.ui
