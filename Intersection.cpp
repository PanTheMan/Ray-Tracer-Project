#include "Intersection.hpp"
#include "SceneNode.hpp"
#include "GeometryNode.hpp"

Intersection::Intersection(const Ray & r, double t, bool hit):
	incident(r), t(t), hit(hit), textureMapCoords(glm::vec2()), tex(nullptr)
{
}

Intersection::Intersection(const Ray & r, double t):
	incident(r), t(t), hit(false), textureMapCoords(glm::vec2()), tex(nullptr)
{
}

Texture *Intersection::getTexture() const
{
	if(node->m_nodeType == NodeType::GeometryNode){
		GeometryNode *gn = dynamic_cast<GeometryNode*>(node);
		return gn->tex;
	} else {
		return nullptr;
	}
}
