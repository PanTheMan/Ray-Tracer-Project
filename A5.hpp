#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

struct ColourHit{
	bool hit;
	glm::vec3 colour;
};

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);
//Multithreaded functions
void *renderRows(void * args);
void *adaptiveAntiAlias(void *args);

glm::vec3 colourLighting(const Intersection &intersect, Light * l);
ColourHit rayColour(const Ray &r, const std::list<Light *> & lights, SceneNode * root, const glm::vec3 & ambient, int maxHits);

// Functions to transform DCS to WCS
glm::mat4 R3(glm::vec3 up, glm::vec3 eye, glm::vec3 view);
glm::mat4 T1(double nx, double ny, double d);
glm::mat4 S2(double h, double ny);
glm::vec4 calcPinWorld(double x, double y, glm::mat4 trans);
////////////////////////////////////

Intersection hit(const Ray &r, SceneNode *root);
glm::vec3 directLight(const std::list<Light *> &lights, const Intersection &intersect, SceneNode *root, const glm::vec3 & ambient, int maxHits);
Ray findRefraction(const Ray &incident, const Intersection &intersect);

// Function to generate a cone of random rays 
std::vector<glm::vec3> randomRays(glm::vec3 r, glm::vec3 norm, double m_gloss);

// fresnel reflection calculation
double simpleFresnel(const glm::vec4 norm, const glm::vec4 incident, double fromRefractIdx, double toRefractIdx);
