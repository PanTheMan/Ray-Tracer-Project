#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat, BumpMap *bump )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
	, m_bump( bump )
	, tex(nullptr)
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

Intersection GeometryNode::intersect(const Ray & r){
	glm::vec4 origin = invtrans*r.origin;
	glm::vec4 dir = invtrans*r.dir; 
	Ray newRay(origin, dir);

	Intersection res = m_primitive->intersect(newRay);

	if(res.hit){
		res.mat = (PhongMaterial *)m_material;
		res.node = this;
	}
	for (std::list<SceneNode *>::iterator it=children.begin(); it != children.end(); ++it) {
		SceneNode * child = *it;
		Intersection childIntersect = child->intersect(newRay);
		if(childIntersect.hit){

			// check if we haven't got a hit previously/compare to find closest one
			if( !res.hit || glm::length(childIntersect.intersectPoint - childIntersect.incident.origin) < glm::length(res.intersectPoint - res.incident.origin) ) {
				res = childIntersect;
			}
		}
	}

	if(res.hit){

		glm::vec3 norm = glm::vec3(res.norm);
		glm::mat3 invtrans3 = glm::mat3(invtrans);
		res.norm = glm::vec4(glm::transpose(invtrans3) * norm, 0);
		res.norm = glm::normalize(res.norm);
		res.incident.origin = trans * res.incident.origin;
		res.incident.dir = trans * res.incident.dir;
		res.fromMat = r.mat;


		res.intersectPoint = res.incident.origin + glm::vec4(res.incident.dir.x * res.t,res.incident.dir.y * res.t,res.incident.dir.z * res.t,0);
		if(m_bump)
			m_bump->applyBumpMap(res); 
	}
	return res;

}
// // For non-hier
// Intersection GeometryNode::intersect(const Ray & r){
// 	// glm::vec4 orig = r.origin;
// 	// glm::vec4 dir = r.dir;
// 	Intersection intersect = m_primitive->intersect(r);
// 	if (intersect.hit) {
// 		intersect.mat = (PhongMaterial *)m_material;
// 		intersect.node = this;
// 	}
// 	return intersect;
// }
