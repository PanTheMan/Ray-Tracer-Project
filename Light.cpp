#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"

Light::Light()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}
Light::~Light()
{
}

AreaLight::AreaLight()
{
  corner[0] = glm::vec3(0,0,0);
  corner[1] = glm::vec3(1,0,0); 
  corner[2] = glm::vec3(0,1,0); 
  corner[3] = glm::vec3(1,1,0); 
}

AreaLight::~AreaLight()
{
}

void AreaLight::preprocess(){
  double unitLen = 1.0f / SOFTSIZE;
  for(int i = 0; i < SOFTSIZE; i++) {
    double v = unitLen * i;
    for(int j = 0; j < SOFTSIZE; j++) {
      double u = unitLen * j;
      glm::vec3 R1 = glm::mix(corner[0], corner[3], u);
      glm::vec3 R2 = glm::mix(corner[1], corner[2], u);
      samplePoints[i * SOFTSIZE + j] = glm::mix(R1, R2, v);
    }
  }
}

glm::vec3 AreaLight::randomPoint(int iteration){
  return samplePoints[iteration];
}


std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.colour) 
  	  << ", " << glm::to_string(l.position) << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.falloff[i];
  }
  out << "]";
  return out;
}
