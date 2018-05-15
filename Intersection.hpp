#ifndef Intersection_hpp
#define Intersection_hpp

#include <stdio.h>

#include "Ray.hpp"
#include "PhongMaterial.hpp"
#include <glm/glm.hpp>
#include "Texture.hpp"
class SceneNode;

class Intersection {
public:
	Ray incident;
	double t;
	bool hit;

	// Normal of intersection
	PhongMaterial *mat;
	glm::vec4 norm;
	SceneNode * node;
	glm::vec4 intersectPoint;
	PhongMaterial * fromMat;
	Texture *tex;
	// Construct an intersection point.
	Intersection(const Ray &r,
				 double t,
				 bool hit);
	Intersection(const Ray &r,
				 double t);
  	glm::vec2 textureMapCoords;
	Texture *getTexture() const;
};

#endif