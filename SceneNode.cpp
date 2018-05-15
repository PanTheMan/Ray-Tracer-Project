#include "SceneNode.hpp"

#include "cs488-framework/MathUtils.hpp"

#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;


// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	m_nodeType(NodeType::SceneNode),
	trans(mat4()),
	invtrans(mat4()),
	m_nodeId(nodeInstanceCount++)
{

}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode & other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans)
{
	for(SceneNode * child : other.children) {
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
	for(SceneNode * child : children) {
		delete child;
	}
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4& m) {
	trans = m;
	invtrans = glm::inverse(m);
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_transform() const {
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_inverse() const {
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
	children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child) {
	children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
	        break;
		case 'z':
			rot_axis = vec3(0,0,1);
	        break;
		default:
			break;
	}
	mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
	set_transform( rot_matrix * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 & amount) {
	set_transform( glm::scale(amount) * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount) {
	set_transform( glm::translate(amount) * trans );
}


//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
	return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream & operator << (std::ostream & os, const SceneNode & node) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType) {
		case NodeType::SceneNode:
			os << "SceneNode";
			break;
		case NodeType::GeometryNode:
			os << "GeometryNode";
			break;
		case NodeType::JointNode:
			os << "JointNode";
			break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}

Intersection SceneNode::intersect(const Ray &r){
	glm::vec4 origin = invtrans*r.origin;
	glm::vec4 dir = invtrans*r.dir; 

	Intersection res(r, 0);
	
	Ray newRay(origin, dir);
	for (std::list<SceneNode *>::iterator it=children.begin(); it != children.end(); ++it) {
		//GeometryNode * child = dynamic_cast<GeometryNode*>(*it);
		SceneNode * child = *it;
		Intersection childIntersect = child->intersect(newRay);
		if(childIntersect.hit){
			// check if we haven't got a hit previously 
			if( !res.hit || glm::length(childIntersect.intersectPoint - childIntersect.incident.origin) < glm::length(res.intersectPoint - res.incident.origin)) {
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
		res.intersectPoint = res.incident.origin + glm::vec4(res.incident.dir.x * res.t,res.incident.dir.y * res.t,res.incident.dir.z * res.t,0);
		// std::cout << "TEST: " << res.intersectPoint.x << " " << res.intersectPoint.y << res.intersectPoint.z << std::endl;
	}

	return res;
}
// NON HIER INTERSECT
// Intersection SceneNode::intersect(const Ray & r){
// 	Intersection res(r, 0);

// 	for (std::list<SceneNode *>::iterator it=children.begin(); it != children.end(); ++it) {
// 		SceneNode * child = *it;
// 		Intersection intersect = child->intersect(r);
// 		if(intersect.hit){
// 			// check if we haven't got a hit previously
// 			if( !res.hit || intersect.t < res.t ) {
// 				res = intersect;
// 			}
// 		}
// 	}

// 	// Got a hit, find info
// 	// if(res.hit){
// 	// 	glm::vec3 norm = res.norm

// 	// }
// 	return res;
// }
