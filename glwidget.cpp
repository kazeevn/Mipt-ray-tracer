#include "glwidget.h"
//#include <QDebug>

//#ifndef GL_MULTISAMPLE
//#define GL_MULTISAMPLE  0x809D
//#endif

#include "core/objects/stubs/lensobjectstub.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    r = 20.0;
    speed = 0.2;
    teta = 30.0;
    phi = 45.0;

    //qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    //qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
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
    int i, type;
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


        for( i = 0; i < Scene::Instance().stub_objects().size(); i++)
        {

            //checking type of object - lense or picture
            LensObjectStub* n = dynamic_cast<LensObjectStub*>(Scene::Instance().stub_objects()[i]);
            if (n)
            {
                type = 1; // Lense
            } else {
                type = 0; // Picture
            }

            float refl;
            int shine;

            if (type == 0)
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
            }

            if (type == 1)
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

              if (type == 1) {

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
            //drawing lens
          /*    glDisable(GL_TEXTURE_2D);
              glColor4f(0.8,0.8,0.8,1.0);

                int x, y;
                for ( y = 0; y < objl->frontSize().height(); y ++) {
                    glBegin(GL_TRIANGLE_STRIP);
                    for (x = 0; x < objl->frontSize().width(); x++) {
                        glVertex3f(objl->frontPoint(x,y).x, objl->frontPoint(x,y).y, objl->frontPoint(x,y).z);
                        glVertex3f(objl->frontPoint(x,y+1).x, objl->frontPoint(x,y+1).y,objl->frontPoint(x,y+1).z);
                    }
                    glEnd();
                }

                for ( y = 0; y < objl->backSize().height(); y ++) {
                    glBegin(GL_TRIANGLE_STRIP);
                    for (x = 0; x < objl->backSize().width(); x++) {
                        glVertex3f(objl->backPoint(x,y).x, objl->backPoint(x,y).y, objl->backPoint(x,y).z);
                        glVertex3f(objl->backPoint(x,y+1).x, objl->backPoint(x,y+1).y,objl->backPoint(x,y+1).z);
                    }
                    glEnd();
                }*/

        }
        //glDepthMask( GL_FALSE/TRUE) - turn off depth check for a moment

    glPopMatrix();

    drawCamera();
    //clear drawing command stack
    swapBuffers();
}

void GLWidget::gluPerspective(double fovy,double aspect, double zNear, double zFar)
{
    // Start in projection mode.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double xmin, xmax, ymin, ymax;
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

void GLWidget::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    //glViewport((width - side) / 2, (height - side) / 2, side, side);

    //set view area like window: place to draw
    glViewport( 0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0f, width/height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
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
            phi = 360 - phi;
        if (phi + speed * dx > 360)
            phi = phi - 360;

        phi += speed * dx;

        if ((teta + speed * dy < 90) && (teta + speed * dy > -90)) {
            teta += speed * dy;
        }
    } else if (event->buttons() & Qt::RightButton) {
        if (r + speed * 0.1 * dy > 0) {
            r += speed * 0.1 * dy;
        };
    }
    lastPos = event->pos();
    updateGL();
}

void GLWidget::rotateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -r);
    glRotatef( teta, 1.0f, 0.0f, 0.0f);
    glRotatef( phi, 0.0f, 1.0f, 0.0f);
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
