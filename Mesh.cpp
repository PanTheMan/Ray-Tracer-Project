#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	// Too lazy to set the min and max to the infinity/ 0 respectively XD
	double minX, minY, minZ, maxX, maxY,maxZ;
	bool firstVertex = false;
	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
			// // For bounding volumes
			if(!firstVertex){
				minX = vx;
				minY = vy;
				minZ = vz;

				maxX = vx;
				maxY = vy;
				maxZ = vz;
				firstVertex = true;
			} else{

				minX = std::min(vx, minX);
				minY = std::min(vy, minY);
				minZ = std::min(vz, minZ);
				maxX = std::max(vx, maxX);
				maxY = std::max(vy, maxY);
				maxZ = std::max(vz, maxZ);
			}
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
	// std::cout << fname.c_str() << std::endl;
	if(fname=="plane.obj")
		boundingVol = NULL;
	else{
		glm::vec3 min(minX,minY,minZ);

		glm::vec3 max(maxX,maxY,maxZ);
		glm::vec3 center = (min+max)/2.0;
		double radius = glm::length(max-center);
		boundingVol = new NonhierSphere(center, radius);
	}
}

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<Triangle> faces):
	m_faces(faces), m_vertices(vertices)
{
	// THIS FUNCTION IS ONLY USED FOR CREATING NONHIER CUBES, SO DON"T SET A BOUNDING VOL
	boundingVol = NULL;
}

//TAKEN FROM WIKI
bool Mesh::triangleRayIntersect(glm::vec3 origin, glm::vec3 dir, Triangle triangle, glm::vec3 &baryPosition){
    const float epsilon = 0.000001; 
    glm::vec3 h, s, q;
    float u,v;
    glm::vec3 edge1 = m_vertices[triangle.v2] - m_vertices[triangle.v1];
    glm::vec3 edge2 = m_vertices[triangle.v3] - m_vertices[triangle.v1];

    h = glm::cross(dir, edge2);
    float det = glm::dot(edge1,h);
    if (det > -epsilon && det < epsilon)
        return false;
    float invDet = 1/det;
    s = origin - m_vertices[triangle.v1];
    u = invDet * (glm::dot(s,h));
    if (u < 0.0 || u > 1.0)
        return false;
    q = glm::cross(s,edge1);
    v = invDet * glm::dot(dir,q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = invDet * glm::dot(edge2,q);
    if (t > epsilon) // ray intersection
    {
        baryPosition = glm::vec3(0,0,t); 
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}


Intersection Mesh::intersect(const Ray &r){
	// Intersection boundingHit(r,0);
	// if (boundingVol){
	// 	boundingHit = boundingVol->intersect(r);
	// 	return boundingHit;
	// }

	// Normal mesh intersection calculation
	Intersection res(r,0);
	glm::vec3 baryPosition;
	for(int i=0; i<m_faces.size(); i++){
		Triangle triangle = m_faces[i];
		glm::vec3 origin = glm::vec3(r.origin);
		glm::vec3 dir = glm::vec3(r.dir);

		bool hit = triangleRayIntersect(origin, dir, triangle, baryPosition);


		// Trianlge hit
		if(hit && baryPosition.z > 0){
			// First hit we found
			if(!res.hit){
				res.t = baryPosition.z;
				res.hit = true;
				glm::vec3 norm = -glm::normalize(glm::cross(m_vertices[triangle.v3]-m_vertices[triangle.v1],
															m_vertices[triangle.v2]-m_vertices[triangle.v1]));
				res.norm = glm::vec4(norm, 0);
				glm::vec4 point = r.origin + glm::vec4(r.dir.x * res.t,r.dir.y * res.t, r.dir.z * res.t, 0);
				res.intersectPoint = point;
			// Found one closer to the ray
			} else if(baryPosition.z < res.t){
				res.t = baryPosition.z;
				glm::vec3 norm = -glm::normalize(glm::cross(m_vertices[triangle.v3]-m_vertices[triangle.v1],
															m_vertices[triangle.v2]-m_vertices[triangle.v1]));
				res.norm = glm::vec4(norm, 0);

			}
		}
	}
	// Test
	if(res.hit){
		// std::cout << "Tri: " << res.intersectPoint.x << " " << res.intersectPoint.y << " " << res.intersectPoint.z << std::endl;
		res.textureMapCoords = glm::vec2((res.intersectPoint.x+1.0)/2.0,(res.intersectPoint.z+1.0)/2.0 );
	}
	return res;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}
