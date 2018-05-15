#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "Intersection.hpp"

class Mesh; 

class Primitive {
public:
  virtual Intersection intersect(const Ray &r);
  virtual ~Primitive();
protected:
  Primitive *prim;
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual Intersection intersect(const Ray &r);
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  Cube();
  virtual Intersection intersect(const Ray &r);
  virtual ~Cube();
};

class Cylinder : public Primitive {
public:
  Cylinder();
  virtual Intersection intersect(const Ray &r);
  virtual ~Cylinder();
};

class Cone : public Primitive {
public:
  Cone();
  virtual Intersection intersect(const Ray &r);
  virtual ~Cone();
};

class Torus : public Primitive {
public:
  Torus();
  virtual Intersection intersect(const Ray &r);
  virtual ~Torus();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual Intersection intersect(const Ray &r);

// private:
  glm::vec3 m_pos;
  double m_radius;

};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size);


  virtual ~NonhierBox();
  virtual Intersection intersect(const Ray &r);

private:
  Mesh *mesh;
  glm::vec3 m_pos;
  double m_size;
};
