# pragma once
#include "Image.hpp"
#include <string>
#include <glm/glm.hpp>

class Texture
{
public:
    Texture(const std::string & filename);
    ~Texture();
    glm::vec3 colour(double u, double v);
	Image *img;
};