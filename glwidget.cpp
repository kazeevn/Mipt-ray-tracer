#include "glwidget.h"
//#include <QDebug>

//#ifndef GL_MULTISAMPLE
//#define GL_MULTISAMPLE  0x809D
//#endif

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    r = 3.0;
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

    glEnable(GL_LIGHTING);
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
    PictureObjectStub* obj;
    QList<Point3D> points;
    QImage image;

    //Set backgroung
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rotateCamera();

    drawGround( 1.0f, 0.1f, 0);

    //Using stack of matrix
    glPushMatrix();

        //Draw all objects

        float specref[] = { 0.8, 0.8, 0.8};
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specref);
        glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 20); //0..128 - reflection

        glEnable(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //colors of glObject are replaced by texture. other - GL_REPLACE - combination
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glColor4f( 1.0, 1.0, 1.0, 1.0);
        for( i = 0; i < Scene::Instance().stub_objects().size(); i++)
        {
            obj = (PictureObjectStub*)Scene::Instance().stub_objects()[i];
            points = obj->getPoints();


            //textures
            image = convertToGLFormat(obj->image());
            glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)image.width(), (GLsizei)image.height(), 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, image.bits());


          glBegin( GL_QUADS);
                //Draw normals for lighting TODO
                //glNormal3f( x, y ,z);
                glTexCoord2f(0, 0);
                glVertex3f( points[0].x, points[0].y, points[0].z);
                glTexCoord2f(1, 0);
                glVertex3f( points[1].x, points[1].y, points[1].z);
                glTexCoord2f(1, 1);
                glVertex3f( points[2].x, points[2].y, points[2].z);
                glTexCoord2f(0, 1);
                glVertex3f( points[3].x, points[3].y, points[3].z);
            glEnd();
        }

/*
        vector<glLight>::iterator itl;
        for(itl = lights.begin(); itl != lights.end(); ++itl)
        {
            (*itl).draw( i);
            i++;
        }
*/
        //glDepthMask( GL_FALSE/TRUE) - turn off depth check for a moment

    glPopMatrix();
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
    gluPerspective(30.0f, width/height, 0.1, 10.0);

    glMatrixMode(GL_MODELVIEW);
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
        if (speed * 0.1 * dy > -100) {
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
            glVertex3f( -1.0, 0.0, 0.0);
            glVertex3f( 1.0, 0.0, 0.0);
            //Green - y
            glColor3f( 0.0, 1.0, 0.0);
            glVertex3f( 0.0, -0.5, 0.0);
            glVertex3f( 0.0, 0.5, 0.0);
            //Blue - z
            glColor3f( 0.0, 0.0, 1.0);
            glVertex3f( 0.0, 0.0, -1.0);
            glVertex3f( 0.0, 0.0, 1.0);
        glEnd();

        //Draw arrows
        glPushMatrix();
            glColor3f( 1.0, 0.0, 0.0);
            glTranslatef(1.0,0.0,0.0);
            glRotatef( 90.0 , 0.0, 1.0, 0.0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.01, 0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
                glVertex3f(-0.01, -0.01, 0.0);
                glVertex3f(-0.01, 0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
                glVertex3f(0.01, -0.01, 0.0);
                glVertex3f(-0.01, -0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
            glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f( 0.0, 1.0, 0.0);
            glTranslatef(0.0,0.5,0.0);
            glRotatef( -90.0 , 1.0, 0.0, 0.0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.01, 0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
                glVertex3f(-0.01, -0.01, 0.0);
                glVertex3f(-0.01, 0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
                glVertex3f(0.01, -0.01, 0.0);
                glVertex3f(-0.01, -0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
            glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f( 0.0, 0.0, 1.0);
            glTranslatef(0.0,0.0,1.0);
            glRotatef( 90.0 , 0.0, 0.0, 1.0);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.01, 0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
                glVertex3f(-0.01, -0.01, 0.0);
                glVertex3f(-0.01, 0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
                glVertex3f(0.01, -0.01, 0.0);
                glVertex3f(-0.01, -0.01, 0.0);
                glVertex3f(0.0, 0.0, 0.03);
            glEnd();
        glPopMatrix();

    glPopAttrib();

    glPopMatrix();
}
