#include "Texture.hpp"
#include <iostream>
#define EPSILON 0.0001


Texture::Texture(const std::string & filename){
	img = new Image(filename);
}

Texture::~Texture(){
	delete img;
}

glm::vec3 Texture::colour(double u, double v){
	assert(u<=1.0 + EPSILON);
	assert(v<=1.0 + EPSILON);
    int xPos = u* (img->width() - 1);
    int yPos = v* (img->height() - 1);
    return glm::vec3((*img)(xPos,yPos,0),(*img)(xPos,yPos,1),(*img)(xPos,yPos,2));
}