#ifndef GLLight_H
#define GLLight_H

#include <vector>
#include <string>
#include <math.h>

#define Pi 3.14
//TODO M_PI
enum LightType { DISTANT, POINT, PROJECTOR};

using namespace std;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} color1;

typedef struct {
    float x;
    float y;
    float z;
    color1 c;
} coord1;

class glLight
{
private:
    LightType type;
    coord1 direction;
    coord1 x;
    float brightness;
    color1 clr;
    float shadowKoef; //this defines proportion between abient and diffuse
public:
    void draw( int i); //i - index nubmer of current glLight on the sciene
    void setPos( float nx, float ny, float nz)
    {
        x.x = nx;
        x.y = ny;
        x.z = nz;
    }
    void setDir( float nx, float ny, float nz)
    {
        direction.x = nx;
        direction.y = ny;
        direction.z = nz;
    }
    void setColor( float r, float g, float b)
    {
        clr.r = r;
        clr.g = g;
        clr.b = b;
    }
    glLight( LightType t );
    ~glLight() {};
};

#endif // GLLight_H
