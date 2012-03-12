#ifndef MIPT_RAY_SCENE_H
#define MIPT_RAY_SCENE_H

#include<string.h>
#include<object.h>
#include<object.h>
typedef Coords double[3];
typedef Colour int[3];

class Scene {
public:
  Scene() {};
  Scene(string filename) {};
  ~Scene() {};
  int add_object(Object object, Coords coords) {};
  // TODO(kazeevn) Add API for
  // listing
  // removing
  Colour cast_ray(Ray ray) {};

#endif // MIPT_RAY_SCENE_H
