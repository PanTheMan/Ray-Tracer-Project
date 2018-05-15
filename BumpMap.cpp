#include "BumpMap.hpp"

BumpMap::BumpMap(const std::string & filename){
	hasBumpMap = false;
	bumpMap = new Image(filename);
}

BumpMap::~BumpMap(){
	delete bumpMap;
}

// FROM MY UNDERSTANDING, applying a bump map, means taking the usual normal, 
//and applying some transformations to get a different normal
void BumpMap::applyBumpMap(Intersection &intersect){
	if(!hasBumpMap)
		return;

	int xPos = intersect.textureMapCoords.x * (bumpMap->width()-1);

	int yPos = intersect.textureMapCoords.y * (bumpMap->height()-1);

	double dx,dy;
	double e = 0.0001;
	if(xPos==0)
		dx = (*bumpMap)(xPos+1,yPos,0) - (*bumpMap)(xPos,yPos,0)/0.5;
	else if(xPos==bumpMap->width()-1)
		dx = (*bumpMap)(xPos,yPos,0) - (*bumpMap)(xPos-1,yPos,0)/0.5;
	else
		dx = (*bumpMap)(xPos+1,yPos,0) - (*bumpMap)(xPos-1,yPos,0);
	if(yPos==0){
		dy = (*bumpMap)(xPos,yPos+1,0) - (*bumpMap)(xPos,yPos,0)/0.5;
	}
	else if(yPos==bumpMap->height()-1){
		dy = (*bumpMap)(xPos,yPos,0) - (*bumpMap)(xPos,yPos-1,0)/0.5;
	}
	else{
		dy = (*bumpMap)(xPos,yPos+1,0) - (*bumpMap)(xPos,yPos-1,0);
	}

	glm::vec3 w = glm::normalize(glm::vec3(intersect.norm));
	// take the unit vectors of tangent plane
	glm::vec3 u = glm::cross(w,glm::vec3(intersect.incident.dir));
	glm::vec3 v = glm::normalize(glm::cross(w,u));
	u = glm::normalize(u);
	
	intersect.norm = glm::vec4(glm::normalize(glm::vec3(intersect.norm) + (float)dx * u + (float)dy * v),0);

}
