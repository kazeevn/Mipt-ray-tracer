#include "gllight.h"
#include <QGLWidget>

glLight :: glLight( LightType t)
{
    type = t;
    x.x = 100.0f;
    x.y = 100.0f;
    x.z = -100.0f;
    clr.r = 1.0f;
    clr.g = 1.0f;
    clr.b = 1.0f;
    shadowKoef = 0.3; //defining by user is not supported
}

void glLight :: draw( int i)
{
    float spotDir[] = { 0.0f, 0.0f, -1.0f}; //const. It's a direction in coord syst of light (e.x. Cone)
                                             //but not the dir of the light itself
    float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    float t;

    /* Here sync dir and +(!)z coord
     */
    glPushMatrix();

        /* rotate round y.
         */
        float a, b, c;
        if ( direction.z == 0)
        {
            a = -Pi/2;
        } else {
            a = atan( fabs(direction.x/direction.z));
        }
        if ( (direction.x > 0) && (direction.z >= 0))
            a = Pi + a;
        if ( (direction.x < 0) && (direction.z > 0))
            a = Pi - a;
        if ( (direction.x > 0) && (direction.z < 0))
            a = -a;

        c = sqrt( direction.z*direction.z + direction.x*direction.x);

        /* rotate round new x (up)
         */
        if ( c == 0)
        {
            b = Pi/2;
        } else {
            b = atan( fabs(direction.y/c));
        }
        if ( direction.y < 0 )
        {
            b = -b;
        }
        glTranslatef( x.x, x.y, x.z);

        glRotatef( a*(180/Pi) , 0.0f, 1.0f, 0.0f);
        glRotatef( b*(180/Pi) , 1.0f, 0.0f, 0.0f);


        /* End of rotating.
         * Now set light correspoding to its type
         */

        switch (type)
        {
            case PROJECTOR:
                //Draw projector (cone)
                glPushMatrix();
                //because cone is drawn to negative z
                //glRotatef(180, 0.0f, 1.0f, 0.0f);

                //TODO Solid cone
                //glutSolidCone( 4.0f, 6.0f, 15, 15);
                glPopMatrix();

                glPushAttrib( GL_LIGHTING_BIT);
                    glDisable( GL_LIGHTING);
                    glColor3ub( 255, 255, 0);
                    //glutSolidSphere( 3.0f, 15, 15);
                glPopAttrib();

                //Set vars for cone of light
                glLightfv( GL_LIGHT0+i, GL_SPOT_DIRECTION, spotDir);
                glLightf( GL_LIGHT0+i, GL_SPOT_CUTOFF, 70.0f);

                lightPos[3] = 1.0f; //defines that it is projector
            break;
            case POINT:
                //Draw point (sphere)
                glPushAttrib( GL_LIGHTING_BIT);
                    glDisable( GL_LIGHTING);
                    glColor3ub( 255, 255, 0);
                    //glutSolidSphere( 3.0f, 15, 15);
                glPopAttrib();

               lightPos[3] = 1.0f; //defines that it is spot
            break;
            case DISTANT:
                /* if it's DISTANT => position var sets the direction */
                lightPos[0] = direction.x;
                lightPos[1] = direction.y;
                lightPos[2] = direction.z;
                lightPos[3] = 0.0f; //defines that it is distant
            break;
        }

        //fonovoje izluchenije istochnika sveta
        float ambientLight[] = { shadowKoef*clr.r, shadowKoef*clr.g,
                                   shadowKoef*clr.b, 1.0f};
        //rassejannoje izluchenije istochnoka
        float diffuseLight[] = { (1-shadowKoef)*clr.r, (1-shadowKoef)*clr.g,
                                   (1-shadowKoef)*clr.b, 1.0f};
        /* cvet zerkalnogo izluchenija; max jarkostj dlja vida - mozet
         * ne sootvetsvovatj rezultatu pri renderinge.
         * pitajemsja sohranitj cvet, proporcionalno vsjo uveliciv
         */
        t = clr.r;
        if (t < clr.g) t = clr.g;
        if (t < clr.b) t = clr.b;
        float specular[] = { clr.r/t, clr.g/t, clr.b/t, 1.0f};

        /* ambient - color in dark;
         * diffuse - color in light;
         * shiness - strength of bliki [1, 128]
         * specular - color of bliki
         */
        glLightfv( GL_LIGHT0+i, GL_POSITION, lightPos);
        glLightfv( GL_LIGHT0+i, GL_AMBIENT, ambientLight);
        glLightfv( GL_LIGHT0+i, GL_DIFFUSE, diffuseLight);
        glLightfv( GL_LIGHT0+i, GL_SPECULAR, specular);

        //glLightModeli(GL_LIGHT_MODEL_COLOR, GL_SEPARATE_SPECULAR_COLOR);
        //glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLight);

        glEnable( GL_LIGHT0+i);

    glPopMatrix();
}
