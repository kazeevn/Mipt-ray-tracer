#include "glwidget.h"
//#include <QDebug>

//#ifndef GL_MULTISAMPLE
//#define GL_MULTISAMPLE  0x809D
//#endif

#include "core/objects/stubs/lensobjectstub.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

    speed = 0.2;
    widget_width = width();
    widget_height = height();

    normalView(); //set normal camera view
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    glClearColor( 0.0, 0.0, 0.0, 1.0); // Select backgroung color

    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_STIPPLE);
    //Set object color as gradient
    glShadeModel(GL_SMOOTH);

    //set type of alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Make normal vector's leng=1 (for lighting)
    glEnable(GL_NORMALIZE);

    glEnable (GL_BLEND); //enabling alpha

    glFrontFace(GL_CCW);
}

void GLWidget::paintGL()
{
    int i;
    QList<Point3D> points;
    QImage image;
    //Set backgroung
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rotateCamera();
    drawGround( 10.0f, 1.0f, 0);

    //Using stack of matrix
    glPushMatrix();

        //Draw all objects

        glEnable(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //colors of glObject are replaced by texture. other - GL_REPLACE - combination
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


        float refl;
        int shine;

        //need to draw pictures then objects


        //Drawing images
        for( i = 0; i < Scene::Instance().stub_objects().size(); i++)
        {
            //checking type of object - lense or picture
            LensObjectStub* n = dynamic_cast<LensObjectStub*>(Scene::Instance().stub_objects()[i]);
            if (!(n))
            {
                shine = 10;
                refl = 0.5;
                //textures
                PictureObjectStub* picobj = (PictureObjectStub*)Scene::Instance().stub_objects()[i];
                image = convertToGLFormat(picobj->image());
                glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)image.width(), (GLsizei)image.height(), 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

                if (Scene::Instance().stub_objects()[i]->selected())
                {
                    glColor4f( 0.1, 0.1, 1.0, 1.0);
                } else {
                    glColor4f( 1.0, 1.0, 1.0, 1.0);
                }

                float specref[] = { refl, refl, refl};
                glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specref);
                glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shine); //0..128 - reflection

                points = Scene::Instance().stub_objects()[i]->getPoints();
                //qDebug() << points;
                //Drawing pictures
                  glBegin( GL_QUADS);
                        //Draw normals for lighting TODO
                        //glNormal3f( x, y ,z);
                        glTexCoord2f(0, 1);
                        glVertex3f( points[0].x, points[0].y, points[0].z);
                        glTexCoord2f(1, 1);
                        glVertex3f( points[1].x, points[1].y, points[1].z);
                        glTexCoord2f(1, 0);
                        glVertex3f( points[2].x, points[2].y, points[2].z);
                        glTexCoord2f(0, 0);
                        glVertex3f( points[3].x, points[3].y, points[3].z);
                  glEnd();
            }

        }

        //Drawing lenses
        for( i = 0; i < Scene::Instance().stub_objects().size(); i++)
        {

            //checking type of object - lense or picture
            LensObjectStub* n = dynamic_cast<LensObjectStub*>(Scene::Instance().stub_objects()[i]);
            if (n)
            {

                shine = 128;
                refl = 1.0;
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);

                image = convertToGLFormat(n->heightMap1());
                glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)image.width(), (GLsizei)image.height(), 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

                if (Scene::Instance().stub_objects()[i]->selected())
                {
                    glColor4f( 0.1, 0.1, 1.0, 0.5);
                } else {
                    glColor4f( 1.0, 1.0, 1.0, 0.5);
                }

                float specref[] = { refl, refl, refl};
                glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specref);
                glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shine); //0..128 - reflection

                points = Scene::Instance().stub_objects()[i]->getPoints();
                //qDebug() << points;
                //Drawing pictures
                  glBegin( GL_QUADS);
                        //Draw normals for lighting TODO
                        //glNormal3f( x, y ,z);
                        glTexCoord2f(0, 1);
                        glVertex3f( points[0].x, points[0].y, points[0].z);
                        glTexCoord2f(1, 1);
                        glVertex3f( points[1].x, points[1].y, points[1].z);
                        glTexCoord2f(1, 0);
                        glVertex3f( points[2].x, points[2].y, points[2].z);
                        glTexCoord2f(0, 0);
                        glVertex3f( points[3].x, points[3].y, points[3].z);
                  glEnd();

                  //draw back with diff texture
                  glCullFace(GL_BACK);

                  image = convertToGLFormat(n->heightMap2());
                  glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)image.width(), (GLsizei)image.height(), 0,
                                   GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

                  glBegin( GL_QUADS);
                        //Draw again wi other texture
                        glTexCoord2f(0, 1);
                        glVertex3f( points[0].x, points[0].y, points[0].z);
                        glTexCoord2f(1, 1);
                        glVertex3f( points[1].x, points[1].y, points[1].z);
                        glTexCoord2f(1, 0);
                        glVertex3f( points[2].x, points[2].y, points[2].z);
                        glTexCoord2f(0, 0);
                        glVertex3f( points[3].x, points[3].y, points[3].z);
                  glEnd();
                  glCullFace(GL_FRONT_AND_BACK);
                  glDisable(GL_CULL_FACE);

            }

         }

        /*glPushMatrix();
            glBegin(GL_LINES);
            glVertex3f(8, -9, 5);
            glVertex3f(0, 0, 0);
            glEnd();
            glPopMatrix();*/
    glPopMatrix();

    drawCamera();
    //clear drawing command stack
    swapBuffers();
}

void GLWidget::gluPerspective()
{
    float aspect = widget_width/widget_height;
    // Start in projection mode.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double xmin, xmax, ymin, ymax;
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::resizeGL(int width, int height)
{
    //set view area like window: place to draw
    glViewport( 0, 0, width, height);
    widget_width = width;
    widget_height = height;

    gluPerspective();

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (r - speed * 0.1 * event->delta() > 0) {
        r -= speed * 0.1 * event->delta();
    };
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        if (phi + speed * dx < 0)
            phi = 360 + phi;
        if (phi + speed * dx > 360)
            phi = phi - 360;

        phi += speed * dx;

        if ((teta + speed * dy < 90) && (teta + speed * dy > -90)) {
            teta += speed * dy;
        }
    } else if (event->buttons() & Qt::RightButton) {
        if (teta > 0) {
            cam_x += dy * sin(-phi*M_PI/180) * 0.0009 * r;
            cam_z += dy * cos(-phi*M_PI/180) * 0.0009 * r; //qDebug()<<phi;
        } else {
            cam_x -= dy * sin(-phi*M_PI/180) * 0.0009 * r;
            cam_z -= dy * cos(-phi*M_PI/180) * 0.0009 * r; //qDebug()<<phi;
        }
        cam_z += - dx * sin(-phi*M_PI/180) *0.0009 * r;
        cam_x += dx * cos(-phi*M_PI/180) * 0.0009 * r;

    }
    lastPos = event->pos();
    updateGL();
}

void GLWidget::rotateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef( 0.0, 0.0, -r);
    glRotatef( teta, 1.0f, 0.0f, 0.0f);
    glRotatef( phi, 0.0f, 1.0f, 0.0f);
    glTranslatef( cam_x, cam_y, cam_z);
}

void GLWidget::drawGround( float fExtent, float fStep, float y)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    float iLine;
    //Multiplyer and Pattern of lines
    glLineStipple( 1, 0x5555);
    glColor3f( 0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
        for( iLine = -fExtent; iLine <= fExtent; iLine += fStep)
        {
            if (iLine!=0)
            {
                glVertex3f( iLine, y, fExtent);
                glVertex3f( iLine, y, -fExtent);
                glVertex3f( fExtent, y, iLine);
                glVertex3f( -fExtent, y, iLine);
            }
        }
    glEnd();

    glLineStipple( 1, 0xFFFF);
    glPushAttrib( GL_LIGHTING_BIT);

        glDisable( GL_LIGHTING);
        glBegin(GL_LINES);
            //Red - x;
            glColor3f( 1.0, 0.0, 0.0);
            glVertex3f( -10.0, 0.0, 0.0);
            glVertex3f( 10.0, 0.0, 0.0);
            //Green - y
            glColor3f( 0.0, 1.0, 0.0);
            glVertex3f( 0.0, -5, 0.0);
            glVertex3f( 0.0, 5, 0.0);
            //Blue - z
            glColor3f( 0.0, 0.0, 1.0);
            glVertex3f( 0.0, 0.0, -10.0);
            glVertex3f( 0.0, 0.0, 10.0);
        glEnd();

        //Draw arrows
        glPushMatrix();
            glColor3f( 1.0, 0.0, 0.0);
            glTranslatef(10.0,0.0,0.0);
            glRotatef( 90.0 , 0.0, 1.0, 0.0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.1, 0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
                glVertex3f(-0.1, -0.1, 0.0);
                glVertex3f(-0.1, 0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
                glVertex3f(0.1, -0.1, 0.0);
                glVertex3f(-0.1, -0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
            glEnd();
            QString str = "X";
            QFont f("Arial");
            f.setWeight(75);
            renderText ( 0, 0, 0.5, str, f );
        glPopMatrix();
        glPushMatrix();
            glColor3f( 0.0, 1.0, 0.0);
            glTranslatef(0.0,5,0.0);
            glRotatef( -90.0 , 1.0, 0.0, 0.0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.1, 0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
                glVertex3f(-0.1, -0.1, 0.0);
                glVertex3f(-0.1, 0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
                glVertex3f(0.1, -0.1, 0.0);
                glVertex3f(-0.1, -0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
            glEnd();
            str = "Y";
            renderText ( 0, 0, 0.5, str, f );
        glPopMatrix();
        glPushMatrix();
            glColor3f( 0.0, 0.0, 1.0);
            glTranslatef(0.0,0.0,10.0);
            glRotatef( 90.0 , 0.0, 0.0, 1.0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.1, 0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
                glVertex3f(-0.1, -0.1, 0.0);
                glVertex3f(-0.1, 0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
                glVertex3f(0.1, -0.1, 0.0);
                glVertex3f(-0.1, -0.1, 0.0);
                glVertex3f(0.0, 0.0, 0.3);
            glEnd();
            str = "Z";
            renderText ( 0, 0, 0.5, str, f );
        glPopMatrix();

    glPopAttrib();

    glPopMatrix();
}

void GLWidget::drawCamera()
{
    Point3D p1 = Scene::Instance().camera()->point() + Scene::Instance().camera()->horizontalVect();
    Point3D p2 = Scene::Instance().camera()->point() + Scene::Instance().camera()->verticalVect();
    Point3D p3 = Scene::Instance().camera()->point() + Scene::Instance().camera()->horizontalVect()
                                                      + Scene::Instance().camera()->verticalVect();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glBegin(GL_TRIANGLES);
            glColor4f(0.9, 0.9, 0.7, 1.0);
            glVertex3d(Scene::Instance().camera()->summit().x,
                       Scene::Instance().camera()->summit().y,
                       Scene::Instance().camera()->summit().z);
            glColor4f(0.9, 0.9, 0.7, 0.3);
            glVertex3d(Scene::Instance().camera()->point().x,
                       Scene::Instance().camera()->point().y,
                       Scene::Instance().camera()->point().z);
            glVertex3d(p1.x, p1.y, p1.z);

            glColor4f(0.85, 0.85, 0.7, 1.0);
            glVertex3d(Scene::Instance().camera()->summit().x,
                       Scene::Instance().camera()->summit().y,
                       Scene::Instance().camera()->summit().z);
            glColor4f(0.85, 0.85, 0.7, 0.3);
            glVertex3d(p1.x, p1.y, p1.z);
            glVertex3d(p3.x, p3.y, p3.z);

            glColor4f(0.9, 0.9, 0.7, 1.0);
            glVertex3d(Scene::Instance().camera()->summit().x,
                       Scene::Instance().camera()->summit().y,
                       Scene::Instance().camera()->summit().z);
            glColor4f(0.9, 0.9, 0.7, 0.3);
            glVertex3d(p2.x, p2.y, p2.z);
            glVertex3d(p3.x, p3.y, p3.z);

            glColor4f(0.85, 0.85, 0.7, 1.0);
            glVertex3d(Scene::Instance().camera()->summit().x,
                       Scene::Instance().camera()->summit().y,
                       Scene::Instance().camera()->summit().z);
            glColor4f(0.8, 0.8, 0.7, 0.3);
            glVertex3d(Scene::Instance().camera()->point().x,
                       Scene::Instance().camera()->point().y,
                       Scene::Instance().camera()->point().z);
            glVertex3d(p2.x, p2.y, p2.z);

        glEnd();
    glPopMatrix();
}

void GLWidget::cameraView()
{
    Point3D p1 = Scene::Instance().camera()->point() + Scene::Instance().camera()->horizontalVect();
    Point3D p2 = Scene::Instance().camera()->point() + Scene::Instance().camera()->verticalVect();
    Point3D p3 = Scene::Instance().camera()->point() + Scene::Instance().camera()->horizontalVect()
                                                             + Scene::Instance().camera()->verticalVect();
    cam_x = -(Scene::Instance().camera()->point().x + p3.x)/2;
    cam_y = -(Scene::Instance().camera()->point().y + p3.y)/2;
    cam_z = -(Scene::Instance().camera()->point().z + p3.z)/2;

    /* get smaller side of quad of camera
     * then count angle between center vector from summit
     *
     * fovy - camera pyramid angle
     */
    Vector3D v1( Scene::Instance().camera()->point(), Scene::Instance().camera()->summit());
    Vector3D v2( p1, Scene::Instance().camera()->summit());
    Vector3D v3( p2, Scene::Instance().camera()->summit());
    Vector3D v4( p3, Scene::Instance().camera()->summit());

    Vector3D smaller_v(1,1,1);
    if ((v1-v2).length() > (v2-v3).length())
    {
        smaller_v = v3 + v2;
    } else {
        smaller_v = v2 + v1;
    }

    Vector3D middle_v = v2 + v3;

    fovy = acos( (v1 + v3).dotProduct( smaller_v) / ( (v1 + v3).length() * smaller_v.length())) * 180/M_PI;

    r = (middle_v).length()/2;

    //rotate camera according to summit
    if ( middle_v.z != 0) {
        if (middle_v.x > 0 && middle_v.z > 0) {
            phi = - atan( fabs(middle_v.x/middle_v.z));
        }
        if (middle_v.x >= 0 && middle_v.z < 0) {
            phi = M_PI + atan( fabs(middle_v.x/middle_v.z));
        }
        if (middle_v.x <= 0 && middle_v.z > 0) {
            phi = atan( fabs(middle_v.x/middle_v.z));
        }
        if (middle_v.x < 0 && middle_v.z < 0) {
            phi = M_PI - atan( fabs(middle_v.x/middle_v.z));
        }
    } else {
        if (middle_v.z>0)
        {
            phi = M_PI/2;
        } else {
            phi = -M_PI/2;
        }
    }
    if (sqrt(middle_v.x * middle_v.z) != 0) {
        teta = atan(middle_v.y/sqrt(middle_v.x*middle_v.x + middle_v.z * middle_v.z)) * 180/M_PI;
    } else {
        if (middle_v.y > 0) {
            teta = M_PI/2;
        } else {
            teta = -M_PI/2;
        }
    }

    qDebug()<<middle_v;
    phi = phi*180/M_PI; qDebug()<<middle_v;

    gluPerspective();
}

void GLWidget::normalView()
{
    fovy = 30.0f;
    zNear = 0.1;
    zFar = 400.0;
    teta = 30.0;
    phi = 45.0;
    r = 20.0;
    cam_x = 0;
    cam_y = 0;
    cam_z = 0;
    gluPerspective();
}
