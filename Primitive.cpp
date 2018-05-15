#include <algorithm>
#include "Primitive.hpp"
#include "polyroots.hpp"
#include "Mesh.hpp"
#include <glm/ext.hpp>


Primitive::~Primitive()
{
}

Intersection Primitive::intersect(const Ray &r)
{
	Intersection res(r, 0);
	return res;
}

Sphere::~Sphere()
{
	delete prim;
}

Sphere::Sphere(){
	prim = new NonhierSphere({0,0,0}, 1.0);
}

Intersection Sphere::intersect(const Ray &r)
{
	Intersection res = prim->intersect(r);
	return res;
}

Cube::Cube()
{
	prim = new NonhierBox({0,0,0}, 1.0);
}

Cube::~Cube()
{
	delete prim;
}

Intersection Cube::intersect(const Ray &r)
{
	Intersection res = prim->intersect(r);
	return res;
}

Torus::~Torus()
{
}

Torus::Torus()
{
}

Intersection Torus::intersect(const Ray &r)
{

	Intersection res(r,0);
	// x^2 + z^2 = 1^2
	// -1 <= y <= 1
	double innerR = 1.0;
	double outerR = 2.0;
	
	double A = 1;
	double B = 2 * (r.origin.x * r.dir.x + r.origin.z * r.dir.z);
	double C = r.origin.x * r.origin.x + r.origin.z * r.origin.z - 1;
	double roots[4];
	size_t numRoots = quadraticRoots(A, B, C, roots);
	if(numRoots == 0){
		res.hit = false;
	} else if(numRoots == 1){
	} else {

	}
	return res;
}

// Assume cylinder has radius 1 and height 2
Cylinder::Cylinder()
{
}

Cylinder::~Cylinder()
{
}

Intersection Cylinder::intersect(const Ray &r)
{
	Intersection res(r,0);
	// x^2 + z^2 = 1^2
	// -1 <= y <= 1
	double A = r.dir.x * r.dir.x + r.dir.z * r.dir.z;
	double B = 2 * (r.origin.x * r.dir.x + r.origin.z * r.dir.z);
	double C = r.origin.x * r.origin.x + r.origin.z * r.origin.z - 1;
	double roots[2];
	size_t numRoots = quadraticRoots(A, B, C, roots);
	if(numRoots == 0){
		res.hit = false;
	} else if(numRoots == 1){
		glm::vec4 intersection = r.origin + r.dir * (float) roots[0];
		if(-1 <= intersection.y && intersection.y <= 1) {
			res.hit = true;
			res.t = roots[0];
			res.intersectPoint = intersection;
			res.norm = glm::normalize(glm::vec4(intersection.x, 0, intersection.z, 0));
		} 
		else
			res.hit = false;
		
	} else {
		if(roots[1] < roots[0])
			std::swap(roots[0], roots[1]);

		glm::vec4 point1 = r.origin + r.dir * (float) roots[0];
		glm::vec4 point2 = r.origin + r.dir * (float) roots[1];
		if(point1.y < -1) {
			if(point2.y < -1)
				res.hit = false;
			else {
				// hit bottom circle
				double t = (-1 - r.origin.y) / r.dir.y;
				if(t <= 0)
					res.hit = false;
				else {
					res.hit = true;
					res.t = t;
					res.norm = glm::vec4(0, -1, 0, 0);
					res.intersectPoint = r.origin + r.dir * (float) t;
				}
			}
		} else if(point1.y > 1) {
			if(point2.y > 1)
				res.hit = false;
			else {
				// hit top circle
				double t = (1 - r.origin.y) / r.dir.y;
				if(t <= 0)
					res.hit = false;
				else {
					res.hit = true;
					res.t = t;
					res.norm = glm::vec4(0, 1, 0, 0);
					res.intersectPoint = r.origin + r.dir * (float) t;
				}
			}
		} else{
			//Hit middle cylinder
			if(roots[0] <= 0)
				res.hit = false;
			else {
				res.hit = true;
				res.t = roots[0];
				res.intersectPoint = r.origin + r.dir * (float) res.t;
				res.norm = glm::normalize(glm::vec4(res.intersectPoint.x, 0, res.intersectPoint.z, 0));
			}
		}
	}
	return res;
}

Cone::Cone()
{
}

Cone::~Cone()
{
}

Intersection Cone::intersect(const Ray &r)
{

	Intersection res(r,0);
	// x^2 + z^2 = y^2
	// r^2 = y^2
	double A = r.dir.x * r.dir.x + r.dir.z * r.dir.z - r.dir.y * r.dir.y;
	double B = 2 * (r.dir.x * r.origin.x + r.dir.z * r.origin.z - r.dir.y * r.origin.y);
	double C = r.origin.x * r.origin.x + r.origin.z * r.origin.z - r.origin.y * r.origin.y;
	double roots[2];
	size_t numRoots = quadraticRoots(A, B, C, roots);

	if(numRoots == 0)
		res.hit = false;
	else if(numRoots == 1){
		glm::vec4 intersection = r.origin + r.dir * (float) roots[0];
		if(0 <= intersection.y && intersection.y <= 1){
			res.hit = true;
			res.t = roots[0];

			res.intersectPoint = intersection;
			double xzDist = sqrt(intersection.x*intersection.x + intersection.z*intersection.z);
			glm::vec3 normal = glm::cross(glm::vec3(1, intersection.x/xzDist, 0), glm::vec3(0, intersection.z/xzDist, 1));
			res.norm = glm::normalize(glm::vec4(normal, 0));
		} else
			res.hit = false;
		
	} else {
		if(roots[1] < roots[0])
			std::swap(roots[0], roots[1]);

		glm::vec4 point1 = r.origin + r.dir * (float) roots[0];
		glm::vec4 point2 = r.origin + r.dir * (float) roots[1];

		if(point1.y < 0) {
			if(point2.y < 0) {
				res.hit = false;
			} else{
				if(point2.y <= 1){
					if(roots[1] > 0) {
						res.hit = true;
						res.t = roots[1];

						res.intersectPoint = point2;
						double xzDist = sqrt(point2.x*point2.x + point2.z*point2.z);
						glm::vec3 normal = glm::cross(glm::vec3(1, point2.x/xzDist, 0), glm::vec3(0, point2.z/xzDist, 1));
						res.norm = glm::normalize(glm::vec4(normal, 0)); 						
					} else
						res.hit = false;
				} else
					res.hit = false;
			}
		} else if(point1.y > 1) {
			if(point2.y > 1) {
				res.hit = false;
			} else{
				if(point2.y >= 0){
					if(roots[1] > 0) {
						// Hit the base
						double t = (1- r.origin.y)/r.dir.y;
						glm::vec4 intersectPoint = r.origin + r.dir * (float) t;
						// CHECK EPAN
						if(t > 0) {
							res.hit = true;
							res.t = t;
							res.norm = glm::vec4(0, 1, 0, 0);
							res.intersectPoint = intersectPoint;
						} else
							res.hit = false;

					} else
						res.hit = false;
				} else
					res.hit = false;
			}
		} else {
			// both points hit the cone itself
			if(roots[0] <= 0)
				res.hit = false;
			else {
				glm::vec4 intersection = r.origin + r.dir * (float) roots[0];

				res.hit = true;
				res.t = roots[0];

				res.intersectPoint = intersection;
				double xzDist = sqrt(intersection.x*intersection.x + intersection.z*intersection.z);
				glm::vec3 normal = glm::cross(glm::vec3(1, intersection.x/xzDist, 0), glm::vec3(0, intersection.z/xzDist, 1));
				res.norm = glm::normalize(glm::vec4(normal, 0)); 
			}
		}
	}
	return res;
}

NonhierSphere::~NonhierSphere()
{
}

Intersection NonhierSphere::intersect(const Ray &r)
{
	Intersection res(r,0);
	glm::vec4 center(m_pos.x, m_pos.y, m_pos.z, 1);
	glm::vec4 originToCenter = r.origin - center;
	double A = glm::dot(r.dir, r.dir);
	double B = 2 * glm::dot(r.dir, originToCenter);
	double C = glm::dot(originToCenter, originToCenter) - m_radius * m_radius;
	double roots[2];

	size_t numRoots = quadraticRoots(A, B, C, roots);
	if(numRoots == 0){
		// missed
		res.hit = false;
	} else if (numRoots == 1){
//		std::cout << " 1 " << std::endl;
		res.hit = roots[0] > 0;
		res.t = roots[0];
	} else{
//		std::cout << "2" << std::endl; 
		res.t = std::min(roots[0], roots[1]);
		res.hit = res.t > 0;
	}
	// if(m_pos.y == -1200)
	// 	std::cout << r.origin.x << " " << r.origin.y << " " << r.origin.z << std::endl;
	glm::vec4 point = r.origin + r.dir * (float) res.t;
	// technically norm is 2(p-c)? from notes, but normalize so we don't care about 2
	res.norm = glm::normalize(point - center);
	res.intersectPoint = point;
	if(res.hit){
		// std::cout << "TEST: " << res.intersectPoint.x << " " << res.intersectPoint.y << res.intersectPoint.z << std::endl;

		// Use uv mapping
		glm::vec3 d = glm::normalize(glm::vec3(res.intersectPoint) - m_pos);

		res.textureMapCoords.x = 0.5 + glm::atan(d.x, d.z) / (2*glm::pi<double>());
		res.textureMapCoords.y = 0.5 - glm::asin(d.y/m_radius) / glm::pi<double>();
		// std::cout << res.textureMapCoords.x << " " << res.textureMapCoords.y << std::endl;
	}

	// std::cout <<
	return res;
	
}

NonhierBox::NonhierBox(const glm::vec3& pos, double size)
: m_pos(pos), m_size(size)
{ 
	std::vector<glm::vec3> vertices = {
	  glm::vec3(pos[0], pos[1], pos[2]),
	  glm::vec3(pos[0]+size, pos[1], pos[2]),
	  glm::vec3(pos[0]+size, pos[1], pos[2]+size),
	  glm::vec3(pos[0], pos[1], pos[2]+size),

	  glm::vec3(pos[0], pos[1]+size, pos[2]),
	  glm::vec3(pos[0]+size, pos[1]+size, pos[2]),
	  glm::vec3(pos[0]+size, pos[1]+size, pos[2]+size),
	  glm::vec3(pos[0], pos[1]+size, pos[2]+size),
	};

	int vertexToFace[12][3] = 
	{
	  {0,1,2},
	  {0,2,3},
	  {7,3,2},
	  {7,2,6},
	  {6,2,1},
	  {6,1,5},
	  {5,1,0},
	  {5,0,4},
	  {4,0,3},
	  {4,3,7},
	  {4,7,6},
	  {4,6,5}
	};
	std::vector<Triangle> faces;
	for (int i=0; i<12; i++){
	   faces.push_back(Triangle( vertexToFace[i][0], vertexToFace[i][1], vertexToFace[i][2]));
	}
	mesh = new Mesh(vertices, faces);
}

NonhierBox::~NonhierBox()
{
}


Intersection NonhierBox::intersect(const Ray &r)
{
	Intersection i = mesh->intersect(r);
	i.textureMapCoords = glm::vec2(i.intersectPoint);

	return i;
}
