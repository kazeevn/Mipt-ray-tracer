#include "scene.h"

#include <QColor>
#include <QThreadPool>
#include <QSettings>
#include <QByteArray>
#include <QBuffer>

#include "core/geometry/point3d.h"
#include "core/tracer/renderedimage.h"
#include "core/tracer/physicalray.h"
#include "core/objects/thinlensobject.h"
#include "core/objects/pictureobject.h"
#include "core/objects/lensobject.h"
#include "core/objects/flatmirrorobject.h"
#include "core/objects/stubs/pictureobjectstub.h"
#include "core/objects/stubs/lensobjectstub.h"
#include "core/objects/stubs/thinlensobjectstub.h"
#include "core/objects/stubs/flatmirrorobjectstub.h"

Scene::Scene(QObject *parent) :
    QObject(parent),
    m_renderingHelper(0),
    m_objects()
{
}

Scene::~Scene()
{
    qDeleteAll(m_objects);
    m_objects.clear();
}

void Scene::addObject(const QString& name, Virtual3DObject *object)
{
    object->setName(name);
    m_objects.append(object);
}

void Scene::addStubObject(const QString &name, Virtual3DObjectStub *object)
{
    object->setName(name);
    m_stubs.append(object);
}

void Scene::traceRay(PhysicalRay *ray)
{
    Point3D *point = 0, *minpoint = 0;
    Virtual3DObject *nearestObj = 0;
    double mindist = 0;
    // Searching for nearest collision with any object
    Q_FOREACH(Virtual3DObject* obj, m_objects)
    {
        point = NULL;
        PictureObject* picobj = dynamic_cast<PictureObject*>(obj);
        if (picobj)
            point = picobj->intercrossWithRay(*ray);
        LensObject* lensobj = dynamic_cast<LensObject*>(obj);
        if (lensobj)
            point = lensobj->intercrossWithRay(*ray);
        ThinLensObject* thinlensobj = dynamic_cast<ThinLensObject*>(obj);
        if (thinlensobj)
            point = thinlensobj->intercrossWithRay(*ray);
        FlatMirrorObject* flatmirrorobj = dynamic_cast<FlatMirrorObject*>(obj);
        if (flatmirrorobj)
            point = flatmirrorobj->intercrossWithRay(*ray);
        if (point != NULL) {
            double dist = point->dist(ray->point());
            if ((minpoint == NULL) || (dist < mindist)) {
                if (minpoint != NULL)
                    delete minpoint;
                minpoint = point;
                mindist = dist;
                nearestObj = obj;
            } else
                delete point;
        }
    }
    if (nearestObj) {
        nearestObj->processIntersection(*ray, *minpoint);
        delete minpoint;
    } else
        RenderedImage::Instance().rayTraceResult(*ray, QColor(Qt::black).rgb());
}

void Scene::startRendering(const Point3D &cameraPos, const Rectangle3D &screen, const QSize &picsize)
{
    RenderedImage::Instance().init(picsize);
    if (m_renderingHelper) delete m_renderingHelper;
    m_renderingHelper = new RenderingHelper(cameraPos, screen, picsize);
    m_renderingHelper->setAutoDelete(false);
    connect(m_renderingHelper, SIGNAL(renderingFinished()), this, SIGNAL(renderingFinished()));
    QThreadPool::globalInstance()->start(m_renderingHelper);
}

void Scene::startRendering()
{
    qDebug() << m_camera->summit();
    startRendering(m_camera->summit(), *m_camera, m_camera->size());
}


void Scene::createObjectsFromStubs()
{
    qDeleteAll(m_objects);
    m_objects.clear();

    Q_FOREACH(Virtual3DObjectStub* obj, m_stubs)
    {
        PictureObjectStub *pstub = dynamic_cast<PictureObjectStub*>(obj);
        if (pstub)
            addObject(pstub->name(), new PictureObject(pstub->point(), pstub->horizontalVect(), pstub->verticalVect(), pstub->image()));
        LensObjectStub *lstub = dynamic_cast<LensObjectStub*>(obj);
        if (lstub)
            addObject(lstub->name(), new LensObject(lstub->point(), lstub->horizontalVect(), lstub->verticalVect(),
                                                    lstub->heightMap1(), lstub->heightMap2(),
                                                    lstub->size(), lstub->height(), lstub->refractiveIndex()));
        ThinLensObjectStub *tlstub = dynamic_cast<ThinLensObjectStub*>(obj);
        if (tlstub)
            addObject(tlstub->name(), new ThinLensObject(tlstub->point(), tlstub->horizontalVect(), tlstub->verticalVect(),
                                                         tlstub->bitmask(), tlstub->focus()));

        FlatMirrorObjectStub *mirstub = dynamic_cast<FlatMirrorObjectStub*>(obj);
        if (mirstub)
            addObject(mirstub->name(), new FlatMirrorObject(mirstub->point(), mirstub->horizontalVect(), mirstub->verticalVect(),
                                                            mirstub->bitmask(), mirstub->reflcoef()));
    }
}

void Scene::saveStubsToFile(const QString &filename)
{
    QSettings sfile(filename, QSettings::IniFormat);
    sfile.clear();
    Q_FOREACH(Virtual3DObjectStub* obj, m_stubs) {
        sfile.beginGroup(obj->name());
        sfile.setValue("PointX", obj->point().x);
        sfile.setValue("PointY", obj->point().y);
        sfile.setValue("PointZ", obj->point().z);
        sfile.setValue("HVectX", obj->horizontalVect().x);
        sfile.setValue("HVectY", obj->horizontalVect().y);
        sfile.setValue("HVectZ", obj->horizontalVect().z);
        sfile.setValue("VVectX", obj->verticalVect().x);
        sfile.setValue("VVectY", obj->verticalVect().y);
        sfile.setValue("VVectZ", obj->verticalVect().z);
        PictureObjectStub *pstub = dynamic_cast<PictureObjectStub*>(obj);
        if (pstub) {
            QByteArray arr;
            QBuffer buf(&arr);
            pstub->image().save(&buf, "PNG");

            sfile.setValue("Type", "Picture");
            sfile.setValue("Image", QString::fromLatin1(arr.toBase64().data()));
        }

        LensObjectStub *lstub = dynamic_cast<LensObjectStub*>(obj);
        if (lstub) {
            QByteArray arr1;
            QBuffer buf1(&arr1);
            lstub->heightMap1().save(&buf1, "PNG");
            QByteArray arr2;
            QBuffer buf2(&arr2);
            lstub->heightMap1().save(&buf2, "PNG");
            sfile.setValue("Type", "Lens");
            sfile.setValue("Height", lstub->height());
            sfile.setValue("RefractiveIndex", lstub->refractiveIndex());
            sfile.setValue("HeightMap1", QString::fromLatin1(arr1.toBase64().data()));
            sfile.setValue("HeightMap2", QString::fromLatin1(arr2.toBase64().data()));
            sfile.setValue("PolyWidth", lstub->size().width());
            sfile.setValue("PolyHeight", lstub->size().height());
        }

        ThinLensObjectStub *tlstub = dynamic_cast<ThinLensObjectStub*>(obj);
        if (tlstub) {
            QByteArray arr;
            QBuffer buf(&arr);
            tlstub->bitmask().save(&buf, "PNG");

            sfile.setValue("Type", "ThinLens");
            sfile.setValue("Focus", tlstub->focus());
            sfile.setValue("Bitmask", QString::fromLatin1(arr.toBase64().data()));
        }

        FlatMirrorObjectStub *fmstub = dynamic_cast<FlatMirrorObjectStub*>(obj);
        if (fmstub) {
            QByteArray arr;
            QBuffer buf(&arr);
            fmstub->bitmask().save(&buf, "PNG");

            sfile.setValue("Type", "FlatMirror");
            sfile.setValue("ReflCoef", fmstub->reflcoef());
            sfile.setValue("Bitmask", QString::fromLatin1(arr.toBase64().data()));
        }

        sfile.endGroup();
    }

    sfile.beginGroup("CameraObject");
    sfile.setValue("Type", "Camera");

    sfile.setValue("SummitX", m_camera->summit().x);
    sfile.setValue("SummitY", m_camera->summit().y);
    sfile.setValue("SummitZ", m_camera->summit().z);

    sfile.setValue("PointX", m_camera->point().x);
    sfile.setValue("PointY", m_camera->point().y);
    sfile.setValue("PointZ", m_camera->point().z);

    sfile.setValue("HVectX", m_camera->horizontalVect().x);
    sfile.setValue("HVectY", m_camera->horizontalVect().y);
    sfile.setValue("HVectZ", m_camera->horizontalVect().z);

    sfile.setValue("VVectX", m_camera->verticalVect().x);
    sfile.setValue("VVectY", m_camera->verticalVect().y);
    sfile.setValue("VVectZ", m_camera->verticalVect().z);

    sfile.setValue("Width", m_camera->size().width());
    sfile.setValue("Height", m_camera->size().height());

    sfile.endGroup();

    sfile.sync();
}

void Scene::loadStubsFromFile(const QString &filename)
{
    qDeleteAll(m_stubs);
    m_stubs.clear();

    QSettings settings(filename, QSettings::IniFormat);
    Q_FOREACH(QString curg, settings.childGroups()) {
        settings.beginGroup(curg);
        Point3D point(settings.value("PointX", 0.0).toDouble(), settings.value("PointY", 0.0).toDouble(), settings.value("PointZ", 0.0).toDouble());
        Vector3D hvect(settings.value("HVectX", 0.0).toDouble(), settings.value("HVectY", 0.0).toDouble(), settings.value("HVectZ", 0.0).toDouble());
        Vector3D vvect(settings.value("VVectX", 0.0).toDouble(), settings.value("VVectY", 0.0).toDouble(), settings.value("VVectZ", 0.0).toDouble());

        QString type = settings.value("Type").toString();
        if (type == "Camera") {
            Point3D summit(settings.value("SummitX", 0.0).toDouble(), settings.value("SummitY", 0.0).toDouble(), settings.value("SummitZ", 0.0).toDouble());
            QSize size(settings.value("Width", 0).toInt(), settings.value("Height", 0).toInt());
            addCamera(new CameraStub(point, hvect, vvect, summit, size));
        } else if (type == "Picture") {
            QImage pic = QImage::fromData(QByteArray::fromBase64(settings.value("Image").toByteArray()), "PNG");
            addStubObject(curg, new PictureObjectStub(point, hvect, vvect, pic));
        } else if (type == "Lens") {
            QImage heightMap1 = QImage::fromData(QByteArray::fromBase64(settings.value("HeightMap1").toByteArray()), "PNG");
            QImage heightMap2 = QImage::fromData(QByteArray::fromBase64(settings.value("HeightMap2").toByteArray()), "PNG");
            QSize size(settings.value("PolyWidth").toInt(), settings.value("PolyHeight").toInt());
            addStubObject(curg, new LensObjectStub(point, hvect, vvect, heightMap1, heightMap2, size, settings.value("Height").toDouble(), settings.value("RefractiveIndex").toDouble()));
        } else if (type == "ThinLens") {
            QImage bitmask = QImage::fromData(QByteArray::fromBase64(settings.value("Bitmask").toByteArray()), "PNG");
            addStubObject(curg, new ThinLensObjectStub(point, hvect, vvect, bitmask, settings.value("Focus").toDouble()));
        } else if (type == "FlatMirror") {
            QImage bitmask = QImage::fromData(QByteArray::fromBase64(settings.value("Bitmask").toByteArray()), "PNG");
            addStubObject(curg, new FlatMirrorObjectStub(point, hvect, vvect, bitmask, settings.value("ReflCoef").toDouble()));
        }

        settings.endGroup();
    }
}
