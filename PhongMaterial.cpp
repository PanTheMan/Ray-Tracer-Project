#include "PhongMaterial.hpp"
// #include "Material.hpp"
#include "Intersection.hpp"
#include <iostream>

static PhongMaterial air = PhongMaterial({0,0,0}, {0,0,0}, 0, 1.0, 0,0);

PhongMaterial *PhongMaterial::Air = &air;
PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double refractIdx, double glossiness, double reflectance )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_refractIdx(refractIdx)
	, m_gloss(glossiness)
	, m_reflect(reflectance)
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::getDiffuse(){
	return m_kd;
}

glm::vec3 PhongMaterial::getSpec(){
	return m_ks;
}

double PhongMaterial::getShiny(){
	return m_shininess;
}

double PhongMaterial::getRefractIdx(){
	return m_refractIdx;
}

double PhongMaterial::getGloss(){
	return m_gloss;
}

double PhongMaterial::getReflectance(){
	return m_reflect;
}