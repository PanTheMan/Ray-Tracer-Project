#pragma once

#include <iosfwd>
#include <glm/glm.hpp>

#define SOFTSIZE 8

// Represents a simple point light.
struct Light {
  Light();
  ~Light();
  glm::vec3 colour;
  glm::vec3 position;
  double falloff[3];

  // Area light functions
  virtual glm::vec3 randomPoint(int iteration) {return position;};
  virtual bool isAreaLight() {return false;};
  virtual void preprocess() {return;};

};

std::ostream& operator<<(std::ostream& out, const Light& l);

struct AreaLight : Light {
	AreaLight();
  ~AreaLight();
	virtual bool isAreaLight() {return true;};
	virtual glm::vec3 randomPoint(int iteration);
	virtual void preprocess();
	glm::vec3 samplePoints[SOFTSIZE * SOFTSIZE];
	glm::vec3 corner[4];
};
