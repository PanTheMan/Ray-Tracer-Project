#include "Ray.hpp"

Ray::Ray(glm::vec4 o, glm::vec4 d): origin(o), dir(d), mat(PhongMaterial::Air)
{
}

