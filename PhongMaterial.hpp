#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double refractIdx, double glossiness, double reflectance);
  virtual ~PhongMaterial();
  glm::vec3 getDiffuse();
  glm::vec3 getSpec();
  double getShiny();
  double getRefractIdx();
  double getGloss();
  double getReflectance();
  static PhongMaterial *Air;
private:
  glm::vec3 m_kd;

  glm::vec3 m_ks;
  double m_refractIdx;
  double m_shininess;
  double m_gloss;
  double m_reflect;
};
