#ifndef MIPT_RAY_OBJECT_H_
#define MIPT_RAY_OBJECT_H

#include<sting.h>
#include<pixmap.h>
enum Type {MIRROR, MEDIUM}

class Object {
public:
  Object() {};
  Object(string filename, Type type) {};
  Object(pixmap map, Type type) {};
  ~Object() {};
  Type get_type() {};
  set_type(Type type) {};
  load_pixmap(string filename) {};
  load_pixmap(Pixmap map) {};
#endif // MIPT_RAY_OBJECT_H_
