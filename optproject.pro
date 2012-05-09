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
    core/geometry/vector3d.cpp \
    core/geometry/point3d.cpp \
    core/geometry/ray3d.cpp \
    core/objects/virtual3dobject.cpp \
    core/objects/trianglepolygon.cpp \
    core/geometry/plane3d.cpp \
    core/tracer/scene.cpp \
    core/tracer/raypool.cpp \
    core/objects/pictureobject.cpp \
    core/geometry/transformationmatrix.cpp \
    core/tracer/renderedimage.cpp \
    core/tracer/physicalray.cpp \
    core/tracer/workerobject.cpp \
    core/tracer/renderinghelper.cpp \
    core/objects/lensobject.cpp \
    core/objects/rectanglepolygon.cpp \
    core/geometry/rectangle3d.cpp

HEADERS  += mainwindow.h \
    core/geometry/vector3d.h \
    core/geometry/point3d.h \
    core/geometry/ray3d.h \
    core/objects/virtual3dobject.h \
    core/objects/trianglepolygon.h \
    core/geometry/plane3d.h \
    core/tracer/scene.h \
    core/tracer/raypool.h \
    core/objects/pictureobject.h \
    core/tracer/renderedimage.h \
    core/tracer/physicalray.h \
    core/tracer/workerobject.h \
    core/tracer/renderinghelper.h \
    core/objects/lensobject.h \
    core/objects/rectanglepolygon.h \
    core/geometry/rectangle3d.h

FORMS    += mainwindow.ui
