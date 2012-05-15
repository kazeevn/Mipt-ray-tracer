# -------------------------------------------------
# Project created by QtCreator 2012-02-21T18:05:04
# -------------------------------------------------
QT += core \
    gui \
    opengl
TARGET = optproject
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    core/geometry/vector3d.cpp \
    core/geometry/point3d.cpp \
    core/geometry/ray3d.cpp \
    core/objects/virtual3dobject.cpp \
    core/objects/pictureobject.cpp \
    core/objects/lensobject.cpp \
    core/objects/helpers/trianglepolygon.cpp \
    core/objects/helpers/rectanglepolygon.cpp \
    core/objects/helpers/physicaltrianglepolygon.cpp \
    core/geometry/plane3d.cpp \
    core/tracer/scene.cpp \
    core/tracer/raypool.cpp \
    core/geometry/transformationmatrix.cpp \
    core/tracer/renderedimage.cpp \
    core/tracer/physicalray.cpp \
    core/tracer/workerobject.cpp \
    core/tracer/renderinghelper.cpp \
    core/geometry/rectangle3d.cpp \
    core/models/model.cpp \
    glwidget.cpp \
    core/objects/stubs/camerastub.cpp \
    core/models/cameramodel.cpp \
    core/models/lensmodel.cpp \
    core/objects/stubs/lensobjectstub.cpp \
    core/objects/stubs/pictureobjectstub.cpp \
    core/objects/stubs/virtual3dobjectstub.cpp \
    core/objects/thinlensobject.cpp \
    core/objects/stubs/thinlensobjectstub.cpp \
    core/objects/flatmirrorobject.cpp \
    core/objects/stubs/flatmirrorobjectstub.cpp \
    additemdialog.cpp \
    core/models/flatmirrormodel.cpp \
    core/models/thinlensmodel.cpp
HEADERS  += mainwindow.h \
    core/geometry/vector3d.h \
    core/geometry/point3d.h \
    core/geometry/ray3d.h \
    core/objects/virtual3dobject.h \
    core/objects/helpers/trianglepolygon.h \
    core/objects/lensobject.h \
    core/objects/helpers/rectanglepolygon.h \
    core/objects/pictureobject.h \
    core/objects/helpers/trianglepolygonsurface.h \
    core/objects/helpers/physicaltrianglepolygon.h \
    core/geometry/plane3d.h \
    core/tracer/scene.h \
    core/tracer/raypool.h \
    core/tracer/renderedimage.h \
    core/tracer/physicalray.h \
    core/tracer/workerobject.h \
    core/tracer/renderinghelper.h \
    core/geometry/rectangle3d.h \
    core/models/model.h \
    glwidget.h \
    core/objects/stubs/camerastub.h \
    core/models/cameramodel.h \
    core/models/lensmodel.h \
    core/objects/stubs/lensobjectstub.h \
    core/objects/stubs/pictureobjectstub.h \
    core/objects/stubs/virtual3dobjectstub.h \
    core/objects/thinlensobject.h \
    core/objects/stubs/thinlensobjectstub.h \
    core/objects/flatmirrorobject.h \
    core/objects/stubs/flatmirrorobjectstub.h \
    additemdialog.h \
    core/models/flatmirrormodel.h \
    core/models/thinlensmodel.h
FORMS    += mainwindow.ui \
    additemdialog.ui
