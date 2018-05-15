#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "PhongMaterial.hpp"

class Ray {
public:
	glm::vec4	origin;
	glm::vec4	dir;
	Ray(glm::vec4 o, glm::vec4 d);
	//material it is in
	PhongMaterial *mat;
	double time;
};

#endif